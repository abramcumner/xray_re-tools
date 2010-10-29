#include "xr_level.h"
#include "xr_level_portals.h"
#include "xr_scene.h"
#include "xr_scene_portals.h"
#include "xr_scene_sectors.h"
#include "xr_name_gen.h"
#include "level_tools.h"

using namespace xray_re;

void level_tools::reconstruct_portals()
{
	if (m_level->portals() == 0)
		return;
	msg("building %s", "portal.part");

	const portal_data_vec& portals = m_level->portals()->portals();
	xr_custom_object_vec& cobjects = m_scene->portals()->objects();
	cobjects.reserve(portals.size());

	xr_custom_object_vec& sectors = m_scene->sectors()->objects();
	xr_assert(!sectors.empty());

	xr_name_gen name("portal");
	for (portal_data_vec_cit it = portals.begin(), end = portals.end(); it != end; ++it, name.next()) {
		const portal_data* portal = *it;
		xr_portal_object* new_portal = new xr_portal_object(*m_scene);
		new_portal->co_name() = name.get();
		new_portal->sector_front() = sectors[portal->sector_front]->co_name();
		new_portal->sector_back() = sectors[portal->sector_back]->co_name();
		new_portal->vertices().assign(portal->vertices.begin(), portal->vertices.end());
		cobjects.push_back(new_portal);
	}
	m_level->clear_portals();
}
