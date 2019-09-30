#ifndef _MPxIkSolver
#define _MPxIkSolver
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
// CLASS:    MPxIkSolver
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxIkSolver)

//! \ingroup OpenMayaAnim MPx
//! \brief OBSOLETE CLASS: Base class for user defined IK solvers  
/*!
\deprecated
This class is obsolete and will be removed in a future version of Maya.
It has been replaced with the MPxIkSolverNode class.

This is the obsolete base class for writing user-defined IK solvers.
Users must at least override the following methods in order to write a solver:

	\li <b>doSolve</b>
	\li <b>solverTypeName</b>
	\li <b>isSingleChainOnly</b>
	\li <b>isPositionOnly</b>
	\li <b>hasJointLimitSupport</b>
	\li <b>hasUniqueSolution</b>
	\li <b>groupHandlesByTopology</b>

Users can optionally override the following methods if they want to perform work
before or after <b>doSolve</b> is called:

	\li <b>preSolve</b>
	\li <b>postSolve</b>

If users override these methods, they are responsible for calling the base class
implementation of the overridden method(s).  The base class handles a few things
such as solver locking/unlocking and FK/IK solution blending.  If users fail to
call the base class implementation, instabilities can occur leading to wrong
evaluation or even crashes.

To register a solver, write a creator method to return an instance of the
user solver:

    \code
    userSolver::creator() {
        return new userSolver;
    }
    \endcode

The solver can then be registered using <b>MFnPlugin::registerSolver</b>.

Once the solver is registered it can be assigned to IK handles and its
solve methods will be called in the same manner as the solvers within
Maya.
*/
class OPENMAYAANIM_EXPORT MPxIkSolver
{
public:
	virtual ~MPxIkSolver();

	static void			registerSolver( const MString & solverName,
								MCreatorFunction creatorFunction );


	virtual MStatus		preSolve();
	virtual MStatus		doSolve();
	virtual MStatus		postSolve( MStatus );


	// These methods MUST be overridden by the user.
	//
	virtual MString		solverTypeName() const;
	virtual bool		isSingleChainOnly() const;
	virtual bool		isPositionOnly() const;
	virtual bool		hasJointLimitSupport() const;
	virtual bool		hasUniqueSolution() const;
	virtual bool		groupHandlesByTopology() const;


	virtual MStatus		setFuncValueTolerance( double tolerance );
	virtual MStatus		setMaxIterations( int value );


	MIkHandleGroup * 	handleGroup() const;
	virtual void 		setHandleGroup( MIkHandleGroup* );
	const MMatrix *		toWorldSpace() const;
	const MMatrix *		toSolverSpace() const;
	double 				funcValueTolerance() const;
	int 				maxIterations() const;
	virtual void 		snapHandle( MObject& handle );

	void				create();

	static 	const char*	className();

protected:

	MStatus				getJointAngles( MDoubleArray& ) const;
	MStatus				setJointAngles( const MDoubleArray& );
	void				setToRestAngles();

	MPxIkSolver();

private:
	void*	instance;

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxIkSolver */
