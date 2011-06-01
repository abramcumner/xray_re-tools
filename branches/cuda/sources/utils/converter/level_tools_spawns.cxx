#include "xr_level.h"
#include "xr_level_spawn.h"
#include "xr_level_env_mod.h"
#include "xr_level_game.h"
#include "xr_entity.h"
#include "xr_scene.h"
#include "xr_scene_spawns.h"
#include "xr_scene_shapes.h"
#include "xr_name_gen.h"
#include "level_tools.h"

using namespace xray_re;

static inline rgba32 to_rgba32(const fvector3& v) { return fcolor().set(v.z, v.y, v.x).get(); }

void level_tools::reconstruct_spawns()
{
	xr_custom_object_vec& cobjects = m_scene->spawns()->objects();
	if (m_level->spawn()) {
		msg("building %s (%s)", "spawn.part", "entities");

		xr_entity_vec& spawns = m_level->spawn()->spawns();
		cobjects.reserve(spawns.size());

		for (xr_entity_vec_it it = spawns.begin(), end = spawns.end(); it != end; ++it) {
			cse_abstract* entity = *it;
			xr_assert(entity);
			// borrow it as usual with spawns
			*it = 0;
			xr_spawn_object* new_spawn = new xr_spawn_object(*m_scene, 0);
			new_spawn->co_name() = entity->name_replace();
			new_spawn->co_position()= entity->position();
			new_spawn->co_rotation() = entity->rotation();
			if (m_spawn_version == CSE_VERSION_SOC)
			{
			entity->version() = CSE_VERSION_SOC;
			entity->script_version() = 6;
			}
			else if (m_spawn_version == CSE_VERSION_CS)
			{
				entity->version() = CSE_VERSION_CS;
				entity->script_version() = 8;
			}
			else if (m_spawn_version == CSE_VERSION_COP)
			{
				entity->version() = CSE_VERSION_COP;
				entity->script_version() = 12;
			}

			new_spawn->entity() = entity;
			if (entity->shape()) {
				xr_shape_object* new_shape = new xr_shape_object(*m_scene);
				new_shape->co_name() = "shape";
				new_shape->co_position()= entity->position();
				new_shape->co_rotation() = entity->rotation();
				new_shape->shapes() = entity->shape()->shapes();
				new_spawn->attached_object() = new_shape;
			}
			cobjects.push_back(new_spawn);
		}
		m_level->clear_spawn();
	}
	if (m_level->game() && !m_level->game()->rpoints().empty()) {
		msg("building %s (%s)", "spawn.part", "$rpoints");

		const mp_rpoint_vec& rpoints = m_level->game()->rpoints();
		cobjects.reserve(cobjects.size() + rpoints.size());

		xr_name_gen name("$rpoint");
		for (mp_rpoint_vec_cit it = rpoints.begin(), end = rpoints.end(); it != end; ++it, name.next()) {
			const mp_rpoint* rpoint = *it;
			xr_spawn_object* new_rpoint = new xr_spawn_object(*m_scene, "$rpoint");
			new_rpoint->co_name() = name.get();
			new_rpoint->co_position().set(rpoint->p);
			new_rpoint->co_rotation().set(rpoint->a);
			new_rpoint->team() = rpoint->team;
			new_rpoint->respawn() = rpoint->respawn;
			new_rpoint->game() = rpoint->game;
			cobjects.push_back(new_rpoint);
		}
	}
	if (m_level->env_mod() && !m_level->env_mod()->env_mods().empty()) {
		msg("building %s (%s)", "spawn.part", "$env_mods");

		const env_mod_data_vec& env_mods = m_level->env_mod()->env_mods();
		cobjects.reserve(cobjects.size() + env_mods.size());

		xr_name_gen name("$env_mod");
		for (env_mod_data_vec_cit it = env_mods.begin(), end = env_mods.end();
				it != end; ++it, name.next()) {
			const env_mod_data* env_mod = *it;
			xr_spawn_object* new_env_mod = new xr_spawn_object(*m_scene, "$env_mod");
			new_env_mod->co_name() = name.get();
			new_env_mod->co_position() = env_mod->position;
			new_env_mod->radius() = env_mod->radius;
			new_env_mod->power() = env_mod->power;
			new_env_mod->view_distance() = env_mod->far_plane;
			new_env_mod->fog_color() = to_rgba32(env_mod->fog_color);
			new_env_mod->fog_density() = env_mod->fog_density;
			new_env_mod->ambient_color() = to_rgba32(env_mod->ambient_color);
			new_env_mod->sky_color() = to_rgba32(env_mod->sky_color);
			new_env_mod->hemi_color() = to_rgba32(env_mod->hemi_color);
			cobjects.push_back(new_env_mod);
		}
		m_level->clear_env_mod();
	}
}
