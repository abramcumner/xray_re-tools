#ifndef _MFnManip3D
#define _MFnManip3D
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
// CLASS:    MFnManip3D
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnTransform.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnManip3D)

//! \ingroup OpenMayaUI MFn
//! \brief 3D manipulator function set 
/*!

  MFnManip3D allows the creation and manipulation of 3D manipulators.
  MFnManip3D is the base class from which MFnFreePointTriadManip,
  MFnDirectionManip, MFnDistanceManip, MFnPointOnCurveManip,
  MFnPointOnSurfaceManip, MFnDiscManip, MFnCircleSweepManip,
  MFnToggleManip, MFnStateManip, and MFnCurveSegmentManip are derived.
*/
class OPENMAYAUI_EXPORT MFnManip3D : public MFnTransform
{
	declareDagMFn(MFnManip3D, MFnTransform);
public:
	bool 			isVisible(MStatus *ReturnStatus) const;
	MStatus			setVisible(bool isVisible);
	float			manipScale(MStatus *ReturnStatus) const;
	MStatus			setManipScale(float size);
	bool			isOptimizePlaybackOn(MStatus *ReturnStatus) const;
	MStatus			setOptimizePlayback(bool optimizePlayback);
	static float	globalSize();
	static void		setGlobalSize(float size);
	static float	handleSize();
	static void		setHandleSize(float size);
	static float	lineSize();
	static void		setLineSize(float size);
	static MStatus	deleteManipulator(MObject& manip);
	static bool		drawPlaneHandles();
	static void		setDrawPlaneHandles(bool drawPlaneHandles);
	MEulerRotation  rotateXYZValue(const unsigned int valIndex,MStatus *ReturnStatus = NULL);

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnManip3D, MFnTransform );

END_NO_SCRIPT_SUPPORT:

protected:

private:
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnManip3D */
