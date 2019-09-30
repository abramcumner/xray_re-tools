#ifndef _MFnSubdNames
#define _MFnSubdNames
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
// CLASS:    MFnSubdNames
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MFnSubdNames)
//
//	The names (ids) utilities for the hierarchical subdivision surfaces
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MDeprecate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnSubdNames)

//! \ingroup OpenMaya MFn
//! \brief Manipulate Subdivision Surface vertex, edge and face ids.
/*!
This function set provides some simple utilities for the interpretation
and manipulation of the subdivision surface vertex, edge and face ids.
*/
class OPENMAYA_EXPORT MFnSubdNames
{
public:
					MFnSubdNames();
					~MFnSubdNames();
	static int		base( MUint64 id );
	static int		first( MUint64 id );
	static int		level( MUint64 id );
	static int		path( MUint64 id );
	static int		corner( MUint64 id );

	static MStatus	fromMUint64( MUint64 id, int& base, int& first,
								 int& level, int& path, int& corner );
	static MStatus	toMUint64( MUint64& id, int base, int first,
								 int level, int path, int corner );

	static MUint64	baseFaceId( MUint64 id );
	// Obsolete
    OPENMAYA_DEPRECATED(2019, "Use MFnSubdNames::baseFaceIndexFromId instead.")
	static long		baseFaceIndex( MUint64 id );
	static unsigned int	baseFaceIndexFromId( MUint64 id );
	static MUint64	levelOneFaceId( MUint64 id );
	// Obsolete
    OPENMAYA_DEPRECATED(2019, "Use MFnSubdNames::levelOneFaceIdFromIndex instead.")
	static long		levelOneFaceAsLong( MUint64 id );
	static unsigned int	levelOneFaceIndexFromId( MUint64 id );

	// Obsolete
    OPENMAYA_DEPRECATED(2019, "Use MFnSubdNames::levelOneFaceIdFromIndex instead.")
	static MUint64	levelOneFaceIdFromLong( long one );
	static MUint64	levelOneFaceIdFromIndex( unsigned int index );

	// Obsolete
	OPENMAYA_DEPRECATED(2019, "Use MFnSubdNames::baseFaceIdFromIndex instead.")
	static MUint64	baseFaceIdFromLong( long base );
	static MUint64	baseFaceIdFromIndex( unsigned int index );

	static MUint64	parentFaceId( MUint64 id );

	static MStatus	nonBaseFaceVertices( MUint64 id,
										 MUint64& vertex1, MUint64& vertex2,
										 MUint64& vertex3, MUint64& vertex4 );
	static MStatus	nonBaseFaceEdges( MUint64 id,
									  MUint64& edge1, MUint64& edge2,
									  MUint64& edge3, MUint64& edge4);


	static MStatus	fromSelectionIndices( MUint64& id,
										  unsigned int firstIndex,
										  unsigned int secondIndex );

	static MStatus	toSelectionIndices( MUint64 id,
										unsigned int& firstIndex,
										unsigned int& secondIndex );

	static const char* className();

protected:
// No protected members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnSubd */
