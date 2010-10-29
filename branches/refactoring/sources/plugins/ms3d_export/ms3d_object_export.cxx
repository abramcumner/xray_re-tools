#include <msLib.h>
#include "ms3d_object_export.h"
#include "ms3d_mesh_builder.h"
#include "xr_object.h"

using namespace xray_re;

struct ms3d_surface_factory: public xr_surface_factory {
				ms3d_surface_factory(msModel* _model);
	virtual xr_surface*	create_surface(const xr_raw_surface& raw_surface) const;
private:
	msModel*		model;
};

inline ms3d_surface_factory::ms3d_surface_factory(msModel* _model): model(_model) {}

xr_surface* ms3d_surface_factory::create_surface(const xr_raw_surface& raw_surface) const
{
	xr_surface* surface = new xr_surface(msModel_GetBoneCount(model) > 0);
	if (raw_surface.two_sided())
		surface->set_two_sided();
	msMaterial* mtl = msModel_GetMaterialAt(model, raw_surface.texture);
	char buf[2*MS_MAX_PATH];
	char* texture = buf + MS_MAX_PATH;
	msMaterial_GetDiffuseTexture(mtl, texture, MS_MAX_PATH);
	if ((texture = std::strrchr(texture, '\\')) == 0)
		texture = buf + MS_MAX_PATH;
	else
		++texture;
	char* s = std::strrchr(texture, '.');
	if (s)
		*s = 0;
	if ((s = std::strchr(texture, '_'))) {
		size_t n = s - texture;
		std::memcpy(texture - n - 1, texture, n);
		*(--texture) = '\\';
		texture -= n;
	}
	surface->texture() = texture;
	return surface;
}

int ms3d_object_export::do_export(const char* path)
{
	int num_meshes = msModel_GetMeshCount(m_model);
	if (num_meshes == 0)
		return -1;

	int num_mtls = msModel_GetMaterialCount(m_model);
	if (num_mtls == 0)
		return -1;

	ms3d_surface_factory surface_factory(m_model);
	xr_object *object = new xr_object(&surface_factory);
	int num_bones = msModel_GetBoneCount(m_model);

	xr_bone_vec& xbones = object->bones();
	for (int i = 0; i != num_bones; ++i) {
		msBone* bone = msModel_GetBoneAt(m_model, i);

		xr_bone* xbone = new xr_bone;

		char name[MS_MAX_NAME];
		msBone_GetName(bone, name, sizeof(name));
		xbone->name() = name;
		xbone->vmap_name() = name;

		msBone_GetParentName(bone, name, sizeof(name));
		xbone->parent_name() = name;

		fvector3 temp;

		msBone_GetRotation(bone, temp.xyz);
		dvector3 rot2;
		dmatrix().set_euler_xyz(temp.x, temp.y, temp.z).get_xyz_i(rot2);
		xbone->bind_rotate().set(float(-rot2.x), float(-rot2.y), float(rot2.z));

		msBone_GetPosition(bone, temp.xyz);
		xbone->bind_offset().set(temp.x, temp.y, -temp.z);

		xbones.push_back(xbone);
	}
	if (num_bones > 0) {
		object->flags() = EOF_DYNAMIC;
		object->partitions().push_back(new xr_partition(xbones));
	}

	ms3d_mesh_builder* xmesh = new ms3d_mesh_builder;
	int status = xmesh->build(m_model, num_bones > 0);
	if (status == 0) {
		xmesh->commit(*object);
		object->denominate();
		status = object->save_object(path) ? 0 : -1;
	}
	delete object;

	return status;
}
