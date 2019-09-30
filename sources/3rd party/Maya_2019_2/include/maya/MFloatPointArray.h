#ifndef _MFloatPointArray
#define _MFloatPointArray
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
// CLASS:    MFloatPointArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MFloatPoint.h>
#include <maya/MArrayIteratorTemplate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFloatPointArray)

//! \ingroup OpenMaya
//! \brief Array of MFloatPoint data type. 
/*!
This class implements an array of MFloatPoints.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MFloatPointArray
{
public:
	typedef MArrayIteratorTemplate<MFloatPointArray, MFloatPoint> Iterator;
	typedef MArrayConstIteratorTemplate<MFloatPointArray, MFloatPoint> ConstIterator;

	MFloatPointArray();
	MFloatPointArray( const MFloatPointArray& other );

BEGIN_NO_SCRIPT_SUPPORT:
	//!	NO SCRIPT SUPPORT
	MFloatPointArray( const MFloatPoint points[], unsigned int count );
END_NO_SCRIPT_SUPPORT:

	MFloatPointArray( const float points[][4], unsigned int count );
	MFloatPointArray( const double points[][4], unsigned int count );
	MFloatPointArray( unsigned int initialSize,
					  const MFloatPoint &initialValue
					  = MFloatPoint::origin );
	~MFloatPointArray();

	const MFloatPoint&	operator[]( unsigned int index ) const;
 	MFloatPointArray &  operator=( const MFloatPointArray & other );
	MStatus			set( const MFloatPoint& element, unsigned int index);
	MStatus 		set( unsigned int index, float x, float y, float z=0,
						 float w=1);
	MStatus			set( float element[4], unsigned int index);
	MStatus			set( double element[4], unsigned int index);
	MStatus			setLength( unsigned int length );
	unsigned int		length() const;
	MStatus			remove( unsigned int index );
	MStatus			insert( const MFloatPoint & element, unsigned int index );
	MStatus			append( const MFloatPoint & element );
	MStatus 		append( float x, float y, float z=0, float w=1 );
 	MStatus         copy( const MFloatPointArray& source );
	MStatus			clear();
	MStatus			get( float [][4] ) const;
	MStatus			get( double [][4] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	MFloatPoint&	    operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MFloatPointArray &array);
#if !defined(SWIG)
	Iterator begin();
	Iterator end();

	ConstIterator begin() const;
	ConstIterator end() const;

	ConstIterator cbegin() const;
	ConstIterator cend() const;
#endif

END_NO_SCRIPT_SUPPORT:
	static const char* className();

protected:
// No protected members

private:
	MFloatPointArray( void* );
	void * arr;
	struct api_data
	{
		float  x;
		float  y;
		float  z;
		float  w;

	};
	const api_data* debugPeekValue ;
	bool   own;
	void syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFloatPointArray */
