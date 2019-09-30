#ifndef _MFnMesh
#define _MFnMesh

// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

// ****************************************************************************
//
// CLASS:    MFnMesh
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MColor.h>
#include <maya/MVectorArray.h>
#include <maya/MStringArray.h>
#include <maya/MColorArray.h>
#include <maya/MDGModifier.h>
#include <maya/MString.h>
#include <maya/MMeshSmoothOptions.h>
#include <maya/MDeprecate.h>

class TmidBody;

// Default tolerance
#define kMFnMeshTolerance 1.0e-3

// Point equivalent tolerance
#define kMFnMeshPointTolerance 1.0e-10

// For per-instance mesh data
#define kMFnMeshInstanceUnspecified -1

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnMesh)

//! \ingroup OpenMaya MFn
//! \brief Polygonal surface function set.
/*!
<b>Note on Terminology: </b> The following terms are used interchangeably:

\li mesh and object,
\li polygon and face,
\li object-relative indices and global/mesh-relative indices,
\li face-relative indices and local indices


<b>object-relative indices </b> - they range between 0..(numVertices()-1)

<b>face-relative indices </b>   - they range between 0..(polygonVertexCount(faceId)-1)

This function set provides access to polygonal meshes. Objects of type
MFn::kMesh, MFn::kMeshData, and MFn::kMeshGeom are supported.
MFn::kMesh objects are shapes in the DAG, MFn::kMeshGeom objects are
the raw geometry that the shapes use, and MFn::kMeshData objects are the
data that is passed through dependency graph connections.

NOTE: If the function set has been initialized with a MFn::kMesh object then
it must have geometry attached.

<b>Mesh Construction</b>
Meshes are created by specifying a vertex list, a list of vertex counts
per-polygon, and a list of vertices that make up the polygons.
Edge data is also maintained for the mesh and can be returned to the caller.
The following lists are used by the create method to construct a mesh:


	\li <b>vertex list</b>
	All of the vertices for the mesh are stored in a master vertex list which is
	an array indexed from 0 to numVertices()-1. Vertices in this list can be
	shared among polygons.

	\li <b>polygon count list</b>
	This is a list containing the number of vertices for each polygon. Each
	polygon in the mesh would have an entry in this list containing its
	number of vertices.

	\li <b>vertex connection list</b>
	This is a list of the vertex id's for each polygon. Since polygons can
	have different numbers of vertices, the polygon count list is
	used to determine which polygons are represented by the various
	sections of this list.


There are several cases in which the create method will alter the
data passed in:

    \li If the data for the mesh is provided in such a way that the
          verticies for some polygons are specified in a clockwise
          direction, and others in a counter-clockwise direction, the
          edges between neighboring polygons with reversed orderings
          will be duplicated.  In this context, "duplicating an edge"
          means that copies will be made of the edge's vertices and the
          index list for the adjoining polygons will be redefined to

          use the appropriate vertices.

    \li If the <i>vertex connection list</i> for a polygon specifies an
	      edge with the same start and end verticies, those edges will
		  be removed.  (ie. a polygon specified as 1 2 2 3 4 will be
		  transformed into 1 2 3 4).

    \li If the same vertex appears in the <i>vertex connection list</i>
          for a polygon more than once, another vertex will be created at
		  the same location, and the list will be altered to use the copy.
          (ie. a polygon specified as 1 2 3 1 4 will be transformed into
          1 2 3 5 4, where 5 is in the same location as 1).



<b>Texture Coordinates (UV's)</b>
You can optionally specify texture (uv) coordinates. UV coordinates are 2-d
coordinates used for mapping textures to the polygons of a mesh.
All of the uv coordinates are stored in one or more named UV sets.
The default mesh uv list stores uv coordinates in an array
indexed from 0 to numUVs()-1.  UV's are referenced on a per-polygon
per-vertex basis, so for a polygon with 4 vertices there would be 4 uv's.
The uv values for a polygon are set by specifying the index of the uv for
each vertex of the polygon. This index is the uv's position in the master uv
array.
UV's are optional so polygons do not have to have uv values associated with
them. There may be only 1 or 2 polygons in a mesh that have uv coordinates
set, or all of the polygons may have uv's.
UV's can also be shared so several different polygons may reference the
same uv's.
If you are creating a UV for the first time or changing the order of
the UV then you need to set the UV using one of the setUV methods and
then assign new indices using one of the assignUV methods. It is not
necessary to call assignUV if editing the position of an existing UV.

<b>Color per vertex</b>
You can optionally specify color per vertex information using this class.
It is possible to create and manipulate color per vertex information
using the methods available. Many of the color per vertex methods have
a default parameter: colorSet.  If unspecified, the method will operate
on the current color set.

<b>Normals</b>
Some of the Normals in Maya are now user-settable. If the vertex
normals are not set or locked, they are computed by maya when the
mesh changes. If set or locked, the normals remain frozen relative
to the object, unless the user unlocks them.
There are 3 types of normals for a mesh:


	\li <b>Per-vertex per-polygon</b>
	Polygonal objects store a list of per-vertex per-polygon normals
	(similar to the vertex list) This list is what is returned by
	MFnMesh::getNormals and MItMeshPolygon::getNormal(index, normal).
	As well, MItMeshPolygon::normalIndex( vertexIndex ) returns the
	index into the normal list for a particular vertex of the polygon.

	For a cube, the list would contain 24 normals (4 vertices * 6
	polygons) since the edges for a cube are hard and the per-vertex
	per-polygon normals cannot be shared.  For a sphere or torus, which
	has smooth edges, the normals can be shared, and thus the normal
	list contains the same number of normals as vertices.

	\li <b>Per-polygon normals</b>
	Polygonal objects store a normal for each polygon.  These normals
	can be accessed via MItMeshPolygon::getNormal(normal) or
	MFnMesh::getPolygonNormal.  So, a cube, which is comprised of 6
	polygons would contain 6 such normals.

	\li <b>Per-vertex</b>
	You can also access a normal for each vertex of the mesh, independent
	of any polygons.  Such normals are not stored in the object,
	but instead are calculated by Maya upon request as the average of
	all the per-polygon normals for the polygons adjacent to the vertex.
	These normals are what is returned by MFnMesh::getVertexNormal or
	MItMeshVertex::getNormal.



<b>Blind Data</b>
MFnMesh allows you to create Blind Data types, and to create and access
mesh component level blind data, but not object level blind data.
Object level data may be assigned through the polyBlindData command
or the Blind Data Editor.
However, object level blind data is just an ordinary compound dynamic
attribute directly on the object. The parent attribute name is
BlindDataNNNN where NNNN is the blind data typeID. e.g. if your typeId
is 1001 your object will have a compound dynamic attr called BlindData1001.

<b>Handling Components</b>
There are also three iterator classes which provide more specific control
over the vertices, edges, and polygons of a mesh and are useful when dealing
with components. See MItMeshPolygon, MItMeshEdge, and MItMeshVertex.
*/
class OPENMAYA_EXPORT MFnMesh : public MFnDagNode
{
	declareDagMFn(MFnMesh, MFnDagNode);

public:
	//! Specifies which color components are used by a colorSet.
        enum MColorRepresentation {
                kAlpha = 1,	//!< Alpha only.
                kRGB = 3,	//!< Red, green and blue only.
                kRGBA = 4	//!< Red, green, blue and alpha.
        };

    MObject     create( int numVertices, int numPolygons,
						const MFloatPointArray &vertexArray,
                        const MIntArray &polygonCounts,
                        const MIntArray &polygonConnects,
						MObject parentOrOwner = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );

    MObject     create( int numVertices, int numPolygons,
						const MPointArray 	&vertexArray,
                        const MIntArray 	&polygonCounts,
                        const MIntArray 	&polygonConnects,
						MObject parentOrOwner  = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );
    MObject     create( int numVertices, int numPolygons,
						const MPointArray 	&vertexArray,
                        const MIntArray 	&polygonCounts,
                        const MIntArray 	&polygonConnects,
						bool				storeDoubles,
						MObject parentOrOwner  = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );

	MObject		create( int numVertices, int numPolygons,
						const MFloatPointArray &vertexArray,
						const MIntArray &polygonCounts,
						const MIntArray &polygonConnects,
						const MFloatArray & uArray,
						const MFloatArray & vArray,
						MObject parentOrOwner  = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );
	MObject		create( int numVertices, int numPolygons,
						const MPointArray &vertexArray,
						const MIntArray &polygonCounts,
						const MIntArray &polygonConnects,
						const MFloatArray & uArray,
						const MFloatArray & vArray,
						MObject parentOrOwner  = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );
	MObject		create( const MFloatPointArray &vertexArray, 
						const MIntArray &edges, 
						const MIntArray &edgeConnectsCount, 
						const MIntArray &edgeFaceConnects, 
						const MIntArray &edgeFaceDesc,
						MObject parentOrOwner = MObject::kNullObj,
						MStatus * ReturnStatus = NULL ); 
	MObject		create( const MPointArray &vertexArray, 
						const MIntArray &edges, 
						const MIntArray &edgeConnectsCount, 
						const MIntArray &edgeFaceConnects, 
						const MIntArray &edgeFaceDesc,
						bool			storeDoubles,
						MObject parentOrOwner = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );
	MObject		generateSmoothMesh ( MObject parentOrOwner = MObject::kNullObj,
						MMeshSmoothOptions * options = NULL,
						MStatus * ReturnStatus = NULL );
	MStatus		getSmoothMeshDisplayOptions ( MMeshSmoothOptions& options ) const;

	MStatus		setSmoothMeshDisplayOptions ( const MMeshSmoothOptions& options );
	MObject     addPolygon( const MPointArray & vertexArray,
                        	bool 		mergeVertices = true,
                        	double 		pointTolerance = kMFnMeshPointTolerance,
							MObject 	parentOrOwner = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );
    MObject     addPolygon( const MPointArray & vertexArray,
							int & 		faceIndex,
                        	bool 		mergeVertices = true,
                        	double 		pointTolerance = kMFnMeshPointTolerance,
							MObject 	parentOrOwner = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );
    MObject     addPolygon( const MPointArray & vertexArray,
							const MIntArray & loopCounts,
							int & 		faceIndex,
                        	bool 		mergeVertices = true,
                        	double 		pointTolerance = kMFnMeshPointTolerance,
							MObject 	parentOrOwner = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );
	MStatus		addHoles( int faceIndex,
						  const MPointArray & vertexArray,
						  const MIntArray & loopCounts,
                          bool 		mergeVertices = true,
                          double 	pointTolerance = kMFnMeshPointTolerance );

	MStatus		deleteFace(int index, MDGModifier *modifier = NULL);
	MStatus		deleteEdge(int index, MDGModifier *modifier = NULL);
	MStatus		deleteVertex(int index, MDGModifier *modifier = NULL);


