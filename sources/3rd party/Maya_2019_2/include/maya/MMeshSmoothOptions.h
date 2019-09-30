#ifndef _MMeshSmoothOptions
#define _MMeshSmoothOptions
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
// CLASS:    MMeshSmoothOptions
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MMeshSmoothOptions)

//! \ingroup OpenMaya
//! \brief Options for control of smooth mesh generation. 
/*!
	The MMeshSmoothOptions class  encapsulates settings that control the
	generation of smoothed version of a mesh. Meant for use with
	MFnMesh::generateSmoothMesh().
*/
class OPENMAYA_EXPORT MMeshSmoothOptions
{
public:
	MMeshSmoothOptions(void);
	virtual ~MMeshSmoothOptions(void);

	//! Specifies subdivision algorithm to use for mesh smoothing. 
    enum SubdivisionType {
		kInvalidSubdivision = -1,			//!< Invalid value.
        kCatmullClark = 0,					//!< Maya Catmull-Clark.
        kOpenSubdivCatmullClarkUniform = 2,	//!< OpenSubdiv Uniform.
        kOpenSubdivCatmullClarkAdaptive,	//!< OpenSubdiv Adaptive.
		kLastSubdivision,					//!< Last enum value.
    };

	//! Specifies how to apply boundary creasing to subd poly object. 
	enum BoundaryRule { 
		kInvalid = -1, //!< Invalid value. 
		kLegacy = 0,   //!< Uses the legacy algorithm, which performed no creasing
		kCreaseAll,  //!< Crease boundary edges & vertices with valence of exactly 2. 
		kCreaseEdge, //!< Crease boundary edges only. 
		kLast		 //!< Last enum value. 
	}; 

	//! OpenSubdiv vertex boundary rules.
	enum OpenSubdivVertexBoundary {
		kInvalidBoundary = -1, //!< Invalid value.
		kNone = 0, //!< None
		kSharpEdgesAndCorners, //!< Don't smooth edges & corners.
		kSharpEdges, //!< Don't smooth edges.
		kAlwaysSharp, //!< Use legacy boundary rules.
		kLastBoundary  //!< Last enum value.
	};

	//! OpenSubdiv crease method.
	enum OpenSubdivCreaseMethod {
		kInvalidCreaseMethod = -1, //!< Invalid value.
		kNormal = 0, //!< Normal crease method.
		kChaikin, //!< Chaikin crease method.
		kLastCreaseMethod, //!< Last enum value.
	};

	void	setDivisions( int numDivisions, MStatus* ReturnStatus=NULL );
	int		divisions( MStatus* ReturnStatus=NULL ) const;
	void	setSmoothness( float smoothness, MStatus* ReturnStatus=NULL );
	float	smoothness( MStatus* ReturnStatus=NULL ) const;
	void	setSmoothUVs( bool smoothUVs, MStatus* ReturnStatus=NULL );
	bool	smoothUVs( MStatus* ReturnStatus=NULL ) const;
	void	setPropEdgeHardness( bool propEdgeHardness, MStatus* ReturnStatus=NULL );
	bool	propEdgeHardness( MStatus* ReturnStatus=NULL ) const;
	void	setKeepBorderEdge( bool keepBorderEdge, MStatus* ReturnStatus=NULL );
	bool	keepBorderEdge( MStatus* ReturnStatus=NULL ) const;
	void	setKeepHardEdge( bool keepHardEdge, MStatus* ReturnStatus=NULL );
	bool	keepHardEdge( MStatus* ReturnStatus=NULL ) const;
	void    setBoundaryRule( BoundaryRule rule, MStatus * ReturnStatus = NULL ); 
	BoundaryRule boundaryRule( MStatus* ReturnStatus = NULL ) const; 
	void	setOpenSubdivVertexBoundary( OpenSubdivVertexBoundary rule, MStatus* ReturnStatus = NULL );
	OpenSubdivVertexBoundary openSubdivVertexBoundary( MStatus* ReturnStatus = NULL ) const;
	void	setOpenSubdivFaceVaryingBoundary(OpenSubdivVertexBoundary smoothing, MStatus* ReturnStatus = NULL );
	OpenSubdivVertexBoundary openSubdivFaceVaryingBoundary( MStatus* ReturnStatus = NULL ) const;
	void	setOpenSubdivSmoothTriangles( bool smoothTriangles, MStatus* ReturnStatus = NULL );
	bool	openSubdivSmoothTriangles( MStatus* ReturnStatus = NULL );
	void	setOpenSubdivCreaseMethod(OpenSubdivCreaseMethod method, MStatus* ReturnStatus = NULL );
	OpenSubdivCreaseMethod openSubdivCreaseMethod( MStatus* ReturnStatus = NULL ) const;
	void    setSubdivisionType( SubdivisionType subdivision, MStatus * ReturnStatus = NULL ); 
	SubdivisionType subdivisionType( MStatus* ReturnStatus = NULL ) const; 

	static const char* className();

protected:
// No protected members
private:
	friend class MFnMesh;
	void *instance;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MMeshIntersector */
