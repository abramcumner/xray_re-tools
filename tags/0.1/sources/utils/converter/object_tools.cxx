#include "object_tools.h"
#include "xr_object.h"
#include "xr_object_format.h"
#include "xr_file_system.h"

using namespace xray_re;

#if 0
void object_tools::process(const cl_parser& cl)
{
	xr_object* object = new xr_object();
	if (object->load_object(source))
		;//object->save_object("GGGGG");
	else
		msg("can't load %s", source.c_str());
	delete object;
}
#endif

void object_tools::save_object(xray_re::xr_object& object, const char* source) const
{
	std::string target;
	make_target_name(target, source, ".object");
	object.to_object();
	if (!object.save_object(target.c_str()))
		msg("can't save object in %s", target.c_str());
}

void object_tools::save_bones(xray_re::xr_object& object, const char* source) const
{
	if (object.bones().empty()) {
		msg("game object has no bones");
	} else {
		std::string target;
		make_target_name(target, source, ".bones");
		if (!object.save_bones(target.c_str()))
			msg("can't save bones in %s", target.c_str());
	}
}

void object_tools::save_skls(xray_re::xr_object& object, const char* source) const
{
	if (object.motions().empty()) {
		msg("game object has no own motions");
	} else {
		std::string target;
		make_target_name(target, source, ".skls");
		if (!object.save_skls(target.c_str()))
			msg("can't save motions in %s", target.c_str());
	}
}

void object_tools::save_skl(xray_re::xr_object& object, const char* source, const cl_parser& cl) const
{
	std::string motion_name;
	cl.get_string("-skl", motion_name);
	xr_skl_motion* smotion = object.find_motion(motion_name);
	if (smotion) {
		std::string target;
		if (m_output_file.empty()) {
			std::string name;
			xr_file_system::split_path(source, 0, &name);
			target = m_output_folder;
			target += name;
			target += '_';
			target += motion_name;
			target += ".skl";
		} else {
			target = m_output_file;
		}
		if (!smotion->save_skl(target.c_str()))
			msg("can't save %s", target.c_str());
	} else {
		msg("can't find motion %s", motion_name.c_str());
	}
}

object_tools::target_format object_tools::get_target_format(const cl_parser& cl) const
{
	unsigned format = 0;
	if (cl.exist("-info"))
		format |= TARGET_INFO;
	if (cl.exist("-skl"))
		format |= TARGET_SKL;
	if (cl.exist("-skls"))
		format |= TARGET_SKLS;
	if (cl.exist("-bones"))
		format |= TARGET_BONES;
	if (cl.exist("-object"))
		format |= TARGET_OBJECT;
	return (format & (format - 1)) == 0 ? static_cast<target_format>(format) : TARGET_ERROR;
}
