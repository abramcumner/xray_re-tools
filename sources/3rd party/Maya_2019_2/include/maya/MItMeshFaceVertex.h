#ifndef _MItMeshFaceVertex
#define _MItMeshFaceVertex
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
// CLASS:    MItMeshFaceVertex
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
// CLASS DECLARATION (MItMeshFaceVertex)

//! \ingroup OpenMaya
//! \brief Face vertex iterator. 
/*!
	This class is the iterator for face vertices on polygonal surfaces.

	The iterator functions in two modes depending on whether a component
	is specified. When a component is not given or is NULL the iteration
	will be over all polygons for the surface.  When a component is given
	this iterator will iterate over the polygons specified in the component.
	When iterating over components a DAG path to the surface must also be
	supplied.
*/
class OPENMAYA_EXPORT MItMeshFaceVertex
{
public:
    MItMeshFaceVertex( const MObject & polyObject, MStatus * ReturnStatus = NULL );
    MItMeshFaceVertex( const MDagPath &polyObject,
					MObject & component = MObject::kNullObj,
					MStatus * ReturnStatus = NULL );
	virtual ~MItMeshFaceVertex();
    bool        isDone( MStatus * ReturnStatus = NULL );
    MStatus     next();
    MStatus     reset();
    MStatus     reset( const MObject & polyObject );
    MStatus     reset( const MDagPath &polyObject,
						MObject & component = MObject::kNullObj );
    int         vertId( MStatus * ReturnStatus = NULL ) const;
    int         faceId( MStatus * ReturnStatus = NULL ) const;
    int         faceVertId( MStatus * ReturnStatus = NULL ) const;
	// Obsolete
	OPENMAYA_DEPRECATED(2019, "Use MItMeshFaceVertex::currentItem instead.")
	MObject		faceVertex(MStatus * ReturnStatus = NULL) const;
	MObject		currentItem(MStatus * ReturnStatus = NULL) const;

    MPoint      position( MSpace::Space space = MSpace::kObject,
						  MStatus * ReturnStatus = NULL );

    // Get/Set methods for normals.
    MStatus     getNormal( MVector & normal,
						   MSpace::Space space = MSpace::kObject ) const;
    int         normalId( MStatus * ReturnStatus = NULL ) const;

    int         tangentId( MStatus * ReturnStatus = NULL ) const;

    MVector     getTangent( MSpace::Space space = MSpace::kObject,
						    const MString * uvSet = NULL,
						    MStatus * ReturnStatus = NULL) const;

    MVector     getBinormal( MSpace::Space space = MSpace::kObject,
						     const MString * uvSet = NULL,
						     MStatus * ReturnStatus = NULL) const;

	// UV methods. Mention sharing in docs when setUVs
	bool		hasUVs( MStatus * ReturnStatus = NULL ) const;
    bool        hasUVs( const MString & uvSet,
						MStatus * ReturnStatus = NULL ) const;
    MStatus     getUV( float2 & uvPoint, const MString * uvSet = NULL) const;
    MStatus     getUVIndex( int & index, const MString *uvSet = NULL );
    MStatus     getUVIndex( int & index, float & u, float & v,
							const MString *uvSet = NULL);

	// Color methods. Mention sharing in docs when setUVs
	bool		hasColor(MStatus * ReturnStatus = NULL ) const;
	MStatus		getColor(MColor &color, const MString *colorSetName = NULL);
	MStatus		getColorIndex( int & colorIndex, const MString *colorSetName = NULL );

    MStatus     updateSurface();
    MStatus     geomChanged();
	MStatus		setIndex(int faceId, int vertFaceId,
					     int &prevFaceId, int &prevVertId );

    static const char*  className();

protected:
    bool		getUVSetIndex( const MString * uvSetName,
							   int & uvSet) const;

	bool		updateColorSet(const MString *colorSetName, int& prevSetId);

	void		resetColorSetToPrevious(const MString *colorSetName, int prevSetId);

private:
    MPtrBase *      f_shape;
    void 	 *      f_path;
    void 	 *      f_geom;
	void	 *		f_ref;
	void	 *		f_trans;

	void	 *		fCompIter;		// face vertex component iterator
	void     *      fElements;		// pointer to the vertex face component list

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItMeshFaceVertex */
