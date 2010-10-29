#define NOMINMAX
#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include "maya_ogf_reader.h"
#include "maya_import_tools.h"
#include "xr_ogf.h"

using namespace xray_re;

const MString maya_ogf_reader::k_name("X-Ray game object");
char maya_ogf_reader::k_options_script[] = "";
char maya_ogf_reader::k_default_options[] = "";

MStatus maya_ogf_reader::initialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.registerFileTranslator(k_name, "",
			creator, k_options_script, k_default_options, true);
}

MStatus maya_ogf_reader::uninitialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.deregisterFileTranslator(k_name);
}

void* maya_ogf_reader::creator() { return new maya_ogf_reader; }

MStatus maya_ogf_reader::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
{
	fix_fpu_cw();

	MStatus status = MS::kFailure;
	if (mode == kImportAccessMode || mode == kOpenAccessMode) {
		start_progress(2, "Loading OGF");
		const char* path = file.resolvedFullName().asChar();
		xr_ogf* ogf = xr_ogf::load_ogf(path);
		if (ogf) {
			advance_progress();
			ogf->to_object();
			advance_progress();
			end_progress();
			maya_import_tools(ogf, &status);
			delete ogf;
		} else {
			msg("can't open %s", path);
			end_progress();
		}
	}
	return status;
}

bool maya_ogf_reader::haveWriteMethod() const { return false; }

MString maya_ogf_reader::defaultExtension() const { return MString("ogf"); }

MString maya_ogf_reader::filter() const { return MString("*.ogf"); }