	//! Specifies how a point along split is to be determined.
	enum SplitPlacement {
		kOnEdge,	//!< Split at a position along an edge.
		kInternalPoint,	//!< Split at a point within a face
		kInvalid	//!< \nop
    };
	MStatus     split(MIntArray & placements,
					  MIntArray & edgeList,
					  MFloatArray & edgeFactors,
					  MFloatPointArray & internalPoints);
	MStatus		subdivideFaces(MIntArray & faceList,
							   int divisionCount);
	MStatus     subdivideEdges(MIntArray & edgeList,
							   int divisionCount);
	MStatus		extrudeFaces(MIntArray & faceList,
							 int extrusionCount,
							 MFloatVector* translation,
							 bool extrudeTogether,
							 float thickness = 0.0f,
							 float offset = 0.0f);
	MStatus		extrudeEdges(MIntArray & edgeList,
							 int extrusionCount,
							 MFloatVector* translation,
							 bool extrudeTogether,
							 float thickness = 0.0f,
							 float offset = 0.0f);
	MStatus		duplicateFaces(MIntArray & faceList,
							   MFloatVector* translation);
	MStatus		extractFaces(MIntArray & faceList,
							 MFloatVector* translation);
	MStatus		collapseFaces(MIntArray & faceList);
	MStatus		collapseEdges(MIntArray & edgeList);

	// Counts
	int        numVertices( MStatus * ReturnStatus = NULL) const;
	int        numEdges( MStatus * ReturnStatus = NULL ) const;
	int        numPolygons( MStatus * ReturnStatus = NULL ) const;
	int		   numFaceVertices( MStatus * ReturnStatus = NULL ) const;
	int        polygonVertexCount( int polygonId,
									MStatus * ReturnStatus = NULL ) const;
	int		   numUVs( MStatus * ReturnStatus = NULL ) const;
	int		   numUVs( const MString &uvSet,
					   MStatus * ReturnStatus = NULL ) const;
	int		   numColors( MStatus * ReturnStatus = NULL ) const;
	int		   numColors( const MString &colorSet,
					   MStatus * ReturnStatus = NULL ) const;
	int		   numNormals( MStatus * ReturnStatus = NULL ) const;

	bool		   hasColorChannels( const MString &colorSet, MStatus * ReturnStatus = NULL ) const;

	bool		   hasAlphaChannels( const MString &colorSet, MStatus * ReturnStatus = NULL ) const;

	MColorRepresentation  getColorRepresentation( const MString &colorSet, MStatus * ReturnStatus = NULL) const;

	bool		   isColorClamped( const MString &colorSet, MStatus * ReturnStatus = NULL ) const;

	MStatus		   setIsColorClamped( const MString &colorSet, bool clamped);

    MStatus     getTriangles( MIntArray & triangleCounts, MIntArray &triangleVertices ) const;

    MStatus     getTriangleOffsets( MIntArray & triangleCounts, MIntArray &triangleIndices ) const;

	//! Boolean operators
	enum BoolOperation {
		//! Boolean union
		kUnion = 1,
		//! Boolean difference
		kDifference,
		//! Boolean intersection
		kIntersection
	};

	//	Obsolete
	OPENMAYA_DEPRECATED(2019, "This method uses Maya's legacy boolean algorithm. Use MFnMesh::booleanOps instead to choose between Maya's newer boolean algorithm and the legacy algorithm.")
	MStatus	booleanOp( BoolOperation op, MFnMesh &mesh1, MFnMesh &mesh2 );

	//	new boolean operator
	MStatus	booleanOps( BoolOperation op, const MObjectArray& meshes, bool useLegacy = false );

	//	Intersection methods with acceleration options
	static MMeshIsectAccelParams uniformGridParams( int xDiv, int yDiv, int zDiv );

	static MMeshIsectAccelParams autoUniformGridParams();

	bool	closestIntersection(	const MFloatPoint& 	raySource,
									const MFloatVector& rayDirection,
									const MIntArray* 	faceIds,
									const MIntArray* 	triIds,
										  bool  		idsSorted,
										  MSpace::Space space,
										  float 		maxParam,
										  bool  		testBothDirections,
										  MMeshIsectAccelParams *accelerator,
										  MFloatPoint& 	hitPoint,
										  float* 		hitRayParam,
										  int* 			hitFace,
										  int* 			hitTriangle,
										  float*		hitBary1,
										  float*		hitBary2,
										  float			tolerance = 1e-6,
										  MStatus*		ReturnStatus = NULL );

	bool	anyIntersection(		const MFloatPoint& 	raySource,
									const MFloatVector& rayDirection,
									const MIntArray* 	faceIds,
									const MIntArray* 	triIds,
										  bool  		idsSorted,
										  MSpace::Space	space,
										  float 		maxParam,
										  bool  		testBothDirections,
										  MMeshIsectAccelParams *accelerator,
										  MFloatPoint& 	hitPoint,
										  float* 		hitRayParam,
										  int* 			hitFace,
										  int* 			hitTriangle,
										  float*		hitBary1,
										  float*		hitBary2,
										  float			tolerance = 1e-6,
										  MStatus*		ReturnStatus = NULL );
	bool	allIntersections(		const MFloatPoint& 			raySource,
									const MFloatVector& 		rayDirection,
									const MIntArray* 			faceIds,
									const MIntArray* 			triIds,
										  bool  				idsSorted,
										  MSpace::Space			space,
										  float 				maxParam,
										  bool  				testBothDirections,
										  MMeshIsectAccelParams *accelerator,
										  bool					sortHits,
										  MFloatPointArray& 	hitPoints,
										  MFloatArray* 			hitRayParams,
										  MIntArray*  			hitFaces,
										  MIntArray* 			hitTriangles,
										  MFloatArray*			hitBary1,
										  MFloatArray*			hitBary2,
										  float					tolerance = 1e-6,
										  MStatus*				ReturnStatus = NULL );

	MStatus		sortIntersectionFaceTriIds(	MIntArray* faceIds, MIntArray* triIds );

	MStatus		freeCachedIntersectionAccelerator();

	MString		cachedIntersectionAcceleratorInfo( MStatus *ReturnStatus = NULL );

	static MString		globalIntersectionAcceleratorsInfo();

	static void			clearGlobalIntersectionAcceleratorInfo();

	// Intersection/getClosestPoint methods.
	bool     	intersect( const MPoint & raySource,
                            const MVector & rayDirection,
							MPointArray & points,
                            double tolerance = kMFnMeshPointTolerance,
							MSpace::Space space = MSpace::kObject,
							MIntArray * polygonIds = NULL,
							MStatus * ReturnStatus = NULL ) const;
    MStatus     getClosestPointAndNormal( const MPoint &  toThisPoint,
                			MPoint & theClosestPoint,
                			MVector & theNormal,
							MSpace::Space space = MSpace::kObject,
							int * closestPolygon = NULL,
							MMeshIsectAccelParams *accelParams = NULL) const;
    MStatus     getClosestPoint(const MPoint &toThisPoint,
                            MPoint &theClosestPoint,
							MSpace::Space space = MSpace::kObject,
							int * closestPolygon = NULL,
							MMeshIsectAccelParams *accelerator = NULL) const;

    MStatus     getClosestNormal(const MPoint &toThisPoint,
                            MVector & theNormal,
							MSpace::Space space = MSpace::kObject,
							int * closestPolygon = NULL,
							MMeshIsectAccelParams *accelerator = NULL ) const;

    MStatus     getClosestUVs(const float2 &toThisUVPoint,
                            MIntArray& uvIds,
							const MString * uvSet = NULL ) const;

	MStatus     intersectFaceAtUV(const float2 &toThisUVPoint,
							int &faceId,
							const MString *uvSet = NULL) const;


	// Shaders methods.
    MStatus     getConnectedShaders(
                            unsigned int instanceNumber,
                            MObjectArray & shaders,
							MIntArray & indices ) const;

	MObject		copy( const MObject & source,
					  MObject parentOrOwner = MObject::kNullObj,
					  MStatus * ReturnStatus = NULL );
	MStatus		updateSurface();
	MStatus		syncObject();


	// Direct access to internal data arrays
	const float* getRawPoints(MStatus*) const;
	const double* getRawDoublePoints(MStatus*) const;
	const float* getRawNormals(MStatus*) const;

	// Get/set methods for mesh vertices.
	MStatus 	getPoints(MFloatPointArray & vertexArray,
						  MSpace::Space space = MSpace::kObject) const;
	MStatus 	getPoints(MPointArray & vertexArray,
						  MSpace::Space space = MSpace::kObject) const;
	MStatus 	setPoints(MFloatPointArray & vertexArray,
						  MSpace::Space space = MSpace::kObject);
	MStatus 	setPoints(MPointArray & vertexArray,
						  MSpace::Space space = MSpace::kObject);
	MStatus 	getVertices( MIntArray & vertexCount,
							 MIntArray & vertexList ) const;
	MStatus 	getPolygonVertices( int polygonId,
							MIntArray & vertexList ) const;
	MStatus	    getPolygonTriangleVertices( int polygonId,
											int triangleId,
			   								int triangleVertices[3] ) const;
	MStatus		setPoint(int vertexId, const MPoint & pos,
						 MSpace::Space space = MSpace::kObject);
	MStatus		getPoint(int vertexId, MPoint & pos,
						 MSpace::Space space = MSpace::kObject) const;


	// Methods for setting/getting Normals
	MStatus 	getNormals( MFloatVectorArray& normals,
							MSpace::Space space = MSpace::kObject ) const;
	MStatus 	setNormals( MFloatVectorArray& normals,
							MSpace::Space space = MSpace::kObject );
    MStatus     getFaceVertexNormal( int faceIndex, int vertexIndex,
                            MVector & normal,
                            MSpace::Space space = MSpace::kObject ) const;
    MStatus     getFaceVertexNormals( int faceIndex, MFloatVectorArray & normals,
                            MSpace::Space space = MSpace::kObject ) const;
    MStatus     getNormalIds( MIntArray & normalIdCounts, MIntArray &normalIds ) const;
    MStatus     getFaceNormalIds( int faceIndex, MIntArray &normals ) const;
	MStatus		setFaceVertexNormal( MVector &normal,
									int faceIndex, int vertexIndex,
									MSpace::Space space = MSpace::kObject,
									MDGModifier *modifier = NULL);
	MStatus		setVertexNormal( MVector &normal,
									int vertexIndex,
									MSpace::Space space = MSpace::kObject,
									MDGModifier *modifier = NULL);
	MStatus		setFaceVertexNormals( MVectorArray & normalArray,
									MIntArray & faceList, MIntArray & vertexList,
									MSpace::Space space = MSpace::kObject );
	MStatus		setVertexNormals( MVectorArray & normalArray,
									MIntArray & vertexList,
									MSpace::Space space = MSpace::kObject );
	OPENMAYA_DEPRECATED(2019, "Use getVertexNormal(int, bool, MVector&, MSpace::Space) instead.")
	MStatus		getVertexNormal( int vertexId, MVector & normal,
								 MSpace::Space space = MSpace::kObject ) const;
	MStatus		getVertexNormal( int vertexId, bool angleWeighted,
								 MVector & normal,MSpace::Space space = MSpace::kObject) const;
	MStatus		getVertexNormals( bool angleWeighted, MFloatVectorArray & normals,
								  MSpace::Space space = MSpace::kObject ) const;

	MStatus		getPolygonNormal( int polygonId, MVector & normal,
							MSpace::Space space = MSpace::kObject ) const;
	bool		isNormalLocked(int normalId, MStatus* ReturnStatus = NULL ) const;
	MStatus		lockVertexNormals(MIntArray & vertexList);
	MStatus		lockFaceVertexNormals(MIntArray & faceList, MIntArray & vertexList);
	MStatus		unlockVertexNormals(MIntArray & vertexList);
	MStatus		unlockFaceVertexNormals( MIntArray & faceList, MIntArray & vertexList);

