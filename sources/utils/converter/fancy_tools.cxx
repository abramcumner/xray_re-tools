#include "fancy_tools.h"
#include "xr_level_version.h"
#include "xr_details.h"
#include "xr_dm.h"
#include "xr_ini_file.h"
#include "xr_file_system.h"

using namespace xray_re;

const char PA_FANCY_LEVEL[] = "$fancy_level$";

fancy_tools::~fancy_tools()
{
	delete m_ini;
}

bool fancy_tools::check_paths() const
{
	bool status = true;
	check_path(PA_GAME_LEVELS, status);
	return status;
}

#ifdef _CONSOLE
void fancy_tools::process(const cl_parser& cl)
{
	if (!check_paths())
		return;

	const char* fancy_config;
	if (!cl.get_string("-fancy", fancy_config))
		return;
	m_ini = new xr_ini_file(fancy_config);
	if (m_ini->empty()) {
		msg("can't load %s", fancy_config);
		return;
	}

	xr_file_system& fs = xr_file_system::instance();

	std::string path("gamedata.fancy\\");
	if (cl.get_string("-dir", path))
		fs.append_path_separator(path);
	path.append("levels\\");
	if (!fs.create_path(path)) {
		msg("can't create output folder %s", path.c_str());
		return;
	}

	const char* level;
	const char* profile;
	for (size_t i = 0; m_ini->r_line("levels", i, &level, &profile); ++i) {
		if (!fs.folder_exist(PA_GAME_LEVELS, level)) {
			msg("can't find level %s", level);
			continue;
		}
		if (!fs.create_folder(path + level)) {
			msg("can't create output folder for %s", level);
			continue;
		}
		fs.update_path(PA_LEVEL, PA_GAME_LEVELS, level);
		fs.update_path(PA_FANCY_LEVEL, path, level);
		msg("processing %s", level);
		if (m_ini->line_exist(profile, "fix_details"))
			fix_details(m_ini->r_string(profile, "fix_details"));
		if (m_ini->line_exist(profile, "fix_fences"))
			fix_fences(m_ini->r_string(profile, "fix_fences"));
	}
}
#endif // _CONSOLE

static float r_float_safe(const xray_re::xr_ini_file* ini, const char* section, const char* line, float def_value)
{
	if (ini->line_exist(section, line))
		return ini->r_float(section, line);
	else
		return def_value;
}

static bool r_bool_safe(const xray_re::xr_ini_file* ini, const char* section, const char* line, bool def_value)
{
	if (ini->line_exist(section, line))
		return ini->r_bool(section, line);
	else
		return def_value;
}

void fancy_tools::fix_details(const char* section) const
{
	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(PA_LEVEL, "level.details");
	if (r == 0) {
		msg("can't load %s", "level.details");
		return;
	}

	float min_scale_factor = r_float_safe(m_ini, section, "min_scale_factor", 1.0);
	float max_scale_factor = r_float_safe(m_ini, section, "max_scale_factor", 1.0);
	bool ignore_no_waving = r_bool_safe(m_ini, section, "ignore_no_waving", false);

	xr_memory_writer* w = new xr_memory_writer();

	unsigned chunk_id;
	for (xr_reader* s = 0; (s = r->open_chunk_next(chunk_id, s)) != 0;) {
		w->open_chunk(chunk_id);
		if (chunk_id == FSD_MESHES) {
			for (uint32_t id = 0; s->find_chunk(id); ++id) {
				w->open_chunk(id);
				xr_dm* dm = new xr_dm;
				dm->load_dm(*s);
				if (ignore_no_waving || (dm->flags() & DOF_NO_WAVING) == 0) {
					dm->min_scale() *= min_scale_factor;
					dm->max_scale() *= max_scale_factor;
				}
				dm->save_dm(*w);
				delete dm;
				w->close_chunk();
			}
		} else {
			w->w_raw(s->data(), s->size());
		}
		w->close_chunk();
	}
	fs.r_close(r);
	w->save_to(PA_FANCY_LEVEL, "level.details");
	delete w;
}

void fancy_tools::fix_fences(const char* section) const
{
	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(PA_LEVEL, "level");
	if (r == 0) {
		msg("can't load %s", "level");
		return;
	}

	xr_memory_writer* w = new xr_memory_writer();

	unsigned chunk_id;
	for (xr_reader* s = 0; (s = r->open_chunk_next(chunk_id, s)) != 0;) {
		w->open_chunk(chunk_id);
		if (chunk_id == FSL13_SHADERS) {
			size_t n = s->r_u32();
			w->w_size_u32(n);
			while (n--) {
				std::string raw, shader, base, lm;
				s->r_sz(raw);
				std::string::size_type slash = raw.find('/'), comma;
				if (slash == std::string::npos) {
					w->w_sz(raw);
					continue;
				}
				shader.assign(raw, 0, slash);
				comma = raw.find(',', ++slash);
				if (comma == std::string::npos) {
					base.assign(raw, slash, std::string::npos);
				} else {
					base.assign(raw, slash, comma - slash);
					lm.assign(raw, ++comma, std::string::npos);
				}
				if (shader.find("def_shaders\\def_aref") == std::string::npos &&
						m_ini->line_exist(section, base.c_str())) {
					msg("%s: %s -> %s%s", base.c_str(), shader.c_str(),
							"def_shaders\\def_aref", lm.empty() ? "_v" : "");
					shader = "def_shaders\\def_aref";
					if (lm.empty())
						shader += "_v";
				}
				raw.assign(shader).append(1, '/').append(base);
				if (!lm.empty())
					raw.append(1, ',').append(lm);
				w->w_sz(raw);
			}
		} else {
			w->w_raw(s->data(), s->size());
		}
		w->close_chunk();
	}
	fs.r_close(r);
	w->save_to(PA_FANCY_LEVEL, "level");
	delete w;
}
