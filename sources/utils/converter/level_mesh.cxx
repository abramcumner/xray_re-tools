#include <algorithm>
#include "xr_limits.h"
#include "xr_object.h"
#include "xr_utils.h"
#include "xr_mesh_utils.h"
#include "xr_remapper.h"
#include "level_mesh_opcode.h"
#ifdef USE_WIN32_MT
#include <process.h>
#endif

using namespace Opcode;
using namespace xray_re;

b_model_instance::b_model_instance(uint32_t _tag, uint16_t _sector):
	tag(_tag), model_id(UINT16_MAX), sector(_sector),
	num_faces(0),
	min_vert(UINT32_MAX), max_vert(0), min_face(UINT32_MAX), max_face(0)
{
	aabb.invalidate();
	xform.identity();
}

void b_model_instance::update_vertices(uint_fast32_t vert_idx)
{
	if (min_vert > vert_idx)
		min_vert = uint32_t(vert_idx & UINT32_MAX);
	if (max_vert < vert_idx)
		max_vert = uint32_t(vert_idx & UINT32_MAX);
}

void b_model_instance::update_faces(uint_fast32_t face_idx)
{
	if (min_face > face_idx)
		min_face = uint32_t(face_idx & UINT32_MAX);
	if (max_face < face_idx)
		max_face = uint32_t(face_idx & UINT32_MAX);
}

////////////////////////////////////////////////////////////////////////////////

const fmatrix& b_model::xform(size_t instance) const
{
	xr_assert(m_locked && instance < m_num_instances);
	return m_instances[instance]->xform;
}

uint16_t b_model::sector(size_t instance) const
{
	xr_assert(m_locked && instance < m_num_instances);
	return m_instances[instance]->sector;
}

uint16_t b_model::model_id() const
{
	xr_assert(m_locked);
	return m_instances[0]->model_id;
}

////////////////////////////////////////////////////////////////////////////////

level_mesh::level_mesh(size_t vb_size, size_t ib_size):
	m_debug_cform(false), m_debug_merge(false),
	m_restricted_merge(false), m_aggressive_merge(false),
	m_use_mt(false),
	m_glue_factor(0.008f),
	m_costs(0),
	m_free_tag(0)
{
	m_vb.set_signature(xray_re::xr_vbuf::S_POINTS|xray_re::xr_vbuf::S_NORMALS|xray_re::xr_vbuf::S_TEXCOORDS);
	m_vb.reserve(vb_size);
	m_refs.reserve(vb_size);
	m_faces.reserve(ib_size/3);
}

level_mesh::~level_mesh()
{
	delete m_costs;
	delete_elements(m_instances);
}

void level_mesh::push(uint16_t sector, const xr_vbuf& vb, const xr_ibuf& ib,
		uint16_t texture, uint16_t eshader)
{
	b_face face_template(sector);
	face_template.surface.set(texture, eshader);
	if (vb.has_lightmaps())
		face_template.surface.flags |= RSF_LIGHTMAP;
	xr_mesh_builder::__push(vb, ib, 0, face_template);
}

void level_mesh::push(uint16_t sector, const xr_vbuf& vb, const xr_ibuf& ib, const fmatrix& xform,
		uint16_t texture, uint16_t eshader, uint16_t gamemtl)
{
	b_face face_template(sector);
	face_template.surface.set(texture, eshader, gamemtl, RSF_COLLISION|RSF_RENDERING);
	if (vb.has_lightmaps())
		face_template.surface.flags |= RSF_LIGHTMAP;
	xr_mesh_builder::__push(vb, ib, &xform, face_template);
}

void level_mesh::push(uint32_t tag, const xr_vbuf& vb, const xr_ibuf& ib, const fmatrix& xform,
		uint16_t texture, uint16_t eshader)
{
	b_face face_template(m_mu_instances.at(tag).sector, tag);
	face_template.surface.set(texture, eshader);
	if (vb.has_lightmaps())
		face_template.surface.flags |= RSF_LIGHTMAP;
	xr_mesh_builder::__push(vb, ib, &xform, face_template);
}


////////////////////////////////////////////////////////////////////////////////

uint32_t level_mesh::instantiate_mu_model(uint16_t model_id, uint16_t sector, const fmatrix& xform)
{
	m_mu_instances.push_back(b_mu_instance(model_id, sector, xform));
	return m_free_tag++;
}

void level_mesh::lock_models()
{
	for (b_model_vec_it it = m_models.begin(), end = m_models.end(); it != end; ++it) {
		xr_assert(!it->m_locked);
		it->m_locked = true;
		it->m_instances = &m_instances[it->m_temp_index];
	}
}

void level_mesh::validate_instances_order()
{
	xr_assert(m_free_tag == m_instances.size());
	uint32_t tag = 0;
	for (b_model_instance_vec_it it = m_instances.begin(), end = m_instances.end(); it != end; ++it, ++tag)
		xr_assert((*it)->tag == tag);
}

struct model_id_pred { bool operator()(const b_model_instance* l, const b_model_instance* r) const {
	return l->model_id < r->model_id;
}};

void level_mesh::collect_mu_models(size_t num_mu_instances)
{
	if (num_mu_instances) {
		b_model_instance_vec_it it0 = m_instances.begin(), it = it0, end = it + num_mu_instances;
		std::sort(it, end, model_id_pred());
		size_t num_instances = 1, instance0 = 0;
		for (uint16_t model_id = (*it++)->model_id; it != end; ++it, ++num_instances) {
			if ((*it)->model_id == model_id)
				continue;
			m_models.push_back(b_model(b_model::MT_MU_MODEL, num_instances, instance0));
			model_id = (*it)->model_id;
			instance0 = it - it0;
			num_instances = 0;
		}
		m_models.push_back(b_model(b_model::MT_MU_MODEL, num_instances, instance0));
	}
}

void level_mesh::create_sector_models()
{
	size_t num_sectors = 0;
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		if (num_sectors < it->sector)
			num_sectors = it->sector;
		if (it->tag == BAD_IDX)
			it->tag = m_free_tag + it->sector;
	}
	++num_sectors;
	size_t num_instances = m_instances.size(), num_mu_instances = m_mu_instances.size();
	if (num_mu_instances) {
		m_instances.reserve(num_instances + num_mu_instances + num_sectors);
		m_instances.insert(m_instances.begin(), num_mu_instances, 0);
		for (b_model_vec_it it = m_models.begin(), end = m_models.end(); it != end; ++it)
			it->m_temp_index += num_mu_instances;
		size_t num_mu_models = 0;
		uint32_t tag = 0;
		for (b_mu_instance_vec_it it = m_mu_instances.begin(),
				end = m_mu_instances.end(); it != end; ++it, ++tag) {
			b_model_instance* instance = new b_model_instance(tag, it->sector);
			if (num_mu_models < it->model_id)
				num_mu_models = it->model_id;
			instance->model_id = it->model_id;
			instance->xform = it->xform;
			m_instances[tag] = instance;
		}
		++num_mu_models;
		b_mu_instance_vec().swap(m_mu_instances);
		m_models.reserve(m_models.size() + num_sectors + num_mu_models);
	} else {
		m_instances.reserve(num_instances + num_sectors);
		m_models.reserve(m_models.size() + num_sectors);
	}
	for (uint16_t sector = 0; sector < num_sectors; ++sector) {
		b_model_instance* instance = new b_model_instance(m_free_tag + sector, sector);
		m_models.push_back(b_model(b_model::MT_RAW, m_instances.size()));
		m_instances.push_back(instance);
	}
	m_free_tag += uint32_t(num_sectors & UINT32_MAX);

	validate_instances_order();

	uint32_t face_idx = 0;
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end();
			it != end; ++it, ++face_idx) {
		xr_assert(it->tag != BAD_IDX);
		xr_assert(it->tag < m_instances.size());
		b_model_instance* instance = m_instances.at(it->tag);
		++instance->num_faces;
		instance->update_faces(face_idx);
		for (uint_fast32_t i = 3; i != 0;) {
			uint32_t vx = it->v[--i];
			instance->update_vertices(vx);
			instance->aabb.extend(m_points[vx]);
		}
	}

	pre_commit();
	collect_mu_models(num_mu_instances);
	lock_models();
}

void level_mesh::change_tag(uint32_t tag, uint32_t min_face, uint32_t max_face, uint32_t new_tag)
{
	for (b_face_vec_it it = m_faces.begin() + min_face,
			end = m_faces.begin() + max_face + 1; it != end; ++it) {
		if (it->tag == tag)
			it->tag = new_tag;
	}
}

static inline bool is_fake(uint16_t flags) { return (flags & ~RSF_TWO_SIDED) == (RSF_COLLISION|RSF_AUTOMATIC); }

size_t level_mesh::create_submeshes(b_submesh_vec& mu_submeshes, b_submesh_vec& fake_submeshes, b_submesh_vec& submeshes)
{
	size_t num_mu_instances = m_mu_instances.size(), num_immediate_joins = 0;

	create_edges();
	std::vector<uint32_t> adjacents;
	adjacents.reserve(1024);
	uint32_t tag = 0x80000000;
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		if (it->tag != BAD_IDX)
			continue;
		bool fake = is_fake(it->surface.flags);
		it->tag = tag;
		uint32_t min_face = UINT32_MAX, max_face = 0, fake_owner = BAD_IDX;
		for (uint32_t face_idx = uint32_t((it - m_faces.begin()) & UINT32_MAX);;) {
			if (min_face > face_idx)
				min_face = face_idx;
			if (max_face < face_idx)
				max_face = face_idx;
			const b_face& face = m_faces[face_idx];
			for (uint_fast32_t i = 3, v0, v1 = face.v0; i != 0; v1 = v0) {
				const b_edge* edge = find_edge(v0 = face.v[--i], v1);
				xr_assert(edge);
				for (uint32_t next = edge->face0; next != BAD_IDX;) {
					b_face& adjacent = m_faces[next];
					if (next != face_idx && face.sector == adjacent.sector) {
						if (fake && adjacent.tag < num_mu_instances) {
							// it is possible to switch owner and re-tag faces here,
							// but let do this later when we are sure everything is fine.
							xr_assert(fake_owner == BAD_IDX || fake_owner == adjacent.tag);
							fake_owner = adjacent.tag;
						} else if (adjacent.tag == BAD_IDX) {
							if (fake)
								fake = is_fake(adjacent.surface.flags);
							adjacents.push_back(next);
							adjacent.tag = tag;
						}
					}
					next = adjacent.next_face_idx(v0, v1);
				}
			}
			if (adjacents.empty())
				break;
			face_idx = adjacents.back();
			adjacents.pop_back();
		}
		if (fake) {
			if (fake_owner == BAD_IDX) {
				fake_submeshes.push_back(new b_submesh(tag++, it->sector));
			} else {
				++num_immediate_joins;
				change_tag(tag, min_face, max_face, fake_owner);
			}
		} else {
			submeshes.push_back(new b_submesh(tag++, it->sector));
		}
	}
	msg("immediate fake/MU joins: %" PRIuSIZET, num_immediate_joins);

	size_t num_mu_models = 0;
	if (num_mu_instances) {
		mu_submeshes.reserve(num_mu_instances);
		tag = 0;
		for (b_mu_instance_vec_it it = m_mu_instances.begin(),
				end = m_mu_instances.end(); it != end; ++it, ++tag) {
			b_submesh* sm = new b_submesh(tag, it->sector);
			if (num_mu_models < it->model_id)
				num_mu_models = it->model_id;
			sm->model_id = it->model_id;
			sm->xform = it->xform;
			mu_submeshes.push_back(sm);
		}
		++num_mu_models;
		b_mu_instance_vec().swap(m_mu_instances);
	}
	return num_mu_models;
}

struct x1_pred { bool operator()(const b_submesh* l, const b_submesh* r) const {
	return l->aabb.x1 < r->aabb.x1;
}};

void level_mesh::setup_collision_models(b_submesh_vec& submeshes) const
{
	for (b_submesh_vec_it it = submeshes.begin(), end = submeshes.end(); it != end; ++it) {
		b_submesh* sm = *it;
		if (sm->opc_mesh.GetTris() != null)
			continue;
		if (sm->num_faces > 1) {
			sm->opc_mesh.SetNbTriangles(udword(sm->num_faces & UINT32_MAX));
			sm->opc_mesh.SetStrides(sizeof(b_face), sizeof(fvector3));
			sm->opc_mesh.SetPointers((IndexedTriangle*)&m_faces[sm->min_face],
					(Point*)&m_fat_points[0]);
		} else {
			// FIXME: hack to handle 1-face meshes
			sm->opc_hack[1] = sm->opc_hack[0] = *(IndexedTriangle*)m_faces[sm->min_face].v;
			sm->opc_mesh.SetNbTriangles(2);
			sm->opc_mesh.SetStrides(sizeof(IndexedTriangle), sizeof(fvector3));
			sm->opc_mesh.SetPointers(sm->opc_hack, (Point*)&m_fat_points[0]);
		}
		sm->opc_mesh.SetNbVertices(udword(m_points.size() & UINT32_MAX));
		OPCODECREATE opcc;
		opcc.mIMesh = &sm->opc_mesh;
		BOOL status = sm->opc_model.Build(opcc);
		xr_assert(status);
	}
	std::sort(submeshes.begin(), submeshes.end(), x1_pred());
}

struct tag_pred { bool operator()(const level_mesh::b_face& l, const level_mesh::b_face& r) const {
	return l.tag < r.tag;
//	return l.tag < r.tag || (l.tag == r.tag && l.surface.blob32[0] < r.surface.blob32[0]);
}};

void level_mesh::reorder_faces()
{
	std::sort(m_faces.begin(), m_faces.end(), tag_pred());
}

void level_mesh::account_submeshes(b_submesh_vec& mu_submeshes, b_submesh_vec& fake_submeshes,
		b_submesh_vec& submeshes)
{
	m_fat_points.assign(m_points.size(), fvector3().set(0, 0, 0));
	uint32_t face_idx = 0, last_mu_tag = uint32_t(mu_submeshes.size() & UINT32_MAX);
	b_submesh_vec_it sm_it = submeshes.begin(), sm_end = submeshes.end();
	b_submesh_vec_it fake_sm_it = fake_submeshes.begin(), fake_sm_end = fake_submeshes.end();
	dvector3 v[3], c, n;
	for (b_face_vec_cit it = m_faces.begin(), end = m_faces.end(); it != end;) {
		b_model_instance* sm = 0;
		uint32_t tag = it->tag;
		xr_assert(tag != BAD_IDX);
		bool fatten = false;
		if (tag < last_mu_tag) {
			sm = mu_submeshes[tag];
		} else if (sm_it != sm_end && (*sm_it)->tag == tag) {
			sm = *sm_it++;
			if (!it->surface.automatic())
				fatten = true;
		} else if (fake_sm_it != fake_sm_end && (*fake_sm_it)->tag == tag) {
			sm = *fake_sm_it++;
		} else if (tag < m_free_tag) {
			sm = m_instances.at(tag - last_mu_tag);
		} else {
			xr_not_expected();
		}
		for (; it != end && tag == it->tag; ++it, ++face_idx) {
			if (fatten) {
				for (uint_fast32_t i = 3; i != 0;) {
					--i;
					v[i].set(m_points[it->v[i]]);
				}
				c.calc_center(v[0], v[1], v[2]);
				n.calc_normal(v[0], v[1], v[2]);
				for (unsigned i = 3; i != 0;) {
					--i;
					m_fat_points[it->v[i]].add(fvector3().set(dvector3().sub(v[i], c).normalize_safe().add(n).normalize_safe()));
				}
			}
			for (uint_fast32_t i = 3, vx; i != 0;) {
				vx = it->v[--i];
				sm->aabb.extend(m_points[vx]);
				sm->update_vertices(vx);
			}
			++sm->num_faces;
			sm->update_faces(face_idx);
		}
	}
	for (std::vector<fvector3>::iterator it = m_fat_points.begin(),
			end = m_fat_points.end(), it1 = m_points.begin();
			it != end; ++it, ++it1) {
		if (it->x == 0 && it->y == 0 && it->z == 0)
			it->set(*it1);
		else
			it->set(dvector3().set(*it1).add(dvector3().set(*it).set_magnitude(m_glue_factor)));
	}
}

void level_mesh::merge(b_model_instance* instance, const b_model_instance* instance1)
{
	change_tag(instance1->tag, instance1->min_face, instance1->max_face, instance->tag);
	instance->aabb.merge(instance1->aabb);
	if (instance->min_vert > instance1->min_vert)
		instance->min_vert = instance1->min_vert;
	if (instance->max_vert < instance1->max_vert)
		instance->max_vert = instance1->max_vert;
	if (instance->min_face > instance1->min_face)
		instance->min_face = instance1->min_face;
	if (instance->max_face < instance1->max_face)
		instance->max_face = instance1->max_face;
	instance->num_faces += instance1->num_faces;
}

#ifdef USE_WIN32_MT
unsigned level_mesh::setup_thread_cb(void* arg)
{
	setup_thread_param* param = static_cast<setup_thread_param*>(arg);
	param->__this->setup_collision_models(*param->submeshes);
	return 0;
}
#endif

size_t level_mesh::assign_contact_groups(b_submesh_vec& submeshes, b_collision_vec& contacts,
		b_submesh_vec& orphans)
{
	std::sort(contacts.begin(), contacts.end());
	b_submesh_vec adjacents;
	adjacents.reserve(128);
	uint32_t group = 0;
	for (b_submesh_vec_it it = submeshes.begin(), end = submeshes.end(); it != end; ++it) {
		b_submesh* sm = *it;
		xr_assert(sm);
		if (sm->group != BAD_IDX)
			continue;
		sm->group = group;
		size_t num_faces = sm->num_faces;
		for (;;) {
#if defined(_MSC_VER) && _MSC_VER >= 1600
			b_collision contact(sm, nullptr);
#else
			b_collision contact(sm, 0);
#endif
			for (b_collision_vec_it end1 = contacts.end(),
					it1 = std::lower_bound(contacts.begin(), end1, contact);
					it1 != end1 && sm == it1->first; ++it1) {
				b_submesh* sm1 = it1->second;
				if (sm1->group == BAD_IDX) {
					sm1->group = group;
					adjacents.push_back(sm1);
				}
			}
			if (adjacents.empty())
				break;
			sm = adjacents.back();
			adjacents.pop_back();
			num_faces += sm->num_faces;
		}
		if (num_faces > 1) {
			++group;
		} else {
			*it = 0;
			orphans.push_back(sm);
		}
	}
	b_collision_vec().swap(contacts);
	return size_t(group);
}

void level_mesh::merge_orphans(b_submesh_vec& orphans, size_t num_groups, bool consider_gamemtl)
{
	if (orphans.empty())
		return;
	msg("merging %" PRIuSIZET " orphan(s)", orphans.size());
	uint32_t min_tag = m_free_tag, max_tag = min_tag + uint32_t(num_groups & UINT32_MAX) - 1;
	for (b_submesh_vec_it it = orphans.begin(), end = orphans.end(); it != end;) {
		b_submesh* sm = *it++;
		const b_face* face = &m_faces[sm->min_face];
		fvector3 p;
		p.calc_center(m_points[face->v0], m_points[face->v1], m_points[face->v2]);
		uint32_t tag1 = BAD_IDX, tag2 = tag1;
		float min_dist1 = xr_numeric_limits<float>::max(), min_dist2 = min_dist1;
		for (b_face_vec_it it1 = m_faces.begin(), end1 = m_faces.end(); it1 != end1; ++it1) {
			uint32_t tag;
			if ((tag = it1->tag) >= min_tag && tag <= max_tag && face != &*it1) {
				float dist = fvector3().calc_center(m_points[it1->v0], m_points[it1->v1], m_points[it1->v2]).distance(p);
				if (min_dist1 > dist) {
					min_dist1 = dist;
					tag1 = tag;
				}
				if (consider_gamemtl && min_dist2 > dist &&
						it1->surface.gamemtl == face->surface.gamemtl) {
					min_dist2 = dist;
					tag2 = tag;
				}
			}
		}
		if (consider_gamemtl && tag2 != BAD_IDX) {
			tag1 = tag2;
			min_dist1 = min_dist2;
		}
		msg("nearest: %" PRIu32 " (%.2f)", tag1, min_dist1);
		merge(m_instances.at(tag1), sm);
		delete sm;
	}
	b_submesh_vec().swap(orphans);
}

static bool check_match(const std::vector<uint16_t>& ids, uint16_t match_id)
{
	return std::binary_search(ids.begin(), ids.end(), match_id);
}

static void sort_match(std::vector<uint16_t>& ids)
{
	std::sort(ids.begin(), ids.end());
}

bool level_mesh::check_fake_mu_contacts(const Opcode::AABBTreeCollider& tc,
		const b_submesh* fake, const b_submesh* mu, size_t& max_contacts) const
{
	size_t num_contacts = 0;
	const Pair *pair = tc.GetPairs(), *end = pair + tc.GetNbPairs();
	const b_face* faces = &m_faces[fake->min_face];
	if (fake->num_faces <= 8*sizeof(unsigned long)) {
		unsigned long mask = 0;
		for (; pair != end; ++pair) {
			unsigned long temp = 1ul << pair->id0;
			if ((mask & temp) == 0) {
				mask |= temp;
				if (check_match(m_fake_mu_gamemtls, faces[pair->id0].surface.gamemtl))
					++num_contacts;
			}
		}
	} else {
		std::vector<bool> mask(fake->num_faces, false);
		for (; pair != end; ++pair) {
			if (!mask[pair->id0]) {
				mask[pair->id0] = true;
				if (check_match(m_fake_mu_gamemtls, faces[pair->id0].surface.gamemtl))
					++num_contacts;
			}
		}
	}
	if (max_contacts < num_contacts) {
		max_contacts = num_contacts;
		return true;
	} else {
		return false;
	}
}

bool level_mesh::check_fake_gamemtls(const b_submesh* sm) const
{
	for (b_face_vec_cit it = m_faces.begin() + sm->min_face,
			end = it + sm->num_faces; it != end; ++it) {
		xr_assert(it->tag == sm->tag);
		if (!check_match(m_fake_gamemtls, it->surface.gamemtl))
			return false;
	}
	return true;
}

inline bool level_mesh::check_fake_fake_contacts(const Opcode::AABBTreeCollider& tc,
		const b_submesh* fake1, const b_submesh* fake2) const
{
	const b_face* faces1 = &m_faces[fake1->min_face];
	const b_face* faces2 = &m_faces[fake2->min_face];
	for (const Pair *pair = tc.GetPairs(), *end = pair + tc.GetNbPairs(); pair != end; ++pair) {
		if (faces1[pair->id0].surface.gamemtl != faces2[pair->id1].surface.gamemtl)
			return false;
	}
	return true;
}

void level_mesh::merge_mu_fake_submeshes(b_submesh_vec& mu_submeshes, b_submesh_vec& fake_submeshes,
		b_submesh_vec& residue_submeshes)
{
	// create model instances now to collect the merged MU submeshes
	size_t num_mu_instances = mu_submeshes.size();
	m_instances.reserve(m_instances.size() + num_mu_instances);
	m_instances.insert(m_instances.begin(), num_mu_instances, 0);
	for (b_submesh_vec_it it = mu_submeshes.begin(), end = mu_submeshes.end(); it != end; ++it) {
		b_model_instance* instance = new b_model_instance(**it);
		xr_assert(instance->tag < m_instances.size() && m_instances[instance->tag] == 0);
		m_instances[instance->tag] = instance;
	}
	// ensure there is a space for the MU models
	for (b_model_vec_it it = m_models.begin(), end = m_models.end(); it != end; ++it)
		it->m_temp_index += num_mu_instances;

	AABBTreeCollider tc;
	tc.SetFirstContact(false);
	tc.SetFullBoxBoxTest(false);
	tc.SetFullPrimBoxTest(false);
	tc.SetTemporalCoherence(false);
	BVTCache ccache;

	// prepare for bisection in check_match()
	sort_match(m_fake_mu_gamemtls);
	sort_match(m_fake_gamemtls);

	msg("merging fake/MU submeshes");
	size_t num_residue = 0;
	b_submesh_vec_it new_end = fake_submeshes.begin();
	for (b_submesh_vec_it it = new_end, end = fake_submeshes.end(),
			mu_begin = mu_submeshes.begin(), mu_end = mu_submeshes.end(); it != end; ++it) {
		b_submesh* sm = *it;
		float x2 = sm->aabb.x2;
		size_t max_contacts = 0;
		b_submesh* best = 0;
		for (b_submesh_vec_it it1 = mu_begin; it1 != mu_end && (*it1)->aabb.x1 <= x2; ++it1) {
			b_submesh* sm1 = *it1;
			if (sm->sector != sm1->sector || !sm->aabb.intersect(sm1->aabb))
				continue;
			ccache.ResetCache();
			ccache.Model0 = &sm->opc_model;
			ccache.Model1 = &sm1->opc_model;
			bool status = tc.Collide(ccache, null, null);
			xr_assert(status);
			if (status && tc.GetContactStatus() &&
					check_fake_mu_contacts(tc, sm, sm1, max_contacts)) {
				best = sm1;
			}
		}
		if (best) {
			merge(m_instances[best->tag], sm);
			delete sm;
		} else if (!check_fake_gamemtls(sm)) {
			residue_submeshes.push_back(sm);
			++num_residue;
		} else {
			if (new_end != it)
				*new_end = sm;
			++new_end;
		}
	}
	trim_container(mu_submeshes);
	msg("non-fake (residue) submeshes: %" PRIuSIZET, num_residue);

	msg("cross-merging fake submeshes");
	b_collision_vec contacts;
	contacts.reserve(32);
	for (b_submesh_vec_it it = fake_submeshes.begin(); it != new_end;) {
		b_submesh* sm = *it++;
		float x2 = sm->aabb.x2;
		for (b_submesh_vec_it it1 = it; it1 != new_end && (*it1)->aabb.x1 <= x2; ++it1) {
			b_submesh* sm1 = *it1;
			if (sm->sector != sm1->sector || !sm->aabb.intersect(sm1->aabb))
				continue;
			ccache.ResetCache();
			ccache.Model0 = &sm->opc_model;
			ccache.Model1 = &sm1->opc_model;
			bool status = tc.Collide(ccache, null, null);
			xr_assert(status);
			if (status && tc.GetContactStatus() &&
					check_fake_fake_contacts(tc, sm, sm1)) {
				contacts.push_back(b_collision(sm, sm1));
				contacts.push_back(b_collision(sm1, sm));
			}
		}
	}
	fake_submeshes.erase(new_end, fake_submeshes.end());

	msg("assigning contact groups (fake)");
	b_submesh_vec orphans;
	size_t num_groups = assign_contact_groups(fake_submeshes, contacts, orphans);
	msg("total groups (fake): %" PRIuSIZET, num_groups);

	// put everything in a separate object for now
	m_instances.resize(m_instances.size() + num_groups, 0);
	for (b_submesh_vec_it it = fake_submeshes.begin(), end = fake_submeshes.end(); it != end;) {
		b_submesh* sm = *it++;
		if (sm == 0)
			continue;
		uint32_t tag = m_free_tag + sm->group;
		b_model_instance* instance = m_instances.at(tag);
		if (instance) {
			merge(instance, sm);
		} else {
			instance = new b_model_instance(*sm);
			instance->tag = tag;
			change_tag(sm->tag, sm->min_face, sm->max_face, tag);
			m_instances[tag] = instance;
			m_models.push_back(b_model(b_model::MT_FAKE, tag));
		}
		delete sm;
	}
	b_submesh_vec().swap(fake_submeshes);
	merge_orphans(orphans, num_groups, true);
	m_free_tag += uint32_t(num_groups & UINT32_MAX);
}

