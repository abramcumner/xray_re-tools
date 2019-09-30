#ifndef _MDynSweptLine
#define _MDynSweptLine
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
// CLASS:    MDynSweptLine
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDynSweptLine)

//! \ingroup OpenMayaFX
//! \brief Class for evaluating curve segments as lines over time. 
/*!
	A MDynSweptLine provides access to a curve segment defined as a line.
	It can only be accessed with the MFnDynSweptGeometryData class
	that is provided as an output from the geoConnector node.

	The class provides parametric time evaluation for a curve.
	Time is in the range 0 to 1, where 1 represents the current frame and 0
	represents the previous frame. In this way you can get interpolated
	values of a curve in motion.
*/
class OPENMAYAFX_EXPORT MDynSweptLine
{
public:
					MDynSweptLine();
					~MDynSweptLine();

	// Time-Dependent Access Methods
	//
	MVector			vertex( int vertexId, double t = 1.0 );
	MVector			normal( double x, double y, double z, double t = 1.0 );
	MVector			tangent( double t = 1.0 );
	double			length( double t = 1.0 );

	static const char* className();

protected:
// No protected members

private:
	MDynSweptLine( void* );
	void * fData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDynSweptLine */
