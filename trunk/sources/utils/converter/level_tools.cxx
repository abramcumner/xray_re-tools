#include <ctime>
#include <cctype>
#include <algorithm>
#include "level_tools.h"
#include "xr_level.h"
#include "xr_level_ltx.h"
#include "xr_level_shaders.h"
#include "xr_level_sectors.h"
#include "xr_level_visuals.h"
#include "xr_scene.h"
#include "xr_shaders_xrlc_lib.h"
#include "xr_ini_file.h"
#include "xr_object.h"
#include "xr_file_system.h"
#include "xr_entity_factory.h"

using namespace xray_re;

const char CONVERTER_INI[] = "converter.ini";

level_tools::level_tools():
	m_ini(0),
	m_shaders_xrlc_lib(0),
	m_level(0), m_scene(0) {}

level_tools::~level_tools()
{
	delete m_ini;
	delete m_shaders_xrlc_lib;
	delete m_scene;
	delete m_level;
}

bool level_tools::check_paths() const
{
	bool status = true;
	check_path(PA_SDK_ROOT, status);
	check_path(PA_GAME_DATA, status);
	check_path(PA_GAME_CONFIG, status);
	check_path(PA_GAME_TEXTURES, status);
	check_path(PA_MAPS, status);
	check_path(PA_OBJECTS, status);
	return status;
}

static bool check_scene_name(const char* p)
{
	for (int c; (c = *p) != '\0'; ++p) {
		if (!isalnum(c) && c != '_')
			return false;
	}
	return true;
}

void level_tools::process(const cl_parser& cl)
{
	if (!check_paths())
		return;

#if 0
	{
		xr_scene* scene = new xr_scene;
		if (!scene->load(cl.parameter(0)))
			throw xr_error();
		if (!scene->save("aaaa"))
			throw xr_error();
		delete scene;
		return;
	}
#endif

	const char* rmode = 0;
	if (!cl.get_string("-mode", rmode)) {
		m_rmode = RM_MAYA;
	} else if (std::strcmp(rmode, "le") == 0) {
		m_rmode = RM_LE;
	} else if (std::strcmp(rmode, "le2") == 0) {
		m_rmode = RM_LE2;
	} else if (std::strcmp(rmode, "maya") == 0) {
		m_rmode = RM_MAYA;
	} else if (std::strcmp(rmode, "raw") == 0) {
		m_rmode = RM_RAW;
	} else {
		msg("invalid mode");
		return;
	}

	m_rflags = 0;
	if (cl.get_bool("-with_lods"))
		m_rflags |= RF_WITH_LODS;
	if (cl.get_bool("-use_mt"))
		m_rflags |= RF_USE_MT;
	if (cl.get_bool("-dbgcfrm"))
		m_rflags |= RF_DEBUG_CFORM;
	if (cl.get_bool("-dbgmrg"))
		m_rflags |= RF_DEBUG_MERGE;

	const char* scene_name = 0;
	if (cl.get_string("-out", scene_name)) {
		if (cl.num_params() > 1) {
			msg("can't set scene name explicitly for multiple input levels");
			return;
		}
		if (!check_scene_name(scene_name)) {
			msg("invalid scene name");
			return;
		}
	}

	m_ini = new xr_ini_file(CONVERTER_INI);
	if (m_ini->empty() && !m_ini->load(PA_SDK_ROOT, CONVERTER_INI)) {
		msg("can't load %s", CONVERTER_INI);
		return;
	}
	// cache profile-independent settings
	m_debug_texture = m_ini->r_string("settings", "debug_texture");
	m_fake_gamemtl = m_ini->r_string("settings", "fake_gamemtl");
	m_ladders_gamemtl = m_ini->r_string("settings", "ladders_gamemtl");
	m_ghost_eshader = m_ini->r_string("settings", "ghost_eshader");

	m_shaders_xrlc_lib = new xr_shaders_xrlc_lib;
	if (!m_shaders_xrlc_lib->load(PA_GAME_DATA, "shaders_xrlc.xr")) {
		msg("can't load %s", "shaders_xrlc.xr");
		return;
	}

	for (size_t i = 0, num_params = cl.num_params(); i != num_params; ++i) {
		const char* name = cl.param(i);
		if (const char* p = std::strchr(name, ':')) {
			m_sect_profile = m_ini->r_string("profiles", std::string(name, p - name).c_str());
			name = p + 1;
		} else {
			m_sect_profile = m_ini->r_string("profiles", "default");
		}
		msg("level name: %s", name);
		xr_file_system& fs = xr_file_system::instance();
		if (m_ini->line_exist(m_sect_profile, PA_GAME_DATA))
			fs.update_path("$temp_game_data$", m_ini->r_string(m_sect_profile, PA_GAME_DATA), "");
		fs.update_path("$temp_level$", m_ini->r_string(m_sect_profile, PA_GAME_LEVELS), name);
		
		//load system ini from build for create spawn entity correctly
		//load_system_ini("$temp_game_data$");

		m_level = new xr_level;
		if (m_level->load("$temp_game_data$", "$temp_level$"))
			reconstruct_scene(name, scene_name);
		else
			msg("can't load %s", name);
		delete m_level;
	}
	m_level = 0;
}

static void build_unique(const std::vector<std::string>* data, std::vector<uint16_t>& uniq_refs)
{
	// FIXME: O(N^2)
	uniq_refs.resize(data->size());
	std::vector<std::string>::const_iterator it0 = data->begin(), it;
	for (size_t i = uniq_refs.size() - 1; i != 0; --i) {
		it = std::find(it0, it0 + i, (*data)[i]);
		uniq_refs[i] = uint16_t((it - it0) & UINT16_MAX);
	}
}

static uint16_t add_unique(std::vector<std::string>* data, std::vector<uint16_t>& uniq_refs,
		const std::string& new_data, size_t hint)
{
	std::vector<std::string>::iterator it0 = data->begin();
	for (std::vector<std::string>::iterator it = it0 + hint,
			end = data->end(); it != end; ++it) {
		if (*it == new_data)
			return uint16_t((it - it0) & UINT16_MAX);
	}
	for (std::vector<std::string>::iterator it = it0,
			end = it0 + hint; it != end; ++it) {
		if (*it == new_data)
			return uint16_t((it - it0) & UINT16_MAX);
	}
	uint16_t idx = uint16_t(data->size() & UINT16_MAX);
	data->push_back(new_data);
	uniq_refs.push_back(idx);
	return idx;
}

uint16_t level_tools::add_texture(const std::string& texture, size_t hint)
{
	return add_unique(m_textures, m_uniq_textures, texture, hint);
}

uint16_t level_tools::add_shader(const std::string& shader, size_t hint)
{
	return add_unique(m_shaders, m_uniq_shaders, shader, hint);
}

void level_tools::reconstruct_scene(const char* level_name, const char* scene_name)
{
	if (scene_name) {
		m_scene_name = scene_name;
	} else if (strncmp(level_name, "testers_", 8) == 0) {
		m_scene_name = level_name + 8;
	} else if (m_ini->line_exist("scene_names", level_name)) {
		m_scene_name = m_ini->r_string("scene_names", level_name);
	} else {
		const char* p;
		if ((p = std::strchr(level_name, '_')) > level_name + 2 && p[1] != '\0' &&
				isdigit(level_name[1]) && isdigit(level_name[2])) {
			m_scene_name = p + 1;
		} else {
			m_scene_name = level_name;
		}
	}
	msg("scene name: %s", m_scene_name);
	m_scene = new xr_scene();
	m_scene->name() = m_level_name = level_name;
	m_scene->name_prefix() = level_name;
	m_scene->set_quality(m_level->xrlc_quality());
	if (const xr_level_ltx* ltx = m_level->ltx()) {
		if (m_level->xrlc_version() >= XRLC_VERSION_12) {
			xr_assert(ltx->data().size() < 4*1024);
			m_scene->custom_data() = ltx->data();
		}
		m_level->clear_ltx();
	}

	// refresh profile-dependent settings
	if (m_ini->line_exist(m_sect_profile, "wallmark_shaders"))
		m_sect_wallmark_shaders = m_ini->r_string(m_sect_profile, "wallmark_shaders");
	else
		m_sect_wallmark_shaders = 0;
	if (m_ini->line_exist(m_sect_profile, "auto_gamemtls"))
		m_sect_auto_gamemtls = m_ini->r_string(m_sect_profile, "auto_gamemtls");
	else
		m_sect_auto_gamemtls = 0;

	// init some defaults
	m_default_sector_idx = 0;

	// and setup shortcuts
	m_subdivisions = &m_level->visuals()->ogfs();
	m_sectors = &m_level->sectors()->sectors();
	m_shaders = &m_level->shaders()->shaders();
	m_textures = &m_level->shaders()->textures();
	m_gamemtls_lib = m_level->gamemtls_lib();

	msg("substituting shaders/textures");
	do_substs(m_shaders, "shader_substs");
	do_substs(m_textures, "texture_substs");

	msg("re-indexing shaders/textures");
	build_unique(m_shaders, m_uniq_shaders);
	build_unique(m_textures, m_uniq_textures);

	xr_file_system& fs = xr_file_system::instance();
	if (!fs.folder_exist(PA_OBJECTS, "levels"))
		fs.create_folder(PA_OBJECTS, "levels");
	m_objects_ref = "levels";
	m_objects_ref += '\\';
	m_objects_ref += m_scene_name;
	if (!fs.folder_exist(PA_OBJECTS, m_objects_ref.c_str()))
		fs.create_folder(PA_OBJECTS, m_objects_ref.c_str());
	m_objects_ref += '\\';
	m_objects_ref += m_scene_name;
	m_objects_ref += "_";

	if ((m_rflags & RF_DEBUG_CFORM) != 0)
		build_debug_cform();

	if (m_rmode == RM_MAYA) {
		reconstruct_visuals();
	} else {
		if (m_ini->line_exist(m_sect_profile, "spawn_version") == false)
			m_spawn_version = CSE_VERSION_SOC;
		else
		{
			std::string& version = std::string(m_ini->r_string(m_sect_profile, "spawn_version"));
			xr_strlwr(version);
			if (version == "soc")
				m_spawn_version = CSE_VERSION_SOC;
			else if (version == "cs")
				m_spawn_version = CSE_VERSION_CS;
			else if (version == "cop")
				m_spawn_version = CSE_VERSION_COP;
		}

		reconstruct_details();
		reconstruct_sectors();
		reconstruct_portals();
		reconstruct_visuals();
		reconstruct_hom();
		reconstruct_som();
		reconstruct_ai_map();
		reconstruct_glows();
		reconstruct_lights();
		reconstruct_particles();
		reconstruct_sound_envs();
		reconstruct_sound_srcs();
		reconstruct_wallmarks();
		reconstruct_ways();
		reconstruct_spawns();
		
		msg("saving %s", "scene");
		if (m_spawn_version == CSE_VERSION_SOC)
		m_scene->save(m_scene_name);
		else
			m_scene->save_v12(m_scene_name);
	}

	delete m_scene;
	m_scene = 0;
}

void level_tools::do_substs(std::vector<std::string>* data, const char* line)
{
	if (m_ini->line_exist(m_sect_profile, line)) {
		const char* section = m_ini->r_string(m_sect_profile, line);
		for (std::vector<std::string>::iterator it = data->begin(),
				end = data->end(); it != end; ++it) {
			if (m_ini->line_exist(section, it->c_str()))
				*it = m_ini->r_string(section, it->c_str());
		}
	}
}

void level_tools::make_object_ref(std::string& reference, const char* name) const
{
	reference.assign(m_objects_ref).append(name);
}

void level_tools::save_object(const xr_object* object, const char* name) const
{
	std::string reference;
	make_object_ref(reference, name);
	object->save_object(PA_OBJECTS, reference.append(".object").c_str());
}
