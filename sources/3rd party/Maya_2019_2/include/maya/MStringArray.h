#ifndef _MStringArray
#define _MStringArray
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
// CLASS:    MStringArray
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
// CLASS DECLARATION (MStringArray)

//! \ingroup OpenMaya
//! \brief Array of MStrings data type.
/*!
This class implements an array of MStrings.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MStringArray
{
public:
	typedef MArrayIteratorTemplate<MStringArray, MString> Iterator;
	typedef MArrayConstIteratorTemplate<MStringArray, MString> ConstIterator;

				MStringArray();
				MStringArray( const MStringArray& other );
				MStringArray( const MString strings[], unsigned int count );
				MStringArray( const char* strings[], unsigned int count );
				MStringArray( unsigned int initialSize,
							  const MString &initialValue );
				~MStringArray();
	MString		operator[]( unsigned int index ) const;
 	MStringArray & operator=( const MStringArray & other );
	MStatus		set( const MString& element, unsigned int index );
	MStatus		set( char* element, unsigned int index );
	MStatus		setLength( unsigned int length );
	unsigned int	length() const;
	MStatus		remove( unsigned int index );
	MStatus		insert( const MString & element, unsigned int index );
	MStatus		append( const MString & element );
	MStatus		clear();
	MStatus		get( MString array[] ) const;
	MStatus		get( char* array[] ) const;
    int			indexOf(const MString& element) const;
	void		setSizeIncrement ( unsigned int newIncrement );
	unsigned int	sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	MString&	operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MStringArray &array);
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
	MStringArray( void* );
	friend class MString;
	void * arr;
	bool   own;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MStringArray */
