#ifndef _MPxIkSolverNode
#define _MPxIkSolverNode
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
// CLASS:    MPxIkSolverNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxIkSolverNode)

//! \ingroup OpenMayaAnim MPx
//! \brief Base class for user defined IK solvers  
/*!
This is the base class for writing user-defined IK solvers.
Users must at least override the following methods in order to write a solver:

	\li <b>doSolve</b>
	\li <b>solverTypeName</b>

Users can optionally override the following methods if they want to perform work
before or after <b>doSolve</b> is called:

	\li <b>preSolve</b>
	\li <b>postSolve</b>

Since Maya 2018, users no longer need to call the base class version of these
functions if overriding them.

Note that the following virtual methods (declared in MPxNode) are
irrelevant for the MPxIkSolverNode. If these methods are overridden in
a class derived from MPxIkSolverNode, they will be ignored.

	\li <b>compute</b>
	\li <b>getInternalValue</b>
	\li <b>setInternalValue</b>
	\li <b>legalConnection</b>
	\li <b>legalDisconnection</b>
	\li <b>connectionMade</b>
	\li <b>connectionBroken</b>


A creator method is necessary to return an instance of the
user solver:

\code
    userSolver::creator() {
        return new userSolver;
    }
\endcode

In order to create and register the solver, execute the mel command:
\code
    createNode userSolver;
\endcode

Once the solver is registered it can be assigned to IK handles and its
solve methods will be called in the same manner as the solvers within
Maya.
*/
class OPENMAYAANIM_EXPORT MPxIkSolverNode : public MPxNode
{
public:
	virtual ~MPxIkSolverNode();
	virtual MPxNode::Type type() const;

	virtual MStatus		preSolve();
	virtual MStatus		doSolve();
	virtual MStatus		postSolve( MStatus );

	virtual MString		solverTypeName() const;

	bool				rotatePlane(MStatus *ReturnStatus = NULL) const;
	MStatus				setRotatePlane(bool rotatePlane);
	bool				singleChainOnly(MStatus *ReturnStatus = NULL) const;
	MStatus				setSingleChainOnly(bool singleChainOnly);
	bool				positionOnly(MStatus *ReturnStatus = NULL) const;
	MStatus				setPositionOnly(bool positionOnly);
	bool				supportJointLimits(MStatus *ReturnStatus = NULL) const;
	MStatus				setSupportJointLimits(bool supportJointLimits);
	bool				uniqueSolution(MStatus *ReturnStatus = NULL) const;
	MStatus				setUniqueSolution(bool uniqueSolution);

	// Obsolete
	virtual bool		isSingleChainOnly() const;
	// Obsolete
	virtual bool		isPositionOnly() const;
	// Obsolete
	virtual bool		hasJointLimitSupport() const;
	// Obsolete
	virtual bool		hasUniqueSolution() const;
	// Obsolete
	virtual bool		groupHandlesByTopology() const;

	virtual MStatus		setFuncValueTolerance( double tolerance );
	virtual MStatus		setMaxIterations( int value );

	MIkHandleGroup * 	handleGroup() const;
	virtual void 		setHandleGroup( MIkHandleGroup* );
	const MMatrix *		toWorldSpace() const;
	const MMatrix *		toSolverSpace() const;
	double 				funcValueTolerance() const;
	int 				maxIterations() const;

    virtual void        snapHandle( MObject& handle );
    virtual bool        isAttributeCreatedBySolver(MObject& attr) const;

	void				create();

	// SCRIPT USE ONLY
	MStatus _getJointAngles( MDoubleArray& da ) const 
		{return getJointAngles(da);}
	MStatus	_setJointAngles( const MDoubleArray& da ) 
		{return setJointAngles(da);}
	void _setToRestAngles() 
		{ setToRestAngles(); }
	//

	static const char*	className();

protected:

	// USE _getJointAngles() IN SCRIPT	
	MStatus				getJointAngles( MDoubleArray& ) const;
	// USE _setJointAngles() IN SCRIPT	
	MStatus				setJointAngles( const MDoubleArray& );
	// USE _setToRestAngles() IN SCRIPT	
	void				setToRestAngles();

	MPxIkSolverNode();

private:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxIkSolverNode */
