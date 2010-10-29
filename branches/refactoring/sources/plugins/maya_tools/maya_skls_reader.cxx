#define NOMINMAX
#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include "maya_skls_reader.h"
#include "maya_import_tools.h"
#include "xr_object.h"

using namespace xray_re;

const MString maya_skls_reader::k_name("X-Ray skeletal motions");
char maya_skls_reader::k_options_script[] = "";
char maya_skls_reader::k_default_options[] = "";

MStatus maya_skls_reader::initialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.registerFileTranslator(k_name, "",
			creator, k_options_script, k_default_options, true);
}

MStatus maya_skls_reader::uninitialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.deregisterFileTranslator(k_name);
}

void* maya_skls_reader::creator() { return new maya_skls_reader; }

MStatus maya_skls_reader::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
{
	fix_fpu_cw();

	MStatus status = MS::kFailure;
	if (mode == kImportAccessMode) {
		maya_import_tools imp_tools;
		const char* path = file.resolvedFullName().asChar();
		xr_object* object = new xr_object;
		if (!object->load_skls(path)) {
			msg("can't open %s", path);
			delete object;
			return MS::kFailure;
		}
		MObject character_obj = imp_tools.lookup_character(&status);
		if (status) {
			imp_tools.reset_animation_state();
			status = imp_tools.import_motions(object->motions(), character_obj);
		}
		delete object;
	}
	return status;
}

bool maya_skls_reader::haveWriteMethod() const { return false; }

MString maya_skls_reader::defaultExtension() const { return MString("skls"); }

MString maya_skls_reader::filter() const { return MString("*.skls"); }

bool maya_skls_reader::canBeOpened() const { return false; }
