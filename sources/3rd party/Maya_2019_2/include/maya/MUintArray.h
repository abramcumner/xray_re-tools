#ifndef _MUintArray
#define _MUintArray
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
// CLASS:    MUintArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MArrayIteratorTemplate.h>

typedef unsigned int MUint;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MUintArray)

//! \ingroup OpenMaya
//! \brief Array of integers data type. 
/*!
This class implements an array of unsigned integers.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MUintArray
{
public:
	typedef MArrayIteratorTemplate<MUintArray, unsigned int> Iterator;
	typedef MArrayConstIteratorTemplate<MUintArray, unsigned int> ConstIterator;

					MUintArray();
					MUintArray( const MUintArray& other );
					MUintArray( const unsigned int src[], unsigned int count );
					MUintArray( unsigned int initialSize,
							   unsigned int initialValue = 0 );
					~MUintArray();
 	MUintArray &     operator=( const MUintArray & other );
	MStatus			set( unsigned int element, unsigned int index );
	MStatus			setLength( unsigned int length );
 	unsigned int        length() const;
 	MStatus			remove( unsigned int index );
 	MStatus			insert( unsigned int element, unsigned int index );
 	MStatus			append( unsigned int element );
 	MStatus         copy( const MUintArray& source );
 	MStatus		 	clear();
	MStatus			get( unsigned int[] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

	//! NO SCRIPT SUPPORT
	unsigned int	operator[]( unsigned int index ) const;

	//!	NO SCRIPT SUPPORT
 	unsigned int&	 operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MUintArray &array);
#if !defined(SWIG)
	Iterator begin();
	Iterator end();

	ConstIterator begin() const;
	ConstIterator end() const;

	ConstIterator cbegin() const;
	ConstIterator cend() const;
#endif

END_NO_SCRIPT_SUPPORT:

#if defined(SWIG)
	swigTPythonObjectConverter(MUint);
	swigExtendAPIArray(MUintArray,MUint);
#endif

	static const char* className();

protected:
// No protected members

private:
	MUintArray( void* );
	friend class MPolyMessage;
 	void* arr;
	const unsigned int* debugPeekValue;
	bool   own;
  	void	syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MUintArray */
