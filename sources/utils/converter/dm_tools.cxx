#include "dm_tools.h"
#include "xr_file_system.h"
#include "xr_dm.h"

using namespace xray_re;

#ifdef _CONSOLE
void dm_tools::process(const cl_parser& cl)
{
	target_format format = get_target_format(cl);
	switch (format) {
	case TARGET_DEFAULT:
	case TARGET_INFO:
	case TARGET_OBJECT:
		break;
	default:
		msg("unsupported options combination");
		return;
	}

	if (!prepare_target_name(cl))
		return;

	for (size_t i = 0, num_params = cl.num_params(); i != num_params; ++i) {
		const char* source = cl.param(i);
		xr_dm* dm = new xr_dm;
		if (dm->load_dm(source)) {
			switch (format) {
			case TARGET_DEFAULT:
			case TARGET_OBJECT:
				save_object(*dm, source);
				break;
			case TARGET_INFO:
				msg("shader: %s", dm->shader().c_str());
				msg("texture: %s", dm->texture().c_str());
				msg("scale (min/max): %f/%f", dm->min_scale(), dm->max_scale());
				msg("flags: %8.8" PRIx32, dm->flags());
				break;
			default:
				break;
			}
		} else {
			msg("can't load %s", source);
		}
		delete dm;
	}
}
#else
void dm_tools::process(const char* source, const char* target, const char* format) {
	prepare_target_name(target);

	xr_dm* dm = new xr_dm;
	if (dm->load_dm(source)) {
		switch (get_target_format(format)) {
			case TARGET_OBJECT:
				save_object(*dm, source);
				break;
			case TARGET_INFO: // TODO
				msg("shader: %s", dm->shader().c_str());
				msg("texture: %s", dm->texture().c_str());
				msg("scale (min/max): %f/%f", dm->min_scale(), dm->max_scale());
				msg("flags: %8.8"PRIx32, dm->flags());
				break;
		}
	} else {
		msg("can't load %s", source);
	}

	delete dm;
}
#endif // _CONSOLE