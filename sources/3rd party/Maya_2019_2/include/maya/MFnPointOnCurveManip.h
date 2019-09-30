#ifndef _MFnPointOnCurveManip
#define _MFnPointOnCurveManip
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
// CLASS:    MFnPointOnCurveManip
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
// CLASS DECLARATION (MFnPointOnCurveManip)

//! \ingroup OpenMayaUI MFn
//! \brief PointOnCurveManip function set 
/*!
The PointOnCurveManip allows the user to manipulate a point
constrained to move along a curve, in order to specify the "u" curve
parameter value. This manipulator generates a single floating point
value corresponding to the curve parameter.
*/
class OPENMAYAUI_EXPORT MFnPointOnCurveManip : public MFnManip3D
{
	declareDagMFn(MFnPointOnCurveManip, MFnManip3D);

public:
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &paramName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToCurvePlug(MPlug &curvePlug);
	MStatus		connectToParamPlug(MPlug &paramPlug);
	MStatus		setDrawCurve(bool state);
	MStatus		setParameter(double parameter);
	bool		isDrawCurveOn(MStatus *ReturnStatus = NULL) const;
	double		parameter(MStatus *ReturnStatus = NULL) const;
	MPoint		curvePoint(MStatus *ReturnStatus = NULL) const;
	unsigned int	curveIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	paramIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnPointOnCurveManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:

private:

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnPointOnCurveManip */
