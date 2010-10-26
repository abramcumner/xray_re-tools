#pragma once
#ifndef __MS3D_MESH_BUILDER_H__
#define __MS3D_MESH_BUILDER_H__

#include "xr_mesh_builder.h"

struct msMesh;
struct msModel;

class ms3d_mesh_builder: public xray_re::xr_mesh_builder {
public:
			ms3d_mesh_builder();
			~ms3d_mesh_builder();

	int		build(msModel* model, bool skin);

private:
	void		push(msMesh* mesh, size_t& vert_offset0, size_t& norm_offset0);
	void		clear();

private:
	xray_re::fvector3*	m_raw_verts;
	xray_re::fvector2*	m_raw_uvs;
	xray_re::finfluence*	m_raw_infls;

	xray_re::fvector3*	m_raw_norms;
	b_proxy_vec		m_norm_refs;
};

inline ms3d_mesh_builder::ms3d_mesh_builder():
	m_raw_verts(0), m_raw_uvs(0), m_raw_infls(0), m_raw_norms(0) {}

#endif
