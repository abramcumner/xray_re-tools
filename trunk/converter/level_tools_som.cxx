#include "xr_scene.h"
#include "xr_scene_visuals.h"
#include "xr_level.h"
#include "xr_level_som.h"
#include "xr_object.h"
#include "xr_gamemtls_lib.h"
#include "level_tools.h"
#include "level_aux_mesh.h"

using namespace xray_re;

struct som_surface_factory: public xr_surface_factory {
				som_surface_factory(const xr_gamemtls_lib* _gamemtls_lib);
	virtual xr_surface*	create_surface(const xr_raw_surface& raw_surface) const;
private:
	const xr_gamemtls_lib*	gamemtls_lib;
};

inline som_surface_factory::som_surface_factory(const xr_gamemtls_lib* _gamemtls_lib):
	gamemtls_lib(_gamemtls_lib) {}

xr_surface* som_surface_factory::create_surface(const xr_raw_surface& raw_surface) const
{
	xr_surface* surface = new xr_surface;
	surface->texture() = "prop\\prop_fake_kollision";
	const xr_gamemtl* gamemtl = gamemtls_lib->get_material(raw_surface.gamemtl);
	if (gamemtl)
		surface->gamemtl() = gamemtl->name;
	if (raw_surface.two_sided())
		surface->set_two_sided();
	return surface;
}

void level_tools::reconstruct_som()
{
	if (m_level->som() == 0)
		return;
	msg("building %s", "SOM object");

	level_aux_mesh* mesh = new level_aux_mesh;
	mesh->build(*m_level->som(), *m_level->gamemtls_lib());
	m_level->clear_som();
	som_surface_factory surface_factory(m_level->gamemtls_lib());
	xr_object* object = new xr_object(&surface_factory);
	mesh->commit(*object);
	mesh->name() = "sound_occShape";
	object->denominate_surfaces();
	object->flags() = EOF_SOUND_OCCLUDER;
	save_scene_object(object, "sound_occ");
	delete object;
}
