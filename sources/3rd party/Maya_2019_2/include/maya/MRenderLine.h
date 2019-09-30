#ifndef _MRenderLine
#define _MRenderLine
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
// CLASS:    MRenderLine
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRenderLine)

//! \ingroup OpenMayaFX
//! \brief Class for accessing paint effects output curve data. 
/*!
	A MRenderLine provides access to paint effects curve rendering info.
	This class contains arrays for per vertex attributes along an individual curve:
	line: the vertices in worldspace
    twist: a twist vector at each vertice
    width: the tube widths
    flatness: the tube flatness along the twist vector direction
    parameter: the u parameterization value for each vertice
    color: color rgb value for each vertice
    incandescence: incandescence rgb value for each vertice
    transpareancy: transparency rgb value for each vertice
*/
class OPENMAYAFX_EXPORT MRenderLine
{
public:
	MRenderLine();
	~MRenderLine();

	MRenderLine&  operator = (const MRenderLine& other );

	// Access Methods
	MVectorArray getLine() const;
	MVectorArray getTwist() const;
	MDoubleArray getWidth() const;
	MDoubleArray getFlatness() const;
	MDoubleArray getParameter() const;
	MVectorArray getColor() const;
	MVectorArray getIncandescence() const;
	MVectorArray getTransparency() const;

	static const char* className();

protected:
// No protected members

private:
	MRenderLine( void* );
	void * fData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderLine */
