#include "xr_level.h"
#include "xr_level_snd_env.h"
#include "xr_scene.h"
#include "xr_scene_sound_envs.h"
#include "xr_name_gen.h"
#include "xr_utils.h"
#include "level_tools.h"

using namespace xray_re;

const float k_zero_tolerance = 1e-3f;

struct temp_box {
			temp_box(const cf_face& face, const cf_vertex_vec& verts);
	void		collect(const cf_face& face, const cf_vertex_vec& verts);

	enum {
		STATE_COLLECTING,
		STATE_DONE,
	};

	uint16_t	inner;
	uint16_t	outer;
	fmatrix		xform;
	uint32_t	base_idx;
	unsigned	state;
};

temp_box::temp_box(const cf_face& face, const cf_vertex_vec& verts): state(STATE_COLLECTING)
{
	fvector3 edge[3];
	for (uint_fast32_t i = 3; i != 0;) {
		--i;
		edge[i].sub(verts[face.v[(i+1)%3]].p, verts[face.v[i]].p);
	}
	for (uint_fast32_t i = 3; i != 0;) {
		uint_fast32_t j = i%3;
		if (equivalent(edge[--i].cosine(edge[j]), 0.f, k_zero_tolerance)) {
			xform.i.invert(edge[i]);
			xform.j.set(edge[j]);
			xform.c.set(verts[base_idx = face.v[j]].p);
			inner = face.sector;
			outer = face.material;
			return;
		}
	}
	xr_not_expected();
}

void temp_box::collect(const cf_face& face, const cf_vertex_vec& verts)
{
	// FIXME: this code will not work in a (legal) situation with intersecting
	// sound volumes. So far I am not aware of any real examples, though.
	xr_assert(inner == face.sector);
	xr_assert(outer == face.material);
	if (state == STATE_DONE)
		return;
	for (uint_fast32_t k = 3; k != 0;) {
		if (face.v[--k] != base_idx)
			continue;
		fvector3 edge[2];
		edge[0].sub(verts[face.v[(k+1)%3]].p, verts[face.v[k]].p);
		edge[1].sub(verts[face.v[(k+2)%3]].p, verts[face.v[k]].p);
		for (uint_fast32_t i = 2; i != 0;) {
			--i;
			if (equivalent(xform.i.cosine(edge[i]), 0.f, k_zero_tolerance) &&
					equivalent(xform.j.cosine(edge[i]), 0.f, k_zero_tolerance)) {
				xform.k.set(edge[i]);
				state = STATE_DONE;
				xform.c.add(fvector3().add(xform.i, xform.j).add(xform.k).mul(0.5f));
				return;
			}
		}
		break;
	}
}

void level_tools::reconstruct_sound_envs()
{
	const xr_level_snd_env* snd_env = m_level->snd_env();
	if (snd_env == 0)
		return;
	msg("building %s", "sound_env.part");

	const cf_face_vec& faces = snd_env->faces();
	const cf_vertex_vec& verts = snd_env->vertices();

	std::vector<temp_box*> boxes;
	boxes.reserve(faces.size()/12);
	std::vector<uint32_t> adjacents;
	adjacents.reserve(12);

	uint32_t* groups = new uint32_t[faces.size()];
	xr_uninitialized_fill_n(groups, faces.size(), BAD_IDX);
	uint32_t group = 0;
	for (size_t k = faces.size(); k != 0;) {
		if (groups[--k] != BAD_IDX)
			continue;
		bool new_group = true;
		for (uint_fast32_t face_idx = uint_fast32_t(k & UINT32_MAX);;) {
			const cf_face& face = faces[face_idx];
			if (new_group) {
				boxes.push_back(new temp_box(face, verts));
				new_group = false;
			} else {
				boxes.back()->collect(face, verts);
			}
			groups[face_idx] = group;
			for (uint_fast32_t i = 3; i != 0;) {
				uint32_t v = face.v[--i];
				for (uint_fast32_t next = verts[v].face0; next != BAD_IDX;) {
					if (next != face_idx && groups[next] == BAD_IDX) {
						adjacents.push_back(uint32_t(next & UINT32_MAX));
						groups[next] = BAD_IDX;
					}
					next = faces[next].next_face_idx(v);
				}
			}
			if (adjacents.empty())
				break;
			face_idx = uint32_t(adjacents.back());
			adjacents.pop_back();
		}
	}
	delete[] groups;

	xr_custom_object_vec& cobjects = m_scene->sound_envs()->objects();
	cobjects.reserve(boxes.size());

	const std::vector<std::string>& env_ids = snd_env->env_ids();

	xr_name_gen name("sound_env");
	for (std::vector<temp_box*>::iterator it = boxes.begin(), end = boxes.end();
			it != end; ++it, name.next()) {
		temp_box* box = *it;
		xr_sound_env_object* new_sound_env = new xr_sound_env_object(*m_scene);
		new_sound_env->co_name() = name.get();
		new_sound_env->co_position() = box->xform.c;
		box->xform.get_xyz(new_sound_env->co_rotation());
		new_sound_env->co_scale().set(box->xform.i.normalize_magn(),
				box->xform.j.normalize_magn(), box->xform.k.normalize_magn());
		new_sound_env->inner() = env_ids.at(box->inner);
		new_sound_env->outer() = env_ids.at(box->outer);
		delete box;
		cobjects.push_back(new_sound_env);
	}
	m_level->clear_snd_env();
}
