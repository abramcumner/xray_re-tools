#include <cctype>
#include "xr_level.h"
#include "xr_level_geom.h"
#include "xr_level_visuals.h"
#include "xr_level_details.h"
#include "xr_level_sectors.h"
#include "xr_level_cform.h"
#include "xr_level_spawn.h"
#include "xr_scene.h"
#include "xr_scene_visuals.h"
#include "xr_scene_sectors.h"
#include "xr_scene_details.h"
#include "xr_ogf.h"
#include "xr_ogf_v3.h"
#include "xr_ogf_v4.h"
#include "xr_ini_file.h"
#include "xr_entity.h"
#include "xr_image.h"
#include "xr_name_gen.h"
#include "xr_shaders_xrlc_lib.h"
#include "xr_gamemtls_lib.h"
#include "xr_file_system.h"
#include "level_tools.h"
#include "level_mesh.h"

using namespace xray_re;

void level_tools::save_scene_object(const xr_object* object, const char* name)
{
	xr_assert(m_rmode != RM_MAYA);

	std::string reference;
	make_object_ref(reference, name);
	xr_visual_object* new_visual = new xr_visual_object(*m_scene);
	new_visual->co_name() = reference;
	new_visual->reference() = reference;
	m_scene_visuals->push_back(new_visual);
	object->save_object(PA_OBJECTS, reference.append(".object").c_str());
	new_visual->file_version() = xr_file_system::instance().file_age(PA_OBJECTS, reference);
}

void level_tools::register_scene_object(const char* name, const std::string& reference, const fmatrix& xform, uint32_t file_version)
{
	xr_visual_object* new_visual = new xr_visual_object(*m_scene);
	// name it just like LE does
	make_object_ref(new_visual->co_name(), name);
	new_visual->co_position().set(xform.c);
	new_visual->co_scale().set(xform.i.magnitude(),
			xform.j.magnitude(), xform.k.magnitude());
	xform.get_xyz(new_visual->co_rotation());
	new_visual->file_version() = file_version;
	new_visual->reference() = reference;
	m_scene_visuals->push_back(new_visual);
}

void level_tools::register_sector_item(uint16_t sector_idx, const char* scene_object, std::string& mesh)
{
	xr_sector_object* sector = static_cast<xr_sector_object*>(m_scene_sectors->at(sector_idx));
	sector->items().push_back(sector_item());
	sector_item* item = &sector->items().back();
	make_object_ref(item->object, scene_object);
	item->mesh = mesh;
}

xr_surface* level_tools::create_surface(const xr_raw_surface& raw_surface) const
{
	xr_surface* surface = new xr_surface;
	if (raw_surface.debug()) {
		surface->texture() = m_debug_texture;
		surface->set_two_sided();
	} else {
		if (m_gamemtls_lib) {
			const xr_gamemtl* gamemtl = m_gamemtls_lib->get_material(raw_surface.gamemtl);
			if (gamemtl)
				surface->gamemtl() = gamemtl->name;
			else
				msg("can't resolve game material %u", raw_surface.gamemtl);
		}
		if (raw_surface.automatic()) {
			if (m_level->xrlc_version() <= XRLC_VERSION_8)
				surface->gamemtl() = m_fake_gamemtl;
			paint_automatic(surface, raw_surface.flags);
		} else {
			surface->texture() = m_textures->at(raw_surface.texture);
			surface->eshader() = m_shaders->at(raw_surface.eshader);
			if (m_sect_auto_gamemtls) {
				surface->gamemtl() = m_ini->r_string(m_sect_auto_gamemtls,
						surface->texture().c_str());
			}
		}
		if (raw_surface.two_sided())
			surface->set_two_sided();
		guess_cshader(surface, raw_surface.flags);
		if (m_rflags & RF_DEBUG_CFORM)
			paint_debug_cform(surface, raw_surface.flags);
	}
	return surface;
}

bool level_tools::is_compiled_wallmark(uint32_t eshader_idx) const
{
	if (m_sect_wallmark_shaders == 0)
		return false;
	return m_ini->line_exist(m_sect_wallmark_shaders, m_shaders->at(eshader_idx).c_str());
}

void level_tools::calculate_subdivisions(size_t& vb_size, size_t& ib_size, uint32_t ogf_idx) const
{
	const xr_ogf* ogf = m_subdivisions->at(ogf_idx);
	if (ogf->hierarchical()) {
		for (std::vector<uint32_t>::const_iterator it = ogf->children_l().begin(),
				end = ogf->children_l().end(); it != end; ++it) {
			calculate_subdivisions(vb_size, ib_size, *it);
		}
	} else if (is_compiled_wallmark(ogf->shader_l())) {
		// FIXME: this might be useful to restore wallmarks
		// even if there is no redundant level.wallmarks
	} else {
		vb_size += ogf->vb().size();
		ib_size += ogf->ib().size();
	}
}

void level_tools::calculate_ext_meshes(size_t& vb_size, size_t& ib_size) const
{
	if (m_level->spawn() == 0)
		return;
	xr_ogf_vec_cit brkbl_it = m_level->brkbl_meshes().begin();
	const xr_entity_vec& spawns = m_level->spawn()->spawns();
	for (xr_entity_vec_cit it = spawns.begin(), end = spawns.end(); it != end; ++it) {
		const cse_abstract* entity = *it;
		if (entity->name() == "climable_object") {
			vb_size += 4;
			ib_size += 6;
		} else if (entity->name() == "breakable_object") {
			const xr_ogf* ogf = *brkbl_it++;
			if (ogf->hierarchical()) {
				for (xr_ogf_vec_cit it1 = ogf->children().begin(),
						end1 = ogf->children().end(); it1 != end1; ++it1) {
					const xr_ogf* ogf1 = *it1;
					vb_size += ogf1->vb().size();
					ib_size += ogf1->ib().size();
				}
			} else {
				vb_size += ogf->vb().size();
				ib_size += ogf->ib().size();
			}
		}
	}
}

uint16_t level_tools::find_or_register_mu_model(xr_ogf_v3* new_mu, fmatrix& xform)
{
	for (std::vector<uint32_t>::const_iterator it0 = new_mu->children_l().begin(),
			it = it0, end = new_mu->children_l().end(); it != end; ++it) {
		const xr_ogf_v3* ogf = static_cast<const xr_ogf_v3*>(m_subdivisions->at(*it));
		switch (ogf->model_type()) {
		case MT3_TREE:
			xform.set(ogf->xform());
			xr_assert(it == it0 || std::memcmp(&ogf->xform(), &xform, sizeof(xform)) == 0);
			xr_assert(xform._14 == 0 && xform._24 == 0 && xform._34 == 0);
			break;
		default:
			xr_not_expected();
			break;
		}
	}

	uint16_t model_idx = 0;
	for (xr_ogf_vec_cit it = m_mu_models.begin(), end = m_mu_models.end();
			it != end; ++it, ++model_idx) {
		const xr_ogf* mu = *it;
		if (mu->children_l().size() != new_mu->children_l().size())
			continue;
		// FIXME: sort children for robustness
		for (std::vector<uint32_t>::const_iterator it1 = mu->children_l().begin(),
				end1 = mu->children_l().end(), it2 = new_mu->children_l().begin();
				it1 != end1; ++it1, ++it2) {
			const xr_ogf_v3* ogf1 = static_cast<const xr_ogf_v3*>(m_subdivisions->at(*it1));
			const xr_ogf_v3* ogf2 = static_cast<const xr_ogf_v3*>(m_subdivisions->at(*it2));
			if (ogf1->model_type() != ogf2->model_type())
				goto skip;
			// FIXME: though it is reasonably to assume MU-models always reference
			// external VB/IB, better have some explicit check here.
			if (ogf1->ext_vb_index() != ogf2->ext_vb_index())
				goto skip;
			if (ogf1->ext_vb_offset() != ogf2->ext_vb_offset())
				goto skip;
			if (ogf1->ext_vb_size() != ogf2->ext_vb_size())
				goto skip;
			if (ogf1->ext_ib_index() != ogf2->ext_ib_index())
				goto skip;
			if (ogf1->ext_ib_offset() != ogf2->ext_ib_offset())
				goto skip;
			if (ogf1->ext_ib_size() != ogf2->ext_ib_size())
				goto skip;
		}
		return model_idx;
skip:;
	}
	xr_assert(model_idx == m_mu_models.size());
	m_mu_models.push_back(new_mu);
	return model_idx;
}

void level_tools::push_subdivisions_v5(level_mesh* mesh, uint16_t sector_idx, uint32_t ogf_idx)
{
	xr_ogf_v3* ogf = static_cast<xr_ogf_v3*>(m_subdivisions->at(ogf_idx));
	if (ogf->hierarchical()) {
		for (std::vector<uint32_t>::const_iterator it = ogf->children_l().begin(),
				end = ogf->children_l().end(); it != end; ++it) {
			push_subdivisions_v5(mesh, sector_idx, *it);
		}
	} else if (is_compiled_wallmark(ogf->shader_l())) {
		;
	} else {
		mesh->push(sector_idx, ogf->vb(), ogf->ib(),
				m_uniq_textures[ogf->texture_l()],
				m_uniq_shaders[ogf->shader_l()]);
	}
}

void level_tools::push_subdivisions_v12(level_mesh* mesh, uint16_t sector_idx, uint32_t ogf_idx)
{
	xr_ogf_v3* ogf = static_cast<xr_ogf_v3*>(m_subdivisions->at(ogf_idx));
	if (ogf->model_type() == MT3_LOD) {
		// fix build 1472 - MT3_LOD contains MT3_TREE and MT3_NORMAL
		bool isMu = true;
		for (std::vector<uint32_t>::const_iterator it = ogf->children_l().begin(),
				end = ogf->children_l().end(); it != end; ++it) {
			const xr_ogf_v3* ogf_child = static_cast<const xr_ogf_v3*>(m_subdivisions->at(*it));
			switch (ogf_child->model_type()) {
			case MT3_TREE:
				break;
			default:
				isMu = false;
				break;
			}
		}
		if (isMu) {
			fmatrix xform;
			uint16_t model_idx = find_or_register_mu_model(ogf, xform);
			uint32_t tag = mesh->instantiate_mu_model(model_idx, sector_idx, xform);
			mesh->set_tc_fix(true);
			for (std::vector<uint32_t>::const_iterator it = ogf->children_l().begin(),
					end = ogf->children_l().end(); it != end; ++it) {
				const xr_ogf* ogf1 = m_subdivisions->at(*it);
				mesh->push(tag, ogf1->vb(), ogf1->ib(), xform,
						m_uniq_textures[ogf1->texture_l()],
						m_uniq_shaders[ogf1->shader_l()]);
			}
			mesh->set_tc_fix(false);
		} else {
			for (std::vector<uint32_t>::const_iterator it = ogf->children_l().begin(),
					end = ogf->children_l().end(); it != end; ++it) {
				push_subdivisions_v12(mesh, sector_idx, *it);
			}
		}
	} else if (ogf->hierarchical()) {
		for (std::vector<uint32_t>::const_iterator it = ogf->children_l().begin(),
				end = ogf->children_l().end(); it != end; ++it) {
			push_subdivisions_v12(mesh, sector_idx, *it);
		}
	} else if (is_compiled_wallmark(ogf->shader_l())) {
		;
	} else {
		mesh->push(sector_idx, ogf->vb(), ogf->ib(),
				m_uniq_textures[ogf->texture_l()],
				m_uniq_shaders[ogf->shader_l()]);
	}
}

uint16_t level_tools::find_or_register_mu_model(xr_ogf_v4* new_mu, fmatrix& xform)
{
	for (std::vector<uint32_t>::const_iterator it0 = new_mu->children_l().begin(),
			it = it0, end = new_mu->children_l().end(); it != end; ++it) {
		const xr_ogf_v4* ogf = static_cast<const xr_ogf_v4*>(m_subdivisions->at(*it));
		switch (ogf->model_type()) {
		case MT4_TREE_ST:
		case MT4_TREE_PM:
			xform.set(ogf->xform());
			xr_assert(it == it0 || std::memcmp(&ogf->xform(), &xform, sizeof(xform)) == 0);
			xr_assert(xform._14 == 0 && xform._24 == 0 && xform._34 == 0);
			break;
		default:
			xr_not_expected();
			break;
		}
	}

	uint16_t model_idx = 0;
	for (xr_ogf_vec_cit it = m_mu_models.begin(), end = m_mu_models.end();
			it != end; ++it, ++model_idx) {
		const xr_ogf* mu = *it;
		if (mu->children_l().size() != new_mu->children_l().size())
			continue;
		// FIXME: sort children for robustness
		for (std::vector<uint32_t>::const_iterator it1 = mu->children_l().begin(),
				end1 = mu->children_l().end(), it2 = new_mu->children_l().begin();
				it1 != end1; ++it1, ++it2) {
			const xr_ogf_v4* ogf1 = static_cast<const xr_ogf_v4*>(m_subdivisions->at(*it1));
			const xr_ogf_v4* ogf2 = static_cast<const xr_ogf_v4*>(m_subdivisions->at(*it2));
			if (ogf1->model_type() != ogf2->model_type())
				goto skip;
			// FIXME: though it is reasonably to assume MU-models always reference
			// external VB/IB, better have some explicit check here.
			if (ogf1->ext_vb_index() != ogf2->ext_vb_index())
				goto skip;
			if (ogf1->ext_vb_offset() != ogf2->ext_vb_offset())
				goto skip;
			if (ogf1->ext_vb_size() != ogf2->ext_vb_size())
				goto skip;
			if (ogf1->ext_ib_index() != ogf2->ext_ib_index())
				goto skip;
			if (ogf1->ext_ib_offset() != ogf2->ext_ib_offset())
				goto skip;
			if (ogf1->ext_ib_size() != ogf2->ext_ib_size())
				goto skip;
		}
		return model_idx;
skip:;
	}
	xr_assert(model_idx == m_mu_models.size());
	m_mu_models.push_back(new_mu);
	return model_idx;
}

void level_tools::push_subdivisions_v13(level_mesh* mesh, uint16_t sector_idx, uint32_t ogf_idx)
{
	xr_ogf_v4* ogf = static_cast<xr_ogf_v4*>(m_subdivisions->at(ogf_idx));
	if (ogf->model_type() == MT4_LOD) {
		fmatrix xform;
		uint16_t model_idx = find_or_register_mu_model(ogf, xform);
		uint32_t tag = mesh->instantiate_mu_model(model_idx, sector_idx, xform);
		mesh->set_tc_fix(true);
		for (std::vector<uint32_t>::const_iterator it = ogf->children_l().begin(),
				end = ogf->children_l().end(); it != end; ++it) {
			const xr_ogf* ogf1 = m_subdivisions->at(*it);
			mesh->push(tag, ogf1->vb(), ogf1->ib(), xform,
					m_uniq_textures[ogf1->texture_l()],
					m_uniq_shaders[ogf1->shader_l()]);
		}
		mesh->set_tc_fix(false);
	} else if (ogf->hierarchical()) {
		for (std::vector<uint32_t>::const_iterator it = ogf->children_l().begin(),
				end = ogf->children_l().end(); it != end; ++it) {
			push_subdivisions_v13(mesh, sector_idx, *it);
		}
	} else if (is_compiled_wallmark(ogf->shader_l())) {
		// FIXME: restore wallmarks here even if there is no redundant level.wallmarks
	} else {
		mesh->push(sector_idx, ogf->vb(), ogf->ib(),
				m_uniq_textures[ogf->texture_l()],
				m_uniq_shaders[ogf->shader_l()]);
	}
}

void level_tools::push_ext_meshes(level_mesh* mesh)
{
	if (m_level->spawn() == 0)
		return;
	const xr_gamemtl* gamemtl = m_gamemtls_lib->get_material(m_ladders_gamemtl);
	xr_assert(gamemtl);
	uint16_t clmbl_gamemtl = gamemtl->id;
	size_t texture_hint = m_textures->size() - 1;
	size_t shader_hint = m_shaders->size() - 1;
	xr_ogf_vec_cit brkbl_it = m_level->brkbl_meshes().begin();;
	xr_entity_vec& spawns = m_level->spawn()->spawns();
	xr_entity_vec_it last = spawns.begin();
	for (xr_entity_vec_it it = last, end = spawns.end(); it != end; ++it) {
		const xr_ogf* ogf;
		cse_abstract* entity = *it;
		if (entity->name() == "breakable_object") {
			ogf = *brkbl_it++;
		} else if (entity->name() == "climable_object") {
			ogf = 0;
		} else {
			if (last != it)
				*last = *it;
			++last;
			continue;
		}

		fmatrix xform;
		xform.set_xyz(entity->rotation());
		xform.c.set(entity->position());

		uint16_t sector_idx = get_sector_idx(entity->position());

		if (ogf == 0) {
			mesh->push(sector_idx, entity->shape()->shapes().front(), xform,
					0, 0, clmbl_gamemtl);
		} else {
			if (ogf->bones().empty()) {
				msg("empty skeleton in %s", ogf->path().c_str());
				goto skip;
			}
			// FIXME: what if there are many different materials?
			gamemtl = m_gamemtls_lib->get_material(ogf->bones().front()->gamemtl().c_str());
			if (gamemtl == 0) {
				msg("can't resolve game material %s", ogf->bones().front()->gamemtl().c_str());
				goto skip;
			}
			if (ogf->hierarchical()) {
				for (xr_ogf_vec_cit it1 = ogf->children().begin(),
						end1 = ogf->children().end(); it1 != end1; ++it1) {
					const xr_ogf* ogf1 = static_cast<const xr_ogf*>(*it1);
					mesh->push(sector_idx, ogf1->vb(), ogf1->ib(), xform,
							add_texture(ogf1->texture(), texture_hint),
							add_shader(ogf1->shader(), shader_hint),
							gamemtl->id);
				}
			} else {
				mesh->push(sector_idx, ogf->vb(), ogf->ib(), xform,
						add_texture(ogf->texture(), texture_hint),
						add_shader(ogf->shader(), shader_hint),
						gamemtl->id);
			}
		}
		delete entity;
skip:;
	}
	spawns.erase(last, spawns.end());
}

void level_tools::reconstruct_visuals()
{
	if (m_rmode != RM_MAYA)
		msg("building %s", "scene_object.part");

	uint32_t xrlc_version = m_level->xrlc_version();
	ogf_version version = m_subdivisions->at(0)->version();

	size_t vb_size = 0, ib_size = 0;
	msg("calculating %s", "subdivisions");
	xr_assert(m_sectors);
	for (sector_data_vec_cit it = m_sectors->begin(), end = m_sectors->end(); it != end; ++it) {
		xr_assert(*it);
		// FIXME: avoid recursion.
		calculate_subdivisions(vb_size, ib_size, (*it)->root);
	}
	if (xrlc_version >= XRLC_VERSION_12) {
		msg("calculating %s", "external meshes");
		calculate_ext_meshes(vb_size, ib_size);
	}

	level_mesh* mesh = new level_mesh(vb_size, ib_size);
	msg("collecting %s", "subdivisions");
	if (xrlc_version >= XRLC_VERSION_9 && xrlc_version <= XRLC_VERSION_12 && version == OGF3_VERSION) {
		uint16_t sector_idx = 0;
		for (sector_data_vec_cit it = m_sectors->begin(),
				end = m_sectors->end(); it != end; ++it) {
			push_subdivisions_v12(mesh, sector_idx++, (*it)->root);
		}
		msg("collecting %s", "external meshes");
		push_ext_meshes(mesh);
		if (m_rflags & RF_WITH_LODS)
			split_lod_textures();
		xr_ogf_vec().swap(m_mu_models);
	} else if (xrlc_version >= XRLC_VERSION_13 || version == OGF4_VERSION) {
		uint16_t sector_idx = 0;
		for (sector_data_vec_cit it = m_sectors->begin(),
				end = m_sectors->end(); it != end; ++it) {
			push_subdivisions_v13(mesh, sector_idx++, (*it)->root);
		}
		msg("collecting %s", "external meshes");
		push_ext_meshes(mesh);
		if (m_rflags & RF_WITH_LODS)
			split_lod_textures();
		xr_ogf_vec().swap(m_mu_models);
	} else {
		uint16_t sector_idx = 0;
		for (sector_data_vec_cit it = m_sectors->begin(),
				end = m_sectors->end(); it != end; ++it) {
			push_subdivisions_v5(mesh, sector_idx++, (*it)->root);
		}
	}

	mesh->build(xrlc_version >= XRLC_VERSION_10 ? 1e-1f : 4.f);
	m_level->clear_geom();

	if (xr_cform* cform = m_level->cform()) {
		msg("cleaning cform");
		cform->optimize();
		if (m_rflags & RF_DEBUG_CFORM)
			mesh->enable_debug_cform();
		mesh->restore_game_materials(*cform);
		m_level->clear_cform();
	}

	configure(mesh);

	if (m_rflags & RF_USE_MT)
		mesh->enable_mt();

	if (m_rmode == RM_MAYA) {
		export_for_maya(mesh);
	} else {
		m_scene_visuals = &m_scene->visuals()->objects();
		xr_assert(m_scene_sectors);
		export_for_level_editor(mesh);

		if (!mesh->build_err().empty()) {
			msg("saving error list");
			std::string name("converter_");
			mesh->build_err().save(PA_LOGS, name.append(m_scene_name).append(".err").c_str());
		}
	}
	m_level->clear_visuals();
	delete mesh;
}

void level_tools::configure(level_mesh* mesh) const
{
	const char* pattern;
	if (m_ini->line_exist(m_sect_profile, "fake_mu_gamemtls")) {
		const char* section = m_ini->r_string(m_sect_profile, "fake_mu_gamemtls");
		for (size_t i = 0; m_ini->r_line(section, i, &pattern, 0); ++i) {
//msg("fake_mu_gamemtl%"PRIuSIZET": %s", i, pattern);
			const xr_gamemtl* gamemtl;
			if ((gamemtl = m_gamemtls_lib->get_material(pattern)))
				mesh->add_fake_mu_gamemtl(gamemtl->id);
		}
	}
	if (m_ini->line_exist(m_sect_profile, "fake_gamemtls")) {
		const char* section = m_ini->r_string(m_sect_profile, "fake_gamemtls");
		for (size_t i = 0; m_ini->r_line(section, i, &pattern, 0); ++i) {
//msg("fake_gamemtl%"PRIuSIZET": %s", i, pattern);
			const xr_gamemtl* gamemtl;
			if ((gamemtl = m_gamemtls_lib->get_material(pattern)))
				mesh->add_fake_gamemtl(gamemtl->id);
		}
	}
	if (m_ini->line_exist(m_sect_profile, "inadhesive_shaders")) {
		const char* section = m_ini->r_string(m_sect_profile, "inadhesive_shaders");
		for (size_t i = 0; m_ini->r_line(section, i, &pattern, 0); ++i) {
//msg("inadhesive_shader%"PRIuSIZET": %s", i, pattern);
			uint16_t eshader = 0;
			for (std::vector<std::string>::const_iterator it = m_shaders->begin(),
					end = m_shaders->end(); it != end; ++it, ++eshader) {
				if (*it == pattern) {
					mesh->add_inadhesive_shader(eshader);
					break;
				}
			}
		}
	}
}

static inline bool is_power_of_two(int x) { return (x & (x - 1)) == 0; }

void level_tools::split_lod_textures() const
{
	if (m_mu_models.empty())
		return;
	const xr_image* lods = m_level->lods();
	const xr_image* lods_nm = m_level->lods_nm();
	if (lods == 0 || lods_nm == 0)
		return;
	msg("splitting %s/%s", "level_lods.dds", "level_lods_nm.dds");
	xr_file_system& fs = xr_file_system::instance();
	if (!fs.folder_exist(PA_GAME_TEXTURES, "lod"))
		fs.create_folder(PA_GAME_TEXTURES, "lod");
	unsigned w = lods->width(), h = lods->height();
	xr_assert(w == lods_nm->width() && h == lods_nm->height());
	std::string path("lod\\lod_");
	path += m_objects_ref;
	path += "lod";
	for (std::string::iterator it = path.begin() + 8, end = path.end(); it != end; ++it) {
		int c = *it;
		*it = (c == '\\') ? '_' : std::tolower(c);
	}
	xr_name_gen reference(path.c_str(), false);
	for (xr_ogf_vec_cit it = m_mu_models.begin(), end = m_mu_models.end();
			it != end; ++it, reference.next()) {
		frect uvs;
		uvs.invalidate();
		const ogf4_lod_face* lod_faces = (*it)->lod_faces();
		for (uint_fast32_t i = 8; i != 0;) {
			const ogf4_lod_vertex* vert = lod_faces[--i].v;
			for (uint_fast32_t j = 4; j != 0;)
				uvs.extend(vert[--j].t);
		}

		irect rect;
		rect.x1 = int(std::floor(w*uvs.x1));
		rect.y1 = int(std::floor(h*uvs.y1));
		rect.x2 = int(std::floor(w*uvs.x2));
		rect.y2 = int(std::floor(h*uvs.y2));
		xr_assert(is_power_of_two(rect.x2 - rect.x1 + 1));
		xr_assert(is_power_of_two(rect.y2 - rect.y1 + 1));

		path = reference.get();
		lods->save_dds(PA_GAME_TEXTURES, path + ".dds", &rect);
		lods_nm->save_dds(PA_GAME_TEXTURES, path + "_nm.dds", &rect);
	}
	m_level->clear_lods();
	m_level->clear_lods_nm();
}

uint16_t level_tools::get_terrain_texture() const
{
	for (std::vector<std::string>::const_iterator it0 = m_textures->begin(), it = it0,
			end = m_textures->end(); it != end; ++it) {
		if (it->compare(0, 8, "terrain\\") == 0)
			return uint16_t((it - it0) & UINT16_MAX);
	}
	return UINT16_MAX;
}

void level_tools::export_for_maya(level_mesh* mesh) const
{
	uint16_t terrain_texture = get_terrain_texture();
	if (terrain_texture != UINT16_MAX) {
		msg("separating %s", "terrain");
		mesh->separate_terrain(terrain_texture);
	}
	msg("separating %s", "continuous texture-joined meshes");
	mesh->enable_restricted_merge();
	mesh->create_models();
	xr_object* maya_object = 0;
	xr_name_gen name("lod", false);
	for (b_model_vec_cit it = mesh->models().begin(), end = mesh->models().end();
			it != end; ++it) {
		if (it->type() != b_model::MT_MU_MODEL) {
			xr_assert(it->num_instances() == 1);
			if (maya_object == 0)
				maya_object = new xr_object(this);
			mesh->commit(*maya_object, *it);
		} else if (m_rflags & RF_WITH_LODS) {
			xr_object* object = new xr_object(this);
			xr_mesh* mesh1 = mesh->commit(*object, *it);
			mesh1->name() = "modelShape";
			object->denominate_surfaces();
			object->flags() = EOF_MULTIPLE_USAGE;
			save_object(object, name.get());
			name.next();
			delete object;
		}
	}
	if (maya_object) {
		maya_object->denominate();
		save_object(maya_object, "cuts");
		delete maya_object;
	}
}

void level_tools::export_for_level_editor(level_mesh* mesh)
{
	uint16_t terrain_texture = get_terrain_texture();
	if (m_scene->details() && terrain_texture != UINT16_MAX) {
		msg("separating terrain");
		mesh->separate_terrain(terrain_texture,
				m_scene->details()->header(), m_scene->details()->slots());
	}

	xr_name_gen model_name("part", false);
	xr_name_gen mu_model_name("lod", false);
	xr_name_gen fake_name("fake", false);
	xr_name_gen *raw_names = 0;

	switch(m_rmode) {
	case RM_RAW:
		raw_names = new xr_name_gen[m_sectors->size()];
		{
			xr_name_gen* temp = raw_names;
			xr_custom_object_vec& sectors = m_scene->sectors()->objects();
			for (xr_custom_object_vec_cit it = sectors.begin(), end = sectors.end();
					it != end; ++it, ++temp) {
				temp->init(((*it)->co_name() + '#').c_str(), false);
			}
		}
		msg("separating models (%s)", "by sectors");
		mesh->create_sector_models();
		break;

	case RM_LE:
		msg("separating models (%s)", "by merged edge-linked groups");
		if (m_rflags & RF_DEBUG_MERGE)
			mesh->enable_debug_merge();
		mesh->enable_aggressive_merge();
		mesh->create_models();
		break;

	case RM_LE2:
		msg("separating models (%s)", "by raw edge-linked groups");
		if (m_rflags & RF_DEBUG_MERGE)
			mesh->enable_debug_merge();
		mesh->create_models();
		break;

	default:
		xr_not_expected();
		break;
	}

	xr_object* default_object = 0;
	xr_object* sectors_object = 0;
	xr_object* fake_object = 0;
	xr_object* terrain_object = 0;

	std::string terrain_name;
	// gr1ph starts
	msg("exporting visuals");
	for (b_model_vec_cit it = mesh->models().begin(), end = mesh->models().end(); it != end; ++it) {
		if (it->type() == b_model::MT_TERRAIN) {
			xr_assert(it->num_instances() == 1);
			xr_assert(terrain_object == 0);
			terrain_object = new xr_object(this);
			make_object_ref(terrain_name, "terrain");
			xr_mesh* mesh1 = mesh->commit(*terrain_object, *it);
			if (mesh1 != NULL)
			{
				mesh1->name() = "terrainShape";
				register_sector_item(it->sector(0), "terrain", mesh1->name());
			}
		} else if (it->type() == b_model::MT_FAKE) {
			xr_assert(it->num_instances() == 1);
			if (fake_object == 0)
				fake_object = new xr_object(this);
			xr_mesh* mesh1 = mesh->commit(*fake_object, *it);
			mesh1->name().assign(fake_name.get()).append("Shape");
			if (mesh1 != NULL)
			{
				register_sector_item(it->sector(0), "fake", mesh1->name());
				fake_name.next();
			}
		} else if (it->type() == b_model::MT_RAW) {
			xr_assert(m_rmode == RM_RAW);
			xr_assert(it->num_instances() == 1);
			uint16_t sector_idx = it->sector(0);
			xr_mesh* mesh1;
			if (sector_idx == m_default_sector_idx) {
				if (default_object == 0)
					default_object = new xr_object(this);
				mesh1 = mesh->commit(*default_object, *it);
			} else {
				if (sectors_object == 0)
					sectors_object = new xr_object(this);
				mesh1 = mesh->commit(*sectors_object, *it);
			}
			mesh1->name().assign(raw_names[sector_idx].get()).append("Shape");
			if (mesh1 != NULL)
			{
				raw_names[sector_idx].next();
				register_sector_item(sector_idx, sector_idx == m_default_sector_idx ?
						"default" : "sectors", mesh1->name());
			}
		} else {
			xr_assert(it->type() == b_model::MT_MODEL || it->type() == b_model::MT_MU_MODEL);
			xr_object* object = new xr_object(this);
			xr_mesh* mesh1 = mesh->commit(*object, *it);
			if (mesh1 != NULL)
			{
				mesh1->name() = "modelShape";
				object->denominate_surfaces();
				std::string reference;
				xr_name_gen name;
				if (it->type() == b_model::MT_MU_MODEL) {
					object->flags() = EOF_MULTIPLE_USAGE;
					name.init(mu_model_name.get());
					make_object_ref(reference, mu_model_name.get());
					mu_model_name.next();
				} else {
					name.init(model_name.get());
					make_object_ref(reference, model_name.get());
					model_name.next();
				}
				object->save_object(PA_OBJECTS, reference + ".object");
				uint32_t age = xr_file_system::instance().file_age(PA_OBJECTS, reference + ".object");
				for (size_t i = 0, n = it->num_instances(); i != n; ++i, name.next()) {
					register_scene_object(name.get(), reference, it->xform(i), age);
					register_sector_item(it->sector(i), name.get(), mesh1->name());
				}
			}
			delete object;
		}
	}
	// gr1ph ends
	delete raw_names;

	if (fake_object) {
		fake_object->denominate_surfaces();
		save_scene_object(fake_object, "fake");
		delete fake_object;
	}
	if (default_object) {
		default_object->denominate_surfaces();
		save_scene_object(default_object, "default");
		delete default_object;
	}
	if (terrain_object) {
		terrain_object->denominate_surfaces();
		save_scene_object(terrain_object, "terrain");
		m_scene->details()->snap_objects().push_back(terrain_name);
		delete terrain_object;
	}
	if (sectors_object) {
		sectors_object->denominate_surfaces();
		save_scene_object(sectors_object, "sectors");
		delete sectors_object;
	}
}

