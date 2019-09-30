#ifndef _MMaterialArray
#define _MMaterialArray
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
// CLASS:    MMaterialArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MMaterial.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MMaterialArray)

//! \ingroup OpenMayaUI
//! \brief Array of pointers of MMaterial data type. 
/*!
This class implements an array of MMaterial.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYAUI_EXPORT MMaterialArray
{

public:
	MMaterialArray();
	MMaterialArray( const MMaterialArray & other );
	~MMaterialArray();
	const MMaterial	operator[]( unsigned index ) const;
	MMaterialArray &  operator=( const MMaterialArray & other );
	MStatus			set( const MMaterial& element, unsigned index );
	MStatus			setLength( unsigned length );
	unsigned		length() const;
	MStatus			remove( unsigned index );
	MStatus			insert( const MMaterial & element, unsigned index );
	MStatus			append( const MMaterial & element );
	MStatus			copy( const MMaterialArray& source );
	MStatus			clear();
	void			setSizeIncrement ( unsigned newIncrement );
	unsigned		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:
	MMaterial		operator[]( unsigned index );
	//!	NO SCRIPT SUPPORT
END_NO_SCRIPT_SUPPORT:

	static const char* className();

protected:
// No protected members

private:
	MMaterialArray( void* );
	void * arr;
	bool   own;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MMaterialArray */
