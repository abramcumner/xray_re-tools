#include "xr_cl_parser.h"
#include "xr_file_system.h"
#include "batch_helper.h"

using namespace xray_re;

batch_helper::~batch_helper() {}

bool batch_helper::prepare_target_name(const cl_parser& cl)
{
	if (cl.get_string("-dir", m_output_folder)) {
		if (!xr_file_system::folder_exist(m_output_folder)) {
			msg("output folder does not exist");
			return false;
		}
		xr_file_system::append_path_separator(m_output_folder);
	} else if (cl.get_string("-out", m_output_file)) {
		if (cl.num_params() > 1) {
			msg("explicitly specified output file expects single input file");
			return false;
		}
	}
	return true;
}

void batch_helper::make_target_name(std::string& target, const char* source, const char* extension) const
{
	if (m_output_file.empty()) {
		std::string name;
		xr_file_system::split_path(source, 0, &name);
		target = m_output_folder;
		target += name;
		target += extension;
	} else {
		target = m_output_file;
	}
}
