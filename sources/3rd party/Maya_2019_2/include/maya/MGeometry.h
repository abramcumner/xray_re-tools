#ifndef _MGeometry
#define _MGeometry
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
// CLASS:    MGeometryLegacy
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MGeometryData.h>
#include <maya/MGeometryPrimitive.h>
#include <maya/MString.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MGeometryLegacy)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
\deprecated
Use MHWRender::MGeometry and MHWRender::MGeometryExtractor instead.

MGeometryLegacy stores the collection of MGeometryData arrays which describe
a Maya surface, including per-component data such as UV mapping and colour.

Various methods are provided for returning MGeometryData containing the
data for different properties of the geometry: the position() method
returns position data, the normal() method returns normals, and so on.

The primitiveArray() method returns MGeometryPrimitive's which provide
indexing into those property arrays for each primitive to be drawn.
*/
class OPENMAYARENDER_EXPORT MGeometryLegacy
{
public:
	~MGeometryLegacy();
	MGeometryLegacy( const MGeometryLegacy & geom);
	MGeometryLegacy& operator = (const MGeometryLegacy &rhs);
	unsigned int primitiveArrayCount() const;
	const MGeometryPrimitive primitiveArray(unsigned int arrayNumber) const;
	const MGeometryData position();
	const MGeometryData normal();
	const MGeometryData componentId();
	const MGeometryData texCoord( const MString& name);
	const MGeometryData color( const MString& name);
	const MGeometryData tangent( const MString& name);
	const MGeometryData binormal( const MString& name);
	const MGeometryData data( MGeometryData::DataType what, const MString &name );

protected:
	// Default constructor is protected
	MGeometryLegacy();
	MGeometryLegacy( void *_pGeometry, void * _pIndexing );
	void set( void *_pGeometry, void * _pIndexing );

	void *_pGeometry;
	MGeometryPrimitive _pIndexing;

private:
// No private members

	friend class MGeometryManager;
	friend class MGeometryList;
	friend class MVaryingParameter;

};
OPENMAYA_NAMESPACE_CLOSE

#endif
#endif // _MGeometry
