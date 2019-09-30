#ifndef _MGeometryManager
#define _MGeometryManager
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MGeometryManager
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MTypes.h>
#include <maya/MGeometry.h>
#include <maya/MGeometryList.h>
#include <maya/MGeometryRequirements.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MGeometryManager)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
  \deprecated
  Use MHWRender::MGeometry and MHWRender::MGeometryExtractor instead.

  This class provides methods for managing MGeometry resources.

  It provides an interface for loading and using hardware
  textures.
*/
class OPENMAYARENDER_EXPORT MGeometryManager
{
public:

	//! Default geometry shapes
	enum GeometricShape {
		//! Sphere with radius 1, centered at 0,0,0.
		kDefaultSphere = 0,
		//! Plane with width and height of 1, centered at 0,0,0.
		//! Assuming "Y-Up" orientation: width = x-axis, and height = y-axis.
		kDefaultPlane,
		//! Cube with width, height and depth of 1, centered at 0,0,0.
		kDefaultCube
	};

	static MGeometryLegacy getGeometry( const MDagPath& shape, 
								  const MGeometryRequirementsLegacy& requirements,
								  MObject* components = NULL);
	static MGeometryList* referenceDefaultGeometry(const MGeometryManager::GeometricShape geomShape,
												   const MGeometryRequirementsLegacy& requirements);
	static MStatus	dereferenceDefaultGeometry(MGeometryList* data);

	static const char* className();

private:
	~MGeometryManager();
#ifdef __GNUC__
	friend class shutUpAboutPrivateDestructors;
#endif

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MGeometryManager */
