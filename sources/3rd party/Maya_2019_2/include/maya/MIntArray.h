#ifndef _MIntArray
#define _MIntArray
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
// CLASS:    MIntArray
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
// CLASS DECLARATION (MIntArray)

//! \ingroup OpenMaya
//! \brief Array of integers data type. 
/*!
This class implements an array of integers.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MIntArray
{
public:
	typedef MArrayIteratorTemplate<MIntArray, int> Iterator;
	typedef MArrayConstIteratorTemplate<MIntArray, int> ConstIterator;

					MIntArray();
					MIntArray( const MIntArray& other );
					MIntArray( const int src[], unsigned int count );
					MIntArray( unsigned int initialSize,
							   int initialValue = 0 );
					~MIntArray();
 	MIntArray &     operator=( const MIntArray & other );
	MStatus			set( int element, unsigned int index );
	MStatus			setLength( unsigned int length );
 	unsigned int        length() const;
 	MStatus			remove( unsigned int index );
 	MStatus			insert( int element, unsigned int index );
 	MStatus			append( int element );
 	MStatus         copy( const MIntArray& source );
 	MStatus		 	clear();
	MStatus			get( int[] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

 	//!	NO SCRIPT SUPPORT
 	int&	 		operator[]( unsigned int index );
	//! NO SCRIPT SUPPORT
	int				operator[]( unsigned int index ) const;

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MIntArray &array);

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
	swigTPythonObjectConverter(int);
	swigExtendAPIArray(MIntArray,int);
#endif

protected:
// No protected members

private:
	MIntArray( void* );
	void* arr;
	const int* debugPeekValue;
	bool   own;
	void	syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MIntArray */
