#ifndef _MUint64Array
#define _MUint64Array
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
// CLASS:    MUint64Array
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
// CLASS DECLARATION (MUint64Array)

//! \ingroup OpenMaya
//! \brief Array of MUint64 data type. 
/*!
This class implements an array of MUint64s.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MUint64Array
{
public:
	typedef MArrayIteratorTemplate<MUint64Array, MUint64> Iterator;
	typedef MArrayConstIteratorTemplate<MUint64Array, MUint64> ConstIterator;

					MUint64Array();
					MUint64Array( const MUint64Array& other );
					MUint64Array( const MUint64 src[], unsigned int count );
					MUint64Array( unsigned int initialSize,
								  MUint64 initialValue = 0 );
					~MUint64Array();
 	MUint64Array &  operator=( const MUint64Array & other );
	MStatus			set( MUint64 element, unsigned int index );
	MStatus			setLength( unsigned int length );
 	unsigned int        length() const;
 	MStatus         remove( unsigned int index );
 	MStatus         insert( MUint64 element, unsigned int index );
 	MStatus         append( MUint64 element );
 	MStatus         copy( const MUint64Array& source );
 	MStatus		 	clear();
	MStatus			get( MUint64[] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

	//! NO SCRIPT SUPPORT
	MUint64			operator[]( unsigned int index ) const;

	//!	NO SCRIPT SUPPORT
 	MUint64 &	 	operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MUint64Array &array);
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
	swigTPythonObjectConverter(MUint64);
	swigExtendAPIArray(MUint64Array,MUint64);
#endif

protected:
// No protected members

private:
	MUint64Array( void* );
 	void * arr;
	const MUint64*  debugPeekValue;
	bool   fOwn;
	void			syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MUint64Array */
