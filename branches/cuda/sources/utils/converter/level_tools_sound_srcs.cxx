#include "xr_scene.h"
#include "xr_scene_sound_srcs.h"
#include "xr_level.h"
#include "xr_level_snd_static.h"
#include "xr_name_gen.h"
#include "level_tools.h"

using namespace xray_re;

void level_tools::reconstruct_sound_srcs()
{
	if (m_level->snd_static() == 0)
		return;
	msg("building %s", "sound_src.part");

	const sound_src_data_vec& sound_srcs = m_level->snd_static()->sound_srcs();

	xr_custom_object_vec& cobjects = m_scene->sound_srcs()->objects();
	cobjects.reserve(sound_srcs.size());

	xr_name_gen name("sound_src");
	for (sound_src_data_vec_cit it = sound_srcs.begin(), end = sound_srcs.end();
			it != end; ++it, name.next()) {
		const sound_src_data* sound_src = *it;
		xr_sound_src_object* new_sound_src = new xr_sound_src_object(*m_scene);
		new_sound_src->co_name() = name.get();
		new_sound_src->co_position().set(sound_src->position);
//		new_sound_src->flags() = 0;
//		new_sound_src->type() = SOUNDSRC_TYPE_0;
		new_sound_src->source_name() = sound_src->name;
		new_sound_src->sound_pos().set(sound_src->position);
		new_sound_src->volume() = sound_src->volume;
		new_sound_src->frequency() = sound_src->freq;
		new_sound_src->pause_time().set(float(sound_src->pause_time.x),
				float(sound_src->pause_time.y));
		new_sound_src->active_time().set(float(sound_src->active_time.x),
				float(sound_src->active_time.y));
		new_sound_src->play_time().set(float(sound_src->play_time.x),
				float(sound_src->play_time.y));
		cobjects.push_back(new_sound_src);
	}
	m_level->clear_snd_static();
}
