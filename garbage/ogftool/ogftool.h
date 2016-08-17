#ifndef __OGFTOOL_H__
#define __OGFTOOL_H__

#include "ogfdefs.h"

// little-endian only
class memory_reader {
private:
	const uint8_t	*m_data;
	size_t		m_size;
	size_t		m_offset;

	void		reset(const uint8_t *data, size_t size, size_t offset = 0);
public:
			memory_reader();
			memory_reader(const void *data, size_t size);
			~memory_reader();

	int		open_chunk(uint32_t, memory_reader &);
	int		open_chunk_next(uint32_t &, memory_reader &);

	void		r(void *, size_t);

	int		eof(void);
	size_t		elapsed(void);

	uint32_t	r_u32(void);
	uint16_t	r_u16(void);
	uint8_t		r_u8(void);

	int32_t		r_s32(void);
	int16_t		r_s16(void);
	int8_t		r_s8(void);

	char		*r_stringZ(void);

	float		r_float(void);

	void		r_vector3f(struct vector3f &);
	void		r_vector2f(struct vector2f &);
};

class memory_writer {
private:
	uint8_t		*m_data;
#define DEFAULT_BUFFER_SIZE		(32*1024)
	size_t		m_size;
	size_t		m_offset;

#define MAX_CHUNK_LEVEL			16
	unsigned	m_chunk_level;
	size_t		m_chunk_pos[MAX_CHUNK_LEVEL];

	void		extend_if_needed(size_t);
	void		clear(void);

public:
			memory_writer(void);
			~memory_writer(void);

	uint8_t		*data(void) { return m_data; }
	size_t		size(void) { return m_offset; }

	void		open_chunk(uint32_t);
	void		close_chunk(void);

	void		w(const void *, size_t);
	void		w_chunk(uint32_t, const void *, size_t);

	void		w_u32(uint32_t);
	void		w_u16(uint16_t);
	void		w_u8(uint8_t);

	void		w_s32(int32_t);
	void		w_s16(int16_t);
	void		w_s8(int8_t);

	void		w_stringZ(const char *);

	void		w_float(float);

	void		w_vector3f(const struct vector3f &);
	void		w_vector2f(const struct vector2f &);
};

struct ogf_bone_data {
	const char		*name;
	const char		*parent_name;
	struct obb		bbox;

	uint32_t		partition;	// really?
	const char		*game_mtl_name;
	struct s_bone_shape	shape;
	struct s_joint_ik_data	joint_ik_data;
	struct vector3f		bind_rotation;
	struct vector3f		bind_position;
	float			mass;
	struct vector3f		center_of_mass;
};

class ogf_data {
private:
	uint32_t		m_chunk_mask;

	// OGF_HEADER chunk
	struct ogf_header	m_header;

	// OGF_TEXTURE chunk
	const char		*m_texture_name;
	const char		*m_shader_name;

	// OGF_VERTICES chunk
	uint32_t		m_vertex_format;
	unsigned		m_vertex_count;
	struct vert_boned_1w	*m_vertices_1w;
	struct vert_boned_2w	*m_vertices_2w;

	// OGF_INDICES chunk
	unsigned		m_index_count;
	uint16_t		*m_indices;

	// OGF_SWIDATA chunk
	uint32_t		m_slide_window_reserved[4];
	unsigned		m_slide_window_count;
	struct slide_window	*m_slide_windows;

	// OGF_CHILDREN chunk
	unsigned		m_children_count;
	ogf_data		*m_children;

	// OGF_S_BONE_NAMES and OGF_S_IKDATA chunks
	unsigned		m_bone_count;
	struct ogf_bone_data	*m_bones;

	// OGF_S_MOTIONS chunk
	size_t			m_s_motions_size;
	void			*m_s_motions_raw;	// FIXME

	// OGF_S_SMPARAMS chunk
	size_t			m_s_smparams_size;
	void			*m_s_smparams_raw;	// FIXME

	// OGF_S_USERDATA chunk
	const char		*m_userdata;

	// OGF_S_DESC chunk
	const char		*m_source;
	const char		*m_convertor;
	uint32_t		m_convert_time;
	const char		*m_creator;
	uint32_t		m_create_time;
	const char		*m_editor;
	uint32_t		m_edit_time;

	// OGF_S_MOTION_REFS chunk
	const char		*m_s_motion_refs;

	void	clear();

	void	w_chunk_texture(memory_writer &);
	void	r_chunk_vertices(memory_reader &);
	void	w_chunk_vertices(memory_writer &);
	void	r_chunk_indices(memory_reader &);
	void	w_chunk_indices(memory_writer &);
	void	r_chunk_swidata(memory_reader &);
	void	w_chunk_swidata(memory_writer &);
	void	r_chunk_children(memory_reader &);
	void	w_chunk_children(memory_writer &);
	void	r_chunk_s_bone_names(memory_reader &);
	void	w_chunk_s_bone_names(memory_writer &);
	void	r_chunk_s_motions(memory_reader &);
	void	w_chunk_s_motions(memory_writer &);
	void	r_chunk_s_smparams(memory_reader &);
	void	w_chunk_s_smparams(memory_writer &);
	void	r_chunk_s_ikdata(memory_reader &);
	void	w_chunk_s_ikdata(memory_writer &);
	void	r_chunk_s_desc(memory_reader &);
	void	w_chunk_s_desc(memory_writer &);
public:
		ogf_data();
		~ogf_data();

	void	read(memory_reader &r);
	void	write(memory_writer &w);

	void	delete_child(unsigned);
	void	move(float, float, float);
};

class ogf_file {
private:
	ogf_data	m_ogf_data;
	uint8_t		*m_raw_data;

	void	clear(void);
public:
		ogf_file(void);
		~ogf_file(void);

	void	move(float dx, float dy, float dz) { m_ogf_data.move(dx, dy, dz); }
	void	delete_child(unsigned id) { m_ogf_data.delete_child(id); }

	int	read(const char *);
	int	write(const char *);
};

#endif
