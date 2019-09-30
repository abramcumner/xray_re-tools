#ifndef _MObjectArray
#define _MObjectArray
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
// CLASS:    MObjectArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MArrayIteratorTemplate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MObjectArray)

//! \ingroup OpenMaya
//! \brief  Array of MObjects data type. 
/*!
  This class implements an array of MObjects.  Common convenience functions
  are available, and the implementation is compatible with the internal
  Maya implementation so that it can be passed efficiently between plugins
  and internal maya data structures.
*/
class OPENMAYA_EXPORT MObjectArray
{

public:
	typedef MArrayIteratorTemplate<MObjectArray, MObject> Iterator;
	typedef MArrayConstIteratorTemplate<MObjectArray, MObject> ConstIterator;

					MObjectArray();
					MObjectArray( const MObjectArray& other );
					MObjectArray( unsigned int initialSize,
								  const MObject &initialValue
								  = MObject::kNullObj );
					~MObjectArray();
 	MObjectArray&	operator = (const MObjectArray&);
 	const MObject&	operator[]( unsigned int index ) const;
 	MStatus			set( const MObject& element, unsigned int index );
	MStatus			setLength( unsigned int length );
 	unsigned int		length() const;
 	MStatus			remove( unsigned int index );
 	MStatus			insert( const MObject & element, unsigned int index );
 	MStatus			append( const MObject & element );
	MStatus			copy( const MObjectArray& source );
 	MStatus			clear();
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

    //!	NO SCRIPT SUPPORT
	MObjectArray( const MObject src[], unsigned int count );

    //!	NO SCRIPT SUPPORT
	MStatus			get( MObject array[] ) const;

	//!	NO SCRIPT SUPPORT
 	MObject&		operator[]( unsigned int index );

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
 	void* fArray;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MObjectArray */
