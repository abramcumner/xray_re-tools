//
// i- X-Ray game object (*.ogf)
// i- X-Ray game skeletal motions (*.omf)
// i- X-Ray game detail object (*.dm)
// ie X-Ray object (*.object)
// -e X-Ray skeletal object (*.object)
// -e X-Ray skeletal motion (*.skl)
// i- X-Ray skeletal motions (.skls;*.skl)

#define NOMINMAX
#include <maya/MFnPlugin.h>
#include "maya_object_translator.h"
#include "maya_ogf_reader.h"
#include "maya_omf_reader.h"
#include "maya_skls_reader.h"
#include "maya_skl_translator.h"
#include "maya_dm_reader.h"
#include "maya_xray_material.h"
#include "xr_file_system.h"
#include "xr_log.h"

using namespace xray_re;

const char PLUGIN_VENDOR[] = "ZENOBIAN mod team";
const char PLUGIN_VERSION[] = __DATE__;

MStatus initializePlugin(MObject obj)
{
	MStatus status;

	MString fs_spec("$MAYA_LOCATION\\bin\\xray_path.ltx");
	xr_file_system& fs = xr_file_system::instance();
	if (!fs.initialize(fs_spec.expandEnvironmentVariablesAndTilde().asChar())) {
		msg("can't initialize the file system");
		return MS::kFailure;
	}
	xr_log::instance().init("xrayMayaTools", "xray_tools");

	MFnPlugin plugin_fn(obj, PLUGIN_VENDOR, PLUGIN_VERSION);
	if (!(status = maya_xray_material::initialize(plugin_fn)))
		return status;
	if (!(status = maya_object_translator::initialize(plugin_fn)))
		return status;
	if (!(status = maya_skl_object_writer::initialize(plugin_fn)))
		return status;
	if (!(status = maya_dm_reader::initialize(plugin_fn)))
		return status;
	if (!(status = maya_ogf_reader::initialize(plugin_fn)))
		return status;
	if (!(status = maya_omf_reader::initialize(plugin_fn)))
		return status;
	if (!(status = maya_skl_translator::initialize(plugin_fn)))
		return status;
	if (!(status = maya_skls_reader::initialize(plugin_fn)))
		return status;

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin_fn(obj);
	maya_xray_material::uninitialize(plugin_fn);
	maya_object_translator::uninitialize(plugin_fn);
	maya_skl_object_writer::uninitialize(plugin_fn);
	maya_dm_reader::uninitialize(plugin_fn);
	maya_ogf_reader::uninitialize(plugin_fn);
	maya_omf_reader::uninitialize(plugin_fn);
	maya_skl_translator::uninitialize(plugin_fn);
	maya_skls_reader::uninitialize(plugin_fn);

	return MS::kSuccess;
}
