#define NOMINMAX
#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include "maya_dm_reader.h"
#include "maya_import_tools.h"
#include "xr_dm.h"

using namespace xray_re;

const MString maya_dm_reader::k_name("X-Ray game detail object");
char maya_dm_reader::k_options_script[] = "";
char maya_dm_reader::k_default_options[] = "";

MStatus maya_dm_reader::initialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.registerFileTranslator(k_name, "",
			creator, k_options_script, k_default_options, true);
}

MStatus maya_dm_reader::uninitialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.deregisterFileTranslator(k_name);
}

void* maya_dm_reader::creator() { return new maya_dm_reader; }

MStatus maya_dm_reader::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
{
	fix_fpu_cw();

	MStatus status = MS::kFailure;
	if (mode == kImportAccessMode || mode == kOpenAccessMode) {
		start_progress(2, "Loading DM");
		const char *path = file.resolvedFullName().asChar();
		xr_dm* dm = new xr_dm;
		if (dm->load_dm(path)) {
			advance_progress();
			dm->to_object();
			advance_progress();
			end_progress();
			maya_import_tools(dm, &status);
		} else {
			msg("can't open %s", path);
			end_progress();
		}
		delete dm;
	}
	return status;
}

bool maya_dm_reader::haveWriteMethod() const { return false; }

MString maya_dm_reader::defaultExtension() const { return MString("dm"); }

MString maya_dm_reader::filter() const { return MString("*.dm"); }
