#ifndef _MFnIkHandle
#define _MFnIkHandle
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
// CLASS:    MFnIkHandle
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnTransform.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnIkHandle)

//! \ingroup OpenMayaAnim MFn
//! \brief Function set for inverse kinematics (IK) handles 
/*!
	This is the function set for inverse kinematics (IK) handles.
	An ik handle specifies the joints in a skeleton that are effected
	by an attached ik solver.
*/
class OPENMAYAANIM_EXPORT MFnIkHandle : public MFnTransform
{
	declareDagMFn( MFnIkHandle, MFn::kHandle );

public:
	//! Specifies a handle's stickiness when the skeleton is being manipulated interactively.
	enum Stickiness {
		kStickyOff,	//!< Handle will move with skeleton's root.
		kStickyOn,	//!< Handle will try to stay where it is.
		kSuperSticky	//!< Not used.
    };
	MObject   create( MDagPath& startJoint,
	                  MDagPath& effector,
					  MStatus * ReturnStatus = NULL );
	MStatus   getStartJoint( MDagPath &jointPath );
	MStatus   setStartJoint( const MDagPath &jointPath );
	MStatus   getEffector( MDagPath &effectorPath );
	MStatus   setEffector( const MDagPath &effectorPath );
	MStatus   setStartJointAndEffector( const MDagPath &jointPath,
										const MDagPath &effectorPath );
	unsigned int  priority( MStatus * ReturnStatus = NULL );
	MStatus   setPriority( unsigned int priority );
	Stickiness stickiness( MStatus * ReturnStatus = NULL );
	MStatus   setStickiness( Stickiness stickiness );
	double    weight( MStatus * ReturnStatus = NULL );
	MStatus   setWeight( double weight );
	double    poWeight( MStatus * ReturnStatus = NULL );
	MStatus   setPOWeight( double weight );
	MObject   solver( MStatus * ReturnStatus = NULL );
	MStatus   setSolver( const MObject &solver );
	MStatus   setSolver( const MString & solverName );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnIkHandle, MFn::kHandle );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnIkHandle */
