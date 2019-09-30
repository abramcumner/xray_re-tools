#ifndef _MPointArray
#define _MPointArray
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
// CLASS:    MPointArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MPoint.h>
#include <maya/MArrayIteratorTemplate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPointArray)

//! \ingroup OpenMaya
//! \brief Array of MPoint data type. 
/*!
This class implements an array of MPoints.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MPointArray
{
public:
	typedef MArrayIteratorTemplate<MPointArray, MPoint> Iterator;
	typedef MArrayConstIteratorTemplate<MPointArray, MPoint> ConstIterator;

					MPointArray();
					MPointArray( const MPointArray& other );
BEGIN_NO_SCRIPT_SUPPORT:
	//!	NO SCRIPT SUPPORT
					MPointArray( const MPoint points[], unsigned int count );
END_NO_SCRIPT_SUPPORT:
					MPointArray( const double points[][4], unsigned int count );
					MPointArray( const float points[][4], unsigned int count );
					MPointArray( unsigned int initialSize,
								 const MPoint &initialValue
								 = MPoint::origin );
					~MPointArray();
	const MPoint&	operator[]( unsigned int index ) const;
 	MPointArray &   operator=( const MPointArray & other );
	MStatus			set( const MPoint& element, unsigned int index);
	MStatus 		set( unsigned int index, double x, double y, double z=0,
						 double w=1);
	MStatus			set( double element[4], unsigned int index);
	MStatus			set( float element[4], unsigned int index);
	MStatus			setLength( unsigned int length );
	unsigned int		length() const;
	MStatus			remove( unsigned int index );
	MStatus			insert( const MPoint & element, unsigned int index );
	MStatus			append( const MPoint & element );
	MStatus 		append( double x, double y, double z=0, double w=1 );
 	MStatus         copy( const MPointArray& source );
	MStatus			clear();
	MStatus			get( double [][4] ) const;
	MStatus			get( float [][4] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	MPoint&	        operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MPointArray &array);
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
	MPointArray( void* );
	void * arr;
	struct api_data
	{
		double  x;
		double  y;
		double  z;
		double  w;

	};
	const api_data* debugPeekValue ;
	bool   own;
	void syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPointArray */
