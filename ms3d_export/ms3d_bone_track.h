#pragma once
#ifndef __MS3D_MESH_BUILDER_H__
#define __MS3D_MESH_BUILDER_H__

#include "xr_vector3.h"
#include "xr_matrix.h"
#include "xr_quaternion.h"

struct pos_key {
	float			time;
	xray_re::dvector3	pos;
	xray_re::dvector3	tan_in;
	xray_re::dvector3	tan_out;
};

struct rot_key {
	float			time;
	xray_re::dquaternion	rot;
};

struct msBone;

class ms3d_bone_track {
public:
				ms3d_bone_track(msBone* bone);
				~ms3d_bone_track();

	void			evaluate(int frame, xray_re::fvector3& pos, xray_re::fvector3& rot) const;

private:
	xray_re::dmatrix	m_bind_xform;
	int			m_num_pos_keys;
	int			m_num_rot_keys;
	pos_key*		m_pos_keys;
	rot_key*		m_rot_keys;
};

#endif
