#ifndef _MPxGeometryIterator
#define _MPxGeometryIterator
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
// CLASS:    MPxGeometryIterator
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>

// ****************************************************************************
// DECLARATIONS

class TcomponentList;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxGeometryIterator)

//! \ingroup OpenMaya MPx
//! \brief Base class for user defined geometry iterators. 
/*!
  This is the base class for user defined geometry iterators.
  Geometry iterator allow iterating over components in geometry
  in a geometry independent manner. This base class defines
  the interface to be used by maya when a generic component iteration
  is required.

  This class is used in conjunction with MPxSurfaceShape to provide
  an iterator for components in a user defined shape. Also this method
  can is used by MPxGeometryData to provide an iterator over geometry
  that is passed through DG connections.

  The main methods to override in this class are point and setPoint.
  The reset, isDone, and next methods have a default implementation
  and should only be overridden if the component being iterator on
  is not a single indexed component type.

  The iterator works in two modes, over components or over all elements
  in some geometry. If the components passed into the constructors are
  null or empty then the iteration is meant to be over the entire object.
*/
class OPENMAYA_EXPORT MPxGeometryIterator
{
public:
	MPxGeometryIterator( void * userGeometry, MObjectArray & components );
	MPxGeometryIterator( void * userGeometry, MObject & components );
	virtual ~MPxGeometryIterator();

	// ************************************************************
	// Methods to overload

	// Item by item traversal methods
	//
	virtual bool			isDone() const;
	virtual void			next();
	virtual void			reset();

	// component for Item
	virtual void			component( MObject &component );

	// Item's point info access methods
	//
	virtual bool			hasPoints() const;
	virtual int				iteratorCount() const;
	virtual MPoint			point() const;
	virtual void			setPoint( const MPoint & ) const;
	virtual int				setPointGetNext( MPoint & );
	virtual int				index() const;


	virtual bool			hasNormals() const;

	virtual int				indexUnsimplified() const;

	// ************************************************************

	// Methods to access the iterator values.
	// These values are used when iterating over all components
	// of a shape when there are no components specified.
	int						currentPoint() const;
	void					setCurrentPoint( int );
	int						maxPoints() const;
	void					setMaxPoints( int );



	//	Method to (optionally) set object (to allow tweaking of history)
	//
	void					setObject( MPxSurfaceShape & );

	void*					geometry() const;

	static const char*	    className();

protected:

private:
	void * instance;
	TcomponentList			*compList;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxGeometryIterator */
