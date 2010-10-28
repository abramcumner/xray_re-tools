#pragma once
#ifndef __MS3D_DM_IMPORT_H__
#define __MS3D_DM_IMPORT_H__

#include "xr_dm.h"

struct msModel;

class ms3d_dm_import {
public:
			ms3d_dm_import(msModel* model);
	int		import(const xray_re::xr_dm* dm);

private:
	msModel*	m_model;
};

inline ms3d_dm_import::ms3d_dm_import(msModel* model): m_model(model) {}

#endif
