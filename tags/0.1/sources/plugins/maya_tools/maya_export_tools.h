#pragma once
#ifndef __MAYA_EXPORT_TOOLS_H__
#define __MAYA_EXPORT_TOOLS_H__

#include <string>
#include <vector>
#ifdef _MSC_VER
#include <hash_map>
#else
#include <map>
#endif
#include <maya/MStatus.h>

class MObject;
class MObjectArray;
class MFnMesh;
class MFnSet;

namespace xray_re {
	class xr_object;
	class xr_surface;
	struct xr_surfmap;
};

#ifdef _MSC_VER
typedef stdext::hash_map<std::string, xray_re::xr_surface*> xr_surface_map;
#else
typedef std::map<std::string, xray_re::xr_surface*> xr_surface_map;
#endif
typedef xr_surface_map::iterator xr_surface_map_it;

class maya_export_tools {
public:
	MStatus			export_object(const char* path, bool selection_only = false);
	MStatus			export_skl_object(const char* path, bool selection_only = false);
	MStatus			export_skl(const char* path, bool selection_only = false);

private:
	MStatus			extract_surfaces(MFnMesh& mesh_fn, std::vector<xray_re::xr_surfmap*>& surfmaps);
	xray_re::xr_surface*	create_surface(const char* surf_name, MFnSet& set_fn);
	xray_re::xr_object*	create_object(MObjectArray& mesh_objs);
	xray_re::xr_object*	create_skl_object(MObject& mesh_obj, MObject& skin_obj);

	void			commit_surfaces(std::vector<xray_re::xr_surface*>& surfaces);

private:
	xr_surface_map		m_shared_surfaces;
	bool			m_skeletal;
};

#endif
