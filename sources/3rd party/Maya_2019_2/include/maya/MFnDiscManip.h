#ifndef _MFnDiscManip
#define _MFnDiscManip
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
// CLASS:    MFnDiscManip
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
// CLASS DECLARATION (MFnDiscManip)

//! \ingroup OpenMayaUI MFn
//! \brief DiscManip function set 
/*!
The DiscManip allows the user to rotate a disc in order to specify a
rotation about an axis. This manipulator generates a single floating
point value corresponding to the rotation.
*/
class OPENMAYAUI_EXPORT MFnDiscManip : public MFnManip3D
{
	declareDagMFn(MFnDiscManip, MFnManip3D);

public:
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &angleName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToAnglePlug(MPlug &anglePlug);
	MStatus		setCenterPoint(const MPoint &centerPoint);
	MStatus		setNormal(const MVector &normal);
	MStatus		setRadius(double radius);
	MStatus		setAngle(MAngle angle);
	unsigned int	centerIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	axisIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	angleIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnDiscManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:

private:

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnDiscManip */
