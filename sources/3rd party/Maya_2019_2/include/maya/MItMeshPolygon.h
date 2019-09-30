#ifndef _MItMeshPolygon
#define _MItMeshPolygon
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
// CLASS:    MItMeshPolygon
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MColor.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MVectorArray.h>
#include <maya/MColorArray.h>
#include <maya/MString.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItMeshPolygon)

//! \ingroup OpenMaya
//! \brief Polygon iterator. 
/*!
	This class is the iterator for polygonal surfaces (meshes).

	The iterator functions in two modes depending on whether a component
	is specified. When a component is not given or is NULL the iteration
	will be over all polygons for the surface.  When a component is given
	this iterator will iterate over the polygons (faces) specified in the
	component. When iterating over components a DAG path to the surface must
	also be supplied.
*/
class OPENMAYA_EXPORT MItMeshPolygon
{
public:
	MItMeshPolygon( const MObject & polyObject, MStatus * ReturnStatus = NULL );
	MItMeshPolygon( const MDagPath &polyObject,
					MObject & component = MObject::kNullObj,
					MStatus * ReturnStatus = NULL );
	virtual ~MItMeshPolygon();
	bool			isDone( MStatus * ReturnStatus = NULL );
	MStatus			next();
	MStatus			reset();
	MStatus			reset( const MObject & polyObject );
	MStatus			reset( const MDagPath &polyObject,
						MObject & component = MObject::kNullObj );
	// Count methods.
	unsigned int	count( MStatus * ReturnStatus = NULL );
	unsigned int	polygonVertexCount( MStatus * ReturnStatus = NULL );


	MPoint		center( MSpace::Space space = MSpace::kObject,
						MStatus * ReturnStatus = NULL ) ;
	// Obsolete
    OPENMAYA_DEPRECATED(2019, "Use the MItMeshPolygon::currentItem method.")
	MObject		polygon( MStatus * ReturnStatus = NULL );

	MObject		currentItem( MStatus * ReturnStatus = NULL );

	unsigned int	index( MStatus * ReturnStatus = NULL );
	MStatus		setIndex(int index, int &prevIndex);


	// Vertex methods.
	unsigned int	vertexIndex( int index, MStatus * ReturnStatus = NULL );
	MStatus		getVertices( MIntArray & vertices );
	MPoint		point( int index, MSpace::Space space = MSpace::kObject,
						MStatus * ReturnStatus = NULL );
	void		getPoints(  MPointArray & pointArray, MSpace::Space space = MSpace::kObject,
						MStatus * ReturnStatus = NULL );
	MStatus		setPoint( const MPoint & point, unsigned int index,
						MSpace::Space space = MSpace::kObject );

	MStatus		setPoints( MPointArray & pointArray, MSpace::Space space = MSpace::kObject );

	// Normal methods.
	unsigned int	normalIndex( int vertex, MStatus * ReturnStatus = NULL) const;
	MStatus		getNormal( MVector & normal,
							MSpace::Space space = MSpace::kObject) const;
	MStatus		getNormal( unsigned int Vertexindex, MVector & normal,
							MSpace::Space space = MSpace::kObject) const;
	MStatus		getNormals( MVectorArray & vectorArray,
						  MSpace::Space space = MSpace::kObject) const;

	unsigned int tangentIndex( int vertex, MStatus * ReturnStatus = NULL ) const;

	// Uv methods.
	bool		hasUVs( MStatus * ReturnStatus = NULL ) const;
	bool		hasUVs( const MString & uvSet,
						MStatus * ReturnStatus = NULL ) const;
	MStatus		setUV( int vertexId, float2 & uvPoint,
					   const MString * uvSet = NULL);
	MStatus		getUV( int vertexId, float2 & uvPoint,
					   const MString * uvSet = NULL) const;
	MStatus 	setUVs( MFloatArray& uArray, MFloatArray& vArray,
						const MString * uvSet = NULL);
	MStatus 	getUVs( MFloatArray& uArray, MFloatArray& vArray,
						const MString * uvSet = NULL) const;
	MStatus		getPointAtUV( MPoint &pt, float2 & uvPoint,
							  MSpace::Space space = MSpace::kObject,
							  const MString * uvSet = NULL,
                              float tolerance = 0.0 );
	MStatus		getAxisAtUV( MVector &normal,
							 MVector &uTangent,
							 MVector &vTangent,
							 float2 &uvPoint,
							 MSpace::Space space = MSpace::kObject,
							 const MString *uvSet = NULL,
							 float tolerance = 0.0 );
	MStatus		getUVAtPoint( MPoint &pt, float2 & uvPoint,
							  MSpace::Space space = MSpace::kObject,
							  const MString * uvSet = NULL);
	MStatus		getUVIndex( int vertex, int & index, const MString *uvSet = NULL );
	MStatus		getUVIndex( int vertex, int & index, float & u, float & v,
							const MString *uvSet = NULL);
	MStatus		getUVSetNames( MStringArray &setNames) const;

