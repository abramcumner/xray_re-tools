#include "xr_ai_version.h"
#include "xr_level_game.h"
#include "xr_level_graph.h"
#include "xr_level_spawn.h"
#include "xr_level_gct.h"
#include "xr_level_ai.h"
#include "xr_level_env_mod.h"
#include "xr_level_ps_static.h"
#include "xr_game_spawn.h"
#include "xr_game_graph.h"
#include "xr_utils.h"
#include "xr_entity_factory.h"
#include "xr_file_system.h"
#include "xr_string_utils.h"
#include "syncer.h"

using namespace xray_re;

void syncer::scan_levels()
{
	if (m_scan_done)
		return;

	msg("loading %s", "game_levels.ltx");
	if (!m_levels_ini.load(PA_GAME_CONFIG, "game_levels.ltx")) {
		msg("can't load game_levels.ltx");
		throw sync_error();
	}
	m_num_levels = m_levels_ini.line_count("levels");
	for (size_t i = 0; i != m_num_levels; ++i) {
		const char* section;
		m_levels_ini.r_line("levels", i, &section, 0);
		if (!m_levels_ini.line_exist(section, "name")) {
			msg("can't read level name in section %s", section);
			throw sync_error();
		}
	}
	m_scan_done = true;
}

const char* syncer::level_name(size_t level_idx) const
{
	const char* section;
	m_levels_ini.r_line("levels", level_idx, &section, 0);
	return m_levels_ini.r_string(section, "name");
}

static void check_or_create_fs_path(const char* path, bool& status, bool may_create = false)
{
	xr_file_system& fs = xr_file_system::instance();
	if (!fs.folder_exist(path, "")) {
		if (may_create && status) {
			if (fs.create_folder(path, ""))
				return;
			msg("can't create path %s", path);
		} else {
			msg("path %s does not exist", path);
		}
		status = false;
	}
}

void syncer::check_fs_paths() const
{
	bool status = true;
	check_or_create_fs_path(PA_GAME_DATA, status);
	check_or_create_fs_path(PA_GAME_CONFIG, status);
	check_or_create_fs_path(PA_GAME_SPAWN, status);
	check_or_create_fs_path(PA_GAME_LEVELS, status);
	check_or_create_fs_path(PA2215_FS_ROOT, status);
	check_or_create_fs_path(PA2215_GAME_DATA, status, true);
	check_or_create_fs_path(PA2215_GAME_CONFIG, status, true);
	check_or_create_fs_path(PA2215_GAME_SPAWN, status, true);
	check_or_create_fs_path(PA2215_GAME_LEVELS, status, true);
	if (!status)
		throw sync_error();
}

void syncer::load_ini(const char* name)
{
	msg("loading %s", name);
	if (!m_ini.load(name) && !m_ini.load(PA_SDK_ROOT, name))
		throw sync_error();
}

void syncer::load_links_ini(const char* name)
{
	msg("loading %s", name);
	if (!m_links_ini.load(name))
		throw sync_error();
}

void syncer::split_spawns(bool use_orig_gp) const
{
	xr_game_spawn gspawn;
	msg("loading %s\\%s", PA_GAME_SPAWN, "all.spawn");
	if (!gspawn.load(PA_GAME_SPAWN, "all.spawn"))
		throw sync_error();

	msg("loading %s\\%s", PA_GAME_DATA, "game.graph");
	if (!gspawn.graph().load(PA_GAME_DATA, "game.graph"))
		throw sync_error();

	xr_level_spawn* by_level_id[256];
	std::uninitialized_fill_n(by_level_id, xr_dim(by_level_id), static_cast<xr_level_spawn*>(0));

	xr_file_system& fs = xr_file_system::instance();

	msg("collecting %s", "graph points");
	for (const gg_level *it = gspawn.graph().levels(),
			*end = it + gspawn.graph().num_levels(); it != end; ++it) {
		const char* name = it->name.c_str();
		if (!fs.folder_exist(PA_GAME_LEVELS, name))
			continue;

		xr_level_spawn* spawn = by_level_id[it->level_id];
		if (spawn == 0)
			by_level_id[it->level_id] = spawn = new xr_level_spawn;

		fs.update_path(PA_LEVEL, PA_GAME_LEVELS, name);
#if 0
		msg("loading %s\\%s\\%s", PA_GAME_LEVELS, name, "level.spawn");
		if (!load_graph_points(*spawn, PA_LEVEL, "level.spawn"))
			throw sync_error();
#else
		bool make_aiw_bak = true;
		const char* level_spawn_name = "level.spawn";
		if (fs.file_exist(PA_LEVEL, "level.spawn.aiw_bak")) {
			level_spawn_name = "level.spawn.aiw_bak";
			make_aiw_bak = false;
		}
		if (make_aiw_bak) {
			msg("making %s\\%s\\%s", PA_GAME_LEVELS, name, "level.spawn.aiw_bak");
			if (!fs.copy_file(PA_LEVEL, "level.spawn", PA_LEVEL, "level.spawn.aiw_bak"))
				throw sync_error();
		}
		msg("loading %s\\%s\\%s", PA_GAME_LEVELS, name, level_spawn_name);
		if (!load_graph_points(*spawn, PA_LEVEL, level_spawn_name))
			throw sync_error();
#endif
	}

	msg("sorting %s", "spawns");
	if (!split_spawns(gspawn, by_level_id))
		throw sync_error();

	for (const gg_level *it = gspawn.graph().levels(),
			*end = it + gspawn.graph().num_levels(); it != end; ++it) {
		const char* name = it->name.c_str();
		if (!fs.folder_exist(PA_GAME_LEVELS, name))
			continue;
		for (const gg_level *it1 = gspawn.graph().levels(),
				*end1 = it1 + gspawn.graph().num_levels();
				it1 != end1; ++it1) {
			if (_stricmp(it1->name.c_str(), name) == 0) {
				xr_assert(it1->level_id < xr_dim(by_level_id));
				xr_level_spawn* spawn = by_level_id[it1->level_id];
				xr_assert(spawn);
				fs.update_path(PA_LEVEL, PA_GAME_LEVELS, name);
				msg("saving %s\\%s\\%s", PA_GAME_LEVELS, name, "level.spawn");
				if (!spawn->save(PA_LEVEL, "level.spawn"))
					throw sync_error();
				break;
			}
		}
	}
	delete_elements(by_level_id, xr_dim(by_level_id));
}

void syncer::split_paths() const
{
	xr_game_spawn gspawn;
	msg("loading %s\\%s", PA_GAME_SPAWN, "all.spawn");
	if (!gspawn.load(PA_GAME_SPAWN, "all.spawn"))
		throw sync_error();

	msg("loading %s\\%s", PA_GAME_DATA, "game.graph");
	if (!gspawn.load_graph(PA_GAME_DATA, "game.graph"))
		throw sync_error();

	xr_level_game* by_level_id[256];
	std::uninitialized_fill_n(by_level_id, xr_dim(by_level_id), static_cast<xr_level_game*>(0));

	xr_file_system& fs = xr_file_system::instance();

	msg("collecting %s", "$rpoints");
	for (const gg_level *it = gspawn.graph().levels(),
			*end = it + gspawn.graph().num_levels(); it != end; ++it) {
		const char* name = it->name.c_str();
		if (!fs.folder_exist(PA_GAME_LEVELS, name))
			continue;

		xr_level_game* game = by_level_id[it->level_id];
		if (game == 0)
			by_level_id[it->level_id] = game = new xr_level_game;

		fs.update_path(PA_LEVEL, PA_GAME_LEVELS, name);
#if 0
		msg("loading %s\\%s\\%s", PA_GAME_LEVELS, name, "level.game");
		if (!load_mp_rpoints(*game, PA_LEVEL, "level.game"))
			throw sync_error();
#else
		bool make_aiw_bak = true;
		const char* level_game_name = "level.game";
		if (fs.file_exist(PA_LEVEL, "level.game.aiw_bak")) {
			level_game_name = "level.game.aiw_bak";
			make_aiw_bak = false;
		}
		if (make_aiw_bak) {
			msg("making %s\\%s\\%s", PA_GAME_LEVELS, name, "level.game.aiw_bak");
			if (!fs.copy_file(PA_LEVEL, "level.game", PA_LEVEL, "level.game.aiw_bak"))
				throw sync_error();
		}
		msg("loading %s\\%s\\%s", PA_GAME_LEVELS, name, level_game_name);
		if (!load_mp_rpoints(*game, PA_LEVEL, level_game_name))
			throw sync_error();
#endif
	}

	msg("sorting %s", "paths");
	if (!split_paths(gspawn, by_level_id, m_ini))
		throw sync_error();

	for (const gg_level *it = gspawn.graph().levels(),
			*end = it + gspawn.graph().num_levels(); it != end; ++it) {
		const char* name = it->name.c_str();
		if (!fs.folder_exist(PA_GAME_LEVELS, name))
			continue;
		for (const gg_level *it1 = gspawn.graph().levels(),
				*end1 = it1 + gspawn.graph().num_levels();
				it1 != end1; ++it1) {
			if (_stricmp(it1->name.c_str(), name) == 0) {
				xr_assert(it1->level_id < xr_dim(by_level_id));
				xr_level_game* game = by_level_id[it1->level_id];
				xr_assert(game);
				fs.update_path(PA_LEVEL, PA_GAME_LEVELS, name);
				msg("saving %s\\%s\\%s", PA_GAME_LEVELS, name, "level.game");
				if (!game->save(PA_LEVEL, "level.game"))
					throw sync_error();
				break;
			}
		}
	}
	delete_elements(by_level_id, xr_dim(by_level_id));
}

void syncer::check_paths() const
{
	xr_game_spawn gspawn;
	msg("loading %s\\%s", PA_GAME_SPAWN, "all.spawn");
	if (!gspawn.load(PA_GAME_SPAWN, "all.spawn"))
		throw sync_error();

	check_paths(gspawn);
}

void syncer::split_graphs()
{
	msg("not yet implemented");
}

void syncer::dump_links(const char* path)
{
	scan_levels();

	xr_file_system& fs = xr_file_system::instance();
	xr_writer* w = fs.w_open(path);
	if (w == 0)
		throw sync_error();
	for (size_t i = 0; i != m_num_levels; ++i) {
		const char* name = level_name(i);
		if (!fs.folder_exist(PA_GAME_LEVELS, name))
			continue;
		fs.update_path(PA_LEVEL, PA_GAME_LEVELS, name);
		if (!fs.file_exist(PA_LEVEL, "level.spawn"))
			continue;
		xr_level_spawn spawn;
		msg("loading %s\\%s\\%s", PA_GAME_LEVELS, name, "level.spawn");
		if (!spawn.load(PA_LEVEL, "level.spawn"))
			throw sync_error();
		dump_links(spawn, *w, name);
	}
	fs.w_close(w);
}

void syncer::upgrade(int bld_ver)
{
#if 0
	if (0) {
		xr_game_spawn gspawn;
		msg("loading %s\\%s", PA_GAME_SPAWN, "all.spawn.orig");
		if (!gspawn.load(PA_GAME_SPAWN, "all.spawn.orig"))
			throw sync_error();
		msg("saving %s\\%s", PA_GAME_SPAWN, "all.spawn.out");
		if (!gspawn.save(PA_GAME_SPAWN, "all.spawn.out"))
			throw sync_error();
	} else {
		bool status = true;
		check_or_create_fs_path(PA9_FS_ROOT, status);
		check_or_create_fs_path(PA9_GAME_DATA, status);
		check_or_create_fs_path(PA9_GAME_CONFIG, status);
		check_or_create_fs_path(PA9_GAME_SPAWN, status);
		if (!status)
			throw sync_error();
		load_system_ini(PA9_GAME_CONFIG);
		xr_game_spawn gspawn;
		msg("loading %s\\%s", PA9_GAME_SPAWN, "marsh.spawn");
		if (!gspawn.load(PA9_GAME_SPAWN, "marsh.spawn"))
			throw sync_error();
		msg("saving %s\\%s", PA9_GAME_SPAWN, "marsh.spawn.out");
		if (!gspawn.save(PA9_GAME_SPAWN, "marsh.spawn.out"))
			throw sync_error();
	}
	return;
#endif

	uint32_t new_ai_version;
	if (bld_ver == 3120) {
		new_ai_version = AI_VERSION_9;
	} else if (bld_ver >= 3502) {
		new_ai_version = AI_VERSION_10;
	} else {
		msg("unsupported version");
		throw sync_error();
	}

	bool status = true;
	check_or_create_fs_path(PA9_FS_ROOT, status);
	check_or_create_fs_path(PA9_GAME_DATA, status, true);
	check_or_create_fs_path(PA9_GAME_SPAWN, status, true);
	check_or_create_fs_path(PA9_GAME_LEVELS, status, true);
	
	check_or_create_fs_path(PA10_FS_ROOT, status);
	check_or_create_fs_path(PA10_GAME_DATA, status, true);
	check_or_create_fs_path(PA10_GAME_SPAWN, status, true);
	check_or_create_fs_path(PA10_GAME_LEVELS, status, true);

	if (!status)
		throw sync_error();

	xr_game_spawn gspawn;
	msg("loading %s\\%s", PA_GAME_SPAWN, "all.spawn");
	if (!gspawn.load(PA_GAME_SPAWN, "all.spawn"))
		throw sync_error();
	msg("loading %s\\%s", PA_GAME_DATA, "game.graph");
	if (!gspawn.load_graph(PA_GAME_DATA, "game.graph"))
		throw sync_error();
	gspawn.graph().version() = gspawn.version() = new_ai_version;

	if (bld_ver >= 3502)
		msg("upgrading spawn version");
	char *PA_UPG_GAME_SPAWN = NULL;
	char *PA_UPG_GAME_LEVELS = NULL;
	char *PA_UPG_LEVEL = NULL;

	if (bld_ver >= 3870) {
		load_system_ini(PA10_GAME_CONFIG);
		PA_UPG_GAME_LEVELS = (char *)PA10_GAME_LEVELS;
		PA_UPG_LEVEL = (char *)PA10_LEVEL;
		PA_UPG_GAME_SPAWN = (char *)PA10_GAME_SPAWN;
		msg("upgrading spawn version to CoP format");
		xr_entity_vec* gs = new xr_entity_vec;
		int i = 0;
		for (xr_entity_vec_it it = gspawn.spawns().begin(),
				end = gspawn.spawns().end(); it != end; ++it) {
			cse_abstract* entity = *it;

			if (get_entity_clsid(entity->name()) != &entity->clsid())
			{
				cse_abstract* new_entity = create_entity(entity->name());
				xr_packet packet;

				entity->spawn_write(packet, true);
				new_entity->spawn_read(packet);

				packet.clear();
				entity->update_write(packet);
				new_entity->update_read(packet);

				new_entity->version() = CSE_VERSION_COP;
				new_entity->script_version() = 12;
				new_entity->spawn_id() = i++;
				
				gs->push_back(new_entity);
			}
			else
			{
				entity->version() = CSE_VERSION_COP;
				entity->script_version() = 12;
				entity->spawn_id() = i++;

				gs->push_back(entity);
			}
		}
		gspawn.spawns() = *gs;
	}
	else if (bld_ver >= 3502) {
		load_system_ini(PA9_GAME_CONFIG);
		PA_UPG_GAME_LEVELS = (char *)PA9_GAME_LEVELS;
		PA_UPG_LEVEL = (char *)PA9_LEVEL;
		PA_UPG_GAME_SPAWN = (char *)PA9_GAME_SPAWN;
		msg("upgrading spawn version to CS format");
		xr_entity_vec* gs = new xr_entity_vec;
		int i = 0;
		for (xr_entity_vec_it it = gspawn.spawns().begin(),
				end = gspawn.spawns().end(); it != end; ++it) {
			cse_abstract* entity = *it;
			entity->version() = CSE_VERSION_CS;
			entity->script_version() = 8;
		}
	}

	xr_file_system& fs = xr_file_system::instance();
	for (const gg_level *it = gspawn.graph().levels(),
			*end = it + gspawn.graph().num_levels(); it != end; ++it) {
		const char* name = it->name.c_str();

		if (!fs.folder_exist(PA_GAME_LEVELS, name))
			continue;

		if (!fs.folder_exist(PA_UPG_GAME_LEVELS, name)) {
			msg("creating %s\\%s\\", PA_UPG_GAME_LEVELS, name);
			if (!fs.create_folder(PA_UPG_GAME_LEVELS, name))
				throw sync_error();
		}

		fs.update_path(PA_LEVEL, PA_GAME_LEVELS, name);
		fs.update_path(PA_UPG_LEVEL, PA_UPG_GAME_LEVELS, name);

		xr_level_ai ai;
		msg("loading %s\\%s\\%s", PA_GAME_LEVELS, name, "level.ai");
		if (!ai.load(PA_LEVEL, "level.ai"))
			throw sync_error();
		ai.version() = new_ai_version;
		msg("saving %s\\%s\\%s", PA_UPG_GAME_LEVELS, name, "level.ai");
		if (!ai.save(PA_UPG_LEVEL, "level.ai"))
			throw sync_error();

		xr_level_gct gct;
		msg("loading %s\\%s\\%s", PA_GAME_LEVELS, name, "level.gct");
		if (!gct.load(PA_LEVEL, "level.gct"))
			throw sync_error();
		msg("embedding cross table");
		if (!import_cross_table(gspawn, gct))
			throw sync_error();

		if (bld_ver >= 3456) {
			xr_level_env_mod env_mod;
			msg("loading %s\\%s\\%s", PA_GAME_LEVELS, name, "level.env_mod");
			if (env_mod.load(PA_LEVEL, "level.env_mod")) {
				env_mod.version() = ENV_MOD_VERSION_23;
				msg("saving %s\\%s\\%s", PA_UPG_GAME_LEVELS, name, "level.env_mod");
				if (!env_mod.save(PA_UPG_LEVEL, "level.env_mod"))
					throw sync_error();
			}

			xr_level_ps_static ps_static;
			msg("loading %s\\%s\\%s", PA_GAME_LEVELS, name, "level.ps_static");
			if (ps_static.load(PA_LEVEL, "level.ps_static")) {
				ps_static.version() = PS_VERSION_1;
				msg("saving %s\\%s\\%s", PA_UPG_GAME_LEVELS, name, "level.ps_static");
				if (!ps_static.save(PA_UPG_LEVEL, "level.ps_static"))
					throw sync_error();
			}
		}
	}
//	msg("moving actor to military");
//	move_actor(gspawn.spawns(), -334.578491210938f, -25.5103607177734f, 45.0102348327637f, 16231, 1546);
	msg("saving %s\\%s", PA_UPG_GAME_SPAWN, "all.spawn");
	if (!gspawn.save(PA_UPG_GAME_SPAWN, "all.spawn"))
		throw sync_error();
}
void syncer::sync_build_aimap()
{
	if (up_to_date(m_src_level, "build.aimap", m_tgt_level)) {
		msg("%s\\%s is up to date", m_tgt_level_path, "build.aimap");
	} else {
		msg("updating %s\\%s", m_tgt_level_path, "build.aimap");
		xr_file_system& fs = xr_file_system::instance();
		if (!fs.copy_file(m_src_level, "build.aimap", m_tgt_level))
			throw sync_error();
	}
}

void syncer::sync_build_cform()
{
	if (up_to_date(m_src_level, "build.cform", m_tgt_level)) {
		msg("%s\\%s is up to date", m_tgt_level_path, "build.cform");
	} else {
		msg("updating %s\\%s", m_tgt_level_path, "build.cform");
		xr_file_system& fs = xr_file_system::instance();
		if (!fs.copy_file(m_src_level, "build.cform", m_tgt_level))
			throw sync_error();
	}
}

void syncer::sync_build_prj()
{
	if (up_to_date(m_src_level, "build.prj", m_tgt_level)) {
		msg("%s\\%s is up to date", m_tgt_level_path, "build.prj");
	} else {
		msg("updating %s\\%s", m_tgt_level_path, "build.prj");
		xr_file_system& fs = xr_file_system::instance();
		if (!fs.copy_file(m_src_level, "build.prj", m_tgt_level))
			throw sync_error();
	}
}

void syncer::sync_level_ai()
{
	if (up_to_date(m_src_level, "level.ai", m_tgt_level)) {
		msg("%s\\%s is up to date", m_tgt_level_path, "level.ai");
	} else {
		msg("updating %s\\%s", m_tgt_level_path, "level.ai");
		xr_file_system& fs = xr_file_system::instance();
		if (!fs.copy_file(m_src_level, "level.ai", m_tgt_level))
			throw sync_error();
	}
}

void syncer::sync_level_gct()
{
	if (up_to_date(m_src_level, "level.gct", m_tgt_level)) {
		msg("%s\\%s is up to date", m_tgt_level_path, "level.gct");
	} else {
		msg("updating %s\\%s", m_tgt_level_path, "level.gct");
		xr_file_system& fs = xr_file_system::instance();
		fs.copy_file(m_src_level, "level.gct", m_tgt_level);
	}
}

void syncer::sync_level_gct_raw()
{
	xr_file_system& fs = xr_file_system::instance();
	bool rebuild = !m_xrai_compat && !fs.file_exist(m_src_level, "level.gct.raw");
	if (up_to_date(m_src_level, rebuild ? "level.gct" : "level.gct.raw", m_tgt_level, "level.gct.raw")) {
		msg("%s\\%s is up to date", m_tgt_level_path, "level.gct.raw");
		return;
	}
	if (rebuild) {
		msg("rebuilding %s\\%s", m_tgt_level_path, "level.gct.raw");
		xr_level_gct gct;
		if (!gct.load(m_src_level, "level.gct"))
			throw sync_error();
		to_raw(gct);
		if (!gct.save(m_tgt_level, "level.gct.raw"))
			throw sync_error();
	} else {
		msg("updating %s\\%s", m_tgt_level_path, "level.gct.raw");
		fs.copy_file(m_src_level, "level.gct.raw", m_tgt_level);
	}
}

void syncer::sync_level_graph()
{
	if (up_to_date(m_src_level, "level.graph", m_tgt_level)) {
		msg("%s\\%s is up to date", m_tgt_level_path, "level.graph");
	} else {
		msg("updating %s\\%s", m_tgt_level_path, "level.graph");
		xr_level_graph graph;
		if (!graph.load(m_src_level, "level.graph"))
			throw sync_error();
		graph.version() = m_xrai_compat ? AI_VERSION_8 : AI_VERSION_2215;
		if (!graph.save(m_tgt_level, "level.graph"))
			throw sync_error();
	}
}

void syncer::sync_level_spawn(const char* name)
{
	xr_assert(!m_xrai_compat);
	if (m_links_ini.empty() && up_to_date(PA_LEVEL, "level.spawn", PA2215_LEVEL)) {
		msg("%s\\%s is up to date", m_tgt_level_path, "level.spawn");
	} else {
		msg("updating %s\\%s", m_tgt_level_path, "level.spawn");
		xr_level_spawn spawn;
		if (!spawn.load(PA_LEVEL, "level.spawn"))
			throw sync_error();
		edit_links(spawn, m_links_ini, name);
		to_xrai_compat(spawn, m_ini, "entity_compat");
		if (!spawn.save(PA2215_LEVEL, "level.spawn"))
			throw sync_error();
	}
}

void syncer::sync_level_sectors_ai()
{
	if (up_to_date(m_src_level, "level_sectors.ai", m_tgt_level)) {
		msg("%s\\%s is up to date", m_tgt_level_path, "level_sectors.ai");
	} else {
		msg("updating %s\\%s", m_tgt_level_path, "level_sectors.ai");
		xr_file_system& fs = xr_file_system::instance();
		fs.copy_file(m_src_level, "level_sectors.ai", m_tgt_level);
	}
}

void syncer::sync_game_ltx(const char* name)
{
	xr_assert(!m_xrai_compat);
	if (up_to_date(PA_GAME_CONFIG, name, PA2215_GAME_CONFIG)) {
		msg("%s\\%s is up to date", PA2215_GAME_CONFIG, name);
	} else {
		msg("updating %s\\%s", PA2215_GAME_CONFIG, name);
		xr_file_system& fs = xr_file_system::instance();
		fs.copy_file(PA_GAME_CONFIG, name, PA2215_GAME_CONFIG);
	}
}

void syncer::sync_game_graph()
{
	if (up_to_date(m_src_game_data, "game.graph", m_tgt_game_data)) {
		msg("%s\\%s is up to date", m_tgt_game_data, "game.graph");
	} else {
		msg("updating %s\\%s", m_tgt_game_data, "game.graph");
		xr_game_graph graph;
		if (!graph.load(m_src_game_data, "game.graph"))
			throw sync_error();
		graph.version() = m_xrai_compat ? AI_VERSION_8 : AI_VERSION_2215;
		if (!graph.save(m_tgt_game_data, "game.graph"))
			throw sync_error();
	}
}

void syncer::sync_game_spawn()
{
	xr_assert(m_xrai_compat);
	m_actor_level = 0;
	xr_game_spawn gspawn;
	xr_file_system& fs = xr_file_system::instance();
	for (size_t i = 0; i != m_num_levels; ++i) {
		const char* name = level_name(i);
		if (!fs.folder_exist(PA_GAME_LEVELS, name))
			continue;

		fs.update_path(PA_LEVEL, PA_GAME_LEVELS, name);
		if (!fs.file_exist(PA_LEVEL, "level.spawn") ||
				!fs.file_exist(PA_LEVEL, "level.game")) {
			continue;
		}

		update_tgt_level_path(name);

		xr_level_spawn spawn;
		msg("loading %s\\%s", m_tgt_level_path, "level.spawn");
		if (!spawn.load(PA_LEVEL, "level.spawn"))
			throw sync_error();

		xr_level_game game;
		msg("loading %s\\%s", m_tgt_level_path, "level.game");
		if (!game.load(PA_LEVEL, "level.game"))
			throw sync_error();

		xr_level_ai ai;
		msg("loading %s\\%s", m_tgt_level_path, "level.ai");
		if (!ai.load(PA_LEVEL, "level.ai"))
			throw sync_error();

		xr_level_gct gct;
		msg("loading %s\\%s", m_tgt_level_path, "level.gct");
		if (!gct.load(PA_LEVEL, "level.gct"))
			throw sync_error();

		msg("adding %s to game spawn", name);
		if (!add_level(gspawn, name, spawn, game, ai, gct))
			throw sync_error();
	}
	if (m_actor_level == 0) {
		msg("can't find %s", "actor");
		throw sync_error();
	}
	char gs2215[256];
	xr_snprintf(gs2215, sizeof(gs2215), "%s.spawn", m_actor_level);
	if (up_to_date(PA2215_GAME_SPAWN, gs2215, PA_GAME_SPAWN, "all.spawn")) {
		msg("%s\\%s is up to date", PA_GAME_SPAWN, "all.spawn");
		return;
	}
	msg("merging xrAI-generated %s\\%s", PA2215_GAME_SPAWN, gs2215);
	if (!merge_xrai_compat(gspawn, PA2215_GAME_SPAWN, gs2215))
		throw sync_error();

	msg("saving %s\\%s", PA_GAME_DATA, "all.spawn");
	gspawn.graph().version() = gspawn.version() = AI_VERSION_8;
	if (!gspawn.save(PA_GAME_SPAWN, "all.spawn"))
		throw sync_error();
}

bool syncer::up_to_date(const char* src_path, const char* src_name,
		const char* tgt_path, const char* tgt_name)
{
	if (m_flags & SYNC_FORCE)
		return false;
	if (tgt_name == 0)
		tgt_name = src_name;
	xr_file_system& fs = xr_file_system::instance();
	uint32_t src_age = fs.file_age(src_path, src_name);
	uint32_t tgt_age = fs.file_age(tgt_path, tgt_name);
	return src_age <= tgt_age;
}

void syncer::set_target(sync_target target, unsigned flags)
{
	m_flags = flags;
	if (target == SYNC_TARGET_FINAL) {
		m_xrai_compat = true;
		m_src_game_data = PA2215_GAME_DATA;
		m_src_game_spawn = PA2215_GAME_SPAWN;
		m_src_game_levels = PA2215_GAME_LEVELS;
		m_src_level = PA2215_LEVEL;
		m_tgt_game_data = PA_GAME_DATA;
		m_tgt_game_spawn = PA_GAME_SPAWN;
		m_tgt_game_levels = PA_GAME_LEVELS;
		m_tgt_level = PA_LEVEL;
	} else {
		m_xrai_compat = false;
		m_src_game_data = PA_GAME_DATA;
		m_src_game_spawn = PA_GAME_SPAWN;
		m_src_game_levels = PA_GAME_LEVELS;
		m_src_level = PA_LEVEL;
		m_tgt_game_data = PA2215_GAME_DATA;
		m_tgt_game_spawn = PA2215_GAME_SPAWN;
		m_tgt_game_levels = PA2215_GAME_LEVELS;
		m_tgt_level = PA2215_LEVEL;
	}
}

void syncer::update_tgt_level_path(const char* name)
{
	xr_snprintf(m_tgt_level_path, sizeof(m_tgt_level_path),
			"%s\\%s", m_tgt_game_levels, name);
}

void syncer::do_sync(const char* name)
{
	xr_file_system& fs = xr_file_system::instance();
	if (!fs.folder_exist(m_src_game_levels, name))
		return;
	if (!fs.folder_exist(m_tgt_game_levels, name)) {
		msg("creating %s\\%s\\", m_tgt_game_levels, name);
		if (!fs.create_folder(m_tgt_game_levels, name))
			throw sync_error();
	}
	update_tgt_level_path(name);
	fs.update_path(m_src_level, m_src_game_levels, name);
	fs.update_path(m_tgt_level, m_tgt_game_levels, name);
	if (m_flags & SYNC_BUILD_AIMAP)
		sync_build_aimap();
	if (m_flags & SYNC_BUILD_CFORM)
		sync_build_cform();
	if (m_flags & SYNC_BUILD_PRJ)
		sync_build_prj();
	if (m_flags & SYNC_LEVEL_AI)
		sync_level_ai();
	if (m_flags & SYNC_LEVEL_GCT)
		sync_level_gct();
	if (m_flags & SYNC_LEVEL_GCT_RAW)
		sync_level_gct_raw();
	if (m_flags & SYNC_LEVEL_GRAPH)
		sync_level_graph();
	if (m_flags & SYNC_LEVEL_SPAWN)
		sync_level_spawn(name);
	if (m_flags & SYNC_LEVEL_SECTORS_AI)
		sync_level_sectors_ai();
}

void syncer::do_sync()
{
	if (m_flags & SYNC_GAME_GRAPH)
		sync_game_graph();
	if (m_flags & SYNC_GAME_SPAWN)
		sync_game_spawn();
	if (m_flags & SYNC_LEVEL) {
		scan_levels();
		for (size_t i = 0; i != m_num_levels; ++i)
			do_sync(level_name(i));
		if (m_flags & SYNC_GAME_LTX) {
			sync_game_ltx("game_graphs.ltx");
			sync_game_ltx("game_levels.ltx");
			sync_game_ltx("game_story_ids.ltx");
		}
	}
}

void syncer::to_xrai(unsigned flags)
{
	set_target(SYNC_TARGET_XRAI, flags|SYNC_GAME_LTX);
	do_sync();
}

void syncer::from_xrai(unsigned flags)
{
	set_target(SYNC_TARGET_FINAL, flags);
	do_sync();
}

void syncer::to_xrai(const char* name, unsigned flags)
{
	set_target(SYNC_TARGET_XRAI, flags|SYNC_GAME_LTX);
	do_sync(name);
}

void syncer::from_xrai(const char* name, unsigned flags)
{
	set_target(SYNC_TARGET_FINAL, flags);
	do_sync(name);
}
