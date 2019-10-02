#ifndef __GNUC__
#pragma once
#endif
#ifndef __XRDEMO_TOOLS_H__
#define __XRDEMO_TOOLS_H__

#include "tools_base.h"
#include "batch_helper.h"

class xrdemo_tools: public tools_base, public batch_helper {
public:
#ifdef _CONSOLE
	virtual void process(const cl_parser& cl);
#else
	void process(const char* source, const char* target_path);
#endif
};

#endif
