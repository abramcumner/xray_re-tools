#ifndef _MDoubleArray
#define _MDoubleArray
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
// CLASS:    MDoubleArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MArrayIteratorTemplate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDoubleArray)

//! \ingroup OpenMaya
//! \brief Array of doubles data type. 
/*!
This class implements an array of doubles.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.

Methods that query the array are threadsafe. Writing to different
array elements at the same time with the set() method is
threadsafe. Methods that modify the array length, namely append(),
insert(), remove() and setLength() are not threadsafe.

*/
class OPENMAYA_EXPORT MDoubleArray
{
public:
	typedef MArrayIteratorTemplate<MDoubleArray, double> Iterator;
	typedef MArrayConstIteratorTemplate<MDoubleArray, double> ConstIterator;

					MDoubleArray();
					MDoubleArray( const MDoubleArray& other );
					MDoubleArray( const double src[], unsigned int count );
					MDoubleArray( const float src[], unsigned int count );
					MDoubleArray( unsigned int initialSize,
								  double initialValue = 0 );
					~MDoubleArray();
 	MDoubleArray &  operator=( const MDoubleArray & other );
	MStatus			set( double element, unsigned int index );
	MStatus			set( float element, unsigned int index );
	MStatus			setLength( unsigned int length );
 	unsigned int        length() const;
 	MStatus         remove( unsigned int index );
 	MStatus         insert( double element, unsigned int index );
 	MStatus         append( double element );
 	MStatus         copy( const MDoubleArray& source );
 	MStatus		 	clear();
	MStatus			get( double[] ) const;
	MStatus			get( float[] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	double &	 	operator[]( unsigned int index );
	//! NO SCRIPT SUPPORT
	double			operator[]( unsigned int index ) const;

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MDoubleArray &array);

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

#if defined(SWIG)
	swigTPythonObjectConverter(double);
	swigExtendAPIArray(MDoubleArray,double);
#endif

protected:
// No protected members

private:
	MDoubleArray( void* );
 	void * arr;
	const double* debugPeekValue;
	bool   own;
	void			syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDoubleArray */
