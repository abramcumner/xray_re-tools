#ifndef __GNUC__
#pragma once
#endif
#ifndef __OGF_TOOLS_H__
#define __OGF_TOOLS_H__

#include "object_tools.h"

class ogf_tools: public object_tools {
public:
	virtual void	process(const cl_parser& cl);
};

class omf_tools: public ogf_tools {
public:
	virtual void	process(const cl_parser& cl);
};

#endif
