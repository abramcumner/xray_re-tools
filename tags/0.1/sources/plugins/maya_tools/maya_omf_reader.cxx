#define NOMINMAX
#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include "maya_omf_reader.h"
#include "maya_import_tools.h"
#include "xr_ogf_v4.h"

using namespace xray_re;

const MString maya_omf_reader::k_name("X-Ray game skeletal motions");
char maya_omf_reader::k_options_script[] = "";
char maya_omf_reader::k_default_options[] = "";

MStatus maya_omf_reader::initialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.registerFileTranslator(k_name, "",
			creator, k_options_script, k_default_options, true);
}

MStatus maya_omf_reader::uninitialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.deregisterFileTranslator(k_name);
}

void* maya_omf_reader::creator() { return new maya_omf_reader; }

MStatus maya_omf_reader::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
{
	fix_fpu_cw();

	MStatus status = MS::kFailure;
	if (mode == kImportAccessMode) {
		start_progress(1, "Loading OMF");
		const char* path = file.resolvedFullName().asChar();
		xr_ogf_v4* omf = new xr_ogf_v4;
		if (omf->load_omf(path)) {
			advance_progress();
			maya_import_tools imp_tools;
			MObject character_obj = imp_tools.lookup_character(&status);
			if (status) {
				imp_tools.reset_animation_state();
				status = imp_tools.import_motions(omf->motions(), character_obj);
			}
			end_progress();
		} else {
			msg("can't open %s", path);
			end_progress();
		}
		delete omf;
	}
	return status;
}

bool maya_omf_reader::haveWriteMethod() const { return false; }

MString maya_omf_reader::defaultExtension() const { return MString("omf"); }

MString maya_omf_reader::filter() const { return MString("*.omf"); }

bool maya_omf_reader::canBeOpened() const { return false; }
