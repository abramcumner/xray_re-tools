#ifndef __GNUC__
#pragma once
#endif
#ifndef __OGF_TOOLS_H__
#define __OGF_TOOLS_H__

#include "object_tools.h"
#include "xr_ogf.h"

class ogf_tools: public object_tools {
public:
#ifdef _CONSOLE
	virtual void process(const cl_parser& cl);
#else
	void process(const char* source, const char* target, const char* format, const char* motion_name);
#endif

protected:
#ifndef _CONSOLE
	virtual xray_re::xr_ogf* load(const char* source);
	void save_skl(xray_re::xr_object& object, const char* source, const char* motion_name) const;
#else
	void save_skl(xray_re::xr_object& object, const char* source, const cl_parser& cl) const;
#endif
	void save_skls(xray_re::xr_object& object, const char* source) const;
	void save_bones(xray_re::xr_object& object, const char* source) const;
};

class omf_tools: public ogf_tools {
#ifdef _CONSOLE
public:
	virtual void process(const cl_parser& cl);
#else
protected:
	xray_re::xr_ogf* load(const char* source);
#endif
};

#endif