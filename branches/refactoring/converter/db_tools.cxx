#define NOMINMAX
#include <windows.h>
#include <cstring>
#include "db_tools.h"
#include "xr_scrambler.h"
#include "xr_lzhuf.h"
#include "xr_file_system.h"
#include "xr_utils.h"
#include "xr_string_utils.h"
#include "lzo/minilzo.h"
#include "crc32/crc32.h"

#define DB_DEBUG	0

using namespace xray_re;

bool db_tools::is_xrp(const std::string& extension)
{
	return extension == ".xrp";
}

bool db_tools::is_xp(const std::string& extension)
{
	return extension.compare(0, 3, ".xp") == 0 &&
			extension.size() == 4 && std::isalnum(extension[3]);
}

bool db_tools::is_xdb(const std::string& extension)
{
	return extension.compare(0, 4, ".xdb") == 0 &&
			extension.size() == 5 && std::isalnum(extension[4]);
}

bool db_tools::is_db(const std::string& extension)
{
	return extension.compare(0, 3, ".db") == 0 &&
			extension.size() == 4 && std::isalnum(extension[3]);
}

bool db_tools::is_known(const std::string& extension)
{
	return is_db(extension) || is_xdb(extension) || is_xrp(extension) || is_xp(extension);
}

db_packer::~db_packer()
{
	delete_elements(m_files);
}

static bool write_file(xr_file_system& fs, const std::string& path, const void* data, size_t size)
{
	xr_writer* w = fs.w_open(path);
	if (w) {
		w->w_raw(data, size);
		fs.w_close(w);
		return true;
	}
	return false;
}

void db_unpacker::process(const cl_parser& cl)
{
	if (cl.num_params() != 1) {
		msg("incorrect number of parameters");
		return;
	}

	const char* source = cl.param(0);
	std::string prefix, extension;
	xr_file_system::split_path(source, 0, &prefix, &extension);

	unsigned version = DB_VERSION_AUTO;
	if (cl.get_bool("-11xx"))
		version |= DB_VERSION_1114;
	if (cl.get_bool("-2215"))
		version |= DB_VERSION_2215;
	if (cl.get_bool("-2945"))
		version |= DB_VERSION_2945;
	if (cl.get_bool("-2947ru"))
		version |= DB_VERSION_2947RU;
	if (cl.get_bool("-2947ww"))
		version |= DB_VERSION_2947WW;
	if (cl.get_bool("-xdb"))
		version |= DB_VERSION_XDB;
	if (version == DB_VERSION_AUTO) {
		if (is_xdb(extension))
			version |= DB_VERSION_XDB;
		else if (is_xrp(extension))
			version |= DB_VERSION_1114;
		else if (is_xp(extension))
			version |= DB_VERSION_2215;
	}
	if (version == DB_VERSION_AUTO || (version & (version - 1)) != 0) {
		msg("unspecified DB format");
		return;
	}

	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(source);
	if (r == 0) {
		msg("can't load %s", source);
		return;
	}

	cl.get_string("-dir", prefix);
	if (fs.create_path(prefix)) {
		xr_file_system::append_path_separator(prefix);
		xr_scrambler scrambler;
		xr_reader* s = 0;
		switch (version) {
		case DB_VERSION_1114:
		case DB_VERSION_2215:
		case DB_VERSION_2945:
		case DB_VERSION_XDB:
			s = r->open_chunk(DB_CHUNK_HEADER);
			break;
		case DB_VERSION_2947RU:
			scrambler.init(xr_scrambler::CC_RU);
			s = r->open_chunk(DB_CHUNK_HEADER, scrambler);
			break;
		case DB_VERSION_2947WW:
			scrambler.init(xr_scrambler::CC_WW);
			s = r->open_chunk(DB_CHUNK_HEADER, scrambler);
			break;
		}
		if (s) {
			const uint8_t* data = static_cast<const uint8_t*>(r->data());
			switch (version) {
			case DB_VERSION_1114:
				extract_1114(prefix, s, data);
				break;
			case DB_VERSION_2215:
				extract_2215(prefix, s, data);
				break;
			case DB_VERSION_2945:
				extract_2945(prefix, s, data);
				break;
			case DB_VERSION_2947RU:
			case DB_VERSION_2947WW:
			case DB_VERSION_XDB:
				extract_2947(prefix, s, data);
				break;
			}
			r->close_chunk(s);
		}
		if (0 && (s = r->open_chunk(DB_CHUNK_USERDATA))) {
			std::string fname;
			fs.split_path(source, 0, &fname);
			write_file(fs, fname.append("_userdata.ltx"), s->data(), s->size());
			r->close_chunk(s);
		}
	} else {
		msg("can't create %s", prefix.c_str());
	}
	fs.r_close(r);
}

static bool write_file(xr_file_system& fs, const std::string& path, const uint8_t* data, uint32_t size_real, uint32_t size_compressed)
{
	if (size_real != size_compressed) {
		lzo_uint size = size_real;
		uint8_t* temp = new uint8_t[size];
		if (lzo1x_decompress_safe(data, size_compressed, temp, &size, 0) != LZO_E_OK) {
			delete temp;
			return false;
		}
		data = temp;
		size_real = uint32_t(size & UINT32_MAX);
	}
	bool success = write_file(fs, path, data, size_real);
	if (!success) {
		std::string folder;
		fs.split_path(path, &folder);
		success = !fs.folder_exist(folder) && fs.create_path(folder) &&
				write_file(fs, path, data, size_real);
	}
	if (size_real != size_compressed)
		delete[] data;
	if (!success)
		msg("can't write %s", path.c_str());
	return success;
}

void db_unpacker::extract_1114(const std::string& prefix, xr_reader* s, const uint8_t* data) const
{
	xr_file_system& fs = xr_file_system::instance();
	for (std::string temp, path, folder; !s->eof(); ) {
		s->r_sz(temp);
		unsigned uncompressed = s->r_u32();
		unsigned offset = s->r_u32();
		unsigned size = s->r_u32();
		if (DB_DEBUG && fs.read_only()) {
			msg("%s", temp.c_str());
			msg("  offset: %u", offset);
			if (uncompressed)
				msg("  size (real): %u", size);
			else
				msg("  size (compressed): %u", size);
		} else {
			path = prefix;
			fs.split_path(path.append(temp), &folder);
			if (!fs.folder_exist(folder))
				fs.create_path(folder);
			if (uncompressed) {
				write_file(fs, path, data + offset, size);
			} else {
				size_t real_size;
				uint8_t* p;
				xr_lzhuf::decompress(p, real_size, data + offset, size);
				if (real_size)
					write_file(fs, path, p, real_size);
				free(p);
			}
		}
	}
}

void db_unpacker::extract_2215(const std::string& prefix, xr_reader* s, const uint8_t* data) const
{
	xr_file_system& fs = xr_file_system::instance();
	for (std::string path; !s->eof(); ) {
		s->r_sz(path);
		unsigned offset = s->r_u32();
		unsigned size_real = s->r_u32();
		unsigned size_compressed = s->r_u32();
		if (DB_DEBUG && fs.read_only()) {
			msg("%s", path.c_str());
			msg("  offset: %u", offset);
			msg("  size (real): %u", size_real);
			msg("  size (compressed): %u", size_compressed);
		} else if (offset == 0) {
			fs.create_folder(prefix + path);
		} else {
			write_file(fs, prefix + path, data + offset, size_real, size_compressed);
		}
	}
}

void db_unpacker::extract_2945(const std::string& prefix, xr_reader* s, const uint8_t* data) const
{
	xr_file_system& fs = xr_file_system::instance();
	for (std::string path; !s->eof(); ) {
		s->r_sz(path);
		unsigned crc = s->r_u32();
		unsigned offset = s->r_u32();
		unsigned size_real = s->r_u32();
		unsigned size_compressed = s->r_u32();
		if (DB_DEBUG && fs.read_only()) {
			msg("%s", path.c_str());
			msg("  crc: 0x%8.8x", crc);
			msg("  offset: %u", offset);
			msg("  size (real): %u", size_real);
			msg("  size (compressed): %u", size_compressed);
		} else if (offset == 0) {
			fs.create_folder(prefix + path);
		} else {
			write_file(fs, prefix + path, data + offset, size_real, size_compressed);
		}
	}
}

void db_unpacker::extract_2947(const std::string& prefix, xr_reader* s, const uint8_t* data) const
{
	xr_file_system& fs = xr_file_system::instance();
	for (std::string path; !s->eof(); ) {
		size_t name_size = s->r_u16() - 16;
		unsigned size_real = s->r_u32();
		unsigned size_compressed = s->r_u32();
		uint32_t crc = s->r_u32();
		path.assign(prefix);
		const char* name = s->skip<char>(name_size);
		path.append(name, name_size);
		uint32_t offset = s->r_u32();
		if (DB_DEBUG && fs.read_only()) {
			msg("%s", std::string(name, name_size).c_str());
			msg("  offset: %u", offset);
			msg("  size (real): %u", size_real);
			msg("  size (compressed): %u", size_compressed);
			msg("  crc: 0x%8.8"PRIx32, crc);
		} else if (offset == 0) {
			fs.create_path(path);
		} else {
			write_file(fs, path, data + offset, size_real, size_compressed);
		}
	}
}

void db_packer::process(const cl_parser& cl)
{
	if (cl.num_params() != 1) {
		msg("incorrect number of parameters");
		return;
	}

	for (size_t i = 0, num_params = cl.num_params(); i != num_params; ++i) {
		const char* source = cl.param(0);
		if (!xr_file_system::folder_exist(source)) {
			msg("can't find %s", source);
			return;
		}
	}

	std::string target, extension;
	if (!cl.get_string("-out", target)) {
		msg("unspecifed output file");
		return;
	}
	xr_file_system::split_path(target, 0, 0, &extension);

	unsigned version = DB_VERSION_AUTO;
	if (cl.get_bool("-11xx") || cl.get_bool("-2215") || cl.get_bool("-2945")) {
		msg("unsupported DB format");
		return;
	}
	if (cl.get_bool("-2947ru"))
		version |= DB_VERSION_2947RU;
	if (cl.get_bool("-2947ww"))
		version |= DB_VERSION_2947WW;
	if (cl.get_bool("-xdb") || is_xdb(extension))
		version |= DB_VERSION_XDB;
	if (version == DB_VERSION_AUTO || (version & (version - 1)) != 0) {
		msg("unspecified DB format");
		return;
	}

	xr_file_system& fs = xr_file_system::instance();
	m_archive = fs.w_open(target);
	if (m_archive == 0) {
		msg("can't load %s", target.c_str());
		return;
	}

	const char* userdata;
	if (version == DB_VERSION_XDB && cl.get_string("-xdb_ud", userdata)) {
		if (xr_reader* r = fs.r_open(userdata)) {
			m_archive->open_chunk(DB_CHUNK_USERDATA);
			m_archive->w_raw(r->data(), r->size());
			m_archive->close_chunk();
			fs.r_close(r);
		} else {
			msg("can't load %s", userdata);
		}
	}

	m_archive->open_chunk(DB_CHUNK_DATA);
	for (size_t i = 0, num_params = cl.num_params(); i != num_params; ++i) {
		m_root = cl.param(i);
		xr_strlwr(m_root);
		xr_file_system::append_path_separator(m_root);
		process_folder();
	}
	m_archive->close_chunk();

	xr_memory_writer* w = new xr_memory_writer;
	msg("folders:");
	for (std::vector<std::string>::iterator it = m_folders.begin(),
			end = m_folders.end(); it != end; ++it) {
		w->w_size_u16(it->size() + 16);
		w->w_u32(0);
		w->w_u32(0);
		w->w_u32(0);
		w->w_raw(it->data(), it->size());
		msg("  %s", it->c_str());
		w->w_u32(0);
	}
	msg("files: ");
	for (std::vector<db_file*>::iterator it = m_files.begin(),
			end = m_files.end(); it != end; ++it) {
		db_file* file = *it;
		w->w_size_u16(file->path.size() + 16);
		w->w_size_u32(file->size_real);
		w->w_size_u32(file->size_compressed);
		w->w_u32(file->crc);
		w->w_raw(file->path.data(), file->path.size());
		msg("  %s", file->path.c_str());
		w->w_size_u32(file->offset);
	}
	uint8_t* data = 0;
	size_t size = 0;
	xr_lzhuf::compress(data, size, w->data(), w->tell());
	delete w;
	if (version == DB_VERSION_2947RU) {
		xr_scrambler scrambler(xr_scrambler::CC_RU);
		scrambler.encrypt(data, data, size);
	} else if (version == DB_VERSION_2947WW) {
		xr_scrambler scrambler(xr_scrambler::CC_WW);
		scrambler.encrypt(data, data, size);
	}
	m_archive->open_chunk(DB_CHUNK_HEADER|xr_reader::CHUNK_COMPRESSED);
	m_archive->w_raw(data, size);
	m_archive->close_chunk();
	delete data;
	fs.w_close(m_archive);
}

void db_packer::process_folder(const std::string& path)
{
	if (!path.empty()) {
		for (std::vector<std::string>::iterator it = m_folders.begin(),
				end = m_folders.end(); it != end; ++it) {
			if (path.compare(0, it->size(), *it) == 0) {
				*it = path;
				goto found;
			}
		}
		m_folders.push_back(path);
	}
found:
	WIN32_FIND_DATAA info;
	HANDLE h = FindFirstFileA((m_root + path + '*').c_str(), &info);
	if (h == INVALID_HANDLE_VALUE)
		return;
	do {
		xr_strlwr(info.cFileName, sizeof(info.cFileName));
		if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (std::strcmp(info.cFileName, ".") != 0 && std::strcmp(info.cFileName, "..") != 0)
				process_folder((path + info.cFileName) + '\\');
		} else {
			process_file(path + info.cFileName);
		}
	} while (FindNextFileA(h, &info));
	FindClose(h);
}

void db_packer::process_file(const std::string& path)
{
	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(m_root + path);
	if (r) {
		size_t offset = m_archive->tell(), size = r->size();
		uint32_t crc = crc32(r->data(), size);
		m_archive->w_raw(r->data(), size);
		fs.r_close(r);

		db_file* file = new db_file;
		file->path = path;
		file->crc = crc;
		file->offset = offset;
		file->size_real = size;
		file->size_compressed = size;
		m_files.push_back(file);
	}
}