bool level_mesh::check_regular_regular_contacts(const Opcode::AABBTreeCollider& tc,
		const b_submesh* sm, const b_submesh* sm1) const
{
	xr_assert(tc.GetNbPairs());
	if (m_restricted_merge) {
		for (const Pair *pair = tc.GetPairs(), *end = pair + tc.GetNbPairs(); pair != end; ++pair) {
			const b_face& face = m_faces[pair->id0 + sm->min_face];
			const b_face& face1 = m_faces[pair->id1 + sm1->min_face];
			if (face.surface.texture != face1.surface.texture)
				return false;
		}
	} else if (m_aggressive_merge) {
		for (const Pair *pair = tc.GetPairs(), *end = pair + tc.GetNbPairs(); pair != end; ++pair) {

			int index = pair->id0 + sm->min_face;
			if (index > sm->max_face)// by // FIXME: hack to handle 1-face meshes
				index = sm->max_face;
			const b_face& face = m_faces[index];
			if (check_match(m_inadhesive_shaders, face.surface.eshader))
				return false;

			index = pair->id1 + sm1->min_face;
			if (index > sm1->max_face)// by // FIXME: hack to handle 1-face meshes
				index = sm1->max_face;

			const b_face& face1 = m_faces[index];
			if (check_match(m_inadhesive_shaders, face1.surface.eshader))
				return false;
		}
	} else {
		for (const Pair *pair = tc.GetPairs(), *end = pair + tc.GetNbPairs(); pair != end; ++pair) {
			const b_face& face = m_faces[pair->id0 + sm->min_face];
			const b_face& face1 = m_faces[pair->id1 + sm1->min_face];
			if (face.surface.texture == face1.surface.texture)
				return true;
		}
		return false;
	}
	return true;
}

