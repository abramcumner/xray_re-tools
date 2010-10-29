#define NOMINMAX
#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include "maya_object_translator.h"
#include "maya_import_tools.h"
#include "maya_export_tools.h"
#include "xr_object.h"

using namespace xray_re;

const MString maya_object_translator::k_name("X-Ray object");
const MString maya_skl_object_writer::k_name("X-Ray skeletal object");
char maya_object_translator::k_options_script[] = "";
char maya_skl_object_writer::k_options_script[] = "";
char maya_object_translator::k_default_options[] = "";
char maya_skl_object_writer::k_default_options[] = "";

maya_object_translator::~maya_object_translator() {}

MStatus maya_object_translator::initialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.registerFileTranslator(k_name, "",
			creator, k_options_script, k_default_options, true);
}

MStatus maya_skl_object_writer::initialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.registerFileTranslator(k_name, "",
			creator, k_options_script, k_default_options, true);
}

MStatus maya_object_translator::uninitialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.deregisterFileTranslator(k_name);
}

MStatus maya_skl_object_writer::uninitialize(MFnPlugin& plugin_fn)
{
	return plugin_fn.deregisterFileTranslator(k_name);
}

void* maya_object_translator::creator() { return new maya_object_translator(); }

void* maya_skl_object_writer::creator() { return new maya_skl_object_writer(); }

MStatus maya_object_translator::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
{
	fix_fpu_cw();

	MStatus status = MS::kFailure;
	if (mode == kImportAccessMode || mode == kOpenAccessMode) {
		const char* path = file.resolvedFullName().asChar();
		xr_object* object = new xr_object;
		if (object->load_object(path))
			maya_import_tools(object, &status);
		else
			msg("can't open %s", path);
		delete object;
	}
	return status;
}

MStatus maya_object_translator::writer(const MFileObject& file, const MString& options, FileAccessMode mode)
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

	return maya_export_tools().export_object(file.resolvedFullName().asChar(),
			mode == kExportActiveAccessMode);
}

MStatus maya_skl_object_writer::writer(const MFileObject& file, const MString& options, FileAccessMode mode)
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

	return maya_export_tools().export_skl_object(file.resolvedFullName().asChar(),
			mode == kExportActiveAccessMode);
}

bool maya_object_translator::haveReadMethod() const { return true; }

bool maya_skl_object_writer::haveReadMethod() const { return false; }

bool maya_object_translator::haveWriteMethod() const { return true; }

MString maya_object_translator::defaultExtension() const { return MString("object"); }

MString maya_object_translator::filter() const { return MString("*.object"); }

bool maya_object_translator::canBeOpened() const { return true; }

bool maya_skl_object_writer::canBeOpened() const { return false; }

MPxFileTranslator::MFileKind maya_object_translator::identifyFile(const MFileObject& file, const char* buffer, short size) const
{
	return extract_extension(file) == defaultExtension() ? kIsMyFileType : kNotMyFileType;
}

MString maya_object_translator::extract_extension(const MFileObject& file)
{
	MString name(file.resolvedName());
	// FIXME: assumes there _is_ extension.
	return name.substringW(name.rindexW('.') + 1, name.numChars() - 1).toLowerCase();
}
