#ifndef _MAttributeSpecArray
#define _MAttributeSpecArray
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
// CLASS:    MAttributeSpecArray
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
// DECLARATIONS

// ****************************************************************************
// CLASS DECLARATION (MAttributeSpecArray)

//! \ingroup OpenMaya
//! \brief An attribute specification array. 
/*!
Array class for MAttributeSpec.

See MAttributeSpec for more information.
*/
class OPENMAYA_EXPORT MAttributeSpecArray
{
public:
	typedef MArrayIteratorTemplate<MAttributeSpecArray, MAttributeSpec> Iterator;
	typedef MArrayConstIteratorTemplate<MAttributeSpecArray, MAttributeSpec> ConstIterator;

	MAttributeSpecArray();
	MAttributeSpecArray( const MAttributeSpecArray& other );
	MAttributeSpecArray(
		unsigned int initializeSize, const MAttributeSpec& initialValue
	);
	~MAttributeSpecArray();

	MAttributeSpec	operator[]( unsigned int index ) const;
 	MAttributeSpecArray & operator=( const MAttributeSpecArray & other );

	MStatus			set( const MAttributeSpec& element, unsigned int index );
	MStatus			setLength( unsigned int length );
	unsigned int		length() const;
	MStatus			remove( unsigned int index );
	MStatus			insert( const MAttributeSpec & element, unsigned int index );
	MStatus			append( const MAttributeSpec & element );
 	MStatus         copy( const MAttributeSpecArray& source );
	MStatus			clear();
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

#if !defined(SWIG)
	Iterator begin();
	Iterator end();

	ConstIterator begin() const;
	ConstIterator end() const;

	ConstIterator cbegin() const;
	ConstIterator cend() const;
#endif

	static const char* className();

protected:
// No protected members

private:
	MAttributeSpecArray( void* );
	void * arr;
	bool   own;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAttributeSpecArray */
