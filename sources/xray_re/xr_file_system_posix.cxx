#ifdef __linux__

#include "xr_file_system_posix.h"

// Заголовочные файлы стандартной библиотеки C.
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Заголовочные файлы стандартной библиотеки C++.
#include <filesystem>


using namespace xray_re;
namespace fs = std::filesystem;


class CPath : public fs::path
{
public:

	CPath(const char* p_Path)
		: fs::path{p_Path}
	{}

	path& make_preferred()
	{
		value_type* ts = const_cast<value_type*>(c_str());

		do
		{
			if(*ts == '\\')
				*ts = preferred_separator;
		} while(*ts++ != '\0');

		return *this;
	}

	path& remove_filename()
	{
		auto ns = native();

		ns.resize(native().find_last_of(preferred_separator) + 1);
		assign(ns);

		return *this;
	}

	bool create_directory_recursive()
	{
		fs::path tp;
		char* ts = const_cast<char*>(c_str());
		char* sep = ts;

		// Пропускаем корневой каталог, если он указан. Его мы не создадим.
		if(*sep == preferred_separator) sep++;

		while(*sep != '\0')
		{
			sep = strchr(sep, preferred_separator);
			if(sep != nullptr)
			{
				*sep = '\0';
				tp.assign(ts);
				if(!fs::exists(tp))
				{
					if(!fs::create_directory(tp))
						return false;
				}

				*sep = preferred_separator;
				sep++;
			}
			else
			{
				tp.assign(ts);
				if(fs::exists(tp))
					return true;

				return fs::create_directory(tp);
			}
		}

		return true;
	}
};

/*****************************************************************************/
/****************************** xr_file_system *******************************/
/*****************************************************************************/

bool xr_file_system::create_folder(const char* path) const
{
	if(read_only())
	{
		dbg("fs_ro: creating folder %s", path);

		return true;
	}
	else
	{
		CPath p(path);

		p.make_preferred();
		if(fs::exists(p))
			return true;

		return fs::create_directory(p);
	}
}

bool xr_file_system::create_path(const char* path) const
{
	if(read_only())
	{
		dbg("fs_ro: creating path %s", path);

		return true;
	}
	else
	{
		CPath p(path);

		p.make_preferred();
		if(fs::exists(p))
			return true;

		return p.create_directory_recursive();
	}
}

void xr_file_system::split_path(const char* path, std::string* folder, std::string* name, std::string* extension)
{
CPath p(path);

	if(folder != nullptr)
		folder->assign(p.parent_path());

	if(name != nullptr)
		name->assign(p.stem());

	if(extension != nullptr)
		extension->assign(p.extension());
}

bool xr_file_system::folder_exist(const char* path)
{
CPath p(path);

	p.make_preferred();

	return fs::exists(p) && fs::is_directory(p);
}

bool xr_file_system::file_exist(const char* path)
{
CPath p(path);

	p.make_preferred();

	return fs::exists(p) && fs::is_regular_file(p);
}

size_t xr_file_system::file_length(const char* path)
{
	return fs::file_size(CPath(path).make_preferred());
}

uint32_t xr_file_system::file_age(const char* path)
{
struct stat st;

	if(stat(CPath(path).make_preferred().c_str(), &st) == 0)
		return uint32_t(st.st_mtime);

	return 0;
}

bool xr_file_system::copy_file(const char* src_path, const char* tgt_path) const
{
	if(read_only())
	{
		dbg("fs_ro: copying %s to %s", src_path, tgt_path);

		return true;
	}
	else
	{
		CPath from(src_path), to(tgt_path);

		from.make_preferred();
		to.make_preferred();

		return fs::copy_file(from, to);
	}
}

xr_reader* xr_file_system::r_open(const char* path) const
{
using namespace std;

CPath p(path);
xr_reader* r = nullptr;
uint8_t* data;
uintmax_t len;

	p.make_preferred();
	len = fs::file_size(p);
	if(len > numeric_limits<uint32_t>::max())
		return nullptr;

	data = (uint8_t*)malloc(len);
	if(data != nullptr)
	{
		FILE* infile = fopen(p.c_str(), "r");

		if((infile != nullptr) && (fread(data, 1, len, infile) == len))
			r = new xr_temp_reader(data, len);
		else
			free(data);
	}

	return r;
}

xr_writer* xr_file_system::w_open(const char* path, bool ignore_ro) const
{
FILE* outfile;
CPath p(path);

	if (!ignore_ro && read_only()) {
		dbg("fs_ro: writing %s", path);
		return new xr_fake_writer();
	}

	p.make_preferred();
	outfile = fopen(p.c_str(), "w");
	if(outfile == nullptr)
	{
		if(errno == ENOENT)
		{
			auto ds{p};

			ds.remove_filename();
			if(!ds.create_directory_recursive())
				return nullptr;

			return w_open(p.c_str(), ignore_ro);
		}
		else
		{
			return nullptr;
		}
	}

	xr_writer* w = new xr_file_writer_posix(outfile);

	if(w == nullptr)
		fclose(outfile);

	return w;
}

/*****************************************************************************/
/*************************** xr_file_writer_posix ****************************/
/*****************************************************************************/

xr_file_writer_posix::xr_file_writer_posix(FILE* p_FD)
	: m_FD(p_FD)
{}

xr_file_writer_posix::~xr_file_writer_posix()
{
	if(m_FD != nullptr)
		fclose(m_FD);
}

void xr_file_writer_posix::w_raw(const void* data, size_t size)
{
	xr_assert(fwrite(data, 1, size, m_FD) == size);
}

void xr_file_writer_posix::seek(size_t pos)
{
	xr_assert(fseek(m_FD, pos, SEEK_SET) == 0);
}

size_t xr_file_writer_posix::tell()
{
	return ftell(m_FD);
}

#endif // __linux__
