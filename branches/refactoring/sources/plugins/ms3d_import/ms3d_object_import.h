#pragma once
#ifndef __MS3D_OBJECT_IMPORT_H__
#define __MS3D_OBJECT_IMPORT_H__

#include "xr_object.h"

struct msModel;

class ms3d_object_import {
public:
			ms3d_object_import(msModel* model);
	int		import(const xray_re::xr_object* object);

private:
	int		import(const xray_re::xr_mesh* mesh, const xray_re::xr_bone_vec& bones);
	int		import(const xray_re::xr_surface* surface);

private:
	msModel*	m_model;
};

inline ms3d_object_import::ms3d_object_import(msModel* model): m_model(model) {}

#endif
