#ifndef _MFnDirectionManip
#define _MFnDirectionManip
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
// CLASS:    MFnDirectionManip
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
// CLASS DECLARATION (MFnDirectionManip)

//! \ingroup OpenMayaUI MFn
//! \brief DirectionManip function set 
/*!
The DirectionManip allows the user to specify a direction, as defined
by the vector from the start point to the manipulator position. It
uses a FreePointTriadManip to specify the end point of a vector
relative to a given start point. This manipulator generates a vector
from the start point to the end point.
*/
class OPENMAYAUI_EXPORT MFnDirectionManip : public MFnManip3D
{
	declareDagMFn(MFnDirectionManip, MFnManip3D);

public:
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &directionName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToDirectionPlug(MPlug &directionPlug);
	MStatus		setNormalizeDirection(bool state);
	MStatus		setDrawStart(bool state);
	MStatus		setStartPoint(MPoint &startPoint);
	MStatus		setDirection(MVector &direction);
	unsigned int	startPointIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	endPointIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	directionIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnDirectionManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:

private:

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnDirectionManip */
