#pragma once
#ifndef __MS3D_OBJECT_EXPORT_H__
#define __MS3D_OBJECT_EXPORT_H__

struct msModel;

class ms3d_object_export {
public:
			ms3d_object_export(msModel* model);
	int		do_export(const char* path);

private:
	msModel*	m_model;
};

inline ms3d_object_export::ms3d_object_export(msModel* model): m_model(model) {}

#endif