	// Methods for getting tangents and binormals
    int			getTangentId( int faceIndex, int vertexIndex, MStatus * ReturnStatus = NULL ) const;
	MStatus 	getTangents( MFloatVectorArray& normals,
							MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL) const;
    MStatus     getFaceVertexTangent( int faceIndex, int vertexIndex,
                            MVector & tangent,
                            MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL ) const;
    MStatus     getFaceVertexTangents( int faceIndex, MFloatVectorArray & tangents,
                            MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL ) const;
	MStatus 	getBinormals( MFloatVectorArray& normals,
							MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL ) const;
    MStatus     getFaceVertexBinormal( int faceIndex, int vertexIndex,
                            MVector & normal,
                            MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL) const;
    MStatus     getFaceVertexBinormals( int faceIndex, MFloatVectorArray & normals,
                            MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL) const;
	bool		isRightHandedTangent(int tangentId,
							const MString * uvSetName = NULL,
							MStatus *ReturnStatus = NULL) const;

	// Face info methods.
	bool		isPolygonConvex( int faceIndex, MStatus * ReturnStatus = NULL ) const;

	// Edge info methods.
	MStatus 	getEdgeVertices( int edgeId, int2 & vertexList ) const;
	bool		isEdgeSmooth( int edgeId, MStatus * ReturnStatus = NULL ) const;
	MStatus		setEdgeSmoothing( int edgeId, bool smooth = true );
	MStatus		setEdgeSmoothings( const MIntArray& edgeIds, const MIntArray& smooths );
	MStatus		cleanupEdgeSmoothing();
	MStatus		getCreaseEdges(MUintArray &edgeIds, MDoubleArray &creaseData) const;
	MStatus		setCreaseEdges(const MUintArray &edgeIds, const MDoubleArray &creaseData);
	MUintArray	getInvisibleFaces(MStatus * ReturnStatus = NULL ) const;
	MStatus		setInvisibleFaces(const MUintArray &faceIds, bool makeVisible = false);

	// Vertex info methods.
	MStatus		getCreaseVertices(MUintArray &vertexIds, MDoubleArray &creaseData) const;
	MStatus		setCreaseVertices(const MUintArray &vertexIds, const MDoubleArray &creaseData);


	// UVs operators and methods.
	int			numUVSets( MStatus *ReturnStatus = NULL) const;
	MString		createUVSetWithName(const MString &uvSetName,
									MDGModifier *modifier = NULL,
									MStatus *ReturnStatus = NULL,
									const MUintArray *instances = NULL);

	MString		createUVSetDataMeshWithName(const MString &uvSetName, MStatus *ReturnStatus = NULL);

	MString		copyUVSetWithName(const MString &fromName, const MString &toName,
						  MDGModifier *modifier = NULL, MStatus *ReturnStatus = NULL);
	MStatus		renameUVSet(const MString &origName,
							const MString &newName,
							MDGModifier *modifier = NULL);
	MStatus		deleteUVSet(const MString &setName,
							 MDGModifier *modifier = NULL,
							 MSelectionList *currentSelection = NULL);
	MStatus		setCurrentUVSetName(const MString &setName,
									 MDGModifier *modifier = NULL,
									MSelectionList *currentSelection = NULL);
	MString		currentUVSetName(MStatus *ReturnStatus = NULL,
								 int instance = kMFnMeshInstanceUnspecified) const;
	MStatus		getUVSetNames(MStringArray &setNames) const;
	MStatus		getUVSetFamilyNames(MStringArray &familyNames) const;
	MStatus		getUVSetsInFamily(const MString& familyName,
								  MStringArray &setNames) const;
	bool        isUVSetPerInstance( const MString& name,
									MStatus *ReturnStatus = NULL) const;
	MStatus		getFaceUVSetNames( int polygonId, MStringArray &setNames) const;
	MStatus		getAssociatedUVSetTextures(const MString uvSetName,
										   MObjectArray & textures) const;
	MStatus		getAssociatedUVSetInstances(const MString& uvSetName,
											MIntArray& instances) const;
	MStatus 	setUVs( const MFloatArray& uArray, const MFloatArray& vArray,
						const MString * uvSet = NULL );
	MStatus 	setSomeUVs( const  MIntArray & uvIds, const MFloatArray& uArray,
						    const MFloatArray& vArray, const MString * uvSet = NULL );
	MStatus 	getUVs( MFloatArray& uArray, MFloatArray& vArray,
						const MString * uvSet = NULL ) const;
	MStatus		setUV( int uvId, float u, float v,
					   const MString * uvSet = NULL );
	MStatus		getUV( int uvId, float & u, float & v,
					   const MString * uvSet = NULL ) const;

	MStatus     getPointAtUV( int polygonId, MPoint &theClosestPoint, float2 & uvPoint,
                       MSpace::Space space = MSpace::kObject,
                       const MString * uvSet = NULL, float tolerance=0.0);
	MStatus     getPointsAtUV( MIntArray &polygonIds, MPointArray &points, const float2 &uvPoint,
					   MSpace::Space space = MSpace::kObject,
					   const MString * uvSet = NULL, float tolerance = 0.0) const;
	MStatus     getUVAtPoint( MPoint &pt, float2 & uvPoint,
                       MSpace::Space space = MSpace::kObject,
                       const MString * uvSet = NULL, int * closestPolygon = NULL);
	MStatus     getAxisAtPoint( MPoint &pt, MVector &normal, MVector &uTangent, MVector &vTangent,
                       MSpace::Space space = MSpace::kObject,
                       const MString * uvSet = NULL, int * closestPolygon = NULL);

	MStatus 	getPolygonUV( int polygonId, int vertexIndex,
								float & u, float & v,
							  const MString * uvSet = NULL ) const;
	MStatus 	getPolygonUVid( int polygonId, int vertexIndex,
								int &uvId,
								const MString * uvSet = NULL ) const;
	MStatus 	assignUV( int polygonId, int vertexIndex, int uvId,
						  const MString * uvSet = NULL );
	MStatus		assignUVs( const MIntArray& uvCounts, const MIntArray& uvIds,
						   const MString * uvSet = NULL );
	MStatus		clearUVs( const MString * uvSet = NULL );
	MStatus		getAssignedUVs( MIntArray& uvCounts, MIntArray& uvIds,
								const MString * uvSet = NULL ) const;

	MStatus		getUvShellsIds( MIntArray& uvShellIds,
								unsigned int & nbUvShells,
								const MString * uvSet = NULL ) const;

	MStatus		getPinUVs( MUintArray &pinIds, MDoubleArray &pinData,
						   const MString * uvSet = NULL ) const;
	MStatus		setPinUVs( const MUintArray &pinIds, const MDoubleArray &pinData,
						   const MString * uvSet = NULL );

	bool		isPolygonUVReversed( int polygonId, 
									 const MString * uvSet = NULL,
									 MStatus * ReturnStatus = NULL ) const;

	// Color Per Vertex Methods
	int			numColorSets( MStatus *ReturnStatus = NULL) const;
	MString		createColorSetWithName(const MString &colorSetName,
									   MDGModifier *modifier = NULL,
									   const MUintArray *instances = NULL,
									   MStatus *ReturnStatus = NULL);

	MStatus		deleteColorSet(const MString &colorSetName,
							 MDGModifier *modifier = NULL,
							 MSelectionList *currentSelection = NULL);
	MStatus		setCurrentColorSetName(const MString &setName,
									 MDGModifier *modifier = NULL,
									 MSelectionList *currentSelection = NULL);
	MString		currentColorSetName(int instance = kMFnMeshInstanceUnspecified,
									MStatus * ReturnStatus = NULL) const;
	MStatus		getColorSetNames(MStringArray &setNames) const;
	MStatus		getColorSetFamilyNames(MStringArray &familyNames) const;
	MStatus		getColorSetsInFamily(const MString& familyName,
									 MStringArray &setNames) const;
	bool        isColorSetPerInstance( const MString& name,
									   MStatus *ReturnStatus = NULL) const;
	MStatus		getAssociatedColorSetInstances(const MString& setName,
											   MIntArray& instances) const;
	MStatus		setFaceColor(MColor &color, int index);
	MStatus		setFaceColor(MColor &color, int index, MColorRepresentation rep);
	MStatus		setVertexColor(MColor &color, int index, MDGModifier *modifier = NULL);
	MStatus		setVertexColor(MColor &color, int index, MDGModifier *modifier, MColorRepresentation rep);
	MStatus		setFaceVertexColor( MColor &color, int faceIndex, int vertexIndex,
									MDGModifier *modifier = NULL);
	MStatus		setFaceVertexColor( MColor &color, int faceIndex, int vertexIndex,
					    MDGModifier *modifier, MColorRepresentation rep);
	MStatus		setFaceVertexColor(  int faceIndex, int localVertexIndex, MColor &color,
									MDGModifier *modifier = NULL);
	MStatus		setFaceVertexColor(  int faceIndex, int localVertexIndex, MColor &color,
					     MDGModifier *modifier,
					     MColorRepresentation rep);
	MStatus		setFaceColors(MColorArray &colors, MIntArray &faceList);
	MStatus		setFaceColors(MColorArray &colors, MIntArray &faceList, MColorRepresentation rep);
	MStatus		setVertexColors(MColorArray &colors, MIntArray &vertexList,
								MDGModifier *modifier = NULL);
	MStatus		setVertexColors(MColorArray &colors, MIntArray &vertexList,
					MDGModifier *modifier,
					MColorRepresentation rep);
	MStatus		setFaceVertexColors(MColorArray &colors,
									MIntArray &faceList, MIntArray &vertexList,
									MDGModifier *modifier = NULL);
	MStatus		setFaceVertexColors(MColorArray &colors,
					    MIntArray &faceList, MIntArray &vertexList,
					    MDGModifier *modifier,
					    MColorRepresentation rep);
	MStatus		removeFaceColors(MIntArray &faceList);
	MStatus		removeVertexColors(MIntArray &vertexList);
	MStatus		removeFaceVertexColors( MIntArray &faceList, MIntArray &vertexList);
	MStatus		getVertexColors(MColorArray &colors, const MString * colorSet = NULL, const MColor * defaultUnsetColor = NULL);
	MStatus		getFaceVertexColors(MColorArray &colors,  const MString *colorSet = NULL, const MColor * defaultUnsetColor = NULL);
	MStatus		getFaceVertexColorIndex(int faceIndex, int localVertexIndex,
										int &colorIndex, const MString * colorSet = NULL);
	MStatus 	setColors( const MColorArray& colors, const MString* colorSet = NULL );
	MStatus 	setColors( const MColorArray& colors, const MString* colorSet, MColorRepresentation rep );
	MStatus 	setSomeColors( const  MIntArray& colorIds, const MColorArray& colors,
								const MString* colorSet = NULL );
	MStatus 	setSomeColors( const  MIntArray& colorIds, const MColorArray& colors,
				       const MString* colorSet, MColorRepresentation rep );
	MStatus 	getColors( MColorArray& colors, const MString * colorSet = NULL , const MColor *defaultUnsetColor = NULL) const;
	MStatus		getColorIndex(int faceIndex, int localVertexIndex,
							  int &colorIndex, const MString *colorSet = NULL ) ;
	MStatus		setColor( int colorId, const MColor &color,
					   const MString * colorSet = NULL );
	MStatus		setColor( int colorId, const MColor &color,
				  const MString * colorSet,
				  MColorRepresentation rep );
	MStatus		getColor( int colorId, MColor &color,
				  const MString * colorSet = NULL,
				  const MColor * defaultUnsetColor = NULL) const;
	MStatus 	assignColor( int polygonId, int vertexIndex, int colorId,
						  const MString* colorSet = NULL );
	MStatus		assignColors( const MIntArray& colorIds,
						   const MString* colorSet = NULL );
	MStatus		clearColors( const MString* colorSet = NULL );
	bool		displayColors(MStatus* ReturnStatus = NULL) const;
	MStatus		setDisplayColors(bool enable);


