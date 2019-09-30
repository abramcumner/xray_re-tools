#ifndef _MFnDistanceManip
#define _MFnDistanceManip
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
// CLASS:    MFnDistanceManip
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
// CLASS DECLARATION (MFnDistanceManip)

//! \ingroup OpenMayaUI MFn
//! \brief DistanceManip function set
/*!
The DistanceManip allows the user to manipulate a point that is
constrained to move along a line. This manipulator generates a single
floating point value. Scaling factors can be used to determine how
int the manipulator appears when it is drawn.
*/
class OPENMAYAUI_EXPORT MFnDistanceManip : public MFnManip3D
{
	declareDagMFn(MFnDistanceManip, MFnManip3D);

public:
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &distanceName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToDistancePlug(MPlug &distancePlug);
	MStatus		setStartPoint(const MPoint &point);
	MStatus		setDirection(const MVector &vector);
	MStatus		setDrawStart(bool state);
	MStatus		setDrawLine(bool state);
	MStatus		setScalingFactor(double scalingFactor);
	bool		isDrawStartOn(MStatus *ReturnStatus = NULL) const;
	bool		isDrawLineOn(MStatus *ReturnStatus = NULL) const;
	double		scalingFactor(MStatus *ReturnStatus = NULL) const;
	unsigned int	distanceIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	directionIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	startPointIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	currentPointIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnDistanceManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnDistanceManip */
