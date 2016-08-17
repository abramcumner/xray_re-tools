#include "converter.h"
#include "tools_base.h"
#include "dm_tools.h"
#include "ogf_tools.h"
#include "ogg_tools.h"
#include "fancy_tools.h"
#include "level_tools.h"
#include "dds_tools.h"
#include "db_tools.h"
#include "xrdemo_tools.h"
#include "xr_file_system.h"
#include "xr_log.h"
#include <time.h>

using namespace xray_re;

void usage()
{
	printf("X-Ray game asset converter (%s)\n", BUILD_DATE);
	printf("Vendor: ZENOBIAN mod team\n");
	printf("Usage: converter <common-options> <format-specific-options> <input-objects>\n\n");
	printf("Common options:\n");
	printf(" -ro		perform all the steps but do not write anything on disk\n");
	printf(" -fs <SPEC>	use this file system specification file (default is %s)\n", DEFAULT_FS_SPEC);
	printf(" -out <PATH>	output file name (OGF, OMF, DM, XRDEMO and DB pack)\n");
	printf(" -dir <PATH>	output folder name (OGF, OMF, DM, XRDEMO and DB unpack)\n\n");
	printf("OGF options:\n");
	printf(" -ogf		assume OGF format for input file(s)\n");
	printf(" -object	save as .object file (default)\n");
	printf(" -skls		save all motions as .skls file\n");
	printf(" -skl <NAME>	save motion <NAME> as .skl file\n");
	printf(" -bones		save all bones as .bones file\n\n");
	printf("OMF options:\n");
	printf(" -omf		assume OMF format for input file(s)\n");
	printf(" -skls		save as .skls file (default)\n");
	printf(" -skl <NAME>	save motion <NAME> as .skl file\n\n");
	printf("DM options:\n");
	printf(" -dm		assume DM format for input file(s)\n");
	printf(" -object	save as .object file (default)\n");
	printf(" -info		display shader, texture, min/max scale and flags\n\n");
//	printf("XRDEMO options:\n");
//	printf(" -xrdemo	assume XRDEMO format for input file(s)\n\n");
	printf("Level options:\n");
	printf(" -level		assume game level format\n");
	printf(" -mode <MODE>	assume output format according to <MODE>:\n");
	printf("	maya	make single object for importing into Maya/3ds Max (default)\n");
	printf("	le	split into terrain, merged edge-linked groups, MU models\n");
	printf("	le2	split into terrain, raw edge-linked groups, MU models\n");
	printf(" -with_lods	produce LOD textures for MU models\n");
	printf(" -fancy <SPEC>	scale detail models and fix fences according to <SPEC>\n\n");
	printf("OGG/WAV options:\n");
	printf(" -ogg2wav	restore *.wav/*.thm in $sounds$ using *.ogg from $game_sounds$\n\n");
	printf("DDS/TGA options:\n");
	printf(" -dds2tga	restore *.tga in $textures$ using *.dds from $game_textures$\n");
	printf(" -with_solid	don't ignore non-transparent textures (for xrLC -gi)\n");
	printf(" -with_bump	don't ignore bump textures\n\n");
	printf("DB options:\n");
	printf(" -unpack	unpack game archive (expects list of file names)\n");
	printf(" -pack		pack game archive (expects folder name)\n");
//	printf(" -strip_thm	remove attached image in texture descriptors\n");
	printf(" -11xx		assume 1114/1154 archive format (unpack only)\n");
	printf(" -2215		assume 2215 archive format (unpack only)\n");
	printf(" -2945		assume 2945/2939 archive format (unpack only)\n");
	printf(" -2947ru	assume release version format\n");
	printf(" -2947ww	assume world-wide release version and 3120 format\n");
	printf(" -xdb		assume .xdb or .db archive format\n");
	printf(" -xdb_ud <FILE>	attach user data from <FILE>\n");
	printf(" -flt <MASK> 	extract only files, filtered by mask\n");
}

int main(int argc, char* argv[])
{
	static const cl_parser::option_desc options[] = {
		{"-ogf",	cl_parser::OT_BOOL},
		{"-omf",	cl_parser::OT_BOOL},
		{"-dm",		cl_parser::OT_BOOL},
		{"-level",	cl_parser::OT_BOOL},
		{"-ogg2wav",	cl_parser::OT_BOOL},
		{"-dds2tga",	cl_parser::OT_BOOL},
		{"-unpack",	cl_parser::OT_BOOL},
		{"-pack",	cl_parser::OT_BOOL},
		{"-xrdemo",	cl_parser::OT_BOOL},

		{"-11xx",	cl_parser::OT_BOOL},
		{"-2215",	cl_parser::OT_BOOL},
		{"-2945",	cl_parser::OT_BOOL},
		{"-2947ru",	cl_parser::OT_BOOL},
		{"-2947ww",	cl_parser::OT_BOOL},
		{"-xdb",	cl_parser::OT_BOOL},
		{"-xdb_ud",	cl_parser::OT_STRING},

		{"-info",	cl_parser::OT_BOOL},
		{"-skl",	cl_parser::OT_STRING},
		{"-skls",	cl_parser::OT_BOOL},
		{"-bones",	cl_parser::OT_BOOL},
		{"-comment",	cl_parser::OT_BOOL},
		{"-object",	cl_parser::OT_BOOL},

		{"-fs",		cl_parser::OT_STRING},
		{"-out",	cl_parser::OT_STRING},
		{"-dir",	cl_parser::OT_STRING},
		{"-flt",	cl_parser::OT_STRING},

		{"-ro",		cl_parser::OT_BOOL},

		{"-with_solid",	cl_parser::OT_BOOL},
		{"-with_bump",	cl_parser::OT_BOOL},

		{"-mode",	cl_parser::OT_STRING},
		{"-with_lods",	cl_parser::OT_BOOL},
		{"-dbgcfrm",	cl_parser::OT_BOOL},
		{"-dbgmrg",	cl_parser::OT_BOOL},
		{"-use_mt",	cl_parser::OT_BOOL},

		{"-fancy",	cl_parser::OT_STRING},
	};

	cl_parser cl;
	if (!cl.parse(argc, argv, xr_dim(options), options)) {
		usage();
		return 1;
	}

	unsigned format = tools_base::TOOLS_AUTO;
	if (cl.exist("-ogf"))
		format |= tools_base::TOOLS_OGF;
	if (cl.exist("-omf"))
		format |= tools_base::TOOLS_OMF;
	if (cl.exist("-dm"))
		format |= tools_base::TOOLS_DM;
	if (cl.exist("-level"))
		format |= tools_base::TOOLS_LEVEL;
	if (cl.exist("-ogg2wav"))
		format |= tools_base::TOOLS_OGG;
	if (cl.exist("-dds2tga"))
		format |= tools_base::TOOLS_DDS;
	if (cl.exist("-unpack"))
		format |= tools_base::TOOLS_DB;
	if (cl.exist("-pack"))
		format |= tools_base::TOOLS_FOLDER;
	if (cl.exist("-fancy"))
		format |= tools_base::TOOLS_FANCY;
	if (cl.exist("-xrdemo"))
		format |= tools_base::TOOLS_XRDEMO;
	if (format == tools_base::TOOLS_AUTO) {
		std::string extension;
		size_t num_params = cl.num_params();
		for (size_t i = 0; i != num_params; ++i) {
			if (xr_file_system::folder_exist(cl.param(i))) {
				format |= tools_base::TOOLS_FOLDER;
			} else {
				xr_file_system::split_path(cl.param(i), 0, 0, &extension);
				if (extension == ".ogf")
					format |= tools_base::TOOLS_OGF;
				else if (extension == ".omf")
					format |= tools_base::TOOLS_OMF;
				else if (extension == ".dm")
					format |= tools_base::TOOLS_DM;
				else if (extension == ".xrdemo")
					format |= tools_base::TOOLS_XRDEMO;
				else if (db_tools::is_known(extension))
					format |= tools_base::TOOLS_DB;
			}
		}
		if (format == tools_base::TOOLS_AUTO) {
			if (num_params)
				msg("can't auto-detect the source format");
			else
				usage();
			return 1;
		}
	}
	if ((format & (format - 1)) != 0) {
		msg("conflicting source formats");
		return 1;
	}

	const char* fs_spec = 0;
	if (format == tools_base::TOOLS_LEVEL ||
			format == tools_base::TOOLS_DDS ||
			format == tools_base::TOOLS_FANCY ||
			format == tools_base::TOOLS_OGG) {
		if (!cl.get_string("-fs", fs_spec))
			fs_spec = DEFAULT_FS_SPEC;
		if (!xr_file_system::file_exist(fs_spec)) {
			msg("can't find %s", fs_spec);
			return 1;
		}
	}
	unsigned fs_flags = 0;
	if (cl.get_bool("-ro")) {
		fs_flags |= xr_file_system::FSF_READ_ONLY;
		msg("working in read-only mode");
	}
	xr_file_system& fs = xr_file_system::instance();
	if (!fs.initialize(fs_spec, fs_flags)) {
		msg("can't initialize the file system");
		return 1;
	}
	xr_log::instance().init("converter", 0);

	tools_base* tools = 0;
	switch (format) {
	case tools_base::TOOLS_OGF:
		tools = new ogf_tools;
		break;
	case tools_base::TOOLS_OMF:
		tools = new omf_tools;
		break;
	case tools_base::TOOLS_DM:
		tools = new dm_tools;
		break;
	case tools_base::TOOLS_LEVEL:
		tools = new level_tools;
		break;
	case tools_base::TOOLS_OGG:
		tools = new ogg_tools;
		break;
	case tools_base::TOOLS_DDS:
		tools = new dds_tools;
		break;
	case tools_base::TOOLS_DB:
		tools = new db_unpacker;
		break;
	case tools_base::TOOLS_FOLDER:
		tools = new db_packer;
		break;
	case tools_base::TOOLS_FANCY:
		tools = new fancy_tools;
		break;
	case tools_base::TOOLS_XRDEMO:
		tools = new xrdemo_tools;
		break;
	}
	if (tools == 0) {
		msg("locked");
		return 0;
	}

	clock_t start = clock();
	tools->process(cl);
	msg("total time: %.3lfs", (clock() - start) / 1.0 / CLOCKS_PER_SEC);

	delete tools;
	return 0;
}

void tools_base::check_path(const char* path, bool& status) const
{
	xr_file_system& fs = xr_file_system::instance();
	if (!fs.folder_exist(path, "")) {
		msg("path %s does not exist", path);
		status = false;
	}
}
