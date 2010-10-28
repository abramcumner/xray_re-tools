#pragma once
#ifndef __MS3D_XRAY_IMPORT_H__
#define __MS3D_XRAY_IMPORT_H__

#include <msPlugIn.h>

class ms3d_xray_import: public cMsPlugIn {
public:
	virtual			~ms3d_xray_import();
	virtual int		GetType();
	virtual const char*	GetTitle();
	virtual int		Execute(msModel* model);
};

#endif
