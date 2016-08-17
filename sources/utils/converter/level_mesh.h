#ifndef __GNUC__
#pragma once
#endif
#ifndef __LEVEL_MESH_H__
#define __LEVEL_MESH_H__

#include "xr_mesh_builder.h"

#ifdef _WIN32
#define USE_WIN32_MT
#endif

#ifdef USE_WIN32_MT
#include <windows.h>
#endif

namespace Opcode {
	class AABBTreeCollider;
	struct VertexPointers;
};

struct b_neighbour {
				b_neighbour(const xray_re::fvector3* _normal, uint16_t gamemtl, bool _side);
	const xray_re::fvector3*normal;		// cform face normal
	uint16_t		gamemtl;	// cform face material
	bool			side;		// right (true) or left
	uint8_t			__pad;
};
TYPEDEF_STD_VECTOR(b_neighbour)

struct b_span: public b_neighbour_vec {
	uint32_t	v0, v1;		// visual mesh vertices, not CFORM
	bool		border;
	uint8_t		__pad[3];
};

struct b_mu_instance {
				b_mu_instance(uint16_t _model_id, uint16_t _sector, const xray_re::fmatrix& _xform);
	uint16_t		model_id;
	uint16_t		sector;
	xray_re::fmatrix	xform;
};
TYPEDEF_STD_VECTOR(b_mu_instance)

struct b_model_instance {
				b_model_instance(uint32_t _tag, uint16_t _sector);
	void			update_vertices(uint_fast32_t vert_idx);
	void			update_faces(uint_fast32_t face_idx);

	uint32_t		tag;
	uint16_t		model_id;	// for MU-models only!
	uint16_t		sector;
	size_t			num_faces;
	uint32_t		min_vert, max_vert;
	uint32_t		min_face, max_face;
	xray_re::fbox		aabb;
	xray_re::fmatrix	xform;
};
TYPEDEF_STD_VECTOR_PTR(b_model_instance)

class b_model {
	friend class level_mesh;
public:
	enum model_type {
		MT_RAW,
		MT_FAKE,
		MT_TERRAIN,
		MT_MODEL,
		MT_MU_MODEL,
	};

				b_model(model_type type, size_t temp_index);
				b_model(model_type type, size_t num_instances, size_t temp_index);

	size_t			num_instances() const;
	model_type		type() const;
	const xray_re::fmatrix&	xform(size_t instance) const;
	uint16_t		sector(size_t instance) const;
	uint16_t		model_id() const;

protected:
	model_type		m_type;
	bool			m_locked;
	uint16_t		m_num_instances;
	union {
		b_model_instance**	m_instances;
		size_t			m_temp_index;
	};
};
TYPEDEF_STD_VECTOR(b_model);

struct b_submesh;
TYPEDEF_STD_VECTOR_PTR(b_submesh);

typedef std::pair<b_submesh*, b_submesh*> b_collision;
typedef std::vector<b_collision> b_collision_vec;
typedef b_collision_vec::iterator b_collision_vec_it;

namespace xray_re {
	class xr_cform;
	class xr_remapper;
	struct shape_def;
	struct details_header;
	struct detail_slot_v3;
};

class level_mesh: public xray_re::xr_mesh_builder {
public:
			level_mesh(size_t vb_size, size_t ib_size);
	virtual		~level_mesh();

	void		build(float normal_tolerance);

	void		enable_debug_cform();
	void		enable_debug_merge();
	void		enable_restricted_merge();
	void		enable_aggressive_merge();
	void		enable_mt();
	uint32_t	instantiate_mu_model(uint16_t model_id, uint16_t sector, const xray_re::fmatrix& xform);
	void		push(uint16_t sector, const xray_re::xr_vbuf& vb, const xray_re::xr_ibuf& ib,
					uint16_t texture, uint16_t eshader);
	void		push(uint16_t sector, const xray_re::xr_vbuf& vb, const xray_re::xr_ibuf& ib, const xray_re::fmatrix& xform,
					uint16_t texture, uint16_t eshader, uint16_t gamemtl);
	void		push(uint32_t tag, const xray_re::xr_vbuf& vb, const xray_re::xr_ibuf& ib, const xray_re::fmatrix& xform,
					uint16_t texture, uint16_t eshader);
	void		push(uint16_t sector, const xray_re::shape_def& shape, const xray_re::fmatrix& xform0,
					uint16_t texture, uint16_t eshader, uint16_t gamemtl);
	void		restore_game_materials(const xray_re::xr_cform& cform);
	void		separate_terrain(uint16_t texture);
	void		separate_terrain(uint16_t texture, const xray_re::details_header& header,
					const xray_re::detail_slot_v3 slots[]);
	void		create_sector_models();
	void		create_models();

	void		add_fake_gamemtl(uint16_t gamemtl);
	void		add_fake_mu_gamemtl(uint16_t gamemtl);
	void		add_inadhesive_shader(uint16_t eshader);

	void		get_triangle(const b_submesh* sm, uint32_t index, Opcode::VertexPointers& triangle) const;

	void			commit(xray_re::xr_object& object);
	xr_mesh*		commit(xray_re::xr_object& object, const b_model& model, uint32_t instance = BAD_IDX) const;
	const b_model_vec&	models() const;

private:
	void		create_uvs();

	void		create_debug_face(uint32_t v0, uint32_t v1);

	uint32_t*	remap_cform_verts(const xray_re::xr_cform& cform) const;
	void		match_edges_faces(const xray_re::xr_cform& cform, const uint32_t cf2m[]);
	void		propagate_gamemtls();

