#include "xr_level_version.h"
#include "xr_level_visuals.h"
#include "xr_level_geom.h"
#include "xr_ogf_v3.h"
#include "xr_ogf_v4.h"
#include "xr_reader.h"

using namespace xray_re;

xr_level_visuals::~xr_level_visuals() {}

void xr_level_visuals::load_d3d7(xr_reader& r, const xr_level_geom* geom)
{
	xr_reader* s;
	for (uint32_t id = 0; (s = r.open_chunk(id)); ++id) {
		xr_ogf_v3* ogf = new xr_ogf_v3;
		ogf->load_ogf(*s);
		ogf->set_ext_geom(geom->vbufs());
		m_ogfs.push_back(ogf);
		r.close_chunk(s);
	}
}

void xr_level_visuals::load_1865(xr_reader& r, const xr_level_geom* geom)
{
	xr_reader* s;
	for (uint32_t id = 0; (s = r.open_chunk(id)); ++id) {
		xr_ogf_v3* ogf = new xr_ogf_v3;
		ogf->load_ogf(*s);
		ogf->set_ext_geom(geom->vbufs(), geom->ibufs());
		m_ogfs.push_back(ogf);
		r.close_chunk(s);
	}
}

void xr_level_visuals::load_d3d9(xr_reader& r, const xr_level_geom* geom)
{
	xr_reader* s;
	for (uint32_t id = 0; (s = r.open_chunk(id)); ++id) {
		xr_ogf_v4* ogf = new xr_ogf_v4;
		ogf->load_ogf(*s);
		ogf->set_ext_geom(geom->vbufs(), geom->ibufs(), geom->swibufs());
		m_ogfs.push_back(ogf);
		r.close_chunk(s);
	}
}

void xr_level_visuals::load(uint32_t xrlc_version, xr_reader& r, const xr_level_geom* geom)
{
	xr_reader* s = 0;
	if (xrlc_version >= XRLC_VERSION_10 && xrlc_version <= XRLC_VERSION_12) {
		s = r.open_chunk(FSL12_VISUALS);
		xr_assert(s);
		load_1865(*s, geom);
	} else if (xrlc_version >= XRLC_VERSION_13) {
		s = r.open_chunk(FSL13_VISUALS);
		xr_assert(s);
		load_d3d9(*s, geom);
	} else if (xrlc_version >= XRLC_VERSION_5 && xrlc_version <= XRLC_VERSION_8) {
		if (xrlc_version == XRLC_VERSION_5)
			s = r.open_chunk(FSL5_VISUALS);
		else if (xrlc_version == XRLC_VERSION_8)
			s = r.open_chunk(FSL8_VISUALS);
		xr_assert(s);
		load_d3d7(*s, geom);
	}
	r.close_chunk(s);
}
