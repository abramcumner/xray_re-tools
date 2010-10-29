// Milkshape does not export motion evaluation methods,
// so let's borrow implementation from MS Viewer.

///////////////////////////////////////////////////////////////////////
// title:          MilkShape 3D Model Viewer Sample
//
// copyright:      The programs and associated files contained in this
//                 distribution were developed by Mete Ciragan.  The
//                 programs are not in the public domain, but they are
//                 freely distributable without licensing fees.  These
//                 programs are provided without guarantee or warrantee
//                 expressed or implied.  Use at your own risk!
//
// email:          mciragan@gmx.net
// web:            http://www.milkshape3d.com
///////////////////////////////////////////////////////////////////////

#include <msLib.h>
#include "ms3d_bone_track.h"

using namespace xray_re;

ms3d_bone_track::ms3d_bone_track(msBone* bone): m_pos_keys(0), m_rot_keys(0)
{
	fvector3 temp;
	msBone_GetRotation(bone, temp.xyz);
	m_bind_xform.set_euler_xyz(temp.x, temp.y, temp.z);
	msBone_GetPosition(bone, temp.xyz);
	m_bind_xform.c.set(temp);

	int num_keys;
	if ((num_keys = msBone_GetPositionKeyCount(bone)) > 0)
		m_pos_keys = new pos_key[num_keys];
	if (num_keys > 2) {
		for (int i = num_keys; --i >= 0;) {
			msPositionKey* key = msBone_GetPositionKeyAt(bone, i);
			m_pos_keys[i].time = key->fTime;
			m_pos_keys[i].pos.set(key->Position[0], key->Position[1], key->Position[2]);
		}
		for (int i = num_keys; --i >= 0;) {
			const pos_key& key0 = m_pos_keys[i > 0 ? (i - 1) : (num_keys - 1)];
			pos_key& key1 = m_pos_keys[i];
			const pos_key& key2 = m_pos_keys[i < (num_keys - 1) ? (i + 1) : 0];

			dvector3 temp;
			temp.sub(key2.pos, key0.pos);
			double dt1 = key1.time - key0.time;
			double dt2 = key2.time - key1.time;
			double dt = dt1 + dt2;
			key1.tan_in.mul(temp, dt1/dt);
			key1.tan_out.mul(temp, dt2/dt);
		}
	} else {
		for (int i = num_keys; --i >= 0;) {
			msPositionKey* key = msBone_GetPositionKeyAt(bone, i);
			m_pos_keys[i].time = key->fTime;
			m_pos_keys[i].pos.set(key->Position[0], key->Position[1], key->Position[2]);
			m_pos_keys[i].tan_in.set();
			m_pos_keys[i].tan_out.set();
		}
	}
	m_num_pos_keys = num_keys;

	if ((num_keys = msBone_GetRotationKeyCount(bone)) > 0) {
		m_rot_keys = new rot_key[num_keys];
		for (int i = num_keys; --i >= 0;) {
			msRotationKey* key = msBone_GetRotationKeyAt(bone, i);
			m_rot_keys[i].time = key->fTime;
			m_rot_keys[i].rot.set(dmatrix().set_euler_xyz(
					key->Rotation[0], key->Rotation[1], key->Rotation[2]));
		}
	}
	m_num_rot_keys = num_keys;
}

ms3d_bone_track::~ms3d_bone_track()
{
	delete[] m_pos_keys;
	delete[] m_rot_keys;
}

void ms3d_bone_track::evaluate(int frame, fvector3& pos, fvector3& rot) const
{
	float time = float(frame);
	dmatrix mot_xform;
	if (m_rot_keys) {
		const rot_key* key1 = m_rot_keys;
		const rot_key* key2 = 0;
		for (const rot_key *end = key1 + m_num_rot_keys - 1; key1 != end;) {
			if (time >= key1->time && time < (++key1)->time) {
				key2 = key1--;
				break;
			}
		}
		if (key2) {
			double t = double((time - key1->time))/(key2->time - key1->time);
			mot_xform.rotation(dquaternion().slerp(key1->rot, key2->rot, t));
		} else {
			if (time < m_rot_keys[0].time)
				mot_xform.rotation(m_rot_keys[0].rot);
			else if (time >= m_rot_keys[m_num_rot_keys - 1].time)
				mot_xform.rotation(m_rot_keys[m_num_rot_keys - 1].rot);
			else
				mot_xform.identity();
		}
	} else {
		mot_xform.identity();
	}
	if (m_pos_keys) {
		const pos_key* key1 = m_pos_keys;
		const pos_key* key2 = 0;
		for (const pos_key *end = key1 + m_num_pos_keys - 1; key1 != end;) {
			if (time >= key1->time && time < (++key1)->time) {
				key2 = key1--;
				break;
			}
		}
		if (key2) {
			double t = double((time - key1->time))/(key2->time - key1->time), t2 = t*t, t3 = t*t2;

			double h1 =  2.*t3 - 3.*t2 + 1.;
			double h2 = -2.*t3 + 3.*t2;
			double h3 =     t3 - 2.*t2 + t;
			double h4 =     t3 -    t2;

			mot_xform.c.x = h1*key1->pos.x + h3*key1->tan_out.x + h2*key2->pos.x + h4*key2->tan_in.x;
			mot_xform.c.y = h1*key1->pos.y + h3*key1->tan_out.y + h2*key2->pos.y + h4*key2->tan_in.y;
			mot_xform.c.z = h1*key1->pos.z + h3*key1->tan_out.z + h2*key2->pos.z + h4*key2->tan_in.z;
		} else {
			if (time < m_pos_keys[0].time)
				mot_xform.c.set(m_pos_keys[0].pos);
			else if (time >= m_pos_keys[m_num_pos_keys - 1].time)
				mot_xform.c.set(m_pos_keys[m_num_pos_keys - 1].pos);
		}
	}

	dmatrix xform;
	xform.mul_43(m_bind_xform, mot_xform);
	pos.set(float(xform.c.x), float(xform.c.y), float(-xform.c.z));

	dvector3 rot2;
	xform.get_xyz_i(rot2);
	rot.set(float(-rot2.x), float(-rot2.y), float(rot2.z));
}
