#include "xr_level.h"
#include "xr_level_game.h"
#include "xr_scene.h"
#include "xr_scene_ways.h"
#include "level_tools.h"

using namespace xray_re;

void level_tools::reconstruct_ways()
{
	const xr_level_game* game = m_level->game();
	if (game == 0 || game->paths().empty())
		return;
	msg("building %s (%s)", "way.part" , "paths");

	const way_path_vec& paths = game->paths();

	xr_custom_object_vec& cobjects = m_scene->ways()->objects();
	cobjects.reserve(paths.size());

	for (way_path_vec_cit it = paths.begin(), end = paths.end(); it != end; ++it) {
		way_path* path = *it;
		xr_way_object* new_path = new xr_way_object(*m_scene);
		new_path->co_name() = path->name;
//		new_path->type() = WAY_TYPE_PATROL_PATH;
		new_path->links() = path->links;
		new_path->points().resize(path->points.size());
		way_point_le_vec_it new_it1 = new_path->points().begin();
		uint16_t point_id = 0;
		for (way_point_vec_cit it1 = path->points.begin(), end1 = path->points.end();
				it1 != end1; ++it1, ++new_it1, ++point_id) {
			new_it1->name = it1->name;
			new_it1->flags = it1->flags;
			new_it1->position = it1->position;
			new_it1->id = point_id;
		}
		cobjects.push_back(new_path);
	}
	m_level->clear_game();
}
