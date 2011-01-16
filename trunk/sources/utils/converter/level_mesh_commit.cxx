#include <ctime>
#include "xr_remapper.h"
#include "xr_object.h"
#include "xr_utils.h"
#include "level_mesh.h"

using namespace xray_re;

void level_mesh::build(float normal_tolerance)
{
	size_t num_faces = m_faces.size();
	msg("compacting %"PRIuSIZET" verts, %"PRIuSIZET" faces", m_vb.size(), num_faces);
	clock_t time = clock();
	xr_mesh_builder::compact_geometry();
	time = clock() - time;
	msg("got %"PRIuSIZET" points, %"PRIuSIZET" normals, %"PRIuSIZET" uvs in %.3fs",
			m_points.size(), m_normals.size(), m_texcoords.size(), float(time)/1000);

	msg("cleaning %s", "geometry");

	time = clock();
	remove_duplicate_faces();
	time = clock() - time;
	msg("removed %"PRIuSIZET" %s faces in %.3fs", num_faces - m_faces.size(), "duplicate", float(time)/1000);

	num_faces = m_faces.size();
	time = clock();
	remove_back_faces(normal_tolerance);
	time = clock() - time;
	msg("removed %"PRIuSIZET" %s faces in %.3fs", num_faces - m_faces.size(), "back", float(time)/1000);
}

void level_mesh::create_mappings(lw_face_vec& faces, lw_vmref_vec& vmrefs, xr_vmap_vec& vmaps,
		const xr_remapper& face_remap, const xr_remapper& vert_remap) const
{
	xr_uv_vmap* uv_vmap = 0;
	xr_face_uv_vmap* face_uv_vmap = 0;
	uint32_t vert_idx = vert_remap.min();
	for (std::vector<uint32_t>::const_iterator it = m_vertex_faces.begin() + vert_idx,
			end = m_vertex_faces.begin() + vert_remap.max() + 1; it != end; ++it, ++vert_idx) {
		if (*it == BAD_IDX)
			continue;
		uint32_t mapped_vert_idx = vert_remap[vert_idx];
		if (mapped_vert_idx == BAD_IDX)
			continue;

		lw_vmref vmref0;
		uint32_t tc0 = BAD_IDX, vmref0_idx = 0;
		for (uint32_t face_idx = *it; face_idx != BAD_IDX;) {
			const b_face& face = m_faces[face_idx];
			uint32_t mapped_face_idx = face_remap[face_idx];
			if (mapped_face_idx == BAD_IDX) {
				face_idx = face.next_face_idx(vert_idx);
				continue;
			}
			uint_fast32_t local_idx = face.local_vert_idx(vert_idx);
			uint32_t tc = face.tc[local_idx], vmref_idx;
			xr_assert(tc != BAD_IDX);
			if (tc0 == BAD_IDX) {
				if (uv_vmap == 0) {
					uv_vmap = new xr_uv_vmap("Texture");
					vmaps.push_back(uv_vmap);
				}
				vmref0.push_back(lw_vmref_entry(0, uv_vmap->add_uv(
						m_texcoords[tc0 = tc], mapped_vert_idx)));
				vmref0_idx = vmref_idx = uint32_t(vmrefs.size() & UINT32_MAX);
				vmrefs.push_back(vmref0);
			} else if (tc0 == tc) {
				vmref_idx = vmref0_idx;
			} else {
				if (face_uv_vmap == 0) {
					face_uv_vmap = new xr_face_uv_vmap("Texture");
					vmaps.push_back(face_uv_vmap);
				}
				lw_vmref vmref;
				vmref.push_back(lw_vmref_entry(1, face_uv_vmap->add_uv(
						m_texcoords[tc], mapped_vert_idx, mapped_face_idx)));
				vmref_idx = uint32_t(vmrefs.size() & UINT32_MAX);
				vmrefs.push_back(vmref);
			}
			faces[mapped_face_idx].ref[local_idx] = vmref_idx;
			face_idx = face.link[local_idx];
		}
	}
#if 0
	for (lw_face_vec_it it = faces.begin(); it != faces.end(); ++it) {
		if (it->ref0 == BAD_IDX) {
			msg("face%u, ref0, verts(%u, %u, %u)", it - faces.begin(), it->v0, it->v1, it->v2);
		}
		if (it->ref1 == BAD_IDX) {
			msg("face%u, ref0, verts(%u, %u, %u)", it - faces.begin(), it->v0, it->v1, it->v2);
		}
		if (it->ref2 == BAD_IDX) {
			msg("face%u, ref0, verts(%u, %u, %u)", it - faces.begin(), it->v0, it->v1, it->v2);
		}
		xr_assert(it->ref0 < vmrefs.size());
		xr_assert(it->ref1 < vmrefs.size());
		xr_assert(it->ref2 < vmrefs.size());
	}
#endif
}

void level_mesh::create_smoothing_groups()
{
	if (m_normals.empty())
		return;

	// FIXME: remove these safety checks
	xr_assert(!m_instances.empty());
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		xr_assert(it->sgroup == EMESH_NO_SG);
		xr_assert(it->tag < m_instances.size());
	}

	create_edges(true);

	uint32_t* next_sgroup = new uint32_t[m_instances.size()];
	xr_uninitialized_fill_n(next_sgroup, m_instances.size(), 0);

	std::vector<uint32_t> adjacents;
	adjacents.reserve(512);
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		if (it->sgroup != EMESH_NO_SG)
			continue;
		uint32_t tag = it->tag, sgroup = next_sgroup[tag];
		xr_assert(m_instances[tag]->tag == tag);
		bool new_sgroup = false;
		for (uint32_t face_idx = uint32_t((it - m_faces.begin()) & UINT32_MAX);;) {
			b_face& face = m_faces[face_idx];
			for (uint_fast32_t i = 3, v0, v1 = face.v0; i != 0; v1 = v0) {
				b_edge* edge = find_edge(v0 = face.v[--i], v1);
				xr_assert(edge);
				if (!edge->smooth)
					continue;
				if (!new_sgroup) {
					new_sgroup = true;
					face.sgroup = sgroup;
				}
				uint32_t adjacent_idx = (edge->face0 == face_idx) ?
						face.link[i] : edge->face0;
				b_face& adjacent = m_faces[adjacent_idx];
				if (adjacent.sgroup == EMESH_NO_SG && adjacent.tag == tag) {
					adjacents.push_back(adjacent_idx);
					adjacent.sgroup = sgroup;
				}
			}
			if (adjacents.empty())
				break;
			face_idx = adjacents.back();
			adjacents.pop_back();
		}
		if (new_sgroup)
			++next_sgroup[tag];
	}
	delete[] next_sgroup;
}

void level_mesh::pre_commit()
{
	xr_assert(!m_instances.empty());
	compact_raw_surfaces();
	create_smoothing_groups();
	std::vector<fvector3>().swap(m_normals);
	b_edge_vec().swap(m_edges);
	std::vector<uint32_t>().swap(m_vertex_edges);
	generate_vertex_faces();
}

xr_mesh* level_mesh::commit(xr_object& object, const b_model& model, uint32_t instance) const
{
	xr_assert(model.m_num_instances);
	if (instance == BAD_IDX)
		return commit(object, model.m_instances[0]);
	else
		return commit(object, model.m_instances[instance]);
}

xr_mesh* level_mesh::commit(xr_object& object, const b_model_instance* instance) const
{
	//gr1ph starts
	if (!instance->xform.can_invert_43())
	{
		msg("skipping mesh %d", instance->model_id);
		return NULL;
	}
	// gr1ph ends
	xr_assert(instance->num_faces);
	xr_assert(instance->num_faces > 1);
	xr_assert(instance->min_face < instance->max_face);

	xr_mesh* mesh = new xr_mesh;
	lw_face_vec& faces = mesh->faces();
	std::vector<fvector3>& points = mesh->points();
	xr_surfmap_vec& surfmaps = mesh->surfmaps();
	std::vector<uint32_t>& sgroups = mesh->sgroups();

	xr_remapper vert_remap(instance->min_vert, instance->max_vert);
	xr_remapper face_remap(instance->min_face, instance->max_face);
	xr_remapper surfmap_remap(m_raw_surfaces.size());

	points.reserve(instance->num_faces*2);
	faces.reserve(instance->num_faces);
	sgroups.reserve(instance->num_faces);

	dmatrix xform;
	bool identity = instance->xform.is_identity();
	if (!identity)
		xform.invert_43(dmatrix().set(instance->xform));

	uint32_t face_idx = instance->min_face;
	lw_face face_template(BAD_IDX, BAD_IDX, BAD_IDX);
	for (b_face_vec_cit it = m_faces.begin() + face_idx,
			end = m_faces.begin() + instance->max_face + 1;
			it != end; ++it, ++face_idx) {
		if (it->tag != instance->tag)
			continue;
		sgroups.push_back(it->sgroup);
		for (uint_fast32_t i = 3; i != 0;) {
			uint32_t& vx = vert_remap[it->v[--i]];
			if (vx == BAD_IDX) {
				vx = uint32_t(points.size() & UINT32_MAX);
				if (identity) {
					points.push_back(m_points[it->v[i]]);
				} else {
					dvector3 temp;
					temp.set(m_points[it->v[i]]).transform(xform);
					points.push_back(fvector3().set(temp));
				}
			}
			face_template.v[i] = vx;
		}
		uint32_t mapped_face_idx = uint32_t(faces.size() & UINT32_MAX);
		face_remap[face_idx] = mapped_face_idx;
		faces.push_back(face_template);
		uint32_t& surf_idx = surfmap_remap[it->surf_idx];
		if (surf_idx == BAD_IDX) {
			surf_idx = uint32_t(surfmaps.size() & UINT32_MAX);
			xr_surfmap* smap = new xr_surfmap(object.attach(m_raw_surfaces[it->surf_idx]));
			surfmaps.push_back(smap);
		}
		surfmaps[surf_idx]->faces.push_back(mapped_face_idx);
	}

	create_mappings(faces, mesh->vmrefs(), mesh->vmaps(), face_remap, vert_remap);

	xr_assert(faces.size() >= 2);
	
	xr_assert(points.size() >= 3);//xr_assert(points.size() >= 4);


	if (identity)
		mesh->bbox().set(instance->aabb);
	else
		mesh->calculate_bbox();

	object.meshes().push_back(mesh);

	return mesh;
}
