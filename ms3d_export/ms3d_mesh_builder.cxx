#include <msLib.h>
#include "ms3d_mesh_builder.h"

using namespace xray_re;

ms3d_mesh_builder::~ms3d_mesh_builder()
{
	delete[] m_raw_verts;
	delete[] m_raw_uvs;
	delete[] m_raw_infls;
	delete[] m_raw_norms;
}

int ms3d_mesh_builder::build(msModel* model, bool skin)
{
	int num_meshes = msModel_GetMeshCount(model);

	size_t num_verts = 0, num_norms = 0, num_faces = 0;
	for (int i = 0; i != num_meshes; ++i) {
		msMesh* mesh = msModel_GetMeshAt(model, i);
		num_verts += size_t(msMesh_GetVertexCount(mesh) & INT_MAX);
		num_norms += size_t(msMesh_GetVertexNormalCount(mesh) & INT_MAX);
		num_faces += size_t(msMesh_GetTriangleCount(mesh) & INT_MAX);
	}
	if (num_verts == 0 || num_norms == 0 || num_faces == 0)
		return -1;

	m_raw_verts = new fvector3[num_verts];
	m_raw_uvs = new fvector2[num_verts];
	if (skin)
		m_raw_infls = new finfluence[num_verts];
	m_refs.reserve(num_verts);

	m_raw_norms = new fvector3[num_norms];
	m_norm_refs.reserve(num_norms);

	m_faces.reserve(num_faces);

	size_t vert_offset = 0, norm_offset = 0;
	for (int i = 0; i != num_meshes; ++i) {
		msMesh* mesh = msModel_GetMeshAt(model, i);
		push(mesh, vert_offset, norm_offset);
	}

	uint32_t* mapping = new uint32_t[std::max(m_refs.size(), m_norm_refs.size())];
	if (m_raw_infls)
		create_unique_array2(m_refs, m_points, m_influences, m_raw_verts, m_raw_infls, mapping);
	else
		create_unique_array(m_refs, m_points, m_raw_verts, mapping);
	delete[] m_raw_verts; m_raw_verts = 0;
	delete[] m_raw_infls; m_raw_infls = 0;

	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		it->v0 = mapping[it->v0];
		it->v1 = mapping[it->v1];
		it->v2 = mapping[it->v2];
	}
	create_unique_array(m_refs, m_texcoords, m_raw_uvs, mapping);
	delete[] m_raw_uvs; m_raw_uvs = 0;
	b_proxy_vec().swap(m_refs);

	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		it->tc0 = mapping[it->tc0];
		it->tc1 = mapping[it->tc1];
		it->tc2 = mapping[it->tc2];
	}
	create_unique_array(m_norm_refs, m_normals, m_raw_norms, mapping);
	delete[] m_raw_norms; m_raw_norms = 0;
	b_proxy_vec().swap(m_norm_refs);

	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		it->n0 = mapping[it->n0];
		it->n1 = mapping[it->n1];
		it->n2 = mapping[it->n2];
	}

	delete[] mapping;

	remove_duplicate_faces();
	remove_back_faces(1e-3f);

	return 0;
}

void ms3d_mesh_builder::push(msMesh* mesh, size_t& vert_offset0, size_t& norm_offset0)
{
	b_face face;
	face.surface.set(uint16_t(msMesh_GetMaterialIndex(mesh) & INT16_MAX));

	size_t vert_offset = vert_offset0, norm_offset = norm_offset0, num_new_faces = 0;
	for (int i = msMesh_GetTriangleCount(mesh); --i >= 0;) {
		word vert_inds[3], norm_inds[3];

		msTriangle* tri = msMesh_GetTriangleAt(mesh, i);
		msTriangle_GetVertexIndices(tri, vert_inds);

		fvector3 p[3];
		fvector2 uv[3];
		for (size_t j = 3; j != 0;) {
			msVertex* vert = msMesh_GetVertexAt(mesh, vert_inds[--j]);
			msVertex_GetVertex(vert, p[j].xyz);
			msVertex_GetTexCoords(vert, uv[j].xy);
		}
		if (p[0] == p[1] || p[1] == p[2] || p[2] == p[0])
			continue;

//		if (uv[0].distance(uv[1]) + uv[1].distance(uv[2]) + uv[2].distance(uv[0]) < 1/8192.f) {
//			;
//		}

		msTriangle_GetNormalIndices(tri, norm_inds);
		for (size_t j = 3; j != 0;) {
			--j;
			face.v[j] = face.tc[j] = uint32_t(vert_offset + vert_inds[2 - j]);
			face.n[j] = uint32_t(norm_offset + norm_inds[2 - j]);
		}
		m_faces.push_back(face);
		++num_new_faces;
	}
	if (num_new_faces == 0)
		return;

	for (int i = 0, num_verts = msMesh_GetVertexCount(mesh); i != num_verts; ++i, ++vert_offset) {
		msVertex* vert = msMesh_GetVertexAt(mesh, i);

		msVec3 p;
		msVertex_GetVertex(vert, p);
		m_raw_verts[vert_offset].set(p[0], p[1], -p[2]);

		msVertex_GetTexCoords(vert, m_raw_uvs[vert_offset].xy);

		if (m_raw_infls) {
			finfluence& infl = m_raw_infls[vert_offset];

			int bone_idx = msVertex_GetBoneIndex(vert);
			// safety measure
			if (bone_idx < 0)
				bone_idx = 0;
			infl.push_back(fbone_weight(bone_idx, 1.f));

			msVertexEx* vert2 = msMesh_GetVertexExAt(mesh, i);
			if ((bone_idx = msVertexEx_GetBoneIndices(vert2, 0)) >= 0) {
				for (int j = 0, sum = 0;;) {
					int weight = msVertexEx_GetBoneWeights(vert2, j);
					sum += weight;
					infl.back().weight = float(weight)/100.f;
					if (bone_idx < 0)
						break;
					infl.push_back(fbone_weight(bone_idx, 0));
					if (++j == 3) {
						infl.back().weight = float(100 - sum)/100.f;
						break;
					}
					bone_idx = msVertexEx_GetBoneIndices(vert2, j);
				}
			}
			infl.reorder();
		}
		m_refs.push_back(b_proxy(vert_offset));
	}
	vert_offset0 = vert_offset;
	for (int i = 0, num_norms = msMesh_GetVertexNormalCount(mesh); i != num_norms; ++i, ++norm_offset) {
#if 1
		// SG building code does not care about normal orientation.
		msMesh_GetVertexNormalAt(mesh, i, m_raw_norms[norm_offset].xyz);
#else
		msVec3 n;
		msMesh_GetVertexNormalAt(mesh, i, n);
		m_raw_norms[norm_offset].set(n[0], n[1], -n[2]);
#endif
		m_norm_refs.push_back(b_proxy(norm_offset));
	}
	norm_offset0 = norm_offset;
}
