#include <algorithm>
#include <iterator>
#include "xr_level_som.h"
#include "xr_level_hom.h"
#include "xr_cform.h"
#include "xr_gamemtls_lib.h"
#include "level_aux_mesh.h"

using namespace xray_re;

template<typename T> struct vertex_less {
	const T* faces;
	explicit vertex_less(const T* _faces): faces(_faces) {}
	bool operator()(uint32_t l, uint32_t r) const {
		return faces[l >> 2].v[l & 3] < faces[r >> 2].v[r & 3];
	}
};

template<typename T> struct vertex_equal {
	const T* faces;
	explicit vertex_equal(const T* _faces): faces(_faces) {}
	bool operator()(uint32_t l, uint32_t r) const {
		return faces[l >> 2].v[l & 3] == faces[r >> 2].v[r & 3];
	}
};

template<typename T> inline
const fvector3& face_vertex(const T* faces, uint32_t idx) { return faces[idx >> 2].v[idx & 3]; }

template<typename T> void level_aux_mesh::compact_vertices(std::vector<uint32_t>& refs, const T* faces)
{
	std::sort(refs.begin(), refs.end(), vertex_less<T>(faces));
	m_points.reserve(refs.size());
	fvector3 p;
	uint32_t point_idx = uint32_t(m_points.size() & UINT32_MAX);
	for (std::vector<uint32_t>::iterator it = refs.begin(), end = refs.end(); it != end; ++point_idx) {
		vertex_point(*it) = point_idx;
		m_points.push_back(p.set(face_vertex<T>(faces, *it)));
		while(++it != end && face_vertex<T>(faces, *it) == p)
			vertex_point(*it) = point_idx;
	}
}

void level_aux_mesh::build(const xr_level_hom& hom)
{
	uint32_t face_idx = 0;
	std::vector<uint32_t> refs;
	refs.reserve(3*hom.num_polys());
	m_faces.reserve(hom.num_polys());
	b_face face;
	for (const hom_poly *it = hom.polys(), *end = it + hom.num_polys(); it != end; ++it) {
		if (it->v0 == it->v1 || it->v0 == it->v2 || it->v1 == it->v2)
			continue;
		refs.push_back(face_idx);
		refs.push_back(face_idx | 1);
		refs.push_back(face_idx | 2);
		face_idx += 4;
		face.surface.set(0, 0, 0, (it->flags & 0x1) ? RSF_TWO_SIDED : 0);
		m_faces.push_back(face);
	}
	if (face_idx) {
		compact_vertices(refs, hom.polys());
		create_uvs();
	}
}

struct sound_occ_pred { bool operator()(const xr_gamemtl* l, const xr_gamemtl* r) const {
	return l->snd_occlusion_factor < r->snd_occlusion_factor;
}};

void level_aux_mesh::build(const xr_level_som& som, const xr_gamemtls_lib& gamemtls_lib)
{
	xr_gamemtl_vec gamemtls(gamemtls_lib.materials());
	std::sort(gamemtls.begin(), gamemtls.end(), sound_occ_pred());

	xr_gamemtl pattern;

	uint32_t face_idx = 0;

	std::vector<uint32_t> refs;
	refs.reserve(3*som.num_polys());
	m_faces.reserve(som.num_polys());
	b_face face_template;
	for (const som_poly *it = som.polys(), *end = it + som.num_polys(); it != end; ++it) {
		if (it->v0 == it->v1 || it->v0 == it->v2 || it->v1 == it->v2)
			continue;
		refs.push_back(face_idx);
		refs.push_back(face_idx | 1);
		refs.push_back(face_idx | 2);
		face_idx += 4;
		pattern.snd_occlusion_factor = it->occ;
		xr_gamemtl_vec_cit gamemtl_it = std::lower_bound(gamemtls.begin(), gamemtls.end(),
				&pattern, sound_occ_pred());
		xr_assert(gamemtl_it != gamemtls.end() && (*gamemtl_it)->snd_occlusion_factor == it->occ);
		face_template.surface.set(0, 0, (*gamemtl_it)->id, it->b2sided ? RSF_TWO_SIDED : 0);
		m_faces.push_back(face_template);
	}
	if (face_idx) {
		compact_vertices(refs, som.polys());
		create_uvs();
	}
}

struct xform_vertex { const fvector3& operator()(const cf_vertex& v) const {
	return v.p;
}};

void level_aux_mesh::build(const xr_cform& cform)
{
	const cf_vertex_vec& verts = cform.vertices();
	m_points.reserve(verts.size());
	transform(verts.begin(), verts.end(), std::back_inserter(m_points), xform_vertex());

	const cf_face_vec& faces = cform.faces();
	m_faces.reserve(faces.size());
	b_face face_template;
	for (cf_face_vec_cit it = faces.begin(), end = faces.end(); it != end; ++it) {
		if(it->v0 == it->v1 || it->v1 == it->v2 || it->v2 == it->v0)
			continue;
		face_template.sector = it->sector;
		face_template.v0 = it->v0;
		face_template.v1 = it->v1;
		face_template.v2 = it->v2;
		face_template.surface.set(0, 0, it->material, 0);
		m_faces.push_back(face_template);
	}
	create_spherical_uvs();
}

void level_aux_mesh::create_uvs()
{
	m_texcoords.resize(3);
	m_texcoords[0].set(0.f, 0.f);
	m_texcoords[1].set(0.f, 1.f);
	m_texcoords[2].set(1.f, 0.f);
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end();
			it != end; ++it) {
		it->tc0 = 0;
		it->tc1 = 1;
		it->tc2 = 2;
	}
}

void level_aux_mesh::create_spherical_uvs()
{
	fvector3 p0;
	p0.set();
	for (std::vector<fvector3>::iterator it = m_points.begin(),
			end = m_points.end(); it != end; ++it) {
		p0.add(*it);
	}
	p0.mul(1.f/m_points.size());

	m_texcoords.reserve(m_points.size());
	for (std::vector<fvector3>::iterator it = m_points.begin(),
			end = m_points.end(); it != end; ++it) {
		fvector3 p;
		p.sub(*it, p0).normalize();
		fvector2 uv;
		uv.set(std::asin(p.x), std::asin(p.y)).mul(16/float(M_PI));
		m_texcoords.push_back(uv);
	}
	for (b_face_vec_it it = m_faces.begin(), end = m_faces.end();
			it != end; ++it) {
		it->tc0 = it->v0;
		it->tc1 = it->v1;
		it->tc2 = it->v2;
	}
}
