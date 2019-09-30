#ifndef _MVectorArray
#define _MVectorArray
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
// CLASS:    MVectorArray
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MVectorArray)
//
//  This class implements an array of integers.  Common convenience functions
//  are available, and the implementation is compatible with the internal
//  Maya implementation so that it can be passed efficiently between plugins
//  and internal maya data structures.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>

#include <maya/MVector.h>
#include <maya/MStatus.h>
#include <maya/MArrayIteratorTemplate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MVectorArray)

//! \ingroup OpenMaya
//! \brief Array of MVectors data type. 
/*!
This class implements an array of MVectors.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MVectorArray
{
public:
	typedef MArrayIteratorTemplate<MVectorArray, MVector> Iterator;
	typedef MArrayConstIteratorTemplate<MVectorArray, MVector> ConstIterator;

					MVectorArray();
					MVectorArray( const MVectorArray& other );
BEGIN_NO_SCRIPT_SUPPORT:
	//!	NO SCRIPT SUPPORT
					MVectorArray( const MVector vectors[], unsigned int count );
END_NO_SCRIPT_SUPPORT:
					MVectorArray( const double vectors[][3], unsigned int count );
					MVectorArray( const float vectors[][3], unsigned int count );
					MVectorArray( unsigned int initialSize,
								  const MVector &initialValue
								  = MVector::zero );
					~MVectorArray();
 	const MVector&	operator[]( unsigned int index ) const;
 	MVectorArray &  operator=( const MVectorArray & other );
	MStatus			set( const MVector& element, unsigned int index );
	MStatus			set( double element[3], unsigned int index );
	MStatus			set( float element[3], unsigned int index );
	MStatus			setLength( unsigned int length );
	unsigned int		length() const;
	MStatus			remove( unsigned int index );
	MStatus			insert( const MVector & element, unsigned int index );
	MStatus			append( const MVector & element );
 	MStatus         copy( const MVectorArray& source );
	MStatus			clear();
	MStatus			get( double [][3] ) const;
	MStatus			get( float [][3] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	MVector &		operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MVectorArray &array);
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
	MVectorArray( void* );
	void * arr;
	struct api_data
	{
		double  x;
		double  y;
		double  z;
	};
	const api_data* debugPeekValue ;
    bool   own;
	void syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MVectorArray */
