#include "xr_level.h"
#include "xr_level_glows.h"
#include "xr_level_shaders.h"
#include "xr_scene.h"
#include "xr_scene_glows.h"
#include "xr_name_gen.h"
#include "level_tools.h"

using namespace xray_re;

void level_tools::reconstruct_glows()
{
	if (m_level->glows() == 0)
		return;
	msg("building %s", "glow.part");

	const glow_data_vec& glows = m_level->glows()->glows();

	xr_custom_object_vec& cobjects = m_scene->glows()->objects();
	cobjects.reserve(glows.size());

	xr_name_gen name("glow");
	for (glow_data_vec_cit it = glows.begin(), end = glows.end(); it != end; ++it, name.next()) {
		const glow_data* glow = *it;

		bool is_duplicate = false;
		for (glow_data_vec_cit it1 = it; ++it1 != end;) {
			const glow_data* glow1 = *it1;
			if (glow->position == glow1->position) {
				const fvector3& p = glow1->position;
				msg("ignoring glow (%s) at %.2f, %.2f, %.2f", "duplicate", p.x, p.y, p.z);
				is_duplicate = true;
				break;
			}
		}
		if (is_duplicate)
			continue;

		xr_glow_object* new_glow = new xr_glow_object(*m_scene);
		new_glow->co_name() = name.get();
		new_glow->co_position() = glow->position;
		new_glow->radius() = glow->radius;
		new_glow->shader() = m_shaders->at(glow->shader_id);
		new_glow->texture() = m_textures->at(glow->texture_id);
		cobjects.push_back(new_glow);
	}
	m_level->clear_glows();
}
