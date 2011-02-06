#ifndef __GNUC__
#pragma once
#endif
#ifndef __SYNCER_H__
#define __SYNCER_H__

#include "xr_ini_file.h"

enum {
	SYNC_BUILD_AIMAP	= 0x00000001,
	SYNC_BUILD_CFORM	= 0x00000002,
	SYNC_BUILD_PRJ		= 0x00000004,
	SYNC_LEVEL_AI		= 0x00000008,
	SYNC_LEVEL_GCT		= 0x00000010,
	SYNC_LEVEL_GCT_RAW	= 0x00000020,
	SYNC_LEVEL_GRAPH	= 0x00000040,
	SYNC_LEVEL_SPAWN	= 0x00000080,
	SYNC_LEVEL_SECTORS_AI	= 0x00000100,

	SYNC_GAME_LTX		= 0x00000200,
	SYNC_GAME_GRAPH		= 0x00000400,
	SYNC_GAME_SPAWN		= 0x00000800,

	SYNC_LEVEL		= 0x000001ff,
	SYNC_GAME		= 0x00000e00,
	SYNC_ALL		= SYNC_LEVEL|SYNC_GAME,

	SYNC_FORCE		= 0x80000000,
};

const char PA2215_FS_ROOT[] = "$2215_fs_root$";
const char PA2215_GAME_DATA[] = "$2215_game_data$";
const char PA2215_GAME_CONFIG[] = "$2215_game_config$";
const char PA2215_GAME_SPAWN[] = "$2215_game_spawn$";
const char PA2215_GAME_LEVELS[] = "$2215_game_levels$";
const char PA2215_LEVEL[] = "$2215_level$";

const char PA9_FS_ROOT[] = "$cs_fs_root$";
const char PA9_GAME_DATA[] = "$cs_game_data$";
const char PA9_GAME_CONFIG[] = "$cs_game_config$";
const char PA9_GAME_SPAWN[] = "$cs_game_spawn$";
const char PA9_GAME_LEVELS[] = "$cs_game_levels$";
const char PA9_LEVEL[] = "$cs_level$";

const char PA10_FS_ROOT[] = "$cop_fs_root$";
const char PA10_GAME_DATA[] = "$cop_game_data$";
const char PA10_GAME_CONFIG[] = "$cop_game_config$";
const char PA10_GAME_SPAWN[] = "$cop_game_spawn$";
const char PA10_GAME_LEVELS[] = "$cop_game_levels$";
const char PA10_LEVEL[] = "$cop_level$";

namespace xray_re {
	class cse_abstract;
	class xr_writer;
	class xr_level_ai;
	class xr_level_gct;
	class xr_level_game;
	class xr_level_spawn;
	class xr_game_spawn;
};

class syncer {
public:
			syncer();

	void		check_fs_paths() const;

	void		load_ini(const char* path);
	void		load_links_ini(const char* path);

	void		split_spawns(bool use_orig_gp) const;
	void		split_paths() const;
	void		check_paths() const;
	void		split_graphs();
	void		dump_links(const char* path);
	void		upgrade(int bld_ver);

	void		to_xrai(unsigned flags);
	void		to_xrai(const char* name, unsigned flags);
	void		from_xrai(unsigned flags);
	void		from_xrai(const char* name, unsigned flags);

	class sync_error {};

private:
	enum sync_target {
		SYNC_TARGET_XRAI	= 0,
		SYNC_TARGET_FINAL	= 1,
	};

	void		scan_levels();

	void		sync_build_aimap();
	void		sync_build_cform();
	void		sync_build_prj();
	void		sync_level_ai();
	void		sync_level_gct();
	void		sync_level_gct_raw();
	void		sync_level_graph();
	void		sync_level_spawn(const char* name);
	void		sync_level_sectors_ai();
	void		sync_game_ltx(const char* name);
	void		sync_game_graph();
	void		sync_game_spawn();

	void		set_target(sync_target target, unsigned flags);

	void		do_sync(const char* name);
	void		do_sync();

	bool		up_to_date(const char* src_path, const char* src_name,
					const char* tgt_path, const char* tgt_name = 0);

	const char*	level_name(size_t level_idx) const;

	void		update_tgt_level_path(const char* name);

	// level.game utils
	bool		load_mp_rpoints(xray_re::xr_level_game& game,
					const char* path, const char* name) const;

	// graph cross table utils
	void		to_raw(xray_re::xr_level_gct& gct) const;

	// level spawn utils
	void		to_xrai_compat(xray_re::xr_level_spawn& spawn,
					const xray_re::xr_ini_file& ini, const char* section);
	void		edit_links(xray_re::xr_level_spawn& spawn,
					const xray_re::xr_ini_file& ini, const char* section);
	void		dump_links(xray_re::xr_level_spawn& spawn,
					xray_re::xr_writer& w, const char* section);
	bool		load_graph_points(xray_re::xr_level_spawn& spawn,
					const char* path, const char* name) const;

	// game spawn/game graph utils
	bool		add_level(xray_re::xr_game_spawn& gspawn, const char* name,
					xray_re::xr_level_spawn& spawn,
					const xray_re::xr_level_game& game,
					const xray_re::xr_level_ai& ai,
					const xray_re::xr_level_gct& gct);
	void		merge_spawns(std::vector<xray_re::cse_abstract*>& spawns,
					xray_re::xr_reader& r) const;
	bool		merge_xrai_compat(xray_re::xr_game_spawn& gspawn,
					const char* path, const char* name) const;
	bool		split_spawns(xray_re::xr_game_spawn& gspawn,
					xray_re::xr_level_spawn* by_level_id[]) const;
	bool		split_paths(xray_re::xr_game_spawn& gspawn,
					xray_re::xr_level_game* by_level_id[],
					const xray_re::xr_ini_file& ini) const;
	void		check_paths(xray_re::xr_game_spawn& gspawn) const;
	bool		import_cross_table(xray_re::xr_game_spawn& gspawn,
					const xray_re::xr_level_gct& gct) const;

	bool		move_actor(std::vector<xray_re::cse_abstract*>& spawns,
					float x, float y, float z,
					uint32_t node_id, uint16_t graph_id) const;

private:
	bool			m_scan_done;
	bool			m_xrai_compat;
	unsigned		m_flags;

	size_t			m_num_levels;
	xray_re::xr_ini_file	m_levels_ini;		// game_levels.ltx

	xray_re::xr_ini_file	m_ini;			// aiwrapper.ini
	xray_re::xr_ini_file	m_links_ini;

	const char*		m_actor_level;

	const char*		m_src_game_data;	// source path alias
	const char*		m_src_game_spawn;
	const char*		m_src_game_levels;
	const char*		m_src_level;
	const char*		m_tgt_game_data;	// target path alias
	const char*		m_tgt_game_spawn;
	const char*		m_tgt_game_levels;
	const char*		m_tgt_level;

	// just for printing, no real use
	char			m_src_level_path[256];
	char			m_tgt_level_path[256];
};

inline syncer::syncer(): m_scan_done(false), m_xrai_compat(true), m_flags(0),
	m_num_levels(0), m_actor_level(0) {}

#endif
