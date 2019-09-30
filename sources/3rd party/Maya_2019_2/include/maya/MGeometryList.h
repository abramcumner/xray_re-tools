#ifndef _MGeometryList
#define _MGeometryList
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
// CLASS:    MGeometryList
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MGeometry.h>
#include <maya/MDagPath.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MGeometryList)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
  \deprecated
  Use MHWRender::MRenderItemList instead.

  This class holds the set of data elements which represent a Maya
  surface. It provides iterated access to a list of geometry items,
  along with the rendering context require to render them
  (e.g. matrix, etc).
*/
class OPENMAYARENDER_EXPORT MGeometryList
{
public:
	MGeometryList( const MRenderingInfo& info, int initialCapacity = 1);

	~MGeometryList();

	// Iteration

	bool			isDone() const;

	void			next();

	void			reset();

	// Random access

	int				length() const;

	void			setCurrentElement( int i);

	// Query the current element

	//! Bit flags for the geometry method that govern which OpenGL state Maya sets for you
	enum
	{
        //! No renderer setup is required for this element
		kNone						= 0,
        //! Setup the model view matrix for this element
		kMatrices					= 1 << 0,
        //! Setup the fixed function lighting state for this element
		kFixedFunctionLighting		= 1 << 1,
        //! Setup the back/front face culling state for this element
		kCulling					= 1 << 2,
        //! Setup all renderer state for this element
		kAll						= -1
	} MSetupFlags;

	MGeometryLegacy&		geometry( int setupFlags = kAll);

	const MMatrix&	objectToWorldMatrix() const;

	const MMatrix&	projectionMatrix() const;

	const MMatrix&	viewMatrix() const;

	MDagPath		path() const;

	//! Defines the culling modes to use when rendering this geometry
	typedef enum
	{
		//! No culling should be performed on this geometry
		kCullNone,
		//! Cull clockwise faces when rendering this geometry
		kCullCW,
		//! Cull counter-clockwise faces when rendering this geometry
		kCullCCW
	} MCullMode;

	MGeometryList::MCullMode cullMode() const;


	// Build a list of geometry to render

	bool			addLast( const MDagPath& path, const MObject* components = NULL);

	bool			addLast( const MGeometryLegacy& geometry, const MMatrix& matrix);

protected:

	MGeometryList( void* pGeometry, int count, const void* info);

private:

	MGeometryLegacy		_geometry;
	void*			_geometryRequests;
	int				_count;
	int				_capacity;
	int				_currentElement;
	const void*		_info;
	int				_ownership;

	void			setCapacity( int capacity);

	friend class MUniformParameter;
	friend class MGeometryManager;
};
OPENMAYA_NAMESPACE_CLOSE

#endif
#endif // _MGeometryList
