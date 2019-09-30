#ifndef _MDynSweptTriangle
#define _MDynSweptTriangle
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
// CLASS:    MDynSweptTriangle
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDynSweptTriangle)

//! \ingroup OpenMayaFX
//! \brief Class for evaluating surfaces as triangles over time. 
/*!
    MDynSweptTriangle provides access to surfaces (Nurbs and Polys)
	defined as triangles.  It can only be accessed with the
	MFnDynSweptGeometryData class that is provided as an output
	from the geoConnector node.

    The class provides parametric time evaluation for a triangle.
    Time is in the range 0 to 1, where 1 represents the current frame and 0
    represents the previous frame. In this way you can get interpolated
    values of a triangle in motion.
*/
class OPENMAYAFX_EXPORT MDynSweptTriangle
{
public:
					MDynSweptTriangle();
					~MDynSweptTriangle();

	// Time-Dependent Access Methods
	//
	//
	MVector			vertex( int vertexId, double t = 1.0 );

	MVector			normal( double t = 1.0 );

	MVector 		normalToPoint(double x, double y, double z, double t = 1.0);

	// Non-Time-Dependent Access Method
	//
	//
	MVector			uvPoint( int vertexId );

	double			area();

	static const char* className();

protected:
// No protected members

private:
	MDynSweptTriangle( void* );
	void * fData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDynSweptTriangle */
