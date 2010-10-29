#ifndef __GNUC__
#pragma once
#endif
#ifndef __DM_TOOLS_H__
#define __DM_TOOLS_H__

#include <string>
#include "object_tools.h"

class dm_tools: public object_tools {
public:
	virtual void	process(const cl_parser& cl);
};

#endif
