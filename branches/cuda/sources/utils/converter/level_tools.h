#ifndef __GNUC__
#pragma once
#endif
#ifndef __LEVEL_TOOLS_H__
#define __LEVEL_TOOLS_H__

#include <vector>
#include <string>
#include "xr_types.h"
#include "xr_surface_factory.h"
#include "tools_base.h"

namespace xray_re {
	class xr_scene;
	class xr_level;
	class xr_ini_file;
	class xr_shaders_xrlc_lib;
	class xr_gamemtls_lib;
	class xr_custom_object;
	class xr_object;
	class xr_ogf;
	class xr_ogf_v4;
	class xr_ogf_v3;
	struct sector_data;
	template<typename T> struct _vector3;
	template<typename T> struct _matrix;

	typedef _vector3<float> fvector3;
	typedef _matrix<float> fmatrix;
}

class level_mesh;

class level_tools: public tools_base, xray_re::xr_surface_factory {
public:
			level_tools();
	virtual		~level_tools();

	virtual void	process(const cl_parser& cl);
	virtual xray_re::xr_surface*
			create_surface(const xray_re::xr_raw_surface& raw_surface) const;

	enum {
		RM_RAW,
		RM_MAYA,
		RM_LE,
		RM_LE2,
	};

	enum {
		RF_WITH_LODS	= 0x01,
		RF_USE_MT	= 0x02,
		RF_DEBUG_CFORM	= 0x04,
		RF_DEBUG_MERGE	= 0x08,
	};

private:
	bool		check_paths() const;

	void		reconstruct_scene(const char* level_name, const char* scene_name);
	void		reconstruct_ai_map();
	void		reconstruct_details();
	void		reconstruct_lights();
	void		reconstruct_glows();
	void		reconstruct_particles();
	void		reconstruct_ways();
	void		reconstruct_hom();
	void		reconstruct_som();
	void		reconstruct_sectors();
	void		reconstruct_portals();
	void		reconstruct_sound_srcs();
	void		reconstruct_sound_envs();
	void		reconstruct_wallmarks();
	void		reconstruct_spawns();

	void		build_debug_cform() const;

	void		do_substs(std::vector<std::string>* data, const char* line);

	bool		is_compiled_wallmark(uint32_t eshader_idx) const;

	uint16_t	add_texture(const std::string& texture, size_t hint = 0);
	uint16_t	add_shader(const std::string& shader, size_t hint = 0);

	void		make_object_ref(std::string& reference, const char* name) const;

	void		save_object(const xray_re::xr_object* object, const char* name) const;
	void		save_scene_object(const xray_re::xr_object* object, const char* name);
	void		register_scene_object(const char* name, const std::string& reference,
					const xray_re::fmatrix& xform, uint32_t file_version);
	void		register_sector_item(uint16_t sector_idx, const char* scene_object,
					std::string& mesh);

	void		reconstruct_visuals();
	void		calculate_subdivisions(size_t& vb_size, size_t& ib_size, uint32_t ogf_idx) const;
	void		calculate_ext_meshes(size_t& vb_size, size_t& ib_size) const;
	void		push_subdivisions_v5(level_mesh* mesh, uint16_t sector_idx, uint32_t ogf_idx);
	void		push_subdivisions_v12(level_mesh* mesh, uint16_t sector_idx, uint32_t ogf_idx);
	void		push_subdivisions_v13(level_mesh* mesh, uint16_t sector_idx, uint32_t ogf_idx);
	void		push_ext_meshes(level_mesh* mesh);
	void		split_lod_textures() const;

	uint16_t	get_terrain_texture() const;

	void		configure(level_mesh* mesh) const;
	void		export_for_maya(level_mesh* mesh) const;
	void		export_for_level_editor(level_mesh* mesh);

	void		guess_cshader(xray_re::xr_surface* surface, uint16_t flags) const;
	void		paint_automatic(xray_re::xr_surface* surface, uint16_t flags) const;
	void		paint_debug_cform(xray_re::xr_surface* surface, uint16_t flags) const;

	uint16_t	find_or_register_mu_model(xray_re::xr_ogf_v4* new_mu, xray_re::fmatrix& xform);
	uint16_t	find_or_register_mu_model(xray_re::xr_ogf_v3* new_mu, xray_re::fmatrix& xform);

	uint16_t	get_sector_idx(const xray_re::fvector3& p) const;
	uint16_t	get_default_sector_idx() const;

private:
	int			m_rmode;
	uint32_t		m_rflags;	// reconstruction flags

	xray_re::xr_ini_file*	m_ini;
	const char*		m_sect_profile;
	const char*		m_sect_auto_gamemtls;
	const char*		m_sect_wallmark_shaders;
	const char*		m_debug_texture;
	const char*		m_fake_gamemtl;
	const char*		m_ladders_gamemtl;
	const char*		m_ghost_eshader;

	xray_re::xr_shaders_xrlc_lib*	m_shaders_xrlc_lib;

	xray_re::xr_level*	m_level;	// current level
	xray_re::xr_scene*	m_scene;	// current scene
	const char*		m_level_name;	// level name (e.g. "l03_agroprom")
	const char*		m_scene_name;	// scene file name (e.g. "agroprom")

	// shortcuts
	const std::vector<xray_re::xr_ogf*>*	m_subdivisions;	// m_level->visuals()->ogfs()
	const std::vector<xray_re::sector_data*>*	m_sectors;	// m_level->sectors()->sectors()
	std::vector<std::string>*		m_shaders;	// m_level->shaders()->shaders()
	std::vector<std::string>*		m_textures;	// m_level->shaders()->textures()
	const xray_re::xr_gamemtls_lib*		m_gamemtls_lib;	// m_level->gamemtls_lib()

	std::vector<xray_re::xr_custom_object*>*	m_scene_visuals;
	std::vector<xray_re::xr_custom_object*>*	m_scene_sectors;

	std::vector<uint16_t>			m_uniq_textures;// unique texture indices
	std::vector<uint16_t>			m_uniq_shaders;	// unique shader indices

	uint16_t		m_spawn_version;

	//std::vector<xray_re::xr_ogf_v4*>	m_mu_models;
	std::vector<xray_re::xr_ogf*>	m_mu_models;

	uint16_t		m_default_sector_idx;

	std::string		m_objects_ref;
};

#endif
