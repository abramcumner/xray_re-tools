#pragma once
#ifndef __MS3D_SKL_EXPORT_H__
#define __MS3D_SKL_EXPORT_H__

struct msModel;

class ms3d_skl_export {
public:
			ms3d_skl_export(msModel* model);
	int		do_export(const char* path);

private:
	msModel*	m_model;
};

inline ms3d_skl_export::ms3d_skl_export(msModel* model): m_model(model) {}

#endif
