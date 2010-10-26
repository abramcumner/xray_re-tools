#ifndef __GNUC__
#pragma once
#endif
#ifndef __DB_TOOLS_H__
#define __DB_TOOLS_H__

#include <string>
#include <vector>
#include "tools_base.h"
#include "xr_types.h"

namespace xray_re {
	class xr_reader;
	class xr_writer;
};

class db_tools: public tools_base {
public:
	virtual void	process(const cl_parser& cl) = 0;
	static bool	is_db(const std::string& extension);
	static bool	is_xdb(const std::string& extension);
	static bool	is_xrp(const std::string& extension);
	static bool	is_xp(const std::string& extension);
	static bool	is_known(const std::string& extension);

	enum {
		DB_CHUNK_DATA		= 0,
		DB_CHUNK_HEADER		= 1,
		DB_CHUNK_USERDATA	= 0x29a,
	};

	enum db_version {
		DB_VERSION_AUTO		= 0,
		DB_VERSION_1114		= 0x01,
		DB_VERSION_2215		= 0x02,
		DB_VERSION_2945		= 0x04,
		DB_VERSION_2947RU	= 0x08,
		DB_VERSION_2947WW	= 0x10,
		DB_VERSION_XDB		= 0x20,
	};

	struct db_file {
		bool		operator<(const db_file& file) const;

		std::string	path;
		size_t		offset;
		size_t		size_real;
		size_t		size_compressed;
		unsigned int	crc;
	};
};

class db_unpacker: public db_tools {
public:
	virtual void		process(const cl_parser& cl);

protected:
	void			extract_1114(const std::string& prefix, xray_re::xr_reader* s, const uint8_t* data) const;
	void			extract_2215(const std::string& prefix, xray_re::xr_reader* s, const uint8_t* data) const;
	void			extract_2945(const std::string& prefix, xray_re::xr_reader* s, const uint8_t* data) const;
	void			extract_2947(const std::string& prefix, xray_re::xr_reader* s, const uint8_t* data) const;
};

class db_packer: public db_tools {
public:
	virtual			~db_packer();

	virtual void		process(const cl_parser& cl);

protected:
	void			process_folder(const std::string& path = "");
	void			process_file(const std::string& path);
	void			add_folder(const std::string& path);

protected:
	xray_re::xr_writer*		m_archive;
	std::string			m_root;
	std::vector<std::string>	m_folders;
	std::vector<db_file*>		m_files;
};

#endif
