#pragma once
#ifndef __MS3D_SKL_IMPORT_H__
#define __MS3D_SKL_IMPORT_H__

#include "xr_skl_motion.h"

struct msModel;

class ms3d_skl_import {
public:
			ms3d_skl_import(msModel* model);
	int		import(const xray_re::xr_skl_motion* smotion);

private:
	msModel*	m_model;
};

inline ms3d_skl_import::ms3d_skl_import(msModel* model): m_model(model) {}

#endif
