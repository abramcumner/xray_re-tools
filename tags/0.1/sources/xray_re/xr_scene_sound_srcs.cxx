#include "xr_scene_sound_srcs.h"
#include "xr_reader.h"
#include "xr_writer.h"

using namespace xray_re;

xr_sound_src_object::xr_sound_src_object(xr_scene& scene):
	xr_custom_object(scene, TOOLS_CLASS_SOUND_SRC),
	m_flags(0x1), m_type(0),
	m_volume(1.f), m_frequency(1.f),
	m_min_dist(1.f), m_max_dist(30.f), m_max_ai_dist(30.f)
{
	m_sound_pos.set();
	m_pause_time.set();
	m_active_time.set();
	m_play_time.set();
}

xr_sound_src_object::~xr_sound_src_object() {}

void xr_sound_src_object::load(xr_reader& r)
{
	uint16_t version;
	if (!r.r_chunk<uint16_t>(SOUNDSRC_CHUNK_VERSION, version))
		xr_not_expected();
	xr_assert(version == SOUNDSRC_VERSION);
	xr_custom_object::load(r);
	if (!r.r_chunk<uint8_t>(SOUNDSRC_CHUNK_TYPE, m_type))
		xr_not_expected();
	if (!r.find_chunk(SOUNDSRC_CHUNK_SOURCE_NAME))
		xr_not_expected();
	r.r_sz(m_source_name);
	r.debug_find_chunk();
	if (r.find_chunk(SOUNDSRC_CHUNK_PARAMS_2) ||
			r.find_chunk(SOUNDSRC_CHUNK_PARAMS_1)) {
		r.r_fvector3(m_sound_pos);
		m_volume = r.r_float();
		m_frequency = r.r_float();
		m_min_dist = r.r_float();
		m_max_dist = r.r_float();
		m_max_ai_dist = r.r_float();
		r.debug_find_chunk();
	} else if (r.find_chunk(SOUNDSRC_CHUNK_PARAMS_0)) {
		r.r_fvector3(m_sound_pos);
		m_volume = r.r_float();
		m_frequency = r.r_float();
		m_min_dist = r.r_float();
		m_max_dist = r.r_float();
		m_max_ai_dist = m_max_dist;
		r.debug_find_chunk();
	} else {
		xr_not_expected();
	}
	r.r_chunk<uint32_t>(SOUNDSRC_CHUNK_FLAGS, m_flags);
	if (r.find_chunk(SOUNDSRC_CHUNK_TIME)) {
		r.r_fvector2(m_pause_time);
		r.r_fvector2(m_active_time);
		r.r_fvector2(m_play_time);
		r.debug_find_chunk();
	}
}

void xr_sound_src_object::save(xr_writer& w) const
{
	xr_custom_object::save(w);
	w.w_chunk<uint16_t>(SOUNDSRC_CHUNK_VERSION, SOUNDSRC_VERSION);
	w.w_chunk<uint8_t>(SOUNDSRC_CHUNK_TYPE, m_type);

	w.w_chunk(SOUNDSRC_CHUNK_SOURCE_NAME, m_source_name);

	w.w_chunk<uint32_t>(SOUNDSRC_CHUNK_FLAGS, m_flags);

	w.open_chunk(SOUNDSRC_CHUNK_PARAMS_2);
	w.w_fvector3(m_sound_pos);
	w.w_float(m_volume);
	w.w_float(m_frequency);
	w.w_float(m_min_dist);
	w.w_float(m_max_dist);
	w.w_float(m_max_ai_dist);
	w.close_chunk();

	w.open_chunk(SOUNDSRC_CHUNK_TIME);
	w.w_fvector2(m_pause_time);
	w.w_fvector2(m_active_time);
	w.w_fvector2(m_play_time);
	w.close_chunk();
}

////////////////////////////////////////////////////////////////////////////////

xr_scene_sound_srcs::xr_scene_sound_srcs(xr_scene& scene):
	xr_scene_objects(scene, "sound_src.part", SCENE_CHUNK_SOUND_SRCS) {}

xr_scene_sound_srcs::~xr_scene_sound_srcs() {}

void xr_scene_sound_srcs::load(xr_reader& r)
{
	uint16_t version = 0;
	r.r_chunk<uint16_t>(TOOLS_CHUNK_VERSION, version);
	xr_assert(version == 0);
	xr_scene_objects::load(r);
}

void xr_scene_sound_srcs::save(xr_writer& w) const
{
	xr_scene_objects::save(w);
	w.w_chunk<uint16_t>(TOOLS_CHUNK_VERSION, 0);
}
