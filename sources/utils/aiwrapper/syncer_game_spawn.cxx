#include "xr_ai_version.h"
#include "xr_ai_map.h"
#include "xr_level_game.h"
#include "xr_level_ai.h"
#include "xr_level_gct.h"
#include "xr_level_spawn.h"
#include "xr_game_spawn.h"
#include "xr_game_graph.h"
#include "xr_ini_file.h"
#include "xr_entity.h"
#include "xr_file_system.h"
#include "syncer.h"

using namespace xray_re;

bool syncer::add_level(xr_game_spawn& gspawn, const char* name,
		xr_level_spawn& spawn, const xr_level_game& game,
		const xr_level_ai& ai, const xr_level_gct& gct)
{
	xr_entity_vec& spawns = spawn.spawns();
	gspawn.spawns().reserve(gspawn.spawns().size() + spawns.size());
	for (xr_entity_vec_it it = spawns.begin(), end = spawns.end(); it != end; ++it) {
		cse_alife_object* alife_obj = dynamic_cast<cse_alife_object*>(*it);
		if (alife_obj == 0) {
			// not a cse_alife_object derivative, assume it is a graph point
			xr_assert((*it)->clsid() == "AI_GRAPH");
			continue;
		}
		if (alife_obj->game_id() != cse_abstract::GAME_ANY &&
				alife_obj->game_id() != cse_abstract::GAME_SINGLE) {
			continue;
		}
		if (alife_obj->clsid() == "O_ACTOR") {
			if (m_actor_level) {
				msg("duplicate actor at %s (previously at %s)", name, m_actor_level);
				return false;
			}
			m_actor_level = name;
		}
		// just borrow the entity
		gspawn.spawns().push_back(alife_obj);
		*it = 0;
	}

	const way_path_vec& paths = game.paths();
	gspawn.paths().reserve(gspawn.paths().size() + paths.size());
	for (way_path_vec_cit it = paths.begin(), end = paths.end(); it != end; ++it) {
		const way_path* path = *it;
		way_path_gs* new_path = new way_path_gs;
		new_path->name = path->name;
		new_path->links = path->links;
		new_path->points.resize(path->points.size());
		way_point_gs_vec_it new_point = new_path->points.begin();
		for (way_point_vec_cit it1 = path->points.begin(), end1 = path->points.end();
				it1 != end1; ++it1, ++new_point) {
			new_point->name = it1->name;
			new_point->position = it1->position;
			new_point->flags = it1->flags;
			fvector3 position(it1->position);
			position.y += 0.15000001f;
			new_point->node_id = ai.vertex_id(position);
			new_point->graph_id = gct.graph_id(new_point->node_id);
		}
		gspawn.paths().push_back(new_path);
	}

	++gspawn.num_levels();
	return true;
}

void syncer::merge_spawns(xr_entity_vec& spawns, xr_reader& r) const
{
	if (!r.find_chunk(0))
		xr_not_expected();
	size_t num_spawns = r.r_u32();
	r.debug_find_chunk();

	xr_assert(num_spawns == spawns.size());

	xr_reader* f = r.open_chunk(1);
	xr_assert(f);
	uint32_t id = 0;
	for (xr_entity_vec_it it = spawns.begin(), end = spawns.end(); it != end; ++it) {
		xr_reader* s = f->open_chunk(id);
		xr_assert(s);
		uint16_t obj_id;
		if (!s->r_chunk(0, obj_id))
			xr_not_expected();
		xr_assert(id == obj_id);

		xr_reader* o = s->open_chunk(1);
		xr_assert(o);

		size_t size = o->find_chunk(0);
		xr_assert(size);
		xr_packet packet;
		size_t size16 = o->r_u16();
		xr_assert(size16 + 2 == size);
		o->r_packet(packet, size16);
		cse_abstract* entity = *it;
		entity->spawn_merge(packet);
		entity->version() = CSE_VERSION_SOC;
		entity->script_version() = 6;
		s->close_chunk(o);
		f->close_chunk(s);
		++id;
	}
	r.close_chunk(f);
}

bool syncer::merge_xrai_compat(xr_game_spawn& gspawn, const char* path, const char* name) const
{
	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(path, name);
	if (r == 0)
		return false;

	if (!r->find_chunk(0))
		xr_not_expected();
	uint32_t version = r->r_u32();
	xr_assert(version == AI_VERSION_8);
	gspawn.guid().load(*r);
	gspawn.graph_guid().load(*r);
	uint_fast32_t spawn_count = r->r_u32();
	xr_assert(spawn_count == gspawn.spawns().size());
	uint_fast32_t level_count = r->r_u32();
	xr_assert(level_count == gspawn.num_levels());
	r->debug_find_chunk();

	xr_reader* s = r->open_chunk(1);
	xr_assert(s);
	merge_spawns(gspawn.spawns(), *s);
	r->close_chunk(s);

	s = r->open_chunk(2);
	xr_assert(s);
	gspawn.load_af_slots(*s);
	r->close_chunk(s);

	fs.r_close(r);
	
	return true;
}

bool syncer::split_spawns(xr_game_spawn& gspawn, xr_level_spawn* by_level_id[]) const
{
	const gg_vertex* verts = gspawn.graph().vertices();
	size_t num_verts = gspawn.graph().num_vertices();
	for (xr_entity_vec_it it = gspawn.spawns().begin(), end = gspawn.spawns().end(); it != end; ++it) {
		cse_alife_object* alife_obj = dynamic_cast<cse_alife_object*>(*it);
		xr_assert(alife_obj);
		if (alife_obj == 0)
			continue;
		uint_fast16_t graph_id = alife_obj->graph_id();
		const gg_vertex* vert = (graph_id < num_verts) ? &verts[graph_id] : 0;
		if (vert == 0) {
			msg("unresolved entity: %s\n", alife_obj->name_replace().c_str());
			return false;
		}
		alife_obj->version() = CSE_VERSION_SOC;
		alife_obj->script_version() = 6;
		alife_obj->spawn_id() = UINT16_MAX;
		alife_obj->graph_id() = AI_GRAPH_BAD_VERTEX;
		alife_obj->distance() = 0;
		alife_obj->node_id() = AI_MAP_BAD_NODE;

		xr_level_spawn* spawn = by_level_id[vert->level_id];
		if (spawn == 0)
			by_level_id[vert->level_id] = spawn = new xr_level_spawn;
		// just borrow the entity
		spawn->spawns().push_back(alife_obj);
		*it = 0;
	}
	return true;
}

bool syncer::split_paths(xr_game_spawn& gspawn, xr_level_game* by_level_id[], const xr_ini_file& ini) const
{
	const gg_vertex* verts = gspawn.graph().vertices();
	uint_fast32_t num_verts = gspawn.graph().num_vertices();
	for (way_path_gs_vec_cit it = gspawn.paths().begin(), end = gspawn.paths().end(); it != end; ++it) {
		const way_path_gs* path = *it;
		const gg_vertex* vert = 0;
		for (way_point_gs_vec_cit it1 = path->points.begin(),
				end1 = path->points.end(); it1 != end1; ++it1) {
			if (it1->graph_id < num_verts) {
				if (vert && vert->level_id != verts[it1->graph_id].level_id) {
					msg("ambiguous path: %s", path->name.c_str());
					xr_not_expected();
				}
				vert = &verts[it1->graph_id];
			}
		}
		unsigned level_id;
		if (vert) {
			level_id = vert->level_id;
		} else if (ini.line_exist("unresolved_paths", path->name.c_str())) {
			const char* name = ini.r_string("unresolved_paths", path->name.c_str());
			for (const gg_level *it1 = gspawn.graph().levels(),
					*end1 = it1 + gspawn.graph().num_levels();
					it1 != end1; ++it1) {
				if (it1->name == name) {
					level_id = it1->level_id;
					goto resolved;
				}
			}
			msg("unresolved path: %s (level '%s')", path->name.c_str(), name);
			return false;
		} else {
			msg("unresolved path: %s\n", path->name.c_str());
			return false;
		}
resolved:
		xr_level_game* game = by_level_id[level_id];
		if (game == 0)
			by_level_id[level_id] = game = new xr_level_game;
		way_path* new_path = new way_path;
		new_path->name = path->name;
		new_path->links = path->links;
		new_path->points.resize(path->points.size());
		way_point_vec_it new_point = new_path->points.begin();
		for (way_point_gs_vec_cit it1 = path->points.begin(), end1 = path->points.end();
				it1 != end1; ++it1, ++new_point) {
			new_point->name = it1->name;
			new_point->position = it1->position;
			new_point->flags = it1->flags;
		}
		game->paths().push_back(new_path);
	}
	return true;
}

void syncer::check_paths(xr_game_spawn& gspawn) const
{
	for (way_path_gs_vec_cit it = gspawn.paths().begin(), end = gspawn.paths().end(); it != end; ++it) {
		const way_path_gs* path = *it;
		if (path->name.find("_walk") == std::string::npos ||
				path->name.find("_walker_") != std::string::npos) {
			continue;
		}
		for (way_point_gs_vec_cit it1 = path->points.begin(),
				end1 = path->points.end(); it1 != end1; ++it1) {
			if (it1->node_id == AI_MAP_BAD_NODE) {
				msg("%s", path->name.c_str());
				break;
			}
		}
	}
}

bool syncer::import_cross_table(xr_game_spawn& gspawn, const xr_level_gct& gct) const
{
	xr_level_gct_vec& cross_tables = gspawn.graph().cross_tables();
	size_t level_idx = gspawn.graph().num_levels();
	if (cross_tables.empty())
		cross_tables.resize(level_idx);
	const gg_level* levels = gspawn.graph().levels();
	for (; level_idx != 0;) {
		if (gct.level_guid() == levels[--level_idx].guid) {
			xr_level_gct* ct = new xr_level_gct(gct);
			ct->version() = gspawn.version();
			cross_tables[level_idx] = ct;
			return true;
		}
	}
	return false;
}

bool syncer::move_actor(xr_entity_vec& spawns, float x, float y, float z, uint32_t node_id, uint16_t graph_id) const
{
	for (xr_entity_vec_it it = spawns.begin(), end = spawns.end(); it != end; ++it) {
		cse_alife_object* alife_obj = dynamic_cast<cse_alife_object*>(*it);
		xr_assert(alife_obj);
		if (alife_obj->clsid() == "O_ACTOR") {
			alife_obj->position().set(x, y, z);
			alife_obj->node_id() = node_id;
			alife_obj->graph_id() = graph_id;
			return true;
		}
	}
	return false;
}