	// Color methods.
	bool		hasColor(MStatus * ReturnStatus = NULL ) const;
	bool		hasColor(int localVertexIndex, MStatus * ReturnStatus = NULL );
	MStatus		getColor(MColor &color, const MString *colorSetName = NULL);
	MStatus		getColor(MColor &color, int vertexIndex);
	MStatus		getColors(MColorArray &colors, const MString *colorSetName = NULL);
	MStatus		numColors( int &count, const MString *colorSetName = NULL );
	MStatus		getColorIndex(int vertexIndex, int & colorIndex, const MString *colorSetName = NULL);
	MStatus		getColorIndices(MIntArray & colorIndex, const MString *colorSetName = NULL);


	// Triangulation methods.
	bool		hasValidTriangulation(MStatus * ReturnStatus = NULL) const;
	MStatus		numTriangles(int &count) const;
	MStatus		getTriangle( int localTriIndex, MPointArray & points,
							 MIntArray & vertexList,
							 MSpace::Space space = MSpace::kObject) const;
	MStatus		getTriangles(MPointArray & points,
							 MIntArray & vertexList,
							 MSpace::Space space = MSpace::kObject) const;

	MStatus		updateSurface();
	MStatus		geomChanged();

	// Info convenience methods.
	MStatus		getEdges( MIntArray & edges );

	MStatus		getConnectedFaces( MIntArray & faces);
	MStatus		getConnectedEdges( MIntArray & edges );
	MStatus		getConnectedVertices( MIntArray & vertices );
	bool		isConnectedToFace( int index, MStatus * ReturnStatus = NULL);
	bool		isConnectedToEdge( int index, MStatus * ReturnStatus = NULL);
	bool		isConnectedToVertex( int index, MStatus * ReturnStatus = NULL);
	MStatus		numConnectedFaces(int & faceCount ) const;
	MStatus		numConnectedEdges(int & edgeCount) const;
	bool		onBoundary(MStatus * ReturnStatus = NULL );
	MStatus		getArea(double &area,
						MSpace::Space space = MSpace::kObject );
	bool		zeroArea(MStatus * ReturnStatus = NULL);
	MStatus		getUVArea(double &area,
						  const MString * uvSet = NULL);
	bool		zeroUVArea(MStatus * ReturnStatus = NULL);
	bool		zeroUVArea(	const MString & uvSet,
							MStatus * ReturnStatus = NULL);
	bool		isConvex(MStatus * ReturnStatus = NULL);
	bool		isStarlike(MStatus * ReturnStatus = NULL);
	bool		isLamina(MStatus * ReturnStatus = NULL);
	bool		isHoled(MStatus * ReturnStatus = NULL);
	bool		isPlanar(MStatus * ReturnStatus = NULL);
	bool		isUVReversed(const MString * uvSet = NULL,
							 MStatus * ReturnStatus = NULL);

	static const char*  className();

protected:
	bool		getUVSetIndex( const MString * uvSetName,
							   int & uvSet) const;

	bool		updateColorSet(const MString *colorSetName, int& prevSetId);

	void		resetColorSetToPrevious(const MString *colorSetName, int prevSetId);

private:
	void	*		f_it;
	MPtrBase*		f_shape;
	void	*		f_path;
	void	*		f_geom;
	void	*		fElements;
	int				fCurrentElement;
	int				fMaxElements;
	int				fCurrentIndex;
	void	*		f_face;
	void	*		f_ref;
	bool			fDirectIndex;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItMeshPolygon */
