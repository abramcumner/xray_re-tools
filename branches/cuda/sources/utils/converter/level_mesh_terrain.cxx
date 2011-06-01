#include "xr_details.h"
#include "level_mesh.h"

using namespace xray_re;

void level_mesh::separate_terrain(uint16_t texture, const details_header& header,
		const detail_slot_v3 slots[])
{
	create_edges();

	uint32_t temp_tag = m_free_tag;

	std::vector<uint32_t> adjacents;
	adjacents.reserve(1024);
	for (b_face_vec_it it0 = m_faces.begin(), it = it0, end = m_faces.end(); it != end; ++it) {
		if (it->tag != BAD_IDX || it->surface.texture != texture)
			continue;
		it->temp = UINT16_MAX;
		it->tag = temp_tag;
		for (uint32_t face_idx = uint32_t((it - it0) & UINT32_MAX);;) {
			const b_face& face = m_faces[face_idx];
			for (uint_fast32_t i = 3, v0, v1 = face.v0; i != 0; v1 = v0) {
				const b_edge* edge = find_edge(v0 = face.v[--i], v1);
				xr_assert(edge);
				for (uint_fast32_t next = edge->face0; next != BAD_IDX;) {
					b_face& adjacent = m_faces[next];
					if (next != face_idx && face.sector == adjacent.sector &&
							adjacent.tag == BAD_IDX) {
						adjacents.push_back(uint32_t(next & UINT32_MAX));
						adjacent.tag = temp_tag;
						adjacent.temp = UINT16_MAX;
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

	typedef std::pair<uint32_t, uint32_t> b_stats;
	typedef std::vector<b_stats> b_stats_vec;

	b_stats_vec stats;
	stats.reserve(256);

	uint16_t free_temp = 0, sector = UINT16_MAX;
	for (b_face_vec_it it0 = m_faces.begin(), it = it0, end = m_faces.end(); it != end; ++it) {
		if (it->tag != temp_tag || it->temp != UINT16_MAX)
			continue;
		it->temp = free_temp;
		if (sector == UINT16_MAX)
			sector = it->sector;
		// in build 2232 l06_rostok, 2571 pripyat terrain is in different sectors
		//else
		//	xr_assert(sector == it->sector);

		uint32_t num_faces = 0, num_contacts = 0;
		for (uint32_t face_idx = uint32_t((it - it0) & UINT32_MAX);;) {
			++num_faces;
			const b_face& face = m_faces[face_idx];
			for (uint_fast32_t i = 3, v0, v1 = face.v0; i != 0; v1 = v0) {
				const b_edge* edge = find_edge(v0 = face.v[--i], v1);
				xr_assert(edge);
				for (uint32_t next = edge->face0; next != BAD_IDX;) {
					b_face& adjacent = m_faces[next];
					if (next != face_idx && adjacent.temp == UINT16_MAX &&
							adjacent.surface.texture == it->surface.texture) {
						adjacents.push_back(next);
						adjacent.temp = free_temp;
					}
					next = adjacent.next_face_idx(v0, v1);
				}
			}
			for (uint_fast32_t i = 3; i != 0;) {
				const fvector3& p = m_points[face.v[--i]];
				int x = int(std::floor(p.x*0.5f)) + header.offs_x;
				if (x < 0 || x >= int(header.size_x))
					goto skip;
				int z = int(std::floor(p.z*0.5f)) + header.offs_z;
				if (z < 0 || z >= int(header.size_z))
					goto skip;
				const detail_slot_v3& slot = slots[z*header.size_x + x];
				if (slot.y_height == 0)
					goto skip;
				float y = slot.r_ybase();
				if (p.y < y - 1e-4f)
					goto skip;
				if (p.y > (y + slot.r_yheight()))
					goto skip;
			}
			++num_contacts;
skip:
			if (adjacents.empty())
				break;
			face_idx = adjacents.back();
			adjacents.pop_back();
		}
		++free_temp;
		xr_assert(free_temp != UINT16_MAX);
		stats.push_back(b_stats(num_faces, num_contacts));
	}
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
		if (it->tag == temp_tag && it->surface.texture != texture) {
			const b_stats& st = stats[it->temp];
			if (5*st.second < 4*st.first)
				it->tag = BAD_IDX;
		}
	}
	if (sector != UINT16_MAX) {
		b_model_instance* instance = new b_model_instance(temp_tag, sector);
		m_models.push_back(b_model(b_model::MT_TERRAIN, m_instances.size()));
		m_instances.push_back(instance);
		++m_free_tag;
	}
}

void level_mesh::separate_terrain(uint16_t texture)
{
	b_model_instance temp(m_free_tag, UINT16_MAX);
	uint32_t face_idx = 0;
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end();
			it != end; ++it, ++face_idx) {
		if (it->surface.texture != texture || it->tag != BAD_IDX)
			continue;
		it->tag = temp.tag;
		if (temp.sector == UINT16_MAX)
			temp.sector = it->sector;
		else
			xr_assert(temp.sector == it->sector);
	}
	if (temp.sector != UINT16_MAX) {
		b_model_instance* instance = new b_model_instance(temp);
		m_models.push_back(b_model(b_model::MT_TERRAIN, m_instances.size()));
		m_instances.push_back(instance);
		++m_free_tag;
	}
}
