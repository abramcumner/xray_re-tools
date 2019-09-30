#ifndef _MIkHandleGroup
#define _MIkHandleGroup
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
// CLASS:    MIkHandleGroup
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MIkHandleGroup)

//! \ingroup OpenMayaAnim
//! \brief IK handle groups 
/*!
Group class for ik handles.  Each group has an associated solver and priority.
A single chain solver handle group has only one handle in its group.
*/
class OPENMAYAANIM_EXPORT MIkHandleGroup
{
public:
					MIkHandleGroup();
					~MIkHandleGroup();

	int			priority(MStatus *ReturnStatus = NULL) const;
	int			solverID(MStatus *ReturnStatus = NULL) const;
	int			solverPriority(MStatus *ReturnStatus = NULL) const;
	MStatus			setPriority( int );
	MStatus			setSolverID( int );
	bool			checkEffectorAtGoal(MStatus *ReturnStatus = NULL);
	MStatus			solve();


	int 			dofCount(MStatus *ReturnStatus = NULL) const;
	int				handleCount(MStatus *ReturnStatus = NULL) const;
	MObject			handle( int ith, MStatus *ReturnStatus = NULL );

	static const char* className();


protected:
// No protected members

private:
	friend class MPxIkSolver;
	friend class MPxIkSolverNode;
	MIkHandleGroup( void * );
 	void * f_data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MIkHandleGroup */
