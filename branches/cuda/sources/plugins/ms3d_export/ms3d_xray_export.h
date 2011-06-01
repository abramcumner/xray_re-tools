#pragma once
#ifndef __MS3D_XRAY_EXPORT_H__
#define __MS3D_XRAY_EXPORT_H__

#include <msPlugIn.h>

class ms3d_xray_export: public cMsPlugIn {
public:
	virtual			~ms3d_xray_export();
	virtual int		GetType();
	virtual const char*	GetTitle();
	virtual int		Execute(msModel* model);
};

#endif
