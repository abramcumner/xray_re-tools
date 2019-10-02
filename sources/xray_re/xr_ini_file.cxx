#include <cstdlib>
#include <algorithm>
#include "xr_file_system.h"
#include "xr_ini_file.h"
#include "xr_clsid.h"
#include "xr_string_utils.h"
#include "xr_utils.h"

using namespace xray_re;

enum {
	IF_EOL	= 0x100,
	IF_EOF	= 0x101,
};

xr_ini_file::xr_ini_file(const char* path, bool isReadOnly) {
	load(path);

	mIsReadOnly = isReadOnly;
}

xr_ini_file::~xr_ini_file()
{
	if (!mIsReadOnly && !empty())
		if (!save_as())
			msg("can`t save inifile:", m_file_name);

	clear();
}

//------------------------------------------------------------------------------
//Тело функций xr_ini_file
//------------------------------------------------------------------------------
bool _parse(char* dest, const char* src)
{
	bool bInsideSTR = false;
	if (src) {
		while (*src) {
			if (isspace((unsigned char)*src)) {
				if (bInsideSTR){
					*dest++ = *src++;
					continue;
				} 
				
				while (*src && isspace(*src)) {
					++src;
				}
				continue;
			} else if (*src=='"') {
				bInsideSTR = !bInsideSTR;
			}
			*dest++ = *src++;
		}
	}
	*dest = 0;
	return bInsideSTR;
}

void _decorate(char* dest, const char* src)
{
	if (src) {
		bool bInsideSTR = false;
		while (*src) {
			if (*src == ',') {
				if (bInsideSTR) { *dest++ = *src++; }
				else			{ *dest++ = *src++; *dest++ = ' '; }
				continue;
			} else if (*src == '"') {
				bInsideSTR = !bInsideSTR;
			}
			*dest++ = *src++;
		}
	}
	*dest = 0;
}

char* _TrimRight(char* str)
{
	char* p = str + ((uint32_t)strlen(str));
	while((p!=str) && (unsigned char(*p) <= unsigned char(' '))) p--;
	*(++p) = 0;
	return str;
}

//------------------------------------------------------------------------------
void xr_ini_file::save_as(xr_writer& writer) const
{
/*  // Writing all not line comments TODO
	for (ini_comment_vec_cit r_it = m_comments.begin(); r_it != m_comments.end(); ++r_it) {
		writer.w_s((*r_it)->comment.c_str());
	}*/

	char temp[4096], val[4096];
	for (ini_section_vec_cit r_it = m_sections.begin(); r_it != m_sections.end(); ++r_it) {
		xr_snprintf(temp, sizeof(temp), "[%s]", (*r_it)->name.c_str());
		writer.w_s(temp);

		for (ini_item_vec_cit s_it=(*r_it)->items.begin(); s_it!=(*r_it)->items.end(); ++s_it) {
			const ini_item*	item = *s_it;
			if (*item->name.c_str()) {
				if (*item->value.c_str()) {
					_decorate(val, item->value.c_str());
					// only name and value
					xr_snprintf(temp, sizeof(temp), "%-30s = %-32s", item->name.c_str(), item->value.c_str()/*, item->comment.c_str()*/);
				} else {
					// only name
					xr_snprintf(temp, sizeof(temp), "%-30s", item->name.c_str()/*, item->comment.c_str()*/);
				}
			} else {
				// no name, so no value
				temp[0] = 0;
			}
			_TrimRight(temp);
			if (temp[0])		
				writer.w_s(temp);
		}
		writer.w_s(" ");
	}
}

bool xr_ini_file::save_as(const char* new_fname)
{
	// save if needed
	if (new_fname && new_fname[0])
		m_file_name = (char*) new_fname;

	xr_assert(m_file_name && m_file_name[0]);

	xr_file_system& fs = xr_file_system::instance();
	xr_writer* F = fs.w_open(m_file_name);
	if (!F)
		return false;

	save_as(*F);
	fs.w_close(F);
	return true;
}

struct xr_ini_file::ini_section_pred {
	const char* name;
	explicit ini_section_pred(const char* _name): name(_name) {}
	bool operator()(const ini_section* l) const { return xr_stricmp(l->name.c_str(), name) < 0; }
};

struct xr_ini_file::ini_item_pred {
	const char* name;
	explicit ini_item_pred(const char* _name): name(_name) {}
	bool operator()(const ini_item* l) const { return xr_stricmp(l->name.c_str(), name) < 0; }
};

/*
struct xr_ini_file::ini_comment_pred {
	const char* comment;
	explicit ini_comment_pred(const char* _comment): comment(_comment) {}
	bool operator()(const ini_comment* l) const { return xr_stricmp(l->comment.c_str(), comment) < 0;}
};*/

void xr_ini_file::clear()
{
	trim_container(m_sections);
}

inline bool xr_ini_file::ini_item::operator<(const ini_item& right) const
{
	return xr_stricmp(name.c_str(), right.name.c_str()) < 0;
}

xr_ini_file::ini_section::~ini_section()
{
	delete_elements(items);
}

xr_ini_file::ini_section* xr_ini_file::r_section(const char* sname) const
{
	ini_section_vec_cit it = lower_bound_if(m_sections.begin(), m_sections.end(), ini_section_pred(sname));
	if (it == m_sections.end() || xr_stricmp((*it)->name.c_str(), sname) != 0) {
		msg("can't find section %s", sname);
		xr_not_expected();
	}
	return *it;
}

bool xr_ini_file::ini_section::line_exist(const char* lname, const char** lvalue) const
{
	ini_item_vec_cit it = lower_bound_if(begin(), end(), ini_item_pred(lname));
	if (it == end() || xr_stricmp((*it)->name.c_str(), lname) != 0)
		return false;
	if (lvalue)
		*lvalue = (*it)->value.c_str();
	return true;
}

bool xr_ini_file::line_exist(const char* sname, const char* lname) const
{
	ini_section_vec_cit it = lower_bound_if(m_sections.begin(), m_sections.end(), ini_section_pred(sname));
	if (it == m_sections.end() || xr_stricmp((*it)->name.c_str(), sname) != 0)
		return false;
	return (*it)->line_exist(lname, 0);
}

size_t xr_ini_file::line_count(const char* sname) const
{
	return r_section(sname)->size();
}

bool xr_ini_file::section_exist(const char* sname) const
{
	ini_section_vec_cit it = lower_bound_if(m_sections.begin(), m_sections.end(), ini_section_pred(sname));
	return it != m_sections.end() && xr_stricmp((*it)->name.c_str(), sname) == 0;
}

uint64_t xr_ini_file::r_clsid(const char* sname, const char* lname) const
{
	return xr_clsid::to_quad(r_string(sname, lname));
}

const char* xr_ini_file::r_string(const char* sname, const char* lname) const
{
	const ini_section* section = r_section(sname);
	ini_item_vec_cit it = lower_bound_if(section->begin(), section->end(), ini_item_pred(lname));
	if (it == section->end() || xr_stricmp((*it)->name.c_str(), lname) != 0) {
		msg("can't find item %s in section %s", lname, sname);
		xr_not_expected();
	}
	return (*it)->value.c_str();
}

bool xr_ini_file::is_true(const char* value)
{
	if (xr_stricmp(value, "true") == 0 || xr_stricmp(value, "on") == 0 ||
			xr_stricmp(value, "yes") == 0 || std::strcmp(value, "1") == 0) {
		return true;
	}
	return false;
}

bool xr_ini_file::r_bool(const char* sname, const char* lname) const
{
	return is_true(r_string(sname, lname));
}

float xr_ini_file::r_float(const char* sname, const char* lname) const
{
	return float(std::atof(r_string(sname, lname)));
}

bool xr_ini_file::r_line(const char* sname, size_t lindex, const char** lname, const char** lvalue) const
{
	const ini_section* section = r_section(sname);
	if (lindex >= section->size())
		return false;
	const ini_item* item = section->items[lindex];
	if (lname)
		*lname = item->name.c_str();
	if (lvalue)
		*lvalue = item->value.c_str();
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Write functions
//--------------------------------------------------------------------------------------
void xr_ini_file::w_string(const char* sname, const char* lname, const char* value/*, const char* comment*/) {
	// section
	char sect[256];
	_parse(sect, sname);
	_strlwr_s(sect);

	if (!section_exist(sect))	
	{
		// create _new_ section
		ini_section_vec_cit it = lower_bound_if(m_sections.begin(), m_sections.end(), ini_section_pred(sect));

		ini_section* section = new ini_section(sect);
		m_sections.insert(it, section);
	}

	// duplicate & insert
	std::string line_name = lname;
	ini_item* item = new ini_item(line_name);
	ini_section* section = r_section(sect);

	item->value = value;
	/*item->comment = comment;*/

	ini_item_vec_it it = lower_bound_if(section->begin(), section->end(), ini_item_pred(lname));

	if (it != section->end()) {
		// Check for "name" matching
		if (0 == xr_stricmp((*it)->name.c_str(), item->name.c_str())) {
			*it = item;
		} else {
			section->items.insert(it, item);
		}
	} else {
		section->items.insert(it, item);
	}
}

void xr_ini_file::ini_section::merge(const ini_section* section)
{
	if (items.empty()) {
		items.reserve(section->size());
		for (ini_item_vec_cit it = section->begin(), last = section->end();
				it != last; ++it) {
			items.push_back(new ini_item(**it));
		}
	} else {
		// FIXME: do it O(max(M, N)) instead of O(M*log(N))
		for (ini_item_vec_cit it = section->begin(), last = section->end();
				it != last; ++it) {
			const ini_item* item = *it;
			ini_item_vec_it it1 = std::lower_bound(begin(), end(), item, ptr_less<ini_item>());
			if (it1 == end() || xr_stricmp((*it1)->name.c_str(), item->name.c_str()) != 0)
				items.insert(it1, new ini_item(*item));
			else
				(*it1)->value = item->value;
		}
	}
}

static inline bool is_name(int c)
{
	return std::isalnum(c) || std::strchr("@$_-?:.\\", c) != 0;
}

static int skip_blank(const char** pp, const char* end)
{
	for (const char* p = *pp; p != end; ++p) {
		int c = *p;
		if (c == ' ' || c == '\t' || c == '\r') {
			// skip whitespace
			continue;
		}
		if (c == ';' || (c == '/' && (p + 1) != end && *(p + 1) == '/')) {
			// skip comment
			for (++p;; ++p) {
				if (p == end)
					goto eof_reached;
				c = *p;
				if (c == '\n') {
					*pp = p;
					return IF_EOL;
				}
			}
		}
		*pp = p;
		return c == '\n' ? IF_EOL : c;
	}
eof_reached:
	*pp = end;
	return IF_EOF;
}

static int read_name(const char **pp, const char* end, size_t buf_size, char* buf)
{
	xr_assert(buf_size != 0);
	int c = skip_blank(pp, end);
	if (c == IF_EOL) {
		*buf = '\0';
		return c;
	}
	--buf_size;
	for (const char* p = *pp; p != end; ++p) {
		c = *p;
		if (is_name(c)) {
			if (buf_size) {
				*buf++ = char(c);
				--buf_size;
			} else {
				xr_not_expected();
			}
			continue;
		}
		*buf = '\0';
		*pp = p;
		return skip_blank(pp, end);
	}
	*buf = '\0';
	*pp = end;
	return IF_EOF;
}

static int read_item(const char** pp, const char* end, std::string& buf, bool left)
{
	int c = skip_blank(pp, end);
	if (c == IF_EOL) {
		buf.clear();
		return c;
	}
	const char* value = *pp;
	const char* value_end = 0;
	for (const char* p = value;; ++p) {
		if (p == end) {
			if (value_end == 0)
				value_end = end;
			*pp = end;
			break;
		}
		c = *p;
		if (c == ' ' || c =='\t') {
			if (value_end == 0)
				value_end = p;
		} else if (c == '\n' || c == '\r' || c == ';' || (!left && c == '#') || (left && c == '=')) {
			if (value_end == 0)
				value_end = p;
			*pp = p;
			break;
		} else {
			value_end = 0;
		}
	}
	xr_assert(value_end);
	buf.assign(value, value_end);
	return skip_blank(pp, end);
}
/*

static int read_comment(const char **pp, const char* end, size_t buf_size, char* buf)
{
	xr_assert(buf_size != 0);
	int c = skip_blank(pp, end);
	if (c == IF_EOL) {
		*buf = '\0';
		return c;
	}
	--buf_size;
	for (const char* p = *pp; p != end; ++p) {
		c = *p;
		if (c != '\n' && c != '\r') {
			if (buf_size) {
				*buf++ = char(c);
				--buf_size;
			} else {
				xr_not_expected();
			}
			continue;
		}
		*buf = '\0';
		*pp = p;
		return skip_blank(pp, end);
	}
	*buf = '\0';
	*pp = end;
	return IF_EOF;
}
*/

bool xr_ini_file::parse(const char* p, const char* end, const char* path)
{
	std::string folder, fname, extension;
	xr_file_system::split_path(path, &folder, &fname, &extension);
	fname.append(extension);
	const char* file = fname.c_str();

	char temp[256];
	ini_section* section = 0;
	ini_item* item;
	/*ini_comment* comment;*/
	std::string name;
	for (unsigned line = 1;; ++line) {
		int c = skip_blank(&p, end);
		xr_assert(p < end || c == IF_EOF);
		xr_assert(c != '\n' && c != '\r');

/*		// Reading all not line

		if (c == ';') {
			c = read_comment(&p, end, sizeof(temp), temp);

			ini_comment_vec_it it = lower_bound_if(m_comments.begin(), m_comments.end(), ini_comment_pred(temp));

			comment = new ini_comment(temp);
			m_comments.insert(it, comment);
		}*/

		if (c == '[') {
			++p;
			c = read_name(&p, end, sizeof(temp), temp);
			if (c != ']' || temp[0] == 0) {
				msg("bad section header at %s:%u", file, line);
				return false;
			}
			ini_section_vec_it it = lower_bound_if(m_sections.begin(), m_sections.end(), ini_section_pred(temp));
			if (it != m_sections.end() && xr_stricmp((*it)->name.c_str(), temp) == 0) {
				msg("duplicate section %s at %s:%u", temp, file, line);
				return false;
			}
			section = new ini_section(temp);
			m_sections.insert(it, section);
			++p;
			c = skip_blank(&p, end);
			if (c == ':') {
				for (;;) {
					++p;
					c = read_name(&p, end, sizeof(temp), temp);
					it = lower_bound_if(m_sections.begin(), m_sections.end(), ini_section_pred(temp));
					if (it == m_sections.end() || xr_stricmp((*it)->name.c_str(), temp) != 0) {
						msg("bad section reference '%s' at %s:%u", temp, file, line);
						break;
					}
					section->merge(*it);
					if (c != ',')
						break;
				}
			}
		} else if (c != IF_EOL && c != IF_EOF && is_name(c)) {
			if (section == 0) {
				msg("item without section at %s:%u", file, line);
				xr_not_expected();
			}
			c = read_item(&p, end, name, true);
			ini_item_vec_it it = lower_bound_if(section->begin(), section->end(), ini_item_pred(name.c_str()));
			if (it != section->end() && xr_stricmp((*it)->name.c_str(), name.c_str()) == 0) {
				item = *it;
			} else {
				item = new ini_item(name);
				section->items.insert(it, item);
			}
			if (c == '=') {
				++p;
				c = read_item(&p, end, item->value, false);
			} else {
				item->value.clear();
			}
/*
			if (c == ';') {
				c = read_comment(&p, end, sizeof(temp), temp);
				item->comment = temp;
			} else {
				item->comment.clear();
			}*/
		} else if (section == 0 && c == '#') {
			++p;
			c = read_name(&p, end, sizeof(temp), temp);
			if (c != '\"' || std::strcmp(temp, "include") != 0) {
				msg("bad directive %s at %s:%u", temp, file, line);
				return false;
			}
			++p;
			c = read_name(&p, end, sizeof(temp), temp);
			if (c != '\"' || temp[0] == 0) {
				msg("bad include at %s:%u", file, line);
				return false;
			}

			if (!load_include((folder + temp).c_str()))
				return false;

			++p;
			c = skip_blank(&p, end);
		}

		if (c == IF_EOF) {
			break;
		} else if (c != IF_EOL) {
			msg("ignoring trailing garbage at %s:%u", file, line);
			while (p != end) {
				if (*p++ == '\n')
					break;
			}
		} else {
			++p;
		}
	}
	return true;
}

bool xr_ini_file::load_include(const char* path)
{
	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(path);
	if (r == 0) {
		msg("can't include %s", path);
		return false;
	}
	const char* p = r->pointer<const char>();
	bool status = parse(p, p + r->size(), path);
	fs.r_close(r);
	if (status)
		return true;
	msg("can't parse %s", path);
	return false;
}

bool xr_ini_file::load(xr_reader& r)
{
	const char* p = r.pointer<const char>();
	if (parse(p, p + r.size(), "embedded"))
		return true;
	clear();
	return false;
}

bool xr_ini_file::load(const char* path)
{
	if (path)
		m_file_name = (char *)path;

	xr_file_system& fs = xr_file_system::instance();
	xr_reader* r = fs.r_open(path);
	if (r == 0)
		return false;
	const char* p = r->pointer<const char>();
	bool status = parse(p, p + r->size(), path);
	fs.r_close(r);

	if (status)
		return true;
	clear();
	return false;
}

bool xr_ini_file::load(const char* path, const char* name)
{
	xr_file_system& fs = xr_file_system::instance();
	std::string full_path;
	if (!fs.resolve_path(path, name, full_path))
		return false;
	return load(full_path.c_str());
}