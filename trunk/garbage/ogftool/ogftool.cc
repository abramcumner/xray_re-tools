#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include "ogftool.h"

memory_reader::memory_reader() : m_data(0), m_size(0), m_offset(0)
{
}

memory_reader::memory_reader(const void *data, size_t size)
{
	reset((const uint8_t *)data, size);
}

memory_reader::~memory_reader()
{
}

void memory_reader::reset(const uint8_t *data, size_t size, size_t offset)
{
	m_data = data;
	m_size = size;
	m_offset = offset;
}

int memory_reader::open_chunk(uint32_t id, memory_reader &chunk_r)
{
//	while (m_size - m_offset >= 8) {
//		uint32_t temp_id = r_u32();
//	}
	return 0;
}

int memory_reader::open_chunk_next(uint32_t &id, memory_reader &chunk_r)
{
	if (eof())
		return 0;
	assert(m_offset + 8 <= m_size);
	uint32_t chunk_id = r_u32();
	size_t chunk_size = r_u32();
	assert(m_offset + chunk_size <= m_size);
	id = chunk_id;
	chunk_r.reset(m_data + m_offset, chunk_size);
	m_offset += chunk_size;
	return 1;
}

int memory_reader::eof(void)
{
	return m_offset >= m_size;
}

size_t memory_reader::elapsed(void)
{
	return m_size - m_offset;
}

void memory_reader::r(void *data, size_t size)
{
	assert(m_offset + size <= m_size);
	memcpy(data, m_data + m_offset, size);
	m_offset += size;
}

char *memory_reader::r_stringZ(void)
{
	size_t offset = m_offset;
	assert(m_offset < m_size);
	while (m_offset < m_size && m_data[m_offset++] != 0) {}
	assert(m_offset <= m_size);
	return strdup((const char *)m_data + offset);
}

#define IMPLEMENT_R_METHOD(n,t) \
	t memory_reader::r_ ## n(void) { t v; r(&v, sizeof(t)); return v; }
IMPLEMENT_R_METHOD(u32, uint32_t)
IMPLEMENT_R_METHOD(u16, uint16_t)
IMPLEMENT_R_METHOD(u8, uint8_t)
IMPLEMENT_R_METHOD(s32, int32_t)
IMPLEMENT_R_METHOD(s16, int16_t)
IMPLEMENT_R_METHOD(s8, int8_t)
IMPLEMENT_R_METHOD(float, float)
void memory_reader::r_vector3f(struct vector3f &v) { r(&v, sizeof(v)); }
void memory_reader::r_vector2f(struct vector2f &v) { r(&v, sizeof(v)); }

memory_writer::memory_writer(void) : m_data(0), m_size(0), m_offset(0), m_chunk_level(0)
{
}

memory_writer::~memory_writer(void)
{
	clear();
}

void memory_writer::clear(void)
{
	if (m_data != 0) {
		delete[] m_data;
		m_data = 0;
		m_size = 0;
	}
}

void memory_writer::open_chunk(uint32_t id)
{
	extend_if_needed(8);
	assert(m_chunk_level < MAX_CHUNK_LEVEL);
	w_u32(id);
	m_chunk_pos[m_chunk_level++] = m_offset;
	m_offset += 4;
}

void memory_writer::close_chunk(void)
{
	assert(m_chunk_level > 0);
	size_t offset = m_offset;
	m_offset = m_chunk_pos[--m_chunk_level];
	w_u32(offset - m_offset - 4);
	m_offset = offset;
}

void memory_writer::extend_if_needed(size_t add_size)
{
	if (m_offset + add_size <= m_size)
		return;
	size_t size;
	if (m_size == 0) {
		assert(m_data == 0);
		size = m_size = DEFAULT_BUFFER_SIZE;
	} else {
		assert(m_data != 0);
		size = m_size << 1;
		if (size < add_size)
			size = m_size + add_size;
	}
	uint8_t *data = new uint8_t[size];
	assert(data != 0);
	if (m_data != 0) {
		memcpy(data, m_data, m_offset);
		delete[] m_data;
	}
	m_data = data;
	m_size = size;
}

void memory_writer::w(const void *data, size_t size)
{
	extend_if_needed(size);
	memcpy(m_data + m_offset, data, size);
	m_offset += size;
}

void memory_writer::w_stringZ(const char *s)
{
	w(s, strlen(s) + 1);
}

void memory_writer::w_chunk(uint32_t id, const void *data, size_t size)
{
	w_u32(id);
	w_u32(size);
	w(data, size);
}

#define IMPLEMENT_W_METHOD(n,t) \
	void memory_writer::w_ ## n(t v) { w(&v, sizeof(t)); }
IMPLEMENT_W_METHOD(u32, uint32_t)
IMPLEMENT_W_METHOD(u16, uint16_t)
IMPLEMENT_W_METHOD(u8, uint8_t)
IMPLEMENT_W_METHOD(s32, int32_t)
IMPLEMENT_W_METHOD(s16, int16_t)
IMPLEMENT_W_METHOD(s8, int8_t)
IMPLEMENT_W_METHOD(float, float)
void memory_writer::w_vector3f(const struct vector3f &v) { w(&v, sizeof(v)); }
void memory_writer::w_vector2f(const struct vector2f &v) { w(&v, sizeof(v)); }

ogf_data::ogf_data() : m_chunk_mask(0), m_children_count(0)
{
	memset(this, 0, sizeof(*this));
}

ogf_data::~ogf_data()
{
	clear();
}

void ogf_data::clear()
{
	if (m_source) {
		delete[] m_source;
		m_source = 0;
	}
	if (m_convertor) {
		delete[] m_convertor;
		m_convertor = 0;
	}
	memset(this, 0, sizeof(*this));
}

void ogf_data::read(memory_reader &base_r)
{
	memory_reader	r;
	uint32_t	id;
	while (base_r.open_chunk_next(id, r)) {
		m_chunk_mask |= 1 << id;
//		printf("id=%x\n", id);
		switch (id) {
		case OGF_HEADER:
			r.r(&m_header, sizeof(m_header));
			break;
		case OGF_TEXTURE:
			m_texture_name = r.r_stringZ();
			m_shader_name = r.r_stringZ();
			break;
		case OGF_VERTICES:
			r_chunk_vertices(r);
			break;
		case OGF_INDICES:
			r_chunk_indices(r);
			break;
		case OGF_SWIDATA:
			r_chunk_swidata(r);
			break;
		case OGF_CHILDREN:
			r_chunk_children(r);
			break;
		case OGF_S_BONE_NAMES:
			r_chunk_s_bone_names(r);
			break;
		case OGF_S_MOTIONS:
			r_chunk_s_motions(r);
			break;
		case OGF_S_SMPARAMS:
			r_chunk_s_smparams(r);
			break;
		case OGF_S_IKDATA:
			r_chunk_s_ikdata(r);
			break;
		case OGF_S_USERDATA:
			m_userdata = r.r_stringZ();
			break;
		case OGF_S_DESC:
			r_chunk_s_desc(r);
			break;
		case OGF_S_MOTION_REFS:
			m_s_motion_refs = r.r_stringZ();
			break;
		default:
			printf("unknown chunk id %d\n", id);
			break;
		}
		assert(r.elapsed() == 0);
	}
}

