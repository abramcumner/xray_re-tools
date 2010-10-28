#include "xr_level.h"
#include "xr_level_visuals.h"
#include "xr_level_cform.h"
#include "xr_gamemtls_lib.h"
#include "xr_object.h"
#include "level_tools.h"
#include "level_aux_mesh.h"

using namespace xray_re;

struct dbgcfrm_surface_factory: public xr_surface_factory {
				dbgcfrm_surface_factory(const xr_gamemtls_lib* _gamemtls_lib);
	virtual xr_surface*	create_surface(const xr_raw_surface& raw_surface) const;
private:
	const xr_gamemtls_lib*	gamemtls_lib;
};

inline dbgcfrm_surface_factory::dbgcfrm_surface_factory(const xr_gamemtls_lib* _gamemtls_lib):
	gamemtls_lib(_gamemtls_lib) {}

xr_surface* dbgcfrm_surface_factory::create_surface(const xr_raw_surface& raw_surface) const
{
	xr_surface* surface = new xr_surface;
	surface->texture() = "prop\\prop_fake_kollision";
	if (gamemtls_lib) {
		const xr_gamemtl* gamemtl = gamemtls_lib->get_material(raw_surface.gamemtl);
		if (gamemtl)
			surface->gamemtl() = gamemtl->name;
	}
	if (raw_surface.two_sided())
		surface->set_two_sided();
	return surface;
}

void level_tools::build_debug_cform() const
{
	if (m_level->cform() == 0)
		return;
	msg("building %s", "debug CFORM object");

	level_aux_mesh* mesh = new level_aux_mesh;
	mesh->build(*m_level->cform());
	dbgcfrm_surface_factory surface_factory(m_gamemtls_lib);
	xr_object* object = new xr_object(&surface_factory);
	mesh->commit(*object);
	mesh->name() = "cformShape";
	object->denominate_surfaces();
	save_object(object, "cform");
	delete object;
}
