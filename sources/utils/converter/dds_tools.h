#ifndef __GNUC__
#pragma once
#endif
#ifndef __DDS_TOOLS_H__
#define __DDS_TOOLS_H__

#include <string>
#include "tools_base.h"

class dds_tools: public tools_base {
public:
	dds_tools();
	virtual void	process(const cl_parser& cl);

protected:
	bool		check_paths() const;
	void		process_folder(const std::string& path = "");
	void		process_file(const std::string& path);

protected:
	bool		m_with_solid;
	bool		m_with_bump;
	std::string	m_textures;
};

#endif
