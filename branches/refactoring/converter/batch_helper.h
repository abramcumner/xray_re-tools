#ifndef __GNUC__
#pragma once
#endif
#ifndef __BATCH_HELPER_H__
#define __BATCH_HELPER_H__

class batch_helper {
public:
	virtual		~batch_helper();

	bool		prepare_target_name(const cl_parser& cl);
	void		make_target_name(std::string& target, const char* source,
					const char* extension) const;

	std::string	m_output_folder;
	std::string	m_output_file;
};

#endif
