#ifndef _MTesselationParams
#define _MTesselationParams
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
// CLASS:    MTesselationParams
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

// ****************************************************************************
// FORWARD DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MTesselationParams)

//! \ingroup OpenMaya
//! \brief Tesselation parameters. 
/*!
This class provides control over the tesselation operation.
This class is meant to be used in conjunction with the tesselate
method of MFnNurbsSurface.

The tesselation operation works by building an initial mesh and then
refining that mesh until certain subdivision criteria are met. This class
provides control over both the method of construction of the initial
mesh and also the criteria for subsequent refinement.
The initial tesselation is specified by the polygon output type, and the
isoparm type. The secondary tesselation (or subdivision) is specified by
the tesselation format.

<b>Output types</b>
The output type can be specified as either <b>kTriangles</b> or <b>kQuads</b>.
If triangles are specified then all the polygons in the initial mesh will be
triangles. If quads are specified then whenever possible the polygons
will be quads.

<b>Isoparm type</b>
The isoparm type specifies how the initial mesh is to be built.

	\li <b>kSurface3DDistance</b> A set of isoparams over entire surface,
					               a specified  3d-distance "udist" apart
	\li <b>kSurface3DEquiSpaced</b> A set of "unum" isoparams over entire
							         surface, equally spaced in 3d
	\li <b>kSurfaceEquiSpaced</b> A set of "unum" isoparams over entire
							       surface, equally spaced in u
	\li <b>kSpanEquiSpaced</b> A set of "unum" isoparams per surface
							    u-span, equally spaced in u


<b>Tesselation format</b>
The subdivision criteria for refinement of the initial mesh is specified
as one of three formats;

	\li <b>kTriangleCountFormat</b> the specified triangle count is used
									 as a rough estimate of the triangles
									 produced in the tesselation.
	\li <b>kGeneralFormat</b> specify which subdivisions types are to be
                               used in the tesselation. For each subdivision
							   type the corresponding parameters must be set.
	\li <b>kStandardFitFormat</b> Tessellate surface with standard fit ratios.
								The parameters for this type of refinement are
								StdChordHeightRatio, StdFractionalTolerance,
								StdMinEdgeLength, and Std3DDelta.


<b>Subdivision flags</b>
If the tesselation format is general then you can choose which criteria
you would like to use in the tesselation.  For each criterion, there is an
"use" flag (in other words, is this used?) together with the data for the
criterion.
*/
class OPENMAYA_EXPORT MTesselationParams
{
public:

	//! Tesselation formats.
	enum TessFormat {
		kTriangleCountFormat,	//!< \nop
		kStandardFitFormat,	//!< \nop
		kGeneralFormat		//!< \nop
	};

	//! Types of polygons to use in the result.
	enum PolyType {
		kTriangles,	//!< \nop
		kQuads		//!< \nop
	};

	//! Isoparm types.
	enum IsoparmType {
		kSurface3DDistance,	//!< \nop
		kSurface3DEquiSpaced,	//!< \nop
		kSurfaceEquiSpaced,	//!< \nop
		kSpanEquiSpaced		//!< \nop
	};

	//! Subdivision flags
	enum SubdivisionType {
		kUseFractionalTolerance,	//!< \nop
		kUseChordHeightRatio,		//!< \nop
		kUseMinEdgeLength,		//!< \nop
		kUseMaxEdgeLength,		//!< \nop
		kUseMaxNumberPolys,		//!< \nop
		kUseMaxSubdivisionLevel,	//!< \nop
		kUseMinScreenSize,		//!< \nop
		kUseMaxUVRectangleSize,		//!< \nop
		kUseTriangleEdgeSwapping,	//!< \nop
		kUseRelativeTolerance,		//!< \nop
		kUseEdgeSmooth,			//!< \nop
		kLastFlag			//!< \nop
	};


	MTesselationParams ( TessFormat format = kStandardFitFormat,
						PolyType = kTriangles );
	MTesselationParams ( const MTesselationParams & );
	~MTesselationParams ();


	// General parameters
	void		setFormatType( TessFormat type );
	void		setOutputType( PolyType type );


	//  Parameters for triangulization format
	void		setTriangleCount( int );


	//  Parameters for standard fit  format
	void		setStdChordHeightRatio( double );
	void		setStdFractionalTolerance( double );
	void		setStdMinEdgeLength( double );


	//  Parameters for general tesselation format
	void		setSubdivisionFlag( SubdivisionType type, bool use );
	void		setFitTolerance( double );
	void		setChordHeightRatio( double );
	void		setMinEdgeLength( double );
	void		setMaxEdgeLength( double );
	void		setMaxNumberPolys( int );
	void		setMaxSubdivisionLevel( double );
	void		setMinScreenSize( double, double );
	void		setWorldspaceToScreenTransform( MMatrix & );
	void		setMaxUVRectangleSize( double, double );
	void		setRelativeFitTolerance( double );
	void		setEdgeSmoothFactor( double );
	void		set3DDelta( double );


	// U,V isoparm based parameters
	void		setUIsoparmType( IsoparmType type );
	void		setVIsoparmType( IsoparmType type );
	void		setUNumber( int count );
	void		setVNumber( int count );
	void		setBoundingBoxDiagonal( double distance );
	void		setUDistanceFraction( double value );
	void		setVDistanceFraction( double value );

	MTesselationParams &operator= ( const MTesselationParams &rhs);

	static const char*	className();

	// The default tesselation parameters
	static	MTesselationParams 	fsDefaultTesselationParams;

protected:
// No protected members

private:
	friend class MFnNurbsSurface;
	void * data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MTesselationParams */
