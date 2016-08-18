#include "xr_level.h"
#include "xr_level_sectors.h"
#include "xr_scene.h"
#include "xr_scene_sectors.h"
#include "xr_name_gen.h"
#include "xr_aabb.h"
#include "xr_ogf.h"
#include "level_tools.h"

using namespace xray_re;

uint16_t level_tools::get_default_sector_idx() const
{
	uint16_t sector_idx = UINT16_MAX;
	float size, max_size = xr_numeric_limits<float>::min();
	for (sector_data_vec_cit it0 = m_sectors->begin(), it = it0,
			end = m_sectors->end(); it != end; ++it) {
		const fbox& aabb = m_subdivisions->at((*it)->root)->bbox();
		if (max_size < (size = aabb.size())) {
			max_size = size;
			sector_idx = uint16_t((it - it0) & UINT16_MAX);
		}
	}
	xr_assert(sector_idx != UINT16_MAX);
	return sector_idx;
}

static inline bool invalid_sector_color(unsigned r, unsigned g, unsigned b)
{
	return (r == 2 && g == 2 && b == 2) ||
			(r == 1 && g == 1 && b == 1) ||
			(r == 2 && g == 0 && b == 0) ||
			(r == 1 && g == 0 && b == 0) ||
			(r == 0 && g == 0 && b == 0);
}

void level_tools::reconstruct_sectors()
{
	if (m_level->sectors() == 0)
		return;
	msg("building %s", "sector.part");

	m_scene_sectors = &m_scene->sectors()->objects();
	m_scene_sectors->reserve(m_sectors->size());

	m_default_sector_idx = get_default_sector_idx();
	msg("selected sector #%u (of %" PRIuSIZET ") as default", m_default_sector_idx, m_sectors->size());

	uint16_t sector_idx = 0;
	unsigned seed = 0;
	xr_name_gen name("sector");
	for (sector_data_vec_cit it = m_sectors->begin(),
			end = m_sectors->end(); it != end; ++it, ++sector_idx) {
		xr_sector_object* new_sector = new xr_sector_object(*m_scene);
		if (sector_idx == m_default_sector_idx) {
			new_sector->co_name() = "Sector_DEFAULT";
			new_sector->co_flags() &= ~COF_VISIBLE;
			new_sector->priv() = true;
			new_sector->color().set(1.f, 0, 0, 0);
		} else {
			new_sector->co_name() = name.get();
			name.next();
			new_sector->priv() = false;
			unsigned r, g, b;
			do {
				r = seed%3;
				g = (seed/3)%3;
				b = (seed/9)%3;
				++seed;
			} while (invalid_sector_color(r, g, b));
			new_sector->color().set(r*0.5f, g*0.5f, b*0.5f);
		}
		m_scene_sectors->push_back(new_sector);
	}
}

uint16_t level_tools::get_sector_idx(const fvector3& p) const
{
	uint16_t sector_idx = UINT16_MAX;
	float size, min_size = xr_numeric_limits<float>::max();
	for (sector_data_vec_cit it0 = m_sectors->begin(), it = it0, end = m_sectors->end();
			it != end; ++it) {
		const fbox& aabb = m_subdivisions->at((*it)->root)->bbox();
		if (aabb.contain(p) && min_size > (size = aabb.size())) {
			min_size = size;
			sector_idx = uint16_t((it - it0) & UINT16_MAX);
		}
	}
	xr_assert(sector_idx != UINT16_MAX);
	return sector_idx;
}
