#include "xr_math.h"
#include "xr_ai_map.h"
#include "xr_level.h"
#include "xr_level_ai.h"
#include "xr_scene.h"
#include "xr_scene_ai_map.h"
#include "level_tools.h"

using namespace xray_re;

void level_tools::reconstruct_ai_map()
{
	const xr_level_ai* ai = m_level->ai();
	if (ai == 0)
		return;
	msg("building %s", "ai_map.part");

	xr_scene_ai_map* ai_map = m_scene->ai_map();
	ai_map->cell_size() = ai->size();
	float cell_size_inv = 1.f/ai->size();
	fbox& bbox = ai_map->bbox();
	bbox.set(ai->aabb());

	size_t num_no_return = 0;
	uint_fast32_t self_idx = 0;

	ai_node_le* new_node = new ai_node_le[ai->num_nodes()];
	ai_map->nodes() = new_node;
	ai_map->num_nodes() = ai->num_nodes();
	const ai_node* nodes = ai->nodes();
	for (const ai_node *it = nodes, *end = it + ai->num_nodes();
			it != end; ++it, ++new_node, ++self_idx) {
		new_node->selected = false;
		for (uint_fast32_t i = 4; i != 0;) {
			uint32_t link = it->link(--i);
			if (!new_node->selected && link != AI_MAP_BAD_LINK &&
					nodes[link].link((i + 2)&3) != self_idx) {
				new_node->selected = true;
				++num_no_return;
			}
			new_node->link[i] = (link == AI_MAP_BAD_LINK) ? 0xffffff : link;
		}
		new_node->plane = it->plane;
		float x, y, z;
		ai->unpack_xz(it->packed_xz, x, z);
		ai->unpack_y(it->packed_y, y);
		int qx = int(std::floor(x*cell_size_inv + 1e-3f));
		int qy = int(std::floor((y - bbox.min.y)*65535.f/(bbox.y2 - bbox.y1) + 1e-3f));
		int qz = int(std::floor(z*cell_size_inv + 1e-3f));
		new_node->packed_x = int16_t(clamp<int>(qx, INT16_MIN, INT16_MAX));
//		new_node->packed_y = uint16_t(clamp<int>(qy, 0, UINT16_MAX));
		new_node->packed_y = uint16_t(clamp<int>(qy + 1, 0, UINT16_MAX));
		new_node->packed_z = int16_t(clamp<int>(qz, INT16_MIN, INT16_MAX));
	}
	msg("found %" PRIuSIZET " (of %" PRIuSIZET ") no-return nodes", num_no_return, ai->num_nodes());

	m_level->clear_ai();
}
