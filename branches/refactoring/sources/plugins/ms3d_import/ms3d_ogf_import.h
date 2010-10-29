#pragma once
#ifndef __MS3D_OGF_IMPORT_H__
#define __MS3D_OGF_IMPORT_H__

#include "xr_ogf.h"

struct msModel;

class ms3d_ogf_import {
public:
			ms3d_ogf_import(msModel* model);
	int		import(const xray_re::xr_ogf* ogf);

private:
	int		import(const xray_re::xr_ogf* ogf, const int* bone_refs);
	int		import(const xray_re::xr_bone* xbone, const xray_re::xr_bone_vec& xbones, int* bone_refs);

private:
	msModel*	m_model;
};

inline ms3d_ogf_import::ms3d_ogf_import(msModel* model): m_model(model) {}

#endif