void level_mesh::merge_regular_submeshes(b_submesh_vec& submeshes, size_t num_models_hint)
{
	AABBTreeCollider tc;
	tc.SetFirstContact(false);
	tc.SetFullBoxBoxTest(false);
	tc.SetFullPrimBoxTest(false);
	tc.SetTemporalCoherence(false);
	BVTCache ccache;

	// prepare for bisection in check_match()
	sort_match(m_inadhesive_shaders);

	b_collision_vec contacts;
	contacts.reserve(3*submeshes.size());
	msg("merging submeshes (regular)");
	for (b_submesh_vec_it it = submeshes.begin(), end = submeshes.end(); it != end;) {
		b_submesh* sm = *it++;
		fbox aabb;
		float x2 = aabb.set(sm->aabb).grow(m_glue_factor).x2;
		for (b_submesh_vec_it it1 = it; it1 != end && (*it1)->aabb.x1 <= x2; ++it1) {
			b_submesh* sm1 = *it1;
			if (sm->sector != sm1->sector || !aabb.intersect(sm1->aabb))
				continue;
			ccache.ResetCache();
			ccache.Model0 = &sm->opc_model;
			ccache.Model1 = &sm1->opc_model;
			bool status = tc.Collide(ccache, null, null);
			xr_assert(status);
			if (status && tc.GetContactStatus() &&
					check_regular_regular_contacts(tc, sm, sm1)) {
				contacts.push_back(b_collision(sm, sm1));
				contacts.push_back(b_collision(sm1, sm));
			}
		}
	}
	msg("contacts (with redundancy): %" PRIuSIZET, contacts.size());

	msg("assigning contact groups (regular)");
	b_submesh_vec orphans;
	size_t num_groups = assign_contact_groups(submeshes, contacts, orphans);
	msg("total groups (regular): %" PRIuSIZET, num_groups);

	msg("building regular models");
	m_models.reserve(num_models_hint + num_groups);
	m_instances.resize(m_instances.size() + num_groups, 0);
	for (b_submesh_vec_it it = submeshes.begin(), end = submeshes.end(); it != end;) {
		b_submesh* sm = *it++;
		if (sm == 0)
			continue;
		uint32_t tag = m_free_tag + sm->group;
		b_model_instance* instance = m_instances.at(tag);
		if (instance) {
			merge(instance, sm);
		} else {
			instance = new b_model_instance(*sm);
			instance->tag = tag;
			change_tag(sm->tag, sm->min_face, sm->max_face, tag);
			m_instances[tag] = instance;
			m_models.push_back(b_model(b_model::MT_MODEL, tag));
		}
		delete sm;
	}
	b_submesh_vec().swap(submeshes);
	merge_orphans(orphans, num_groups, true);
	m_free_tag += uint32_t(num_groups & UINT32_MAX);
}

void level_mesh::create_models()
{
	size_t num_models_hint = m_models.size(), num_mu_instances = m_mu_instances.size();

	msg("building edge-connected submeshes");
	b_submesh_vec mu_submeshes, fake_submeshes, submeshes;
	num_models_hint += create_submeshes(mu_submeshes, fake_submeshes, submeshes);
	msg("submeshes (MU): %" PRIuSIZET, mu_submeshes.size());
	msg("submeshes (fake): %" PRIuSIZET, fake_submeshes.size());
	msg("submeshes (regular): %" PRIuSIZET, submeshes.size());

	msg("reordering faces");
	reorder_faces();

	msg("accounting submeshes");
	account_submeshes(mu_submeshes, fake_submeshes, submeshes);

#ifdef USE_WIN32_MT
	HANDLE setup_thread = NULL;
	if (m_use_mt) {
		msg("creating collision models (regular) - starting thread");
		setup_thread_param param;
		param.__this = this;
		param.submeshes = &submeshes;
		setup_thread = (HANDLE)_beginthreadex(NULL, 0, &level_mesh::setup_thread_cb, &param, 0, NULL);
		xr_assert(setup_thread != NULL);
	}
#endif
	msg("creating collision models (MU)");
	setup_collision_models(mu_submeshes);
	msg("creating collision models (fake)");
	setup_collision_models(fake_submeshes);

#ifdef USE_WIN32_MT
	if (setup_thread != NULL) {
		b_submesh_vec temp_submeshes;
		merge_mu_fake_submeshes(mu_submeshes, fake_submeshes, temp_submeshes);
		msg("creating collision models (regular) - waiting thread");
		WaitForSingleObject(setup_thread, INFINITE);
		submeshes.insert(submeshes.end(), temp_submeshes.begin(), temp_submeshes.end());
	} else
#endif
	{
		merge_mu_fake_submeshes(mu_submeshes, fake_submeshes, submeshes);
		msg("creating collision models (regular)");
		setup_collision_models(submeshes);
	}
	merge_regular_submeshes(submeshes, num_models_hint);

	if (m_debug_merge) {
		for (b_model_instance_vec_it it = m_instances.begin(),
				end = m_instances.end(); it != end; ++it) {
			(*it)->aabb.grow(m_glue_factor);
		}
		m_points.swap(m_fat_points);
	}
	std::vector<fvector3>().swap(m_fat_points);

	validate_instances_order();

	pre_commit();

	collect_mu_models(num_mu_instances);

	lock_models();
}