	// Polygon Hole Query Method
	int			getHoles( MIntArray& holeInfoArray,
						  MIntArray& holeVertexArray,
						  MStatus * ReturnStatus = NULL );

	bool		onBoundary( int polygonId, MStatus * ReturnStatus = NULL) const;

	// Blind Data Query Methods
	bool		isBlindDataTypeUsed( int blindDataId,
								   MStatus * ReturnStatus = NULL ) const;
	MStatus		createBlindDataType( int blindDataId,
									 MStringArray longNames,
									 MStringArray shortNames,
									 MStringArray formatNames);
	bool		hasBlindDataComponentId( int compID,
							  MFn::Type compType,
							  int blindDataId,
							  MStatus * ReturnStatus = NULL ) const;
	bool		hasBlindData( MFn::Type compType,
							  int blindDataId,
							  MStatus * ReturnStatus = NULL ) const;
	bool		hasBlindDataComponentId( int compID,
							  MFn::Type compType,
							  MStatus * ReturnStatus = NULL ) const;
	bool		hasBlindData( MFn::Type compType,
							  MStatus * ReturnStatus = NULL ) const;
	// Get Blind Data Methods.
	MStatus		getBlindDataTypes(MFn::Type compType,
							MIntArray& blindDataIds ) const;

	MStatus		getBlindDataAttrNames(int blindDataId,
							 MStringArray& longNames,
							 MStringArray& shortNames,
							 MStringArray& formatNames) const;

	MStatus		getFaceVertexBlindDataIndex(int faceIndex,
							  int vertexIndex,
							  int &blindDataIndex) const;
    MStatus		getBlindDataFaceVertexIndices(int blindDataIndex,
							int &faceIndex,
							int &vertexIndex) const;
	MStatus		getIntBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  int &data) const;
	MStatus		getIntBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MIntArray& data) const;
	MStatus		getFloatBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  float &data) const;
	MStatus		getFloatBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MFloatArray& data) const;
	MStatus		getDoubleBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  double &data) const;
	MStatus		getDoubleBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MDoubleArray& data) const;
	MStatus		getBoolBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  bool &data) const;
	MStatus		getBoolBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MIntArray& data) const;
	MString		stringBlindDataComponentId( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  MStatus *ReturnStatus = NULL) const;
	MStatus		getStringBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MStringArray& data) const;
	MString		binaryBlindDataComponentId( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  MStatus *ReturnStatus = NULL) const;
	MStatus		getBinaryBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MStringArray& data) const;

	// Set Blind Data Methods
	MStatus		setIntBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  int data);
	MStatus		setIntBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  int data);
	MStatus		setIntBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MIntArray& data);
	MStatus		setFloatBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  float data);
	MStatus		setFloatBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  float data);
	MStatus		setFloatBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MFloatArray &data);
	MStatus		setDoubleBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  double data);
	MStatus		setDoubleBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  double data);
	MStatus		setDoubleBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MDoubleArray &data);
	MStatus		setBoolBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  bool data);
	MStatus		setBoolBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  bool data);
	MStatus		setBoolBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MIntArray& data);
	MStatus		setStringBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MString data);
	MStatus		setStringBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MString data);
	MStatus		setStringBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MStringArray &data);
	MStatus		setBinaryBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MString data);
	MStatus		setBinaryBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MString data);
	MStatus		setBinaryBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MStringArray &data);
    MStatus     setBinaryBlindData( int compID, MFn::Type compType,
                              int blindDataId,
                              MString attrName,
                              char *data, int length);
	MStatus     clearBlindData(int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName);
	MStatus     clearBlindData(MFn::Type compType,
							  int blindDataId,
							  MString attrName);

	MStatus		clearBlindData(MFn::Type compType);
	MStatus     clearBlindData(int compID,
							  MFn::Type compType,
							  int blindDataId);
	MStatus     clearBlindData(MFn::Type compType,
							  int blindDataId);

	static MString	 componentTypeName	  ( MFn::Type compType,
											MStatus&  status );
	static MFn::Type componentTypeFromName( const MString& assocName );

	void	setCheckSamePointTwice( bool check = true );
	bool	getCheckSamePointTwice( void );

	MStatus 	createInPlace( int numVertices,
			int numPolygons,
			const MFloatPointArray &vertexArray,
			const MIntArray &polygonCounts,
			const MIntArray &polygonConnects );

	MStatus		copyInPlace( const MObject& source );

	// Triangulation Methods
	static MStatus polyTriangulate(const float* pointsAndHoles, unsigned int pointsCount,
			unsigned int outerPointsCount, unsigned int holesCount, bool considerNormal,
			const float* normals, unsigned short* triangles, int& trianglesCount);

	static MStatus polyTriangulate(const MFloatPointArray& points, const MIntArray& holes,
			unsigned int outerPointsCount, const MFloatVectorArray& normals, MIntArray& triangles);

	//	Obsolete: This method was added to provide backwards compatability.
	OPENMAYA_DEPRECATED(2019, "Use createColorSetWithName(const MString &, MDGModifier *, const MUintArray *, MStatus *)")
	MString		createColorSetWithName(const MString &colorSetName,
									   MDGModifier *modifier,
									   MStatus *ReturnStatus);

	//	Obsolete: This method was added to provide backwards compatability.
	OPENMAYA_DEPRECATED(2019, "Use generateSmoothMesh(MObject, MMeshSmoothOptions * , MStatus *) instead.")
	MObject		generateSmoothMesh(MObject parentOrOwner, MStatus* ReturnStatus);


BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnMesh, MFnDagNode );

	//	Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use MFnMesh::currentColorSetName(int, MStatus*) instead.")
	MString		currentColorSetName(MStatus * ReturnStatus) const;

    // Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use the other MFnMesh::addPolygon method.")
    MObject     addPolygon( MPointArray & vertexArray,
                        	double 		polyTolerance,
                        	bool     	mergeVertices = true,
                        	double 		pointTolerance = kMFnMeshPointTolerance,
                        	bool     	forceGeometry = true,
							MObject 	parentOrOwner = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );

	// Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use the other MFnMesh::intersect method.")
	bool     	intersect( MPoint & raySource, MVector & rayDirection,
							MPointArray & points,
							MStatus * ReturnStatus = NULL ) const;

	OPENMAYA_DEPRECATED(2019, "Use the other MFnMesh::intersect method.")
	// Obsolete & no script support
	bool     	intersect( const MPoint & raySource,
                            const MVector & rayDirection,
							MPointArray & points,
							MStatus * ReturnStatus = NULL ) const;

	// Obsolete & no script support
	MStatus		getBlindDataAttrNames(int blindDataId,
							 MStringArray& longNames,
							 MStringArray& shortNames) const;

	// Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use MFnMesh::hasBlindDataComponentId instead.")
	bool		hasBlindData( int compID,
							  MFn::Type compType,
							  int blindDataId,
							  MStatus * ReturnStatus = NULL ) const;
	// Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use MFnMesh::hasBlindDataComponentId instead.")
	bool		hasBlindData( int compID,
							  MFn::Type compType,
							  MStatus * ReturnStatus = NULL ) const;

	// No script support
	MStatus		createUVSet(MString &uvSetName,
							MDGModifier *modifier = NULL,
							const MUintArray *instances = NULL);

	MStatus		createUVSetDataMesh(MString &uvSetName);
	// No script support
	MStatus		copyUVSet(const MString &fromName, MString &toName,
						  MDGModifier *modifier = NULL);
	// No script support
	MStatus		getCurrentUVSetName(MString &setName,
									int instance = kMFnMeshInstanceUnspecified) const;
	// No script support
	MStatus		createColorSet(MString &colorSetName,
							   MDGModifier *modifier = NULL,
							   const MUintArray *instances = NULL);
	// No script support
	MStatus		createColorSet(MString &colorSetName,
							   MDGModifier *modifier,
							   bool clamped,
							   MColorRepresentation rep,
							   const MUintArray *instances = NULL);
	// No script support
	MStatus		getCurrentColorSetName(MString &setName,
									   int instance = kMFnMeshInstanceUnspecified) const;
	//	Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use MFnMesh::stringBlindDataComponentId instead.")
	MStatus		getStringBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  MString &data) const;
	// Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use MFnMesh::binaryBlindDataComponentId instead.")
	MStatus		getBinaryBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  MString &data) const;

	//----------------------------------------------------------------------
	//
	// Metadata Associations Methods (eventual replacement for blind data)
	//
	virtual	const adsk::Data::Associations*	metadata	( MStatus* ReturnStatus=NULL ) const;
	virtual	MStatus							setMetadata	( const adsk::Data::Associations& );
	//
    // Simple validation provided to see if the mesh data is in sync with the
    // metadata channel (e.g. number of vertexes equals number of entries in
	// the per-vertex channel)
    //
    virtual	MStatus 	validateMetadata( MString& errors )	const;
	//
	//----------------------------------------------------------------------

END_NO_SCRIPT_SUPPORT:

protected:
	MObject	createMObject(MObject & parentOrOwner,
    						  TmidBody * body,
    						  MStatus * ReturnStatus);

	virtual bool	objectChanged	(MFn::Type tp, MStatus * ReturnStatus);

    bool	getUVSetIndex			( const MString * uvSetName, int & uvSet) const;
	bool	getColorSetIndex		( const MString * colorSetName, int &colorSet ) const;
	bool	getMeshForIntersection	( MSpace::Space, MFloatPoint&, MFloatVector&, void*&, MStatus* );

public:
	MStatus		createColorSetDataMesh( MString &colorSetName,
										bool clamped = false,
										MColorRepresentation rep = kRGBA );
	MString		createColorSetWithNameDataMesh( const MString &colorSetName,
												bool clamped,
												MColorRepresentation rep,
												MStatus *ReturnStatus = NULL );

private:
// Private members
	void *f_meshRef;
	bool f_checkSamePoint;

};

/*!
	Class to define parameters for accelerated intersection operations.
	Use MFnMesh::uniformGridParams() or MFnMesh::autoUniformGridParams()
	to create one of these to pass into the allIntersections(),
	closestIntersection(), and anyIntersection() methods
*/
class OPENMAYA_EXPORT MMeshIsectAccelParams
{
public:
		MMeshIsectAccelParams();

private:

		int type;
		int xDiv;
		int yDiv;
		int zDiv;
		bool verbose;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MFnMesh */
