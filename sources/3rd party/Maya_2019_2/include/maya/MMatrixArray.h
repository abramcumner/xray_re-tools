#ifndef _MMatrixArray
#define _MMatrixArray
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
// CLASS:    MMatrixArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MMatrix.h>
#include <maya/MArrayIteratorTemplate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MMatrixArray)

//! \ingroup OpenMaya
//! \brief Array of MMatrix data type. 
/*!
This class implements an array of MMatrix's.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MMatrixArray
{

public:
	typedef MArrayIteratorTemplate<MMatrixArray, MMatrix> Iterator;
	typedef MArrayConstIteratorTemplate<MMatrixArray, MMatrix> ConstIterator;

					MMatrixArray();
					MMatrixArray( const MMatrixArray & other );
					MMatrixArray( unsigned initialSize );
					~MMatrixArray();
    const MMatrix&	operator[]( unsigned index ) const;
 	MMatrixArray &  operator=( const MMatrixArray & other );
	MStatus			set( const MMatrix& element, unsigned index );
	MStatus			setLength( unsigned length );
	unsigned		length() const;
	MStatus			remove( unsigned index );
	MStatus			insert( const MMatrix & element, unsigned index );
	MStatus			append( const MMatrix & element );
 	MStatus			copy( const MMatrixArray& source );
	MStatus			clear();
	void			setSizeIncrement ( unsigned newIncrement );
	unsigned		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:
    MMatrix &		operator[]( unsigned index );
    //!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
                                                    const MMatrixArray &array);

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
	MMatrixArray( void* );
	void * arr;
	bool   own;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MMatrixArray */
