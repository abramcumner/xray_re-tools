#ifndef _MRenderLineArray
#define _MRenderLineArray
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
// CLASS:    MRenderLineArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS


// ****************************************************************************
// CLASS DECLARATION (MRenderLineArray)

//! \ingroup OpenMayaFX
//! \brief Class for accessing paint effects output curve data.
/*!
	A MRenderLineArray provides access to paint effects curve rendering info.
	This class contains arrays for per vertex attributes along an individual curve:
	line: the vertices in worldspace
    twist: a twist vector at each vertex
    width: the tube widths
    flatness: the tube flatness along the twist vector direction
    parameter: the u parameterization value for each vertex
    color: color rgb value for each vertex
    incandescence: incandescence rgb value for each vertex
    transpareancy: transparency rgb value for each vertex
*/
class OPENMAYAFX_EXPORT MRenderLineArray
{
public:
	MRenderLineArray();
	~MRenderLineArray();

	MRenderLineArray&  operator = (const MRenderLineArray& other );
	void deleteArray();

	// Access Methods
	int	  length() const;
	MRenderLine renderLine( unsigned int index, MStatus *status ) const;

	static const char* className();

protected:
// No protected members

private:
	MRenderLineArray( void* );
	void * fData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderLineArray */
