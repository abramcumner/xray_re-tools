#include "object_tools.h"
#include "xr_object.h"
#include "xr_object_format.h"
#include "xr_file_system.h"
#include <regex>

using namespace xray_re;

#if 0
void object_tools::process(const cl_parser& cl)
{
	xr_object* object = new xr_object();
	if (object->load_object(source))
		;//object->save_object("GGGGG");
	else
		msg("can't load %s", source.c_str());
	delete object;
}
#endif

void object_tools::save_object(xray_re::xr_object& object, const char* source) const
{
	std::string target;
	make_target_name(target, source, ".object");
	object.to_object();
	if (!object.save_object(target.c_str()))
		msg("can't save object in %s", target.c_str());
}

void object_tools::save_bones(xray_re::xr_object& object, const char* source) const
{
	if (object.bones().empty()) {
		msg("game object has no bones");
	} else {
		std::string target;
		make_target_name(target, source, ".bones");
		if (!object.save_bones(target.c_str()))
			msg("can't save bones in %s", target.c_str());
	}
}

void object_tools::save_skls(xray_re::xr_object& object, const char* source) const
{
	if (object.motions().empty()) {
		msg("game object has no own motions");
	} else {
		std::string target;
		make_target_name(target, source, ".skls");
		if (!object.save_skls(target.c_str()))
			msg("can't save motions in %s", target.c_str());
	}
}

void replace_all(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void escape_regex(std::string& regex)
{
	replace_all(regex, "\\", "\\\\");
	replace_all(regex, "^", "\\^");
	replace_all(regex, ".", "\\.");
	replace_all(regex, "$", "\\$");
	replace_all(regex, "|", "\\|");
	replace_all(regex, "(", "\\(");
	replace_all(regex, ")", "\\)");
	replace_all(regex, "[", "\\[");
	replace_all(regex, "]", "\\]");
	replace_all(regex, "*", "\\*");
	replace_all(regex, "+", "\\+");
	replace_all(regex, "?", "\\?");
	replace_all(regex, "/", "\\/");
}

std::function<bool(const std::string&)> create_filter(const std::string& motion_name)
{
	// filter all
	if (motion_name == "all")
		return [](const std::string&) { return true; };

	// filter with regex
	if (motion_name.size() >= 2 && motion_name.front() == '/' && motion_name.back() == '/') {
		std::regex re(motion_name.substr(1, motion_name.size() - 2));
		return [re](const std::string& name) {return std::regex_match(name, re); };
	}

	// filter with wildcards
	if (motion_name.find('*') != std::string::npos || motion_name.find('?') != std::string::npos) {
		std::string re_str(motion_name);
		escape_regex(re_str);
		replace_all(re_str, "\\?", ".");
		replace_all(re_str, "\\*", ".*");
		std::regex re(re_str);
		return [re](const std::string& name) {return std::regex_match(name, re); };
	}

	// filter by name
	return [motion_name](const std::string& name) {return name == motion_name; };
}

void object_tools::save_skl(xray_re::xr_object& object, const char* source, const cl_parser& cl) const
{
	if (m_output_file.empty() && !m_output_folder.empty())
		xr_file_system::instance().create_path(m_output_folder);

	std::string motion_name;
	cl.get_string("-skl", motion_name);

	auto filter = create_filter(motion_name);

	bool found = false;
	for (auto el : object.motions()) {
		if (!filter(el->name()))
			continue;

		std::string name{ el->name() };
		std::string target = m_output_file.empty() ? m_output_folder + name + ".skl" : m_output_file;
		if (!el->save_skl(target.c_str()))
			msg("can't save %s", target.c_str());

		found = true;
	}
	if (!found)
		msg("can't find motion %s", motion_name.c_str());
}

object_tools::target_format object_tools::get_target_format(const cl_parser& cl) const
{
	unsigned format = 0;
	if (cl.exist("-info"))
		format |= TARGET_INFO;
	if (cl.exist("-skl"))
		format |= TARGET_SKL;
	if (cl.exist("-skls"))
		format |= TARGET_SKLS;
	if (cl.exist("-bones"))
		format |= TARGET_BONES;
	if (cl.exist("-object"))
		format |= TARGET_OBJECT;
	return (format & (format - 1)) == 0 ? static_cast<target_format>(format) : TARGET_ERROR;
}
