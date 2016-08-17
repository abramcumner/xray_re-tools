#include <list>
#ifdef _MSC_VER
#include <hash_map>
#else
#include <map>
#endif
#include "xr_cform.h"
#include "xr_utils.h"
#include "level_mesh.h"

using namespace xray_re;

void level_mesh::create_debug_face(uint32_t v0, uint32_t v1)
{
	if (!m_debug_cform)
		return;
	size_t count = 0;
	fvector3 offset;
	offset.set();
	for (uint_fast32_t next = m_vertex_faces[v0]; next != BAD_IDX;) {
		const b_face& face = m_faces[next];
		uint_fast32_t i0 = face.local_vert_idx(v0);
		if (face.v[(i0 + 1)%3] == v1 || face.v[(i0 + 2)%3] == v1) {
			offset.add(fvector3().calc_normal(m_points[face.v0],
					m_points[face.v1], m_points[face.v2]));
			++count;
		}
		next = face.link[i0];
	}
	fvector3 p2;
	if (count == 0) {
		p2.set(0, 0, 0);
	} else {
		float koeff = 0.2f*p2.sub(m_points[v0], m_points[v1]).magnitude();
		if (koeff > 0.2f)
			koeff = 0.2f;
		else if (koeff < 0.03f)
			koeff = 0.03f;
		p2.average(m_points[v0], m_points[v1]).add(offset.mul(koeff/count));
	}
	uint32_t v2 = uint32_t(m_points.size() & UINT32_MAX);
	m_points.push_back(p2);
	b_face face;
	face.v0 = v0;
	face.v1 = v1;
	face.v2 = v2;
	face.tc0 = 0;
	face.tc1 = 0;
	face.tc2 = 0;
	face.n0 = 0;
	face.n1 = 0;
	face.n2 = 0;
	face.surface.set(0, 0, 0, RSF_TWO_SIDED|RSF_RENDERING|RSF_COLLISION|RSF_DEBUG);
	m_faces.push_back(face);
	m_vertex_faces.push_back(BAD_IDX);
}

bool level_mesh::search_colinear_path(uint32_t v0, uint32_t v1)
{
	fvector3 px, span;
	span.sub(m_points[v0], m_points[v1]);

	m_path.assign(1, v1);
	for (uint_fast32_t vx = v1, vm;; vx = vm) {
		px.set(m_points[vm = vx]);
		for (uint_fast32_t next = m_vertex_faces[vx]; next != BAD_IDX;) {
			const b_face& face = m_faces[next];
			uint_fast32_t ix = face.local_vert_idx(vx), vt;
			if (equivalent(span.cosine(fvector3().sub(m_points[vt = face.v[(ix + 1)%3]], px)), 1.f, 2e-6f)) {
				vm = vt;
				break;
			}
			if (equivalent(span.cosine(fvector3().sub(m_points[vt = face.v[(ix + 2)%3]], px)), 1.f, 2e-6f)) {
				vm = vt;
				break;
			}
			next = face.link[ix];
		}
		if (vm == vx)
			return false;
		m_path.push_back(uint32_t(vm & UINT32_MAX));
		if (vm == v0)
			return true;
	}
	return false;
}

bool level_mesh::search_path(uint32_t v0, uint32_t v1)
{
	std::vector<bool> used(m_points.size(), false);
	std::vector<bool> pushed(m_points.size(), false);

	m_costs[v0] = 0;
	float& cost_v1 = m_costs[v1];
	cost_v1 = xr_numeric_limits<float>::max();

	for (std::list<uint32_t> pool(1, v0); !pool.empty();) {
		float best_cost = m_costs[pool.front()];
		std::list<uint32_t>::iterator best_it = pool.begin();
		for (std::list<uint32_t>::iterator it = best_it, end = pool.end(); ++it != end;) {
			xr_assert(used[*it] == false);
			if (best_cost > m_costs[*it]) {
				best_cost = m_costs[*it];
				best_it = it;
			}
		}
		uint32_t vx = *best_it;
		pool.erase(best_it);
		used[vx] = true;

		fvector3 px;
		px.set(m_points[vx]);

		for (uint_fast32_t next = m_vertex_faces[vx]; next != BAD_IDX;) {
			const b_face& face = m_faces[next];
			uint_fast32_t ix = face.local_vert_idx(vx), vt;
			if (!used[vt = face.v[(ix + 1)%3]]) {
				float cost = px.distance(m_points[vt]) + m_costs[vx];
				if (pushed[vt]) {
					if (m_costs[vt] > cost)
						m_costs[vt] = cost;
				} else {
					m_costs[vt] = cost;
					if (cost < cost_v1)
						pool.push_back(uint32_t(vt & UINT32_MAX));
					pushed[vt] = true;
				}
			}
			if (!used[vt = face.v[(ix + 2)%3]]) {
				float cost = px.distance(m_points[vt]) + m_costs[vx];
				if (pushed[vt]) {
					if (m_costs[vt] > cost)
						m_costs[vt] = cost;
				} else {
					m_costs[vt] = cost;
					if (cost < cost_v1)
						pool.push_back(uint32_t(vt & UINT32_MAX));
					pushed[vt] = true;
				}
			}
			next = face.link[ix];
		}
	}
	if (cost_v1 == xr_numeric_limits<float>::max()) {
		const fvector3& p0 = m_points[v0];
		const fvector3& p1 = m_points[v1];
		msg("no edge path from %"PRIu32" (%f, %f, %f) to %"PRIu32" (%f, %f, %f)",
				v0, p0.x, p0.y, p0.z, v1, p1.x, p1.y, p1.z);
//		create_debug_face(v0, v1);
		return false;
	}

	m_path.assign(1, v1);
	for (uint_fast32_t vx = v1, vm = UINT32_MAX; vx != v0;) {
		fvector3 px;
		px.set(m_points[vx]);
		const float rest = m_costs[vx] + 1e-4f;
		for (uint32_t next = m_vertex_faces[vx]; next != BAD_IDX;) {
			const b_face& face = m_faces[next];
			uint_fast32_t ix = face.local_vert_idx(vx), vt;
			if (used[vt = face.v[(ix + 1)%3]]) {
				if (m_costs[vt] + px.distance(m_points[vt]) < rest) {
					vm = vt;
					break;
				}
			}
			if (used[vt = face.v[(ix + 2)%3]]) {
				if (m_costs[vt] + px.distance(m_points[vt]) < rest) {
					vm = vt;
					break;
				}
			}
			next = face.link[ix];
		}
		xr_assert(vm != UINT32_MAX);
		vx = vm;
		m_path.push_back(uint32_t(vx & UINT32_MAX));
	}
	return true;
}

void level_mesh::paint_neighbours(const b_span& span, uint32_t v0, uint32_t v1, const fvector3 mesh_normals[])
{
	bool full_test = (span.border && span.size() > 1) || span.size() > 2;
	for (uint_fast32_t next = m_vertex_faces[v0]; next != BAD_IDX;) {
		b_face& face = m_faces[next];
		bool side;
		uint_fast32_t i0 = face.local_vert_idx(v0);
		if (face.v[(i0 + 1)%3] == v1)
			side = true;
		else if (face.v[(i0 + 2)%3] == v1)
			side = false;
		else
			goto skip;
		if (full_test) {
			const fvector3& normal = mesh_normals[next];
			for (b_neighbour_vec_cit it = span.begin(), end = span.end(); it != end; ++it) {
				if (it->side == side && it->normal->similar(normal, 1e-1f)) {
					face.surface.gamemtl = it->gamemtl;
					face.surface.flags |= RSF_COLLISION;
				}
			}
		} else {
			for (b_neighbour_vec_cit it = span.begin(), end = span.end(); it != end; ++it) {
				if (it->side == side) {
					face.surface.gamemtl = it->gamemtl;
					face.surface.flags |= RSF_COLLISION;
				}
			}
		}
skip:
		next = face.link[i0];
	}
}

void level_mesh::propagate_span(const b_span& span, const fvector3 mesh_normals[])
{
	uint32_t v0 = span.v0, v1 = span.v1;
	b_edge* edge = find_edge(v0, v1);
	if (edge) {
		edge->border = span.border;
		paint_neighbours(span, v0, v1, mesh_normals);
		if (false && span.border)
			create_debug_face(v0, v1);
	} else {
		bool colinear = search_colinear_path(v0, v1);
		if (!colinear && (!span.border || !search_path(v0, v1)))
			return;
		for (std::vector<uint32_t>::const_reverse_iterator it = m_path.rbegin() + 1,
				end = m_path.rend(); it != end; ++it, v0 = v1) {
			edge = find_edge(v0, v1 = *it);
			xr_assert(edge);
			edge->border = span.border;
			if (span.border || colinear)
				paint_neighbours(span, v0, v1, mesh_normals);
			if (false && span.border)
				create_debug_face(v0, v1);
		}
	}
}

void level_mesh::update_span(b_span& span, uint_fast32_t v0, uint_fast32_t v1,
		const fvector3* normal, uint16_t gamemtl)
{
	bool side;
	if (span.empty()) {
		span.v0 = uint32_t(v0 & UINT32_MAX);
		span.v1 = uint32_t(v1 & UINT32_MAX);
		span.border = true;
		side = true;
	} else {
		if (v0 == span.v0) {
			xr_assert(v1 == span.v1);
			side = true;
		} else {
			xr_assert(v0 == span.v1 && v1 == span.v0);
			side = false;
		}
		if (span.size() == 1)
			span.border = false;
		for (b_neighbour_vec_cit it = span.begin(),
				end = span.end(); it != end; ++it) {
			if (it->gamemtl != gamemtl) {
				span.border = true;
				break;
			}
		}
	}
	span.push_back(b_neighbour(normal, gamemtl, side));
}

uint32_t* level_mesh::remap_cform_verts(const xr_cform& cform) const
{
	const cf_vertex_vec& verts = cform.vertices();
	msg("mapping %"PRIuSIZET" CFORM verts to %"PRIuSIZET" visual", verts.size(), m_points.size());

	uint32_t* cf2m = new uint32_t[verts.size()];
	xr_uninitialized_fill_n(cf2m, verts.size(), BAD_IDX);

	size_t num_matches = 0;
	std::vector<fvector3>::const_iterator begin1 = m_points.begin(), end1 = m_points.end();
	for (cf_vertex_vec_cit it0 = verts.begin(), it = it0, end = verts.end(); it != end; ++it) {
		fvector3 p;
		std::vector<fvector3>::const_iterator it1 = std::lower_bound(begin1, end1, p.sub(it->p, 1e-4f)), best;
		float max_dist = xr_numeric_limits<float>::max(), dist;
		for (p.add(it->p, 1e-4f); it1 != end1 && *it1 < p; ++it1) {
			if ((dist = it1->distance(it->p)) < max_dist) {
				max_dist = dist;
				best = it1;
			}
		}
		if (max_dist < 5e-5f) {
			cf2m[it - it0] = uint32_t((best - begin1) & UINT32_MAX);
			++num_matches;
		}
	}
	msg("got %"PRIuSIZET" matches", num_matches);
	return cf2m;
}

void level_mesh::match_edges_faces(const xr_cform& cform, const uint32_t cf2m[])
{
	msg("matching edges/faces");
	m_vertex_faces.assign(m_points.size(), BAD_IDX);
	m_vertex_edges.assign(m_points.size(), BAD_IDX);
	m_edges.clear();
	uint32_t face_idx = 0;
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end();
			it != end; ++it, ++face_idx) {
		for (uint_fast32_t i = 3, v0, v1 = it->v0; i != 0; v1 = v0) {
			b_edge* edge = find_edge(v0 = it->v[--i], v1);
			if (edge == 0) {
				uint32_t next = m_vertex_edges[v0];
				m_vertex_edges[v0] = uint32_t(m_edges.size() & UINT32_MAX);
				m_edges.push_back(b_edge(v1, face_idx, next));
			} else {
				++edge->size;
			}
		}
		it->link0 = m_vertex_faces[it->v0];
		it->link1 = m_vertex_faces[it->v1];
		it->link2 = m_vertex_faces[it->v2];
		m_vertex_faces[it->v0] = face_idx;
		m_vertex_faces[it->v1] = face_idx;
		m_vertex_faces[it->v2] = face_idx;
	}

	m_costs = new float[m_points.size()];
	xr_uninitialized_fill_n(m_costs, m_points.size(), xr_numeric_limits<float>::max());

	const cf_vertex_vec& verts = cform.vertices();
	const cf_face_vec& faces = cform.faces();

	fvector3* cform_normals = new fvector3[faces.size()];
	fvector3* normal = cform_normals;
	for (cf_face_vec_cit it = faces.begin(), end = faces.end(); it != end; ++it, ++normal)
		normal->calc_normal(verts[it->v0].p, verts[it->v1].p, verts[it->v2].p);

	fvector3* mesh_normals = new fvector3[m_faces.size()];
	normal = mesh_normals;
	for (b_face_vec_cit it = m_faces.begin(), end = m_faces.end(); it != end; ++it, ++normal)
		normal->calc_normal(m_points[it->v0], m_points[it->v1], m_points[it->v2]);

#ifdef _MSC_VER
	stdext::hash_map<uint_fast32_t, b_span> spans;
#else
	std::map<uint_fast32_t, b_span> spans;
#endif
	std::vector<bool> done(faces.size(), false);
	for (uint_fast32_t v0 = uint32_t(verts.size() & UINT32_MAX), v0m; v0 != 0;) {
		if ((v0m = cf2m[--v0]) == BAD_IDX)
			continue;
		for (uint_fast32_t next = verts[v0].face0; next != BAD_IDX;) {
			const cf_face& face = faces[next];
			uint_fast32_t i0 = face.local_vert_idx(v0), vx, v1m, v2m;
			v1m = cf2m[vx = face.v[(i0 + 1)%3]];
			if (v0 < vx && v1m != BAD_IDX)
				update_span(spans[vx], v0m, v1m, &cform_normals[next], face.material);
			v2m = cf2m[vx = face.v[(i0 + 2)%3]];
			if (v0 < vx && v2m != BAD_IDX)
				update_span(spans[vx], v2m, v0m, &cform_normals[next], face.material);
#if 1
			if (v1m != BAD_IDX && v2m != BAD_IDX && !done[next]) {
				done[next] = true;
				for (uint_fast32_t link = m_vertex_faces[v0m]; link != BAD_IDX;) {
					b_face& tri = m_faces[link];
					uint_fast32_t i0m = tri.local_vert_idx(v0m);
					if (tri.sector == face.sector && tri.v[(i0m + 1)%3] == v1m &&
							tri.v[(i0m + 2)%3] == v2m) {
						tri.surface.gamemtl = face.material;
						tri.surface.flags |= RSF_COLLISION;
						break;
					}
					link = tri.link[i0m];
				}
			}
#endif
			next = face.link[i0];
		}
#ifdef _MSC_VER
		for (stdext::hash_map<uint_fast32_t, b_span>::iterator
#else
		for (std::map<uint_fast32_t, b_span>::iterator
#endif
				 it = spans.begin(), end = spans.end(); it != end; ++it) {
			propagate_span(it->second, mesh_normals);
		}
		spans.clear();
	}
	delete[] m_costs; m_costs = 0;
	delete[] cform_normals;
	delete[] mesh_normals;
}

void level_mesh::propagate_gamemtls()
{
	msg("propagating game materials");

	for (b_edge_vec_it it = m_edges.begin(), end = m_edges.end(); it != end; ++it)
		it->face0 = BAD_IDX;
	uint32_t face_idx = 0;
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it, ++face_idx) {
		if (it->surface.debug())
			continue;
		for (uint_fast32_t i = 3, v0, v1 = it->v0; i != 0; v1 = v0) {
			b_edge* edge = find_edge(v0 = it->v[--i], v1);
			xr_assert(edge);
			it->link[i] = edge->face0;
			edge->face0 = face_idx;
		}
	}

#if 1
	std::vector<uint32_t> adjacents;
	adjacents.reserve(256);
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		if (it->surface.debug())
			continue;
		uint16_t gamemtl = it->surface.gamemtl;
		if (gamemtl == UINT16_MAX)
			continue;
		uint16_t eshader = it->surface.eshader;
		for (face_idx = uint32_t((it - m_faces.begin()) & UINT32_MAX);;) {
			const b_face& face = m_faces[face_idx];
			for (uint_fast32_t i = 3, v0, v1 = face.v0; i != 0; v1 = v0) {
				b_edge* edge = find_edge(v0 = face.v[--i], v1);
				xr_assert(edge);
				if (edge->border)
					continue;
				for (uint32_t next = edge->face0; next != BAD_IDX;) {
					b_face& adjacent = m_faces[next];
					if (next != face_idx && !adjacent.surface.debug() &&
							adjacent.surface.eshader == eshader &&
							adjacent.surface.gamemtl == UINT16_MAX) {
						adjacents.push_back(next);
						adjacent.surface.gamemtl = gamemtl;
						adjacent.surface.flags |= RSF_COLLISION;
					}
					next = adjacent.next_face_idx(v0, v1);
				}
			}
			if (adjacents.empty())
				break;
			face_idx = adjacents.back();
			adjacents.pop_back();
		}
	}
#endif

	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		if (it->surface.gamemtl == UINT16_MAX)
			it->surface.gamemtl = 0;
	}
}

void level_mesh::restore_game_materials(const xr_cform& cform)
{
	uint32_t* cf2m = remap_cform_verts(cform);
	match_edges_faces(cform, cf2m);
	propagate_gamemtls();

	msg("restoring fake colliders");
	b_face face_template;
	size_t num_normals = m_normals.size(), num_faces = m_faces.size();
	const cf_vertex* verts = &cform.vertices()[0];
	for (cf_face_vec_cit it = cform.faces().begin(), end = cform.faces().end(); it != end; ++it) {
		unsigned ghost = 0;
		for (uint32_t i = 3, vx; i != 0;) {
			const uint32_t VF_SHARED = 0x80000000;
			uint32_t vxm = cf2m[vx = it->v[--i]];
			if (vxm == BAD_IDX) {
				vxm = uint32_t(m_points.size() & UINT32_MAX);
				m_points.push_back(verts[vx].p);
				cf2m[vx] = vxm | VF_SHARED;
				++ghost;
			} else if (vxm & VF_SHARED) {
				vxm &= ~VF_SHARED;
				++ghost;
			}
			face_template.v[i] = vxm;
		}
		if (ghost) {
			face_template.tc0 = face_template.tc1 = face_template.tc2 = 0;
			face_template.n0 = face_template.n1 = face_template.n2 =
					uint32_t(m_normals.size() & UINT32_MAX);
			face_template.sector = it->sector;
			face_template.surface.set(0, 0, it->material, RSF_COLLISION|RSF_AUTOMATIC);
			m_faces.push_back(face_template);
			m_normals.push_back(fvector3().calc_normal(m_points[face_template.v0],
					m_points[face_template.v1], m_points[face_template.v2]));
		}
	}
	delete[] cf2m;
	if (num_normals != m_normals.size()) {
		remove_back_faces(1e-3f);

		uint32_t tc_base = uint32_t(m_texcoords.size() & UINT32_MAX);
		m_texcoords.push_back(fvector2().set(0.f, 0.f));
		m_texcoords.push_back(fvector2().set(0.f, 1.f));
		m_texcoords.push_back(fvector2().set(1.f, 0.f));

		for (b_face_vec_it it = m_faces.begin() + num_faces, end = m_faces.end(); it != end; ++it) {
			if (it->n0 >= num_normals) {
				it->tc0 = tc_base;
				it->tc1 = tc_base + 1;
				it->tc2 = tc_base + 2;
			}
		}
	}
}