void ogf_data::write(memory_writer &w)
{
	if (m_chunk_mask & (1 << OGF_HEADER))
		w.w_chunk(OGF_HEADER, &m_header, sizeof(m_header));
	if (m_chunk_mask & (1 << OGF_S_DESC))
		w_chunk_s_desc(w);
	if (m_chunk_mask & (1 << OGF_TEXTURE))
		w_chunk_texture(w);
	if (m_chunk_mask & (1 << OGF_VERTICES))
		w_chunk_vertices(w);
	if (m_chunk_mask & (1 << OGF_INDICES))
		w_chunk_indices(w);
	if (m_chunk_mask & (1 << OGF_SWIDATA))
		w_chunk_swidata(w);
	if (m_chunk_mask & (1 << OGF_CHILDREN))
		w_chunk_children(w);
	if (m_chunk_mask & (1 << OGF_S_BONE_NAMES))
		w_chunk_s_bone_names(w);
	if (m_chunk_mask & (1 << OGF_S_IKDATA))
		w_chunk_s_ikdata(w);
	if (m_chunk_mask & (1 << OGF_S_USERDATA))
		w.w_chunk(OGF_S_USERDATA, m_userdata, strlen(m_userdata) + 1);
	if (m_chunk_mask & (1 << OGF_S_MOTIONS))
		w_chunk_s_motions(w);
	if (m_chunk_mask & (1 << OGF_S_SMPARAMS))
		w_chunk_s_smparams(w);
	if (m_chunk_mask & (1 << OGF_S_MOTION_REFS))
		w.w_chunk(OGF_S_MOTION_REFS, m_s_motion_refs, strlen(m_s_motion_refs) + 1);
}

void ogf_data::w_chunk_texture(memory_writer &w)
{
	w.open_chunk(OGF_TEXTURE);
	w.w_stringZ(m_texture_name);
	w.w_stringZ(m_shader_name);
	w.close_chunk();
}

void ogf_data::r_chunk_vertices(memory_reader &r)
{
	m_vertex_format = r.r_u32();
	m_vertex_count = r.r_u32();
//printf("vert_cnt=%d\n", m_vertex_count);
	if (m_vertex_format == OGF_VERTEXFORMAT_FVF_1L) {
		m_vertices_1w = new vert_boned_1w[m_vertex_count];
		r.r(m_vertices_1w, m_vertex_count * sizeof(vert_boned_1w));
	} else if (m_vertex_format == OGF_VERTEXFORMAT_FVF_2L) {
		m_vertices_2w = new vert_boned_2w[m_vertex_count];
		r.r(m_vertices_2w, m_vertex_count * sizeof(vert_boned_2w));
	} else if (m_vertex_format == OGF_VERTEXFORMAT_FVF_NL) {
		assert(0);
		// FIXME
	} else {
		assert(0);
		// FIXME
	}
}

void ogf_data::w_chunk_vertices(memory_writer &w)
{
	w.open_chunk(OGF_VERTICES);
	w.w_u32(m_vertex_format);
	w.w_u32(m_vertex_count);
	if (m_vertex_format == OGF_VERTEXFORMAT_FVF_1L) {
		w.w(m_vertices_1w, m_vertex_count * sizeof(vert_boned_1w));
	} else if (m_vertex_format == OGF_VERTEXFORMAT_FVF_2L) {
		w.w(m_vertices_2w, m_vertex_count * sizeof(vert_boned_2w));
	} else {
		assert(0);
	}
	w.close_chunk();
}

void ogf_data::r_chunk_indices(memory_reader &r)
{
	m_index_count = r.r_u32();
	m_indices = new uint16_t[m_index_count];
	r.r(m_indices, m_index_count * sizeof(uint16_t));
}

void ogf_data::w_chunk_indices(memory_writer &w)
{
	w.open_chunk(OGF_INDICES);
	w.w_u32(m_index_count);
	w.w(m_indices, m_index_count * sizeof(uint16_t));
	w.close_chunk();
}

void ogf_data::r_chunk_swidata(memory_reader &r)
{
	r.r(m_slide_window_reserved, sizeof(m_slide_window_reserved));
	m_slide_window_count = r.r_u32();
	m_slide_windows = new slide_window[m_slide_window_count];
	r.r(m_slide_windows, m_slide_window_count * sizeof(slide_window));
}

void ogf_data::w_chunk_swidata(memory_writer &w)
{
	w.open_chunk(OGF_SWIDATA);
	w.w(m_slide_window_reserved, sizeof(m_slide_window_reserved));
	w.w_u32(m_slide_window_count);
	w.w(m_slide_windows, m_slide_window_count * sizeof(slide_window));
	w.close_chunk();
}

void ogf_data::r_chunk_children(memory_reader &frame_r)
{
	memory_reader	r;
	uint32_t	id;

	m_children = new ogf_data[100];
	for (unsigned i = 0; frame_r.open_chunk_next(id, r); i++) {
		assert(i < 100);
		m_children[i].read(r);
		m_children_count = i + 1;
	}
}

void ogf_data::w_chunk_children(memory_writer &w)
{
	w.open_chunk(OGF_CHILDREN);
	for (unsigned i = 0; i < m_children_count; i++) {
		w.open_chunk(i);
		m_children[i].write(w);
		w.close_chunk();
	}
	w.close_chunk();
}

void ogf_data::r_chunk_s_bone_names(memory_reader &r)
{
	m_bone_count = r.r_u32();
	m_bones = new ogf_bone_data[m_bone_count];
	for (unsigned i = 0; i < m_bone_count; i++) {
		ogf_bone_data *bone = m_bones + i;
		bone->name = r.r_stringZ();
		bone->parent_name = r.r_stringZ();
		r.r(&bone->bbox, sizeof(struct obb));
	}
}

void ogf_data::w_chunk_s_bone_names(memory_writer &w)
{
	w.open_chunk(OGF_S_BONE_NAMES);
	w.w_u32(m_bone_count);
	for (unsigned i = 0; i < m_bone_count; i++) {
		ogf_bone_data *bone = m_bones + i;
		w.w_stringZ(bone->name);
		w.w_stringZ(bone->parent_name);
		w.w(&bone->bbox, sizeof(struct obb));
	}
	w.close_chunk();
}

void ogf_data::r_chunk_s_ikdata(memory_reader &r)
{
	for (unsigned i = 0; i < m_bone_count; i++) {
		ogf_bone_data *bone = m_bones + i;
		bone->partition = r.r_u32();
		bone->game_mtl_name = r.r_stringZ();
		r.r(&bone->shape, sizeof(struct s_bone_shape));
		r.r(&bone->joint_ik_data, sizeof(struct s_joint_ik_data));
		r.r_vector3f(bone->bind_rotation);
		r.r_vector3f(bone->bind_position);
		bone->mass = r.r_float();
		r.r_vector3f(bone->center_of_mass);
	}
}

void ogf_data::w_chunk_s_ikdata(memory_writer &w)
{
	w.open_chunk(OGF_S_IKDATA);
	for (unsigned i = 0; i < m_bone_count; i++) {
		ogf_bone_data *bone = m_bones + i;
		w.w_u32(bone->partition);
		w.w_stringZ(bone->game_mtl_name);
		w.w(&bone->shape, sizeof(struct s_bone_shape));
		w.w(&bone->joint_ik_data, sizeof(struct s_joint_ik_data));
		w.w_vector3f(bone->bind_rotation);
		w.w_vector3f(bone->bind_position);
		w.w_float(bone->mass);
		w.w_vector3f(bone->center_of_mass);
	}
	w.close_chunk();
}

void ogf_data::r_chunk_s_motions(memory_reader &r)
{
	m_s_motions_size = r.elapsed();
	m_s_motions_raw = new uint8_t[m_s_motions_size];
	r.r(m_s_motions_raw, m_s_motions_size);
}

void ogf_data::w_chunk_s_motions(memory_writer &w)
{
	w.open_chunk(OGF_S_MOTIONS);
	w.w(m_s_motions_raw, m_s_motions_size);
	w.close_chunk();
}

void ogf_data::r_chunk_s_smparams(memory_reader &r)
{
	m_s_smparams_size = r.elapsed();
	m_s_smparams_raw = new uint8_t[m_s_smparams_size];
	r.r(m_s_smparams_raw, m_s_smparams_size);
}

void ogf_data::w_chunk_s_smparams(memory_writer &w)
{
	w.open_chunk(OGF_S_SMPARAMS);
	w.w(m_s_smparams_raw, m_s_smparams_size);
	w.close_chunk();
}

void ogf_data::r_chunk_s_desc(memory_reader &r)
{
	m_source = r.r_stringZ();
	m_convertor = r.r_stringZ();
	m_convert_time = r.r_u32();
	m_creator = r.r_stringZ();
	m_create_time = r.r_u32();
	m_editor = r.r_stringZ();
	m_edit_time = r.r_u32();
}

void ogf_data::w_chunk_s_desc(memory_writer &w)
{
	w.open_chunk(OGF_S_DESC);
	w.w_stringZ(m_source);
	w.w_stringZ(m_convertor);
	w.w_u32(m_convert_time);
	w.w_stringZ(m_creator);
	w.w_u32(m_create_time);
	w.w_stringZ(m_editor);
	w.w_u32(m_edit_time);
	w.close_chunk();
}

void ogf_data::delete_child(unsigned id)
{
	assert(id == m_children_count - 1);
	--m_children_count;
}

void ogf_data::move(float dx, float dy, float dz)
{
	if (m_chunk_mask & (1 << OGF_HEADER)) {
		m_header.bb.min.y += dy;
		m_header.bb.max.y += dy;
		m_header.bs.c.y += dy;
	}
	if (m_chunk_mask & (1 << OGF_VERTICES)) {
		for (unsigned i = 0; i < m_vertex_count; i++) {
			if (m_vertex_format == OGF_VERTEXFORMAT_FVF_1L) {
				m_vertices_1w[i].p.y += dy;
			} else if (m_vertex_format == OGF_VERTEXFORMAT_FVF_2L) {
				m_vertices_2w[i].p.y += dy;
			}
		}
	}
	if (m_chunk_mask & (1 << OGF_S_BONE_NAMES)) {
//		for (unsigned i = 0; i < m_bone_count; i++) {
		for (unsigned i = 0; i < 1; i++) {
			ogf_bone_data *bone = m_bones + i;
			bone->bbox.translate.y += dy;
			switch (bone->shape.type) {
			case ST_BOX:
				bone->shape.box.translate.y += dy;
				break;
			case ST_SPHERE:
				bone->shape.sphere.p.y += dy;
				break;
			case ST_CYLINDER:
				bone->shape.cylinder.center.y += dy;
				break;
			}
			bone->bind_position.y += dy;
		}
	}
	for (unsigned i = 0; i < m_children_count; i++)
		m_children[i].move(dx, dy, dz);
}

ogf_file::ogf_file(void) : m_raw_data(0)
{
}

ogf_file::~ogf_file(void)
{
	clear();
}

void ogf_file::clear(void)
{
	if (m_raw_data) {
		delete[] m_raw_data;
		m_raw_data = 0;
	}
}

int ogf_file::read(const char *name)
{
	FILE *fp = fopen(name, "rb");
	assert(fp != NULL);
	if (fp == NULL) {
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	m_raw_data = new uint8_t[size];
	assert(m_raw_data != 0);
	fread(m_raw_data, 1, size, fp);
	memory_reader r(m_raw_data, size);
	m_ogf_data.read(r);
	fclose(fp);

	return 0;
}

int ogf_file::write(const char *name)
{
	FILE *fp = fopen(name, "wb+");
	assert(fp != NULL);
	if (fp == NULL) {
		return -1;
	}
	memory_writer w;
	m_ogf_data.write(w);
	fwrite(w.data(), 1, w.size(), fp);
	fclose(fp);
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 4) {
#if DY
		fprintf(stderr, "usage: ogftool <input_file> <output_file> <dy>\n");
#else
		fprintf(stderr, "usage: ogftool <input_file> <output_file> <child_id>\n");
#endif
		return -1;
	}
	const char *input_file = argv[1];
	const char *output_file = argv[2];
	ogf_file ogf;
	ogf.read(input_file);
#if DY
	float dy = atof(argv[3]);
	printf("dy: %f\n", dy);
	ogf.move(0, dy, 0);
#else
	unsigned id = atol(argv[3]);
	printf("child_id: %d\n", id);
	ogf.delete_child(id);
#endif
	ogf.write(output_file);
	return 0;
}
