#ifndef _MPxGeometryData
#define _MPxGeometryData
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
// CLASS:    MPxGeometryData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MPxData.h>
#include <maya/MMatrix.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxGeometryData)

//! \ingroup OpenMaya MPx
//! \brief Base Class for User-defined Dependency Graph Geometry Data Types. 
/*!

  This class is used to provide a wrapper or container for some
  arbitrary geometry type that you have defined. This allows your data
  type to exist as typed attributes on dependency nodes and to be
  passed through dependency graph connections.

  MP`xGeometryData is similar to MPxData but includes methods to
  support sets (also known as groups) and component iteration.

  For user defined shapes (MPxSurfaceShape derived classes), in order
  to support maya's deformers you must declare an MPxGeometryData
  class as well as a geometry iterator (MPxGeometryIterator).

  To register geometry data use MFnPlugin::registerData with the type
  argument equal to MPxData::kGeometryData.
*/
class OPENMAYA_EXPORT MPxGeometryData : public MPxData
{
public:
	MPxGeometryData();
	virtual ~MPxGeometryData();

	// Methods to provide a component iterator for the geometry
	//
	virtual MPxGeometryIterator* iterator( MObjectArray & componentList,
											MObject & component,
											bool useComponents);

	virtual MPxGeometryIterator* iterator( MObjectArray & componentList,
											MObject & component,
											bool useComponents,
											bool world) const;

	virtual bool	updateCompleteVertexGroup( MObject & component ) const;

	virtual bool	deleteComponent( const MObjectArray& compList );
	virtual bool	deleteComponentsFromGroups( const MObjectArray& compList,
												MIntArray& groupIdArray,
												MObjectArray& groupComponentArray );

	virtual bool	smartCopy( const MPxGeometryData *srcGeom );
	virtual	void			copy( const MPxData& src ) = 0;

	virtual MTypeId         typeId() const = 0;
	virtual MString         name() const = 0;

	void			setMatrix(const MMatrix &);
	const MMatrix		&matrix() const;
	bool			matrix(MMatrix &) const;

protected:
// No protected members

private:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxGeometryData */
