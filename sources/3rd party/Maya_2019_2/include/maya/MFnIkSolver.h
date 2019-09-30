#ifndef _MFnIkSolver
#define _MFnIkSolver
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
// CLASS:    MFnIkSolver
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MFnTransform.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS

// ****************************************************************************
// CLASS DECLARATION (MFnIkSolver)

//! \ingroup OpenMayaAnim MFn
//! \brief Function set for inverse kinematics (IK) solvers 
/*!
	This is the function set for inverse kinematics (IK) solvers.
	This function set is used for setting and querying attached
	ik solvers.
*/
class OPENMAYAANIM_EXPORT MFnIkSolver : public MFnDependencyNode
{
	declareMFn( MFnIkSolver, MFn::kIkSolver );

public:
	unsigned int maxIterations( MStatus * ReturnStatus = NULL );
	MStatus setMaxIterations( unsigned int maxIters );
	double tolerance( MStatus * ReturnStatus = NULL );
	MStatus setTolerance( double tolerance );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnIkSolver, MFn::kIkSolver );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnIkSolver */
