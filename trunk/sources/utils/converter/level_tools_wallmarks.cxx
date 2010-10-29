#include "xr_level.h"
#include "xr_level_wallmarks.h"
#include "xr_scene.h"
#include "xr_scene_wallmarks.h"
#include "level_tools.h"

using namespace xray_re;

void level_tools::reconstruct_wallmarks()
{
	if (m_level->wallmarks() == 0)
		return;
	msg("building %s", "wallmark.part");

	xr_scene_wallmarks* wallmarks_le = m_scene->wallmarks();
	wm_slot_le_vec& slots_le = wallmarks_le->slots();

	const wm_slot_vec& slots = m_level->wallmarks()->slots();

	slots_le.reserve(slots.size());
	for (wm_slot_vec_cit it = slots.begin(), end = slots.end(); it != end; ++it) {
		const wm_slot* slot = *it;
		wm_slot_le* new_slot = new wm_slot_le;
		new_slot->shader = slot->shader;
		new_slot->texture = slot->texture;
		new_slot->wallmarks.resize(slot->wallmarks.size());
		wm_object_vec_it new_it1 = new_slot->wallmarks.begin();
		for (wm_data_vec_cit it1 = slot->wallmarks.begin(), end1 = slot->wallmarks.end();
				it1 != end1; ++it1, ++new_it1) {
			new_it1->selected = false;
			new_it1->bsphere.set(it1->bounds);
			new_it1->width = 1.f;
			new_it1->height = 1.f;
			new_it1->rotate = 0;
			if (it1->vertices.empty()) {
				new_it1->bbox.null();
			} else {
				new_it1->vertices.reserve(it1->vertices.size());
				new_it1->bbox.invalidate();
				for (wm_vertex_vec_cit it2 = it1->vertices.begin(),
						end2 = it1->vertices.end(); it2 != end2; ++it2) {
					new_it1->bbox.extend(it2->p);
					new_it1->vertices.push_back(*it2);
				}
			}
		}
		slots_le.push_back(new_slot);
	}
	if (!slots.empty()) {
		wallmarks_le->shader() = slots[0]->shader;
		wallmarks_le->texture() = slots[0]->texture;
	}
	m_level->clear_wallmarks();
}
