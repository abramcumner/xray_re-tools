#ifndef _MFnPointOnSurfaceManip
#define _MFnPointOnSurfaceManip
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
// CLASS:    MFnPointOnSurfaceManip
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
// CLASS DECLARATION (MFnPointOnSurfaceManip)

//! \ingroup OpenMayaUI MFn
//! \brief PointOnSurfaceManip function set 
/*!
The PointOnSurfaceManip allows the user to manipulate a point
constrained to move along a surface, in order to specify the (u, v)
surface parameter values. This manipulator generates two floating
point values corresponding to the surface (u, v) parameters.
*/
class OPENMAYAUI_EXPORT MFnPointOnSurfaceManip : public MFnManip3D
{
	declareDagMFn(MFnPointOnSurfaceManip, MFnManip3D);

public:
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &paramName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToSurfacePlug(MPlug &surfacePlug);
	MStatus		connectToParamPlug(MPlug &paramPlug);
	MStatus		setDrawSurface(bool state);
	MStatus		setDrawArrows(bool state);
	MStatus		setParameters(double u, double v);
	MStatus		getParameters(double &u, double &v);
	bool		isDrawSurfaceOn(MStatus *ReturnStatus = NULL) const;
	unsigned int	surfaceIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	paramIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnPointOnSurfaceManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:

private:

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnPointOnSurfaceManip */
