#ifndef _MPlugArray
#define _MPlugArray
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
// CLASS:    MPlugArray
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
// CLASS DECLARATION (MPlugArray)

//! \ingroup OpenMaya
//! \brief Array of MPlugs data type.
/*!
  This class implements an array of MPlugs.  Common convenience functions
  are available, and the implementation is compatible with the internal
  Maya implementation so that it can be passed efficiently between plugins
  and internal maya data structures.
*/
class OPENMAYA_EXPORT MPlugArray
{
public:
	typedef MArrayIteratorTemplate<MPlugArray, MPlug> Iterator;
	typedef MArrayConstIteratorTemplate<MPlugArray, MPlug> ConstIterator;

					MPlugArray();
					MPlugArray( const MPlugArray& other );
					MPlugArray(
						unsigned int initializeSize, const MPlug& initialValue
					);
					~MPlugArray();
	const MPlug&	operator[]( unsigned int index ) const;
	MPlugArray&		operator=( const MPlugArray & other );
	MStatus			set( const MPlug& element, unsigned int index );
	MStatus			setLength( unsigned int length );
	unsigned int		length() const;
	MStatus			remove( unsigned int index );
	MStatus			insert( const MPlug & element, unsigned int index );
	MStatus			append( const MPlug & element );
	MStatus			copy( const MPlugArray & source );
	MStatus			clear();
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:
	//!	NO SCRIPT SUPPORT
	MPlugArray( const MPlug src[], unsigned int count );

	//!	NO SCRIPT SUPPORT
	MStatus			get( MPlug dest[] ) const;

	//!	NO SCRIPT SUPPORT
	MPlug&			operator[]( unsigned int index );

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
	MPlugArray ( void * );
	friend class MPlug;
	void * arr;
	bool   own;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPlugArray */
