#define NOMINMAX
#include <windows.h>
#include <cstring>
#include "xr_file_system.h"
#include "xr_ini_file.h"
#include "xr_string_utils.h"
#include "xr_cl_parser.h"
#include "xr_log.h"
#include "aiwrapper.h"
#include "syncer.h"

using namespace xray_re;

const void usage()
{
	printf("X-Ray AI compiler wrapper (%s)\n", BUILD_DATE);
	printf("Vendor: ZENOBIAN mod team\n");
	printf("Usage: aiwrapper <options>\n\n");
	printf("xrAI derived options:\n");
	printf(" -h or -?	this help\n");
	printf(" -verify <NAME>	verify AI map in gamedata\\levels\\<NAME>\\\n");
	printf(" -f <NAME>	build AI map in gamedata\\levels\\<NAME>\\\n");
	printf(" -g <NAME>	build off-line AI graph and cross-table to AI map in gamedata\\levels\\<NAME>\\\n");
	printf(" -m		merge level graphs\n");
	printf(" -s		build game spawn data\n");
	printf(" -draft		build draft AI map\n\n");
	printf("Custom options:\n");
	printf(" -fs <SPEC>	use <SPEC> as file system specification file (default is %s)\n", DEFAULT_FS_SPEC);
	printf(" -fast_sync	update just older files in shadow xrAI tree\n");
	printf(" -split_spawns	restore level.spawn files from all.spawn/game.graph\n");
	printf(" -split_paths	restore level.game files from all.spawn\n");
	printf(" -split_graphs	restore level.graph files from all.spawn/game.graph\n");
	printf(" -check_paths	check walk paths in all.spawn\n");
	printf(" -upgrade <BLD>	convert levels into CS (3502) or CoP (3870) format\n\n");
	printf("Deprecated options:\n");
	printf(" -m2 <FILE>	adjust level graphs according to <FILE> and merge them\n");
	printf(" -dump_gp	dump list of graph points in all level.spawn files\n");
//	printf(" -use_orig_gp	read graph points from original level.spawn\n");
}

static void run_xrai(const char* opt, const char* arg = 0)
{
	xr_file_system& fs = xr_file_system::instance();
	const char* root = fs.resolve_path(PA2215_FS_ROOT);
	char cmd[2*MAX_PATH];
	xr_snprintf(cmd, sizeof(cmd), "%sxrAI.exe %s%s%s", root, opt, arg ? " " : "", arg ? arg : "");
	STARTUPINFOA si = {0};
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi = {0};
	msg("launching '%s'", cmd + std::strlen(root));
	BOOL success = CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, root, &si, &pi);
	if (!success)
		throw syncer::sync_error();
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

int main(int argc, char* argv[])
{
	static const cl_parser::option_desc options[] = {
		{"-h",			cl_parser::OT_BOOL},
		{"-?",			cl_parser::OT_BOOL},
		{"-verify",		cl_parser::OT_STRING},
		{"-f",			cl_parser::OT_STRING},
		{"-g",			cl_parser::OT_STRING},
		{"-m",			cl_parser::OT_BOOL},
		{"-s",			cl_parser::OT_BOOL},
		{"-draft",		cl_parser::OT_BOOL},
		{"-fs",			cl_parser::OT_STRING},
		{"-fast_sync",		cl_parser::OT_BOOL},
		{"-split_spawns",	cl_parser::OT_BOOL},
		{"-split_paths",	cl_parser::OT_BOOL},
		{"-split_graphs",	cl_parser::OT_BOOL},
		{"-check_paths",	cl_parser::OT_BOOL},
		{"-upgrade",		cl_parser::OT_INTEGER},
		{"-m2",			cl_parser::OT_STRING},
		{"-dump_gp",		cl_parser::OT_BOOL},
		{"-use_orig_dp",	cl_parser::OT_BOOL},
	};
	cl_parser cl;
	if (!cl.parse(argc, argv, xr_dim(options), options) || cl.num_params() != 0) {
		usage();
		return 1;
	}
	if (cl.get_bool("-h") || cl.get_bool("-?") || cl.num_options() == 0) {
		usage();
		return 0;
	}

	const char* fs_spec;
	if (!cl.get_string("-fs", fs_spec))
		fs_spec = DEFAULT_FS_SPEC;
	if (!xr_file_system::file_exist(fs_spec)) {
		msg("can't find %s", fs_spec);
		return 1;
	}
	xr_file_system& fs = xr_file_system::instance();
	if (!fs.initialize(fs_spec, 0)) {
		msg("can't initialize the file system");
		return 1;
	}
	xr_log::instance().init("aiwrapper");

	try {
		syncer sync;
		sync.check_fs_paths();
		sync.load_ini("aiwrapper.ini");
		unsigned force = cl.get_bool("-fast_sync") ? 0 : SYNC_FORCE;
		const char* name;
		int bld_ver;
		if (cl.get_string("-f", name)) {
			sync.to_xrai(name, SYNC_BUILD_AIMAP|SYNC_BUILD_CFORM|SYNC_BUILD_PRJ|force);
			run_xrai((cl.get_bool("-draft") ? "-draft -f" : "-f"), name);
			sync.from_xrai(name, SYNC_LEVEL_AI|SYNC_FORCE);
		} else if (cl.get_string("-verify", name)) {
			sync.to_xrai(name, SYNC_LEVEL_AI|force);
			run_xrai("-verify", name);
		} else if (cl.get_string("-g", name)) {
			sync.to_xrai(name, SYNC_LEVEL_AI|SYNC_LEVEL_SECTORS_AI|SYNC_LEVEL_SPAWN|force);
			run_xrai("-g", name);
			sync.from_xrai(name, SYNC_LEVEL_GRAPH|SYNC_LEVEL_GCT_RAW|SYNC_LEVEL_SECTORS_AI|SYNC_FORCE);
		} else if (cl.get_bool("-m")) {
			sync.to_xrai(SYNC_GAME_LTX|SYNC_LEVEL_AI|SYNC_LEVEL_GCT_RAW|SYNC_LEVEL_GRAPH|SYNC_LEVEL_SPAWN|force);
			run_xrai("-m");
			sync.from_xrai(SYNC_GAME_GRAPH|SYNC_LEVEL_GCT|SYNC_LEVEL_GCT_RAW|SYNC_FORCE);
		} else if (cl.get_bool("-s")) {
			sync.to_xrai(SYNC_GAME_LTX|SYNC_GAME_GRAPH|SYNC_LEVEL_AI|SYNC_LEVEL_SECTORS_AI|
					SYNC_LEVEL_GRAPH|SYNC_LEVEL_GCT|SYNC_LEVEL_GCT_RAW|SYNC_LEVEL_SPAWN|force);
			run_xrai("-s");
			sync.from_xrai(SYNC_GAME_SPAWN|SYNC_LEVEL_SECTORS_AI|SYNC_FORCE);
		} else if (cl.get_bool("-split_spawns")) {
			sync.split_spawns(cl.get_bool("-use_orig_gp"));
		} else if (cl.get_bool("-split_paths")) {
			sync.split_paths();
		} else if (cl.get_bool("-split_graphs")) {
			sync.split_graphs();
		} else if (cl.get_bool("-check_paths")) {
			sync.check_paths();
		} else if (cl.get_integer("-upgrade", bld_ver)) {
			sync.upgrade(bld_ver);
		} else if (cl.get_string("-m2", name)) {
			sync.load_links_ini(name);
			sync.to_xrai(SYNC_GAME_LTX|SYNC_LEVEL_AI|SYNC_LEVEL_GCT_RAW|SYNC_LEVEL_GRAPH|SYNC_LEVEL_SPAWN|force);
			run_xrai("-m");
			sync.from_xrai(SYNC_GAME_GRAPH|SYNC_LEVEL_GCT|SYNC_LEVEL_GCT_RAW|SYNC_FORCE);
		} else if (cl.get_bool("-dump_gp")) {
			sync.dump_links("links.output");
		} else {
			usage();
		}
	} catch (syncer::sync_error) {
		msg("ERROR occured! Aborting.");
	}
	return 0;
}
