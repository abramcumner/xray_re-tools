#include "xr_scene_sectors.h"
#include "xr_reader.h"
#include "xr_writer.h"

using namespace xray_re;

xr_sector_object::xr_sector_object(xr_scene& scene):
	xr_custom_object(scene, TOOLS_CLASS_SECTOR),
	m_private(0)
{
	m_color.set(1.f, 1.f, 1.f, 0);
}

xr_sector_object::~xr_sector_object() {}

struct read_item { void operator()(sector_item& item, xr_reader& r) const {
	if (!r.find_chunk(SECTOR_CHUNK_ONE_ITEM))
		xr_not_expected();
	r.r_sz(item.object);
	r.r_sz(item.mesh);
	r.debug_find_chunk();
}};

void xr_sector_object::load(xr_reader& r)
{
	uint16_t version;
	if (!r.r_chunk<uint16_t>(SECTOR_CHUNK_VERSION, version))
		xr_not_expected();
	xr_assert(version == SECTOR_VERSION);
	xr_custom_object::load(r);
	if (!r.r_chunk<fcolor>(SECTOR_CHUNK_COLOR, m_color))
		xr_not_expected();
	if (!r.r_chunk<uint8_t>(SECTOR_CHUNK_PRIVATE, m_private))
		xr_not_expected();
	xr_reader* s = r.open_chunk(SECTOR_CHUNK_ITEMS);
	if (s) {
		s->r_chunks(m_items, read_item());
		r.close_chunk(s);
	}
}

struct write_item { void operator()(const sector_item& item, xr_writer& w) const {
	w.open_chunk(SECTOR_CHUNK_ONE_ITEM);
	w.w_sz(item.object);
	w.w_sz(item.mesh);
	w.close_chunk();
}};

void xr_sector_object::save(xr_writer& w) const
{
	xr_custom_object::save(w);
	w.w_chunk<uint16_t>(SECTOR_CHUNK_VERSION, SECTOR_VERSION);
	w.w_chunk<fcolor>(SECTOR_CHUNK_COLOR, m_color);
	w.w_chunk<uint8_t>(SECTOR_CHUNK_PRIVATE, m_private);
	w.open_chunk(SECTOR_CHUNK_ITEMS);
	w.w_chunks(m_items, write_item());
	w.close_chunk();
}

////////////////////////////////////////////////////////////////////////////////

xr_scene_sectors::xr_scene_sectors(xr_scene& scene):
	xr_scene_objects(scene, "sector.part", SCENE_CHUNK_SECTORS), m_flags(0) {}

xr_scene_sectors::~xr_scene_sectors() {}

void xr_scene_sectors::load(xr_reader& r)
{
	uint16_t version = 0;
	r.r_chunk<uint16_t>(TOOLS_CHUNK_VERSION, version);
	xr_assert(version == 0);
	xr_scene_objects::load(r);
	r.r_chunk<uint32_t>(SECTORS_CHUNK_COMMON_FLAGS, m_flags);
}

void xr_scene_sectors::save(xr_writer& w) const
{
	xr_scene_objects::save(w);
	w.w_chunk<uint16_t>(TOOLS_CHUNK_VERSION, 0);
	w.w_chunk<uint32_t>(SECTORS_CHUNK_COMMON_FLAGS, m_flags);
}
