#include "ogf_tools.h"
#include "xr_ogf_v4.h"
#include "xr_file_system.h"

using namespace xray_re;

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
