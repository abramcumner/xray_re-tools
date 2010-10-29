#include "xr_scene.h"
#include "xr_scene_visuals.h"
#include "xr_level.h"
#include "xr_level_hom.h"
#include "xr_object.h"
#include "level_tools.h"
#include "level_aux_mesh.h"

using namespace xray_re;

struct hom_surface_factory: public xr_surface_factory {
	virtual xr_surface*	create_surface(const xr_raw_surface& raw_surface) const;
};

xr_surface* hom_surface_factory::create_surface(const xr_raw_surface& raw_surface) const
{
	xr_surface* surface = new xr_surface;
	surface->texture() = "prop\\prop_fake_kollision";
	if (raw_surface.two_sided())
		surface->set_two_sided();
	return surface;
}

void level_tools::reconstruct_hom()
{
	if (m_level->hom() == 0)
		return;
	msg("building %s", "HOM object");

	level_aux_mesh* mesh = new level_aux_mesh;
	mesh->build(*m_level->hom());
	m_level->clear_hom();

	hom_surface_factory surface_factory;
	xr_object* object = new xr_object(&surface_factory);
	mesh->commit(*object);
	mesh->name() = "occShape";
	object->denominate_surfaces();
	object->flags() = EOF_HOM;
	save_scene_object(object, "occ");
	delete object;
}
