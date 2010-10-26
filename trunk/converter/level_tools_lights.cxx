#include "xr_level.h"
#include "xr_level_lights.h"
#include "xr_build_lights.h"
#include "xr_scene.h"
#include "xr_scene_lights.h"
#include "xr_name_gen.h"
#include "level_tools.h"

using namespace xray_re;

void level_tools::reconstruct_lights()
{
	if (m_level->lights() == 0)
		return;
	msg("building %s", "light.part");

	const light_data_vec& lights = m_level->lights()->lights();

	xr_custom_object_vec& cobjects = m_scene->lights()->objects();
	// usually half is removed because of duplication
	cobjects.reserve(lights.size()/2);

	xr_name_gen name("light");
	for (light_data_vec_cit it = lights.begin(), end = lights.end(); it != end; ++it, name.next()) {
		const light_data* light = *it;
		xr_light_object* new_light;
		if (light->controller_id == 1) {
//			const fvector3& p = light->d3d_params.position;
//			msg("ignoring d-light (%s) at %.2f, %.2f, %.2f", "sun", p.x, p.y, p.z);
			continue;
		}
		for (light_data_vec_cit it1 = it; ++it1 != end;) {
			const light_data* light1 = *it1;
			if (light->d3d_params.position == light1->d3d_params.position) {
//				const fvector3& p = light1->d3d_params.position;
//				msg("ignoring d-light (%s) at %.2f, %.2f, %.2f", "duplicate", p.x, p.y, p.z);
				goto skip;
			}
		}
		new_light = new xr_light_object(*m_scene);
		new_light->type() = light->d3d_params.type;
		new_light->color() = light->d3d_params.diffuse;
		new_light->co_name() = name.get();
		new_light->co_position().set(light->d3d_params.position);
		// FIXME: is this correct?
		new_light->co_rotation().set(light->d3d_params.direction);
		new_light->range() = light->d3d_params.range;
		new_light->attenuation_constant() = light->d3d_params.attenuation0;
		new_light->attenuation_linear() = light->d3d_params.attenuation1;
		new_light->attenuation_quadratic() = light->d3d_params.attenuation2;
		new_light->cone_angle() = light->d3d_params.phi;
		new_light->use_in_d3d() = 1;
		new_light->flags() = LIGHT_FLAG_LIGHTMAP|LIGHT_FLAG_DYNAMIC;
		// in-game level uses the same ordering for light controllers
		// as build.prj: $hemi (0), $sun (1), $static (2).
		// It differs from the light.part order, so remap ids here.
		switch (light->controller_id) {
		case 2:
			new_light->control() = 0;
			break;
		case 1:
			new_light->type() = D3D_LIGHT_POINT;
			new_light->control() = 2;
			break;
		case 0:
			new_light->control() = 1;
			break;
		}
		cobjects.push_back(new_light);
skip:;
	}
	m_level->clear_lights();
}
