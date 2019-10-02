#include "xr_matrix.h"
#include "xr_obj_motion.h"
#include "xr_envelope.h"
#include "xr_file_system.h"
#include "xrdemo_tools.h"

using namespace xray_re;

static bool load_xrdemo(xr_obj_motion& omotion, const char* path)
{
	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(path);
	if (r == 0)
		return false;
	size_t size = r->size();
	if (size == 0 || (size % sizeof(fmatrix))) {
		fs.r_close(r);
		return false;
	}
	omotion.create_envelopes();
	xr_envelope* const* envelopes = omotion.envelopes();
	int32_t frame = 0;

	// FIXME: implement correct interpretation of .xrdemo keys.
	for (size /= sizeof(fmatrix); size; --size, ++frame) {
		fmatrix xform;
		r->r(xform);
		float time = float(frame);
#if 0
	fmatrix m;
	m.set(xform);
	msg("key%d", frame);
	msg("  %5.3f %5.3f %5.3f %5.3f", m._11, m._12, m._13, m._14);
	msg("  %5.3f %5.3f %5.3f %5.3f", m._21, m._22, m._23, m._24);
	msg("  %5.3f %5.3f %5.3f %5.3f", m._31, m._32, m._33, m._34);
	msg("  %5.3f %5.3f %5.3f %5.3f", m._41, m._42, m._43, m._44);
	//fvector3 p(xform.c);
	//msg("p: %g, %g, %g", p.x, p.y, p.z);
#endif
		envelopes[0]->insert_key(new xr_key(xr_key::SHAPE_TCB, time, xform.c.x));
		envelopes[1]->insert_key(new xr_key(xr_key::SHAPE_TCB, time, xform.c.y));
		envelopes[2]->insert_key(new xr_key(xr_key::SHAPE_TCB, time, xform.c.z));
		fvector3 rot;
		xform.get_hpb(rot);
		rot.set(0, 0, 0);
		envelopes[4]->insert_key(new xr_key(xr_key::SHAPE_TCB, time, rot.x));
		envelopes[3]->insert_key(new xr_key(xr_key::SHAPE_TCB, time, rot.y));
		envelopes[5]->insert_key(new xr_key(xr_key::SHAPE_TCB, time, rot.z));
	}
	omotion.set_frame_range(0, frame*30);
	fs.r_close(r);
	return true;
}

#ifdef _CONSOLE
void xrdemo_tools::process(const cl_parser& cl)
{
	if (!prepare_target_name(cl))
		return;

	for (size_t i = 0, num_params = cl.num_params(); i != num_params; ++i) {
		const char* source = cl.param(i);
		std::string target;
		make_target_name(target, source, ".anm");
		xr_obj_motion omotion;
		if (load_xrdemo(omotion, source)) {
			if (!omotion.save_anm(target.c_str()))
				msg("can't save %s", target.c_str());
		} else {
			msg("can't load %s", source);
		}
	}
}
#else
void xrdemo_tools::process(const char* source, const char* target_path) {
	prepare_target_name(target_path);

	std::string target;
	make_target_name(target, source, ".anm");
	xr_obj_motion omotion;
	if (load_xrdemo(omotion, source)) {
		if (!omotion.save_anm(target.c_str()))
			msg("can't save %s", target.c_str());
	} else {
		msg("can't load %s", source);
	}
}
#endif // _CONSOLE