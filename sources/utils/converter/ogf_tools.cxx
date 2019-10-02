#include "ogf_tools.h"
#include "xr_ogf_v4.h"
#include "xr_file_system.h"

using namespace xray_re;

void ogf_tools::save_bones(xr_object& object, const char* source) const
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

void ogf_tools::save_skls(xr_object& object, const char* source) const
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


#ifdef _CONSOLE
void ogf_tools::save_skl(xr_object& object, const char* source, const cl_parser& cl) const
{
	std::string motion_name;
	cl.get_string("-skl", motion_name);
	if (motion_name != "all") {
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
	} else {
		for (xr_skl_motion_vec_it it = object.motions().begin(), end = object.motions().end(); it != end; ++it) {
			std::string name, target;
			xr_skl_motion* smotion = *it;
			cl.get_string("-out", name);
			if (!name.empty()) {
				xr_file_system& fs = xr_file_system::instance();
				fs.create_folder(name);
				target = name;
				target += "//";
				target += smotion->name();
			} else {
				target = smotion->name();
			}
			target += ".skl";
			if (!smotion->save_skl(target.c_str()))
				msg("can't save %s", target.c_str());
		}
	}
}

void ogf_tools::process(const cl_parser& cl)
{
	target_format format = get_target_format(cl);
	switch (format) {
	case TARGET_INFO:
	case TARGET_ERROR:
		msg("unsupported options combination");
		return;
	default:
		break;
	}

	if (!prepare_target_name(cl))
		return;

	for (size_t i = 0, num_params = cl.num_params(); i != num_params; ++i) {
		const char* source = cl.param(i);
		xr_ogf* ogf = xr_ogf::load_ogf(source);
		if (ogf) {
			switch (format) {
			case TARGET_DEFAULT:
			case TARGET_OBJECT:
				save_object(*ogf, source);
				break;
			case TARGET_SKLS:
				if (ogf->motions().empty()) {
					msg("object has no own motions");
				} else {
					save_skls(*ogf, source);
				}
				break;
			case TARGET_SKL:
				save_skl(*ogf, source, cl);
				break;
			case TARGET_BONES:
				save_bones(*ogf, source);
				break;
			default:
				break;
			}
		} else {
			msg("can't load %s", source);
		}
		delete ogf;
	}
}

void omf_tools::process(const cl_parser& cl)
{
	target_format format = get_target_format(cl);
	switch (format) {
	case TARGET_DEFAULT:
	case TARGET_SKLS:
	case TARGET_SKL:
		break;
	default:
		msg("unsupported options combination");
		return;
	}

	if (!prepare_target_name(cl))
		return;

	for (size_t i = 0, num_params = cl.num_params(); i != num_params; ++i) {
		const char* source = cl.param(i);
		xr_ogf_v4* omf = new xr_ogf_v4;
		if (omf->load_omf(source)) {
			switch (format) {
			case TARGET_DEFAULT:
			case TARGET_SKLS:
				save_skls(*omf, source);
				break;
			case TARGET_SKL:
				save_skl(*omf, source, cl);
				break;
			default:
				break;
			}
		} else {
			msg("can't load %s", source);
		}
		delete omf;
	}
}
#else
void ogf_tools::save_skl(xr_object& object, const char* source, const char* motion_name/*, bool bIsAll*/) const
{
	//if (bIsAll) {
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
			msg("can't find motion %s", motion_name);
		}
	/*} else { // TODO
		for (xr_skl_motion_vec_it it = object.motions().begin(), end = object.motions().end(); it != end; ++it) {
			std::string name, target;
			xr_skl_motion* smotion = *it;
			cl.get_string("-out", name);
			if (!name.empty()) {
				xr_file_system& fs = xr_file_system::instance();
				fs.create_folder(name);
				target = name;
				target += "//";
				target += smotion->name();
			} else {
				target = smotion->name();
			}
			target += ".skl";
			if (!smotion->save_skl(target.c_str()))
				msg("can't save %s", target.c_str());
		}
	}*/
}

void ogf_tools::process(const char* source, const char* target, const char* format, const char* motion_name) {
	prepare_target_name(target);

	xr_ogf* ogf = load(source);

	if (ogf) {
		switch (get_target_format(format)) {
			case TARGET_OBJECT:
				save_object(*ogf, source);
				break;
			case TARGET_SKLS:
				save_skls(*ogf, source);
				break;
			case TARGET_SKL:
				save_skl(*ogf, source, motion_name);
				break;
			case TARGET_BONES:
				save_bones(*ogf, source);
				break;
		}
	}

	delete ogf;
}

xr_ogf* ogf_tools::load(const char* source) {
	return xr_ogf::load_ogf(source);
}

xr_ogf* omf_tools::load(const char* source) {
	xr_ogf_v4* omf = new xr_ogf_v4;
	omf->load_omf(source);

	return omf;
}
#endif // _CONSOLE