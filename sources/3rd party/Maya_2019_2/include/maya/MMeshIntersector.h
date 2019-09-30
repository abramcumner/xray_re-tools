#ifndef _MMeshIntersector
#define _MMeshIntersector
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
// CLASS:    MMeshIntersector
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <float.h>
#include <maya/MStatus.h>
#include <maya/MPoint.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>
#include <maya/MMatrix.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MMeshIntersector)

//! \ingroup OpenMaya
//! \brief Mesh intersector.
/*!
	The MMeshIntersector class contains methods for efficiently finding
	the closest point on a mesh.  An octree algorithm is used to
	find the closest point.

	The create() method builds the internal data required for the algorithm.
	As a result, calls to it should be minimized as it is a heavy operation.

	This class allows multiple threads to evaluate closest points
	simultaneously as the method getClosestPoint is threadsafe.

*/
class OPENMAYA_EXPORT MMeshIntersector
{
public:
	MMeshIntersector(void);
	virtual ~MMeshIntersector(void);

	MStatus create( MObject &meshObject, const MMatrix& matrix = MMatrix::identity );

	bool isCreated(void) const;

	MStatus getClosestPoint( const MPoint& point, MPointOnMesh& meshPoint, double maxDistance = DBL_MAX ) const;

	static const char*	className();

protected:
// No protected members
private:
	void *instance;
};

// Mesh point information. (OpenMaya) (OpenMaya.py)
//! \ingroup OpenMaya
//! \brief Mesh intersector result.
/*!
    This class is used to return information about a point on a mesh: 3D
    position, normal, barycentric coordinates, etc. Note that this can be a
    point anywhere on the surface of the mesh, not just at vertices.
*/
class OPENMAYA_EXPORT MPointOnMesh
{
public:
	MPointOnMesh();

	MFloatPoint  &	getPoint();
	MFloatVector & 	getNormal();
	void			getBarycentricCoords(float& u, float& v) const;

	int				faceIndex() const;
	int				triangleIndex() const;

protected:
// No protected members

private:
	friend class MMeshIntersector;
	MFloatPoint point;
	MFloatVector normal;
	int faceId;
	int triangleId;
	float baryU;
	float baryV;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MMeshIntersector */