static float evaluate(const std::string& s1, const std::string& s2)
{
	int matched = 0;
	std::string::const_iterator it1 = s1.begin(), end1 = s1.end();
	std::string::const_iterator it2 = s2.begin(), end2 = s2.end();
	for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++matched) {
		if (*it1 != *it2)
			break;
	}
	return float(matched)/s2.size();
}

// FIXME: very ugly (зато работает, гг)
void level_tools::guess_cshader(xr_surface* surface, uint16_t flags) const
{
	// it might be already set in paint_automatic()
	if (surface->cshader() != "default")
		return;

	xr_shader_xrlc pattern;
	pattern.name = surface->eshader();
	pattern.dummy = 0;
	pattern.vert_translucency = 0;
	pattern.vert_ambient = 0;
	pattern.lm_density = 1.f;
	if (flags & RSF_COLLISION)
		pattern.collision = true;
	if (flags & RSF_RENDERING) {
		pattern.rendering = true;
		pattern.optimize_uv = true;
	}
	if ((flags & RSF_LIGHTMAP) == 0)
		pattern.light_vertex = true;

	const std::string* best = 0;

	float estimate = 0;
	const xr_shader_xrlc_vec& shaders = m_shaders_xrlc_lib->shaders();
	for (xr_shader_xrlc_vec_cit it = shaders.begin(),
			end = shaders.end(); it != end; ++it) {
		const xr_shader_xrlc* s = *it;
		if (s->collision == pattern.collision &&
				s->rendering == pattern.rendering &&
				s->optimize_uv == pattern.optimize_uv &&
				s->light_vertex == pattern.light_vertex) {
			float value = evaluate(s->name, pattern.name);
			if (estimate < value) {
				estimate = value;
				best = &s->name;
			} else if (best == 0) {
				if (pattern.light_vertex) {
					if (s->name.compare(0, 22, "def_shaders\\def_vertex") == 0)
						best = &s->name;
				} else if (s->lm_density == pattern.lm_density) {
					if (s->name == "default" || s->name.compare(0, 16, "def_shaders\\def_") == 0)
						best = &s->name;
				}
			}
		}
	}
	if (best == 0) {
		msg("can't find compiler shader for %s", pattern.name.c_str());
	} else {
		surface->cshader() = *best;
	}
//	msg("match: %s (%#x) -> %s (%g)", surface->eshader().c_str(), flags, surface->cshader().c_str(), estimate);
}

static inline const char* skip_comma(const char* p)
{
	int c;
	for (; (c = *p++) && (c == ' ' || c == '\t');) {}
	if (c != ',')
		return 0;
	for (; (c = *++p) && (c == ' ' || c == '\t');) {}
	return p;
}

static inline const char* scan_value(const char* p)
{
	for (int c; (c = *p); ++p) {
		if (!std::isalnum(c) && c != '_' && c != '\\')
			break;
	}
	return p;
}

void level_tools::paint_automatic(xr_surface* surface, uint16_t flags) const
{
	const char* gamemtl = surface->gamemtl().c_str();
	if (m_ini->line_exist("collider_presets", gamemtl)) {
		const char* preset = m_ini->r_string("collider_presets", gamemtl);

		const char* p = scan_value(preset);
		xr_assert(p != preset);
		surface->texture().assign(preset, p - preset);

		if ((p = skip_comma(p)) == 0)
			return;
		if ((p = scan_value(preset = p)) == preset)
			return;
		surface->eshader().assign(preset, p - preset);

		if ((p = skip_comma(p)) == 0)
			return;
		if ((p = scan_value(preset = p)) == preset)
			return;
		surface->cshader().assign(preset, p - preset);

		msg("%s -> %s, %s, %s", gamemtl,
				surface->texture().c_str(), surface->eshader().c_str(),
				surface->cshader().c_str());
	} else {
		msg("no invisible collider preset for game material %s", gamemtl);
		surface->texture() = "prop\\prop_fake_kollision";
		surface->eshader() = "def_shaders\\def_vertex";
	}
}

void level_tools::paint_debug_cform(xr_surface* surface, uint16_t flags) const
{
	const char* gamemtl = surface->gamemtl().c_str();
	if (m_ini->line_exist("debug_cform_textures", gamemtl))
		surface->texture() = m_ini->r_string("debug_cform_textures", gamemtl);
	else
		msg("can't assign texture for game material %s", gamemtl);
	if ((flags & RSF_COLLISION) == 0)
		surface->eshader() = m_ghost_eshader;
}
