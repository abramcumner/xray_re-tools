#include <algorithm>
#include <cstring>
#include <cctype>
#include "xr_entity.h"
#include "xr_ini_file.h"
#include "xr_level_spawn.h"
#include "xr_entity_factory.h"
#include "xr_file_system.h"
#include "syncer.h"

using namespace xray_re;

void syncer::to_xrai_compat(xr_level_spawn& spawn, const xr_ini_file& ini, const char* section)
{
	for (xr_entity_vec_it it = spawn.spawns().begin(), end = spawn.spawns().end(); it != end; ++it) {
		cse_abstract* entity = *it;
		char clsid[9];
		entity->clsid().get(clsid);
		if (ini.line_exist(section, clsid))
			entity->name() = ini.r_string(section, clsid);
		entity->version() = CSE_VERSION_2215;
		entity->script_version() = 2;
	}
}

static inline const char* skip_ws(const char* p)
{
	for (int c; (c = *p) && (c == ' ' || c == '\t'); ++p) {}
	return p;
}

static inline const char* skip_name(const char* p)
{
	for (int c; (c = *p); ++p) {
		if (!std::isalnum(c) && c != '_')
			break;
	}
	return p;
}

static inline bool split_link(const char* link_spec, std::string& level_name, std::string& point_name)
{
	const char* p = skip_name(link_spec);
	if (p == link_spec) {
		// to delete links
		level_name.clear();
		point_name.clear();
		return true;
	}
	level_name.assign(link_spec, p);
	p = skip_ws(p);
	if (*p++ != ',')
		return false;
	p = skip_name(link_spec = skip_ws(p));
	if (p == link_spec)
		return false;
	point_name.assign(link_spec, p);
	return true;
}

void syncer::edit_links(xr_level_spawn& spawn, const xr_ini_file& ini, const char* section)
{
	if (!ini.section_exist(section))
		return;

	std::string level_name, point_name;
	for (xr_entity_vec_it it = spawn.spawns().begin(), end = spawn.spawns().end();
			it != end; ++it) {
		cse_alife_graph_point* gp = dynamic_cast<cse_alife_graph_point*>(*it);
		if (gp == 0)
			continue;
		const char* name = gp->name_replace().c_str();
		if (!ini.line_exist(section, name))
			continue;
		const char* link_spec = ini.r_string(section, name);
		if (split_link(link_spec, level_name, point_name)) {
			gp->connection_level() = level_name;
			gp->connection_point() = point_name;
		} else {
			msg("can't parse link fixup for %s:%s", section, name);
		}
	}
}

struct graph_point_pred { bool operator()(const cse_abstract* l, const cse_abstract* r) const {
	const cse_alife_graph_point* gp1 = dynamic_cast<const cse_alife_graph_point*>(l);
	const cse_alife_graph_point* gp2 = dynamic_cast<const cse_alife_graph_point*>(r);
	if (gp1 == 0)
		return gp2 == 0 ? false : true;
	else if (gp2 == 0)
		return false;
	int cmp = gp2->connection_level().compare(gp1->connection_level());
	if (cmp)
		return cmp < 0;
	const fvector3& p1 = l->position();
	const fvector3& p2 = r->position();
	return p1.x < p2.x || (p1.x == p2.x && (p1.z < p2.z || (p1.z == p2.z && p1.y < p2.y)));
}};

void syncer::dump_links(xr_level_spawn& spawn, xr_writer& w, const char* section)
{
	std::sort(spawn.spawns().begin(), spawn.spawns().end(), graph_point_pred());
	w.w_sf("[%s]\n", section);
	for (xr_entity_vec_it it = spawn.spawns().begin(), end = spawn.spawns().end(); it != end; ++it) {
		cse_alife_graph_point* gp = dynamic_cast<cse_alife_graph_point*>(*it);
		if (gp == 0)
			continue;
		w.w_sf("%-32s= ", gp->name_replace().c_str());
		if (!gp->connection_level().empty()) {
			w.w_sf("%s", gp->connection_level().c_str());
			if (!gp->connection_point().empty())
				w.w_sf(", %s", gp->connection_point().c_str());
			w.w_sf("\t");
		}
		const fvector3& p = gp->position();
		w.w_sf("; %7.2f %7.2f %7.2f\n", p.x, p.y, p.z);
	}
	w.w_sf("\n");
}

bool syncer::load_graph_points(xr_level_spawn& spawn, const char* path, const char* name) const
{
	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(path, name);
	if (r == 0)
		return false;
	xr_reader* s;
	for (uint32_t id = 0; (s = r->open_chunk(id)); ++id) {
		xr_packet packet;
		s->r_packet(packet, s->size());
		uint16_t pkt_id;
		packet.r_begin(pkt_id);
		xr_assert(pkt_id == M_SPAWN);
		const char* name = packet.skip_sz();
#if 1
		packet.r_seek(0);
		cse_abstract* entity = create_entity(name);
		if (entity && entity->clsid() == "AI_GRAPH") {
			entity->spawn_read(packet);
			entity->version() = CSE_VERSION_SOC;
			entity->script_version() = 6;
			spawn.spawns().push_back(entity);
		} else {
			delete entity;
		}
#else
		if (std::strcmp(name, "graph_point") == 0) {
			packet.r_seek(0);
			cse_abstract* entity = create_entity(name);
			xr_assert(entity);
			entity->spawn_read(packet);
			entity->version() = CSE_VERSION_SOC;
			entity->script_version() = 6;
			spawn.spawns().push_back(entity);
		}
#endif
		r->close_chunk(s);
	}
	fs.r_close(r);
	return true;
}
