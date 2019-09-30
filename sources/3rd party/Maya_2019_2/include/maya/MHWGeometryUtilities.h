#ifndef _MHWGeometryUtilities
#define _MHWGeometryUtilities
//
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
// ****************************************************************************

#if defined __cplusplus


// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>


namespace MHWRender
{

//! This enum defines the possible drawing modes for individual objects.
enum DisplayStatus {
    kActive,				//!< Object is active (selected). Note that the last selected object will have status kLead instead of kActive. If only one object is selected the status will be kLead.
    kLive,					//!< Object is live (construction surface).
    kDormant,				//!< Object is domant.
    kInvisible,				//!< Object is invisible (not drawn).
    kHilite,				//!< Object is hilited (has selectable components).
    kTemplate,				//!< Object is templated (Not renderable).
    kActiveTemplate,		//!< Object is active and templated.
    kActiveComponent,		//!< Object has active components.
	kLead,					//!< Last selected object.
	kIntermediateObject,	//!< Construction object (not drawn).
	kActiveAffected,		//!< Affected by active object(s).
    kNoStatus				//!< Object does not have a valid display status.
};
} // namespace MHWRender

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// CLASS DECLARATION (MGeometryUtilities)
//! \ingroup OpenMayaRender
//! \brief Utilities for Viewport 2.0
/*!
This class is a utility class for rendering geometry in Viewport 2.0
*/
class OPENMAYARENDER_EXPORT MGeometryUtilities
{
public:

	static MColor wireframeColor(const MDagPath& path);

    static ::MHWRender::DisplayStatus displayStatus(const MDagPath& path,
                                       MStatus* returnStatus = NULL);

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

	static MGeometry* acquireReferenceGeometry(GeometricShape shape, const MGeometryRequirements& requirements);

	static void releaseReferenceGeometry(MGeometry* geometry);

	static const char* className();

private:
    ~MGeometryUtilities();
#ifdef __GNUC__
	friend class shutUpAboutPrivateDestructors;
#endif
};

} // namespace MHWRender

// ****************************************************************************

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MHWGeometryUtilities */

