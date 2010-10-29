#ifndef __GNUC__
#pragma once
#endif
#ifndef __OGG_TOOLS_H__
#define __OGG_TOOLS_H__

#include "tools_base.h"

class ogg_tools: public tools_base {
public:
	virtual void	process(const cl_parser& cl);
};

#endif
