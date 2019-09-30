#ifndef _MIkSystem
#define _MIkSystem
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
// CLASS:    MIkSystem
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStringArray.h>
#include <maya/MStatus.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MIkSystem)

//! \ingroup OpenMayaAnim
//! Inverse kinematics (IK) system class
/*!
	This class provides an interface to the inverse kinematics (IK) system.
	The ik system is used to set/query the global snapping flag for handles,
	set/query the global solve flag for solvers, and to find the ik solvers
	available in maya.
*/
class OPENMAYAANIM_EXPORT MIkSystem
{
public:
	static MObject	findSolver( MString name, MStatus * ReturnStatus = NULL );
	static MStatus	getSolvers( MStringArray & names );
	static bool		isGlobalSnap( MStatus * ReturnStatus = NULL );
	static MStatus	setGlobalSnap( bool isSnap );
	static bool		isGlobalSolve( MStatus * ReturnStatus = NULL );
	static MStatus	setGlobalSolve( bool isSnap );

	static const char* className();

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MIkSystem */
