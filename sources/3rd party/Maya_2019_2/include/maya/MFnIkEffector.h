#ifndef _MFnIkEffector
#define _MFnIkEffector
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
// CLASS:    MFnIkEffector
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnTransform.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnIkEffector)

//! \ingroup OpenMayaAnim MFn
//! \brief Inverse kinematics end effector function set 
/*!
MFnIkEffector is the function set for inverse kinematic end effectors.  An end
effector is a point on the skeleton, usually the last bone.  When an IK system
solves, it is trying to calculate the rotations on the joints necessary to get the
end effector to the target point.

The methods of the parent class MFnTransform used to position the end effector.
*/
class OPENMAYAANIM_EXPORT MFnIkEffector : public MFnTransform
{
	declareDagMFn( MFnIkEffector, MFn::kEffector );

public:
	MObject create( MObject parent = MObject::kNullObj,
					MStatus * ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnIkEffector, MFn::kEffector );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnIkEffector */
