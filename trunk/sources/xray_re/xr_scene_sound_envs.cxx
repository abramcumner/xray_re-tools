#include "xr_scene_sound_envs.h"
#include "xr_reader.h"
#include "xr_writer.h"

using namespace xray_re;

xr_sound_env_object::xr_sound_env_object(xr_scene& scene):
	xr_shape_object(scene, TOOLS_CLASS_SOUND_ENV) {}

xr_sound_env_object::~xr_sound_env_object() {}

void xr_sound_env_object::load(xr_reader& r)
{
	uint16_t version;
	if (!r.r_chunk<uint16_t>(SOUNDENV_CHUNK_VERSION, version))
		xr_not_expected();
	xr_assert(version == SOUNDENV_VERSION);
	xr_custom_object::load(r);
	if (!r.find_chunk(SOUNDENV_CHUNK_ENV_REFS))
		xr_not_expected();
	r.r_sz(m_inner);
	r.r_sz(m_outer);
	r.debug_find_chunk();
}

void xr_sound_env_object::save(xr_writer& w) const
{
	xr_custom_object::save(w);
	w.w_chunk<uint16_t>(SOUNDENV_CHUNK_VERSION, SOUNDENV_VERSION);
	w.open_chunk(SOUNDENV_CHUNK_ENV_REFS);
	w.w_sz(m_inner);
	w.w_sz(m_outer);
	w.close_chunk();
}

////////////////////////////////////////////////////////////////////////////////

xr_scene_sound_envs::xr_scene_sound_envs(xr_scene& scene):
	xr_scene_objects(scene, "sound_env.part", SCENE_CHUNK_SOUND_ENVS) {}

xr_scene_sound_envs::~xr_scene_sound_envs() {}

void xr_scene_sound_envs::load(xr_reader& r)
{
	uint16_t version = 0;
	r.r_chunk<uint16_t>(TOOLS_CHUNK_VERSION, version);
	xr_assert(version == 0);
	xr_scene_objects::load(r);
}

void xr_scene_sound_envs::save(xr_writer& w) const
{
	xr_scene_objects::save(w);
	w.w_chunk<uint16_t>(TOOLS_CHUNK_VERSION, 0);
}
