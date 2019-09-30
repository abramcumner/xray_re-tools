#ifndef _MNurbsIntersector
#define _MNurbsIntersector
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
// CLASS:    MNurbsIntersector
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <float.h>
#include <maya/MStatus.h>
#include <maya/MPointArray.h>
#include <maya/MMatrix.h>

// ****************************************************************************
// DECLARATIONS

class TnurbsSurface;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MNurbsIntersector)

//! \ingroup OpenMaya
//! \brief Nurbs intersector. 
/*!
	The MNurbsIntersector class contains methods for efficiently finding
	the closest point to a NURBS surface. A tree-based algorithm is used to
	find the closest point.

	The create() method builds the internal data required for the algorithm.
	As a result, calls to it should be minimized as it is a heavy operation.
	If the NURBS surface or matrix changes, you will need to recreate the
	tree based data to correspond, again using the create() method.

	This class is not threadsafe.
*/
class OPENMAYA_EXPORT MNurbsIntersector
{
public:
	MNurbsIntersector( void );
	virtual			~MNurbsIntersector( void );

	MStatus			create( MObject &nurbsObject,
						const MMatrix &matrix = MMatrix::identity );

	bool			isCreated( void ) const;

	MStatus			getClosestPoint( const MPoint& point,
						MPointOnNurbs& nurbsPoint,
						double maxDistance = DBL_MAX ) const;

	MStatus			getIntersects(const MPoint &rayStartingPoint,
									const MVector &alongThisDirection,
									MDoubleArray & uArray, MDoubleArray & vArray,
									MPointArray &points,
									double tolerance) const;

	MStatus			getIntersect(const MPoint &rayStartingPoint,
									const MVector &alongThisDirection,
									double & u, double & v,
									MPoint &pntOfIntersection,
									double tolerance) const;

	static const char*	className();

protected:
// No protected members

private:
	mutable void	*instance;
	mutable void	*instanceRay;
	TnurbsSurface	*surf;

	MMatrix			matrix;

	bool			created;
};

// NURBS point information. (OpenMaya) (OpenMaya.py)
//
class OPENMAYA_EXPORT MPointOnNurbs
{
public:
	//
	MPointOnNurbs();
	//
	MPointOnNurbs( const MPointOnNurbs& other );
	//
	MPointOnNurbs&	operator=( const MPointOnNurbs& other );
	//
	MPoint			&getPoint();
	//
	MPoint			&getUV();

private:
	friend class	MNurbsIntersector;
	MPoint			point;
	MPoint			uv;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MNurbsIntersector */
