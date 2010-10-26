#include <string>
#include <cstring>
#include <cstdarg>
#include "xr_writer.h"
#include "xr_file_system.h"
#include "xr_packet.h"

using namespace xray_re;

void xr_writer::open_chunk(uint32_t id)
{
	w_u32(id);
	w_u32(0);
	m_open_chunks.push(tell());
}

void xr_writer::close_chunk()
{
	xr_assert(!m_open_chunks.empty());
	size_t pos = tell();
	size_t chunk_pos = m_open_chunks.top();
	xr_assert(chunk_pos <= pos);
	seek(chunk_pos - 4);
	w_size_u32(pos - chunk_pos);
	seek(pos);
	m_open_chunks.pop();
}

void xr_writer::w_raw_chunk(uint32_t id, const void* data, size_t size)
{
	w_u32(id);
	w_size_u32(size);
	w_raw(data, size);
}

void xr_writer::w_sz(const std::string& value)
{
	// do not write extra '\0'
//	size_t length = value.length() + 1;
//	const char* c_str = value.c_str();
//	if (len > 0 && c_str[len] == '\0')
	w_raw(value.data(), value.length() + 1);
}

void xr_writer::w_sz(const char* value)
{
	xr_assert(value);
	w_raw(value, std::strlen(value) + 1);
}

void xr_writer::w_s(const char* value)
{
	w_raw(value, std::strlen(value));
	w_raw("\r\n", 2);
}

void xr_writer::w_s(const std::string& value)
{
	w_raw(value.data(), value.length());
	w_raw("\r\n", 2);
}

void xr_writer::w_sf(const char* format, ...)
{
	char buf[256];
	va_list ap;
	va_start(ap, format);
#if defined(_MSC_VER) && _MSC_VER >= 1400
	int n = vsprintf_s(buf, sizeof(buf), format, ap);
#else
	int n = vsnprintf(buf, sizeof(buf), format, ap);
#endif
	va_end(ap);
	if (n > 0)
		w_raw(buf, n);
}

void xr_writer::w_float_q16(float value, float min, float max)
{
	w_u16(uint16_t((value - min)*65535.f/(max - min)));
}

void xr_writer::w_float_q8(float value, float min, float max)
{
	w_u8(uint8_t((value - min)*255.f/(max - min)));
}

void xr_writer::w_packet(const xr_packet& packet)
{
	w_raw(packet.buf(), packet.w_tell());
}

xr_memory_writer::xr_memory_writer(): m_pos(0) {}

xr_memory_writer::~xr_memory_writer() {}

void xr_memory_writer::w_raw(const void* data, size_t size)
{
	if (size) {
		if (m_pos + size > m_buffer.size())
			m_buffer.resize(m_pos + size);
		std::memmove(&m_buffer[m_pos], data, size);
		m_pos += size;
	}
}

void xr_memory_writer::seek(size_t pos)
{
	xr_assert(pos <= m_buffer.size());
	m_pos = pos;
}

size_t xr_memory_writer::tell()
{
	return m_pos;
}

bool xr_memory_writer::save_to(const char* path, const std::string& name)
{
	xr_file_system& fs = xr_file_system::instance();
	xr_writer* w = fs.w_open(path, name);
	if (w == 0)
		return false;
	w->w_raw(&m_buffer[0], m_buffer.size());
	fs.w_close(w);
	return true;
}

bool xr_memory_writer::save_to(const char* path)
{
	xr_file_system& fs = xr_file_system::instance();
	xr_writer* w = fs.w_open(path);
	if (w == 0)
		return false;
	w->w_raw(&m_buffer[0], m_buffer.size());
	fs.w_close(w);
	return true;
}

bool xr_memory_writer::save_to(const std::string& path)
{
	return save_to(path.c_str());
}

xr_fake_writer::xr_fake_writer(): m_pos(0), m_size(0) {}

xr_fake_writer::~xr_fake_writer() {}

void xr_fake_writer::w_raw(const void* data, size_t size)
{
	m_pos += size;
	if (m_size < m_pos)
		m_size = m_pos;
}

void xr_fake_writer::seek(size_t pos)
{
	assert(pos < m_size);
	m_pos = m_size;
}

size_t xr_fake_writer::tell()
{
	return m_pos;
}
