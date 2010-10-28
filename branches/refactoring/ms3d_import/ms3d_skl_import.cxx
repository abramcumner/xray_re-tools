#include <msLib.h>
#include "ms3d_skl_import.h"
#include "xr_matrix.h"

using namespace xray_re;

struct key {
			key(int _frame, const fvector3& _value);
	int		frame;
	fvector3	value;
};

inline key::key(int _frame, const fvector3& _value): frame(_frame), value(_value) {}

class anim_track: public std::vector<key> {
public:
	void	append(int frame, const fvector3& value);
};

inline void anim_track::append(int frame, const fvector3& value)
{
	if (empty() || back().value != value)
		push_back(key(frame, value));
}

int ms3d_skl_import::import(const xr_skl_motion *smotion)
{
	int status = 0;
	const xr_bone_motion_vec& bmotions = smotion->bone_motions();
	size_t num_bones = bmotions.size();
	int* bone_refs = new int[num_bones];
	anim_track* positions = new anim_track[num_bones];
	anim_track* rotations = new anim_track[num_bones];
	float fps = smotion->fps();
	for (size_t i = 0; i != num_bones; ++i) {
		const xr_bone_motion* bmotion = bmotions[i];
		int bone_idx = msModel_FindBoneByName(m_model, bmotion->name().c_str());
		if (bone_idx < 0) {
			status = -1;
			break;
		}
		bone_refs[i] = bone_idx;

		msBone* bone = msModel_GetBoneAt(m_model, bone_idx);
		fvector3 pos, rot;
		msBone_GetRotation(bone, rot.xyz);
		msBone_GetPosition(bone, pos.xyz);
		dmatrix bind_xform;
		bind_xform.set_euler_xyz(rot.x, rot.y, rot.z);
		bind_xform.c.set(pos);

		for (int xframe = smotion->frame_start(), xframe_end = smotion->frame_end(), frame = 1;
				xframe < xframe_end; ++frame, ++xframe) {
			bmotion->evaluate(xframe/fps, pos, rot);
			dmatrix mot_xform;
			mot_xform.set_xyz_i(-rot.x, -rot.y, rot.z);
			mot_xform.c.set(pos.x, pos.y, -pos.z);

			dmatrix xform;
			xform.invert_43(bind_xform).mul_b_43(mot_xform);

			dvector3 rot2;
			xform.get_euler_xyz(rot2);

			rotations[i].append(frame, rot.set(rot2));
			positions[i].append(frame, pos.set(xform.c));
		}
	}
	if (status == 0) {
		msModel_SetTotalFrames(m_model, smotion->frame_end() - smotion->frame_start());
		msModel_SetFrame(m_model, 0);
		for (size_t i = 0; i != num_bones; ++i) {
			msBone* bone = msModel_GetBoneAt(m_model, bone_refs[i]);
			for (anim_track::iterator it = positions[i].begin(), end = positions[i].end();
					it != end; ++it) {
				msBone_AddPositionKey(bone, float(it->frame), it->value.xyz);
			}
			for (anim_track::iterator it = rotations[i].begin(), end = rotations[i].end();
					it != end; ++it) {
				msBone_AddRotationKey(bone, float(it->frame), it->value.xyz);
			}
		}
	}
	delete[] positions;
	delete[] rotations;
	delete[] bone_refs;
	return status;
}
