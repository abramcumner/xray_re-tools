#ifndef _MFnFreePointTriadManip
#define _MFnFreePointTriadManip
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
// CLASS:    MFnFreePointTriadManip
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MFnManip3D.h>
#include <maya/MObject.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnFreePointTriadManip)

//! \ingroup OpenMayaUI MFn
//! \brief FreePointTriadManip function set
/*!
  The FreePointTriadManip provides a moveable point, which can be
  moved anywhere, and has axes for constrained x, y, and z movement
  and obeys grid snapping, point snapping, and curve snapping. The
  FreePointTriadManip generates the 3D position of the moveable
  point. It is useful for specifying the position of an object in
  space.

  Note that only the MFnNumericData::k3Double data type is supported when 
  connecting to a pointPlug via connectToPointPlug.
*/
class OPENMAYAUI_EXPORT MFnFreePointTriadManip : public MFnManip3D
{
	declareDagMFn(MFnFreePointTriadManip, MFnManip3D);

public:
	//! Interaction plane for the manip center
	enum ManipPlane {
		kYZPlane = 0,						//!< Y-Z Plane
		kXZPlane,							//!< X-Z Plane
		kXYPlane,							//!< X-Y Plane
		kViewPlane							//!< View Plane
	};

	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &pointName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToPointPlug(MPlug &pointPlug);
	MStatus		setDrawAxes(bool state);
	MStatus		setSnapMode(bool state);
	MStatus		setKeyframeAll(bool state);
	MStatus		setDrawArrowHead(bool state);
	MStatus		setGlobalTriadPlane(ManipPlane whichPlane);
	MStatus		setPoint(const MPoint& pointValue);
	MStatus		setDirection(const MVector& direction);
	bool		isDrawAxesOn(MStatus *ReturnStatus = NULL) const;
	bool		isSnapModeOn(MStatus *ReturnStatus = NULL) const;
	bool		isKeyframeAllOn(MStatus *ReturnStatus = NULL) const;
	unsigned int	pointIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnFreePointTriadManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:

private:

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnFreePointTriadManip */
