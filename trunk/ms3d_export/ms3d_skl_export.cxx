#include <msLib.h>
#include "ms3d_skl_export.h"
#include "ms3d_bone_track.h"
#include "xr_skl_motion.h"
#include "xr_envelope.h"

using namespace xray_re;

int ms3d_skl_export::do_export(const char* path)
{
	int num_bones = msModel_GetBoneCount(m_model);
	if (num_bones == 0)
		return -1;

	int num_frames = msModel_GetTotalFrames(m_model);
	xr_bone_motion_vec bmotions(size_t(num_bones & INT_MAX));
	for (int i = 0; i != num_bones; ++i) {
		msBone* bone = msModel_GetBoneAt(m_model, i);

		char name[MS_MAX_NAME];
		msBone_GetName(bone, name, sizeof(name));
		xr_bone_motion* bmotion = new xr_bone_motion(name);

		// prepare interpolation data
		ms3d_bone_track track(bone);

		// interpolate just like Milkshape does
		bmotion->create_envelopes();
		xr_envelope* const* envelopes = bmotion->envelopes();
		for (int frame = 0; frame != num_frames; ++frame) {
			fvector3 pos, rot;
			track.evaluate(frame + 1, pos, rot);
			float time = float(frame)/30.f;
			envelopes[0]->insert_key(time, pos.x);
			envelopes[1]->insert_key(time, pos.y);
			envelopes[2]->insert_key(time, pos.z);
			envelopes[4]->insert_key(time, rot.x);
			envelopes[3]->insert_key(time, rot.y);
			envelopes[5]->insert_key(time, rot.z);
		}
		bmotions[i] = bmotion;
	}
	xr_skl_motion* smotion = new xr_skl_motion;
	smotion->name() = "unnamed";
	smotion->fps() = 30.f;
	smotion->set_frame_range(0, num_frames);
	smotion->bone_motions().swap(bmotions);
	smotion->save_skl(path);
	delete smotion;

	return 0;
}
