#include "xr_level.h"
#include "xr_level_ps_static.h"
#include "xr_scene.h"
#include "xr_scene_particles.h"
#include "xr_name_gen.h"
#include "level_tools.h"

using namespace xray_re;

void level_tools::reconstruct_particles()
{
	if (m_level->ps_static() == 0)
		return;
	msg("building %s", "ps.part");

	const particle_data_vec& particles = m_level->ps_static()->particles();

	xr_custom_object_vec& cobjects = m_scene->particles()->objects();
	cobjects.reserve(particles.size());

	xr_name_gen name("ps");
	for (particle_data_vec_cit it = particles.begin(), end = particles.end(); it != end; ++it, name.next()) {
		particle_data* ps = *it;
		xr_particle_object* new_ps = new xr_particle_object(*m_scene);
		new_ps->co_name() = name.get();
		new_ps->co_position().set(ps->xform.c);
		ps->xform.get_xyz(new_ps->co_rotation());
		// FIXME: this is not correct
		new_ps->co_scale().set(ps->xform.i.magnitude(),
				ps->xform.j.magnitude(), ps->xform.k.magnitude());
		new_ps->reference() = ps->reference;
		cobjects.push_back(new_ps);
	}
	m_level->clear_ps_static();
}
