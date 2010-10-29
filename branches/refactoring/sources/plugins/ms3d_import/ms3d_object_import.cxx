#include <msLib.h>
#include "ms3d_object_import.h"

using namespace xray_re;

#if 0
		const xr_bone_vec& bones = ogf->bones();
		for (xr_bone_vec_it it = bones.begin(), end = bones.end(); it != end; ++it) {
			if ((*it)->is_root()) {
				enumerate(*it, 0, 
				status = import(*it);
				break;
			}
		}
#endif

int ms3d_object_import::import(const xr_object *object)
{
	int status = 0;

	const xr_bone_vec& bones = object->bones();
	for (xr_surface_vec_cit it = object->surfaces().begin(),
			end = object->surfaces().end(); it != end; ++it) {
		if ((status = import(*it)) < 0)
			break;
	}

	for (xr_mesh_vec_cit it = object->meshes().begin(),
			end = object->meshes().end(); it != end; ++it) {
		if ((status = import(*it, bones)) < 0)
			break;
	}
	return status;
}

int ms3d_object_import::import(const xr_surface* surface)
{
	int mtl_idx = msModel_AddMaterial(m_model);
	msMaterial* mtl = msModel_GetMaterialAt(m_model, mtl_idx);
	if (mtl == 0)
		return -1;
	msMaterial_SetName(mtl, surface->name().c_str());
	msMaterial_SetDiffuseTexture(mtl, surface->texture().c_str());
	std::string comment;
	comment.append("eshader = ").append(surface->eshader()).append("\n");
	comment.append("cshader = ").append(surface->cshader()).append("\n");
	comment.append("gamemtl = ").append(surface->gamemtl()).append("\n");
	if (surface->two_sided())
		comment.append("two_sided = true");
	msMaterial_SetComment(mtl, comment.c_str());
	return 0;
}

int ms3d_object_import::import(const xr_mesh* mesh, const xr_bone_vec& bones)
{
	const lw_face_vec& faces = mesh->faces();
	const xr_vmap_vec& vmaps = mesh->vmaps();
	const lw_vmref_vec& vmrefs = mesh->vmrefs();
//	xr_assert(points.size() < MAX_VERTICES);
//	xr_assert(faces.size() < MAX_TRIANGLES);
	for (xr_surfmap_vec_cit it = mesh->surfmaps().begin(), end = mesh->surfmaps().end();
			it != end; ++it) {
		int submesh_idx = msModel_AddMesh(m_model);
		msMesh* submesh = msModel_GetMeshAt(m_model, submesh_idx);
		if (submesh == 0)
			return -1;
//		create_submesh(
	}
	return 0;
}

#if 0
int ms3d_object_import::(const xray_re::)
{
	const std::vector<fvector3>& points = mesh->points();
	std::vector<int> vert_remap(points.size(), -1);
	for (std::vector<uint32_t>::const_iterator it = sm_faces.begin(),
			end = sm_faces.end(); it != end; ++it) {
		const lw_face& face = faces[*it];
	}
	for (lw_face_vec_cit it = 
}
#endif
