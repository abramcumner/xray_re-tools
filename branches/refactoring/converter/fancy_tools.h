#ifndef __GNUC__
#pragma once
#endif
#ifndef __FANCY_TOOLS_H__
#define __FANCY_TOOLS_H__

#include <string>
#include "tools_base.h"

namespace xray_re {
	class xr_ini_file;
};

class fancy_tools: public tools_base {
public:
			fancy_tools();
	virtual		~fancy_tools();
	virtual void	process(const cl_parser& cl);

protected:
	bool		check_paths() const;
	void		fix_details(const char* section) const;
	void		fix_fences(const char* section) const;

private:
	xray_re::xr_ini_file*	m_ini;
};

inline fancy_tools::fancy_tools(): m_ini(0) {}

#endif
