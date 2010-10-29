#ifndef __GNUC__
#pragma once
#endif
#ifndef __LEVEL_AUX_MESH_H__
#define __LEVEL_AUX_MESH_H__

#include "xr_mesh_builder.h"

namespace xray_re {
	class xr_gamemtls_lib;
	class xr_level_hom;
	class xr_level_som;
	class xr_cform;
};

class level_aux_mesh: public xray_re::xr_mesh_builder {
public:
	void		build(const xray_re::xr_level_hom& hom);
	void		build(const xray_re::xr_level_som& som, const xray_re::xr_gamemtls_lib& gamemtls_lib);
	void		build(const xray_re::xr_cform& cform);

private:
	template<typename T> void
			compact_vertices(std::vector<uint32_t>& refs, const T* faces);
	void		create_uvs();
	void		create_spherical_uvs();
	uint32_t&	vertex_point(uint_fast32_t idx);
};

inline uint32_t& level_aux_mesh::vertex_point(uint_fast32_t idx) { return m_faces[idx >> 2].v[idx & 3]; }

#endif
