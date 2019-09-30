#ifndef _MFnCircleSweepManip
#define _MFnCircleSweepManip
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
// CLASS:    MFnCircleSweepManip
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MFnManip3D.h>
#include <maya/MObject.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnCircleSweepManip)

//! \ingroup OpenMayaUI MFn
//! \brief CircleSweepManip function set 
/*!
The CircleSweepManip allows the user to manipulate a point constrained
to move around a circle, in order to specify a sweep angle. This
manipulator generates a single floating point value corresponding to
the sweep angle.
*/
class OPENMAYAUI_EXPORT MFnCircleSweepManip : public MFnManip3D
{
	declareDagMFn(MFnCircleSweepManip, MFnManip3D);

public:
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &angleName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToAnglePlug(MPlug &anglePlug);
	MStatus		setCenterPoint(const MPoint &centerPoint);
	MStatus		setStartPoint(const MPoint &startPoint);
	MStatus		setEndPoint(const MPoint &endPoint);
	MStatus		setNormal(const MVector &normal);
	MStatus		setRadius(double radius);
	MStatus		setAngle(MAngle angle);
	MStatus		setDrawAsArc(bool state);
	MPoint		startPoint(MStatus *ReturnStatus = NULL) const;
	MPoint		endPoint(MStatus *ReturnStatus = NULL) const;
	unsigned int	centerIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	axisIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	angleIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	startCircleIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	endCircleIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnCircleSweepManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:

private:
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnCircleSweepManip */
