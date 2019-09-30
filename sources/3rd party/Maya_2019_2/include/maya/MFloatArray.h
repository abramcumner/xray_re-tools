#ifndef _MFloatArray
#define _MFloatArray
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
// CLASS:    MFloatArray
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
// CLASS DECLARATION (MFloatArray)

//! \ingroup OpenMaya
//! \brief Array of floats data type.
/*!
This class implements an array of floats.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.

Methods that query the array are threadsafe. Writing to different
array elements at the same time with the set() method is
threadsafe. Methods that modify the array length, namely append(),
insert(), remove() and setLength() are not threadsafe.

*/
class OPENMAYA_EXPORT MFloatArray
{
public:
	typedef MArrayIteratorTemplate<MFloatArray, float> Iterator;
	typedef MArrayConstIteratorTemplate<MFloatArray, float> ConstIterator;

					MFloatArray();
					MFloatArray( const MFloatArray& other );
					MFloatArray( const float src[], unsigned int count );
					MFloatArray( const double src[], unsigned int count );
					MFloatArray( unsigned int initialSize,
								 float initialValue = 0 );
					~MFloatArray();
 	MFloatArray &   operator=( const MFloatArray & other );
	MStatus			set( float element, unsigned int index );
	MStatus			set( double element, unsigned int index );
	MStatus			setLength( unsigned int length );
 	unsigned int        length() const;
 	MStatus         remove( unsigned int index );
 	MStatus         insert( float element, unsigned int index );
 	MStatus         append( float element );
 	MStatus         copy( const MFloatArray& source );
 	MStatus		 	clear();
	MStatus			get( float[] ) const;
	MStatus			get( double[] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	float			operator[]( unsigned int index ) const;

	//!	NO SCRIPT SUPPORT
 	float &	 	    operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MFloatArray &array);
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
	swigTPythonObjectConverter(float);
	swigExtendAPIArray(MFloatArray,float);
#endif

protected:
// No protected members

private:
	MFloatArray( void* );
 	void * arr;
	const float* debugPeekValue;
	bool   own;
	void			syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFloatArray */
