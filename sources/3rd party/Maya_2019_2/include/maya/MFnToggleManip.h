#ifndef _MFnToggleManip
#define _MFnToggleManip
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
// CLASS:    MFnToggleManip
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
// CLASS DECLARATION (MFnToggleManip)

//! \ingroup OpenMayaUI MFn
//! \brief ToggleManip function set 
/*!

  The ToggleManip allows the user to switch between two modes or some
  on/off state. It is drawn as a circle with or without a dot. When
  the mode is on, the dot is drawn in the circle; when the mode is
  off, the circle is drawn without the dot. This manipulator generates
  a boolean value corresponding to whether or not the mode is on or
  off.
*/
class OPENMAYAUI_EXPORT MFnToggleManip : public MFnManip3D
{
	declareDagMFn(MFnToggleManip, MFnManip3D);

public:
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &toggleName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToTogglePlug(MPlug &togglePlug);
	MPoint		startPoint(MStatus *ReturnStatus = NULL) const;
	MVector		direction(MStatus *ReturnStatus = NULL) const;
	double		length(MStatus *ReturnStatus = NULL) const;
	bool		toggle(MStatus *ReturnStatus = NULL) const;
	unsigned int	startPointIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	directionIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	lengthIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	toggleIndex(MStatus *ReturnStatus = NULL) const;
	MStatus		setStartPoint(MPoint &startPoint);
	MStatus		setDirection(MVector &direction);
	MStatus		setLength(double length);
	MStatus		setToggle(bool toggle);

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnToggleManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:

private:

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnToggleManip */
