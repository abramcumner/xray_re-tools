#ifndef _MFnCurveSegmentManip
#define _MFnCurveSegmentManip
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
// CLASS:    MFnCurveSegmentManip
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
// CLASS DECLARATION (MFnCurveSegmentManip)

//! \ingroup OpenMayaUI MFn
//! \brief CurveSegmentManip function set
/*!
The CurveSegmentManip allows the user to manipulate two points on a
curve, in order to specify a curve segment. This manipulator generates
two floating point values, which correspond to the parameters of the
start and end of the curve segment.
*/
class OPENMAYAUI_EXPORT MFnCurveSegmentManip : public MFnManip3D
{
	declareDagMFn(MFnCurveSegmentManip, MFnManip3D);

public:
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &startParamName,
					   const MString &endParamName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToCurvePlug(MPlug &curvePlug);
	MStatus		connectToStartParamPlug(MPlug &startParamPlug);
	MStatus		connectToEndParamPlug(MPlug &endParamPlug);
	MStatus		setStartParameter(double startParameter);
	MStatus		setEndParameter(double endParameter);
	double		startParameter(MStatus *ReturnStatus = NULL) const;
	double		endParameter(MStatus *ReturnStatus = NULL) const;
	unsigned int	curveIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	startParamIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	endParamIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnCurveSegmentManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:

private:

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnCurveSegmentManip */
