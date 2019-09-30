#ifndef _MFnRotateManip
#define _MFnRotateManip
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
// CLASS:    MFnRotateManip
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
// CLASS DECLARATION (MFnRotateManip)

//! \ingroup OpenMayaUI MFn
//! \brief RotateManip function set 
/*!
  This class provides access to the built-in Maya rotate manipulator.
  The manipulator consists of three constrained-axis rotation rings, a
  view rotation ring, as well as an invisible trackball that allows
  the user to rotate in arbitrary directions on the sphere.

  The manipulator provides data to the plugin through the rotation
  manipVal. The rotation value is a vector consisting of x, y, and z
  rotations. Rotations are measured from the initial rotation
  (usually <0,0,0>) of the manipulator.

  The manipulator can be configured either to display with an object
  (which must be a DAG node) or to display at an arbitrary point using
  the rotationCenter manipVal.
*/
class OPENMAYAUI_EXPORT MFnRotateManip : public MFnManip3D
{
	declareDagMFn(MFnRotateManip, MFnManip3D);

public:
	//! Manipulator rotation modes.
	enum RotateMode {
		kObjectSpace = 0,		//!< Object Space (default)
		kWorldSpace,			//!< World Space
		kGimbal					//!< Gimbal
	};

	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &rotationName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToRotationPlug(const MPlug &rotationPlug);
	MStatus		connectToRotationCenterPlug(const MPlug &rotationCenterPlug);
	MStatus		setInitialRotation(const MEulerRotation& rotation);
	MStatus		displayWithNode(const MObject& node);
	MStatus		setRotateMode(RotateMode mode);
	RotateMode	rotateMode() const;
	MStatus		setSnapMode(bool snapEnabled);
	bool		isSnapModeOn() const;
	MStatus		setSnapIncrement(double snapInc);
	double		snapIncrement() const;
	unsigned int	rotationIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	rotationCenterIndex(MStatus *ReturnStatus = NULL) const;
	MStatus		setRotationCenter(const MPoint &rotationCenter);

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor(MFnRotateManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected members
private:
	// No private members
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnRotateManip */
