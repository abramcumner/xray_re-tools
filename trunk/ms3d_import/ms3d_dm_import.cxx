#include <msLib.h>
#include "ms3d_dm_import.h"
#include "ms3d_xray_utils.h"

using namespace xray_re;

int ms3d_dm_import::import(const xr_dm *dm)
{
	int mtl_idx = msModel_AddMaterial(m_model);
	msMaterial* mtl = msModel_GetMaterialAt(m_model, mtl_idx);
	if (mtl == 0)
		return -1;
	msMaterial_SetName(mtl, "default");
	set_texture(mtl, dm->texture());
	set_comment(mtl, dm->texture().c_str());

	int mesh_idx = msModel_AddMesh(m_model);
	msMesh* mesh = msModel_GetMeshAt(m_model, mesh_idx);
	if (mesh == 0)
		return -1;
	msMesh_SetName(mesh, "default");
	msMesh_SetMaterialIndex(mesh, mtl_idx);

	const xr_vbuf& vb = dm->vb();
	size_t num_verts = vb.size();
	if (num_verts > MAX_VERTICES)
		return -1;

	const xr_ibuf& ib = dm->ib();
	size_t num_inds = ib.size();
	if (num_inds > MAX_TRIANGLES*3 || (num_inds % 3) != 0)
		return -1;

	// FIXME: the refs array is certainly an overkill, simple offset should be enough.
	int* vert_refs = new int[vb.size()];
	for (size_t i = 0; i != num_verts; ++i) {
		int vert_idx = msMesh_AddVertex(mesh);
		msVertex* vert = msMesh_GetVertexAt(mesh, vert_idx);

		fvector3 p = vb.p(i);
		p.z = -p.z;
		msVertex_SetVertex(vert, p.xyz);

		vert_refs[i] = vert_idx;
	}
	for (size_t i = 0; i != num_inds;) {
		uint_fast16_t v0 = ib[i++], v1 = ib[i++], v2 = ib[i++];

		word inds[3];
		inds[2] = word(vert_refs[v0] & UINT16_MAX);
		inds[1] = word(vert_refs[v1] & UINT16_MAX);
		inds[0] = word(vert_refs[v2] & UINT16_MAX);

		int tri_idx = msMesh_AddTriangle(mesh);
		msTriangle* tri = msMesh_GetTriangleAt(mesh, tri_idx);
		msTriangle_SetVertexIndices(tri, inds);

		fvector2 uv;
		msTriangleEx* tri2 = msMesh_GetTriangleExAt(mesh, tri_idx);
		msTriangleEx_SetTexCoord(tri2, 2, uv.set(vb.tc(v0)).xy);
		msTriangleEx_SetTexCoord(tri2, 1, uv.set(vb.tc(v1)).xy);
		msTriangleEx_SetTexCoord(tri2, 0, uv.set(vb.tc(v2)).xy);
	}
	delete[] vert_refs;

	return 0;
}
