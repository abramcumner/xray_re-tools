#include "xr_level.h"
#include "xr_level_details.h"
#include "xr_level_dm.h"
#include "xr_scene.h"
#include "xr_scene_details.h"
#include "xr_name_gen.h"
#include "xr_file_system.h"
#include "level_tools.h"

using namespace xray_re;

void level_tools::reconstruct_details()
{
	xr_level_details* details = m_level->details();
	if (details == 0)
		return;

	msg("building %s", "detail_object.part");
	std::string texture;
	if (details->raw_texture()) {
		texture += "det\\det_";
		texture += m_scene_name;
	}

	xr_scene_details* scene_details = m_scene->details();

	details_header& header = scene_details->header();
	header = details->header();
	header.version = DETMGR_VERSION;

	xr_dm_vec& models = details->models();
	detail_object_vec& objects = scene_details->objects();
	objects.reserve(models.size());

	xr_name_gen name("detail", false);
	for (xr_dm_vec_it it = models.begin(), end = models.end(); it != end; ++it, name.next()) {
		xr_dm* dm = *it;
		detail_object* new_detail = new detail_object;
		make_object_ref(new_detail->reference, name.get());
		new_detail->min_scale = dm->min_scale();
		new_detail->max_scale = dm->max_scale();
		new_detail->flags = dm->flags();
		objects.push_back(new_detail);

		if (!texture.empty())
			dm->texture() = texture;
		dm->to_object();
		save_object(dm, name.get());
	}

	fbox& bbox = scene_details->bbox();
	bbox.min.set(float(-2*header.offs_x),
			xr_numeric_limits<float>::max(),
			float(-2*header.offs_z));
	bbox.max.set(bbox.x1 + float(2*header.size_x),
			xr_numeric_limits<float>::real_min(),
			bbox.z1 + float(2*header.size_z));

	uint32_t num_slots = details->num_slots();
	detail_slot_v3* new_slot = new detail_slot_v3[num_slots];
	scene_details->num_slots() = num_slots;
	scene_details->slots() = new_slot;
	for (const detail_slot_v3 *it = details->slots(), *end = it + num_slots;
			it != end; ++it) {
		float temp;
		if (bbox.min.y > (temp = it->r_ybase()))
			bbox.min.y = temp;
		if (bbox.max.y < (temp += it->r_yheight()))
			bbox.max.y = temp;
		*new_slot++ = *it;
	}

//	scene_details->density() = 0.2f;
//	scene_details->texture() = "";

	// scene_details->snap_objects() will be set later in reconstruct_visuals()

	if (details->raw_texture()) {
		msg("saving %s", texture.c_str());
		xr_file_system& fs = xr_file_system::instance();
		xr_writer* w = fs.w_open(PA_GAME_TEXTURES, texture.append(".dds"));
		if (w == 0)
			xr_not_expected();
		w->w_raw(details->raw_texture(), details->raw_texture_size());
		fs.w_close(w);
	}
	m_level->clear_details();
}

#if 0
	std::string texture;
	std::vector<frect> bounds(m_details->raw.size(), frect().invalidate());
	std::vector<frect>::iterator it1 = bounds.begin();
	for (xr_dm_vec_it it = m_details->raw.begin(), end = m_details->raw.end(); it != end; ++it) {
		xr_dm* dm = *it;
		xr_assert(texture.empty() || texture == dm->texture());
		if (texture.empty())
			texture = dm->texture();
		dm->calc_uv_bounds(*it1++);
	}
	xr_assert(!texture.empty());

	xr_image build_details;
	if (!build_details.load_dds(PA_LEVEL, texture.append(".dds").c_str())) {
		msg("can't load %s", texture.c_str());
		throw xr_error();
	}
	unsigned w = build_details.width(), h = build_details.height();
	for (std::vector<frect>::iterator it = bounds.begin(), end = bounds.end(); it != end; ++it) {
		irect uvs;
		uvs.x1 = int(std::floor(w*it->x1));
		uvs.y1 = int(std::floor(h*it->y1));
		uvs.x2 = int(std::floor(w*it->x2));
		uvs.y2 = int(std::floor(h*it->y2));
		msg("uv: %f,%f - %f,%f (%d,%d - %d,%d)", it->x1, it->y1, it->x2, it->y2,
				rect.x1, rect.y1, rect.x2, rect.y2);
	}
#endif
