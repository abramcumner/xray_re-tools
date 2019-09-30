#ifndef _MInt64Array
#define _MInt64Array
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
// CLASS:    MInt64Array
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
// CLASS DECLARATION (MInt64Array)

//! \ingroup OpenMaya
//! \brief Array of 64-bit integers data type. 
/*!
This class implements an array of 64-bit integers.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MInt64Array
{
public:
	typedef MArrayIteratorTemplate<MInt64Array, MInt64> Iterator;
	typedef MArrayConstIteratorTemplate<MInt64Array, MInt64> ConstIterator;

					MInt64Array();
					MInt64Array( const MInt64Array& other );
					MInt64Array( const MInt64 src[], unsigned int count );
					MInt64Array( unsigned int initialSize,
							   MInt64 initialValue = 0 );
					~MInt64Array();
 	MInt64Array &	operator=( const MInt64Array & other );
	MStatus			set( MInt64 element, unsigned int index );
	MStatus			setLength( unsigned int length );
 	unsigned int	length() const;
 	MStatus			remove( unsigned int index );
 	MStatus			insert( MInt64 element, unsigned int index );
 	MStatus			append( MInt64 element );
 	MStatus         copy( const MInt64Array& source );
 	MStatus		 	clear();
	MStatus			get( MInt64[] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int	sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

 	//!	NO SCRIPT SUPPORT
 	MInt64&	 		operator[]( unsigned int index );
	//! NO SCRIPT SUPPORT
	MInt64			operator[]( unsigned int index ) const;

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MInt64Array &array);
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
	swigTPythonObjectConverter(MInt64);
	swigExtendAPIArray(MInt64Array,MInt64);
#endif

protected:
// No protected members

private:
	MInt64Array( void* );
	void* arr;
	const MInt64* debugPeekValue;
	bool   own;
	void	syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MInt64Array */
