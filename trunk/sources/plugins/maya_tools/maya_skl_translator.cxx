#define NOMINMAX
#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include "maya_skl_translator.h"
#include "maya_import_tools.h"
#include "maya_export_tools.h"
#include "xr_skl_motion.h"

using namespace xray_re;

const MString maya_skl_translator::k_name("X-Ray skeletal motion");
char maya_skl_translator::k_options_script[] = "";
char maya_skl_translator::k_default_options[] = "";

MStatus maya_skl_translator::initialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.registerFileTranslator(k_name, "",
			creator, k_options_script, k_default_options, true);
}

MStatus maya_skl_translator::uninitialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.deregisterFileTranslator(k_name);
}

void* maya_skl_translator::creator() { return new maya_skl_translator; }

MStatus maya_skl_translator::writer(const MFileObject& file, const MString& options, FileAccessMode mode)
{
	fix_fpu_cw();

	switch (mode) {
	case kExportAccessMode:
	case kSaveAccessMode:
	case kExportActiveAccessMode:
		break;
	default:
		return MS::kFailure;
	}

	return maya_export_tools().export_skl(file.resolvedFullName().asChar(),
			mode == kExportActiveAccessMode);
}

MStatus maya_skl_translator::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
{
	fix_fpu_cw();

	MStatus status = MS::kFailure;
	if (mode == kImportAccessMode) {
		maya_import_tools imp_tools;
		const char* path = file.resolvedFullName().asChar();
		xr_skl_motion* smotion = new xr_skl_motion;
		if (!smotion->load_skl(path)) {
			msg("can't open %s", path);
			delete smotion;
			return MS::kFailure;
		}
		MObject character_obj = imp_tools.lookup_character(&status);
		if (status) {
			imp_tools.reset_animation_state();
			status = imp_tools.import_motion(smotion, character_obj);
		}
		delete smotion;
	}
	return status;
}

MString maya_skl_translator::defaultExtension() const { return MString("skl"); }

MString maya_skl_translator::filter() const { return MString("*.skl"); }

bool maya_skl_translator::canBeOpened() const { return false; }