	void		update_span(b_span& span, uint_fast32_t v0, uint_fast32_t v1,
					const xray_re::fvector3* normal, uint16_t material);
	void		propagate_span(const b_span& span, const xray_re::fvector3 mesh_normals[]);
	void		paint_neighbours(const b_span& span, uint32_t v0, uint32_t v1,
					const xray_re::fvector3 mesh_normals[]);

	bool		search_colinear_path(uint32_t v0, uint32_t v1);
	bool		search_path(uint32_t v0, uint32_t v1);

	void		lock_models();
	void		pre_commit();

	void		create_smoothing_groups();
	void		create_mappings(xray_re::lw_face_vec& faces, xray_re::lw_vmref_vec& vmrefs, xray_re::xr_vmap_vec& vmaps,
				const xray_re::xr_remapper& face_remap, const xray_re::xr_remapper& vert_remap) const;
	xray_re::xr_mesh*	commit(xray_re::xr_object& object, const b_model_instance *instance) const;

	void		validate_instances_order();
	size_t		create_submeshes(b_submesh_vec& mu_submeshes,
					b_submesh_vec& fake_submeshes, b_submesh_vec& submeshes);
	void		account_submeshes(b_submesh_vec& mu_submeshes,
					b_submesh_vec& fake_submeshes, b_submesh_vec& submeshes);
	size_t		assign_contact_groups(b_submesh_vec& submeshes,
					b_collision_vec& contacts, b_submesh_vec& orphans);
	void		merge_mu_fake_submeshes(b_submesh_vec& mu_submeshes, b_submesh_vec& fake_submeshes,
					b_submesh_vec& residue_submeshes);
	void		merge_regular_submeshes(b_submesh_vec& submeshes, size_t num_models_hint);
	void		merge_orphans(b_submesh_vec& orphans, size_t num_groups, bool consider_material);
	void		reorder_faces();
	void		setup_collision_models(b_submesh_vec& submeshes) const;
	void		change_tag(uint32_t tag, uint32_t min_face, uint32_t max_face, uint32_t new_tag);
	void		merge(b_model_instance* instance, const b_model_instance* instance1);
	bool		validate_contacts(const Opcode::AABBTreeCollider& tc,
					const b_submesh* sm, const b_submesh* sm1) const;
	void		collect_mu_models(size_t num_mu_instances);

	bool		check_fake_mu_contacts(const Opcode::AABBTreeCollider& tc,
					const b_submesh* fake, const b_submesh* mu, size_t& max_contacts) const;
	bool		check_fake_fake_contacts(const Opcode::AABBTreeCollider& tc,
					const b_submesh* fake1, const b_submesh* fake2) const;
	bool		check_regular_regular_contacts(const Opcode::AABBTreeCollider& tc,
					const b_submesh* sm1, const b_submesh* sm2) const;
	bool		check_fake_gamemtls(const b_submesh* sm) const;

#ifdef USE_WIN32_MT
	struct setup_thread_param {
		level_mesh*	__this;
		b_submesh_vec*		submeshes;
	};
	static unsigned WINAPI	setup_thread_cb(void* arg);
#endif

protected:
	bool				m_debug_cform;
	bool				m_debug_merge;
	bool				m_restricted_merge;
	bool				m_aggressive_merge;
	bool				m_use_mt;

	float				m_glue_factor;

	float*				m_costs;
	std::vector<uint32_t>		m_path;

	std::vector<xray_re::fvector3>	m_fat_points;
	uint32_t			m_free_tag;
	b_mu_instance_vec		m_mu_instances;
	b_model_vec			m_models;
	b_model_instance_vec		m_instances;

	std::vector<uint16_t>		m_fake_mu_gamemtls;
	std::vector<uint16_t>		m_fake_gamemtls;
	std::vector<uint16_t>		m_inadhesive_shaders;
};

inline b_neighbour::b_neighbour(const xray_re::fvector3* _normal, uint16_t _gamemtl, bool _side):
	normal(_normal), gamemtl(_gamemtl), side(_side) {}

inline b_model::b_model(model_type type, size_t num_instances, size_t temp_index):
	m_type(type), m_locked(false),
	m_num_instances(uint16_t(num_instances & UINT16_MAX)), m_temp_index(temp_index) {}

inline b_model::b_model(model_type type, size_t temp_index):
	m_type(type), m_locked(false), m_num_instances(1), m_temp_index(temp_index) {}

inline size_t b_model::num_instances() const { return m_num_instances; }

inline b_model::model_type b_model::type() const { return m_type; }

inline b_mu_instance::b_mu_instance(uint16_t _model_id, uint16_t _sector, const xray_re::fmatrix& _xform):
	model_id(_model_id), sector(_sector), xform(_xform) {}

inline void level_mesh::enable_debug_cform() { m_debug_cform = true; }
inline void level_mesh::enable_debug_merge() { m_debug_merge = true; }
inline void level_mesh::enable_restricted_merge() { m_restricted_merge = true; }
inline void level_mesh::enable_aggressive_merge() { m_aggressive_merge = true; }
inline void level_mesh::enable_mt() { m_use_mt = true; }
inline void level_mesh::commit(xray_re::xr_object& object) { xr_mesh_builder::commit(object); }
inline const b_model_vec& level_mesh::models() const { return m_models; }

inline void level_mesh::add_fake_mu_gamemtl(uint16_t id) { m_fake_mu_gamemtls.push_back(id); }
inline void level_mesh::add_fake_gamemtl(uint16_t id) { m_fake_gamemtls.push_back(id); }
inline void level_mesh::add_inadhesive_shader(uint16_t id) { m_inadhesive_shaders.push_back(id); }

#endif
