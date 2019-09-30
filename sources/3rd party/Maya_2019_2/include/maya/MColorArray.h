#ifndef _MColorArray
#define _MColorArray
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
//
// CLASS:    MColorArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MColor.h>
#include <maya/MArrayIteratorTemplate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MColorArray)

//! \ingroup OpenMaya
//! \brief Array of MColor data type. 
/*!
This class implements an array of MColor's.  Common convenience functions
are available, and the implementation is compatible with the internal
Maya implementation so that it can be passed efficiently between plugins
and internal maya data structures.
*/
class OPENMAYA_EXPORT MColorArray
{
public:
	typedef MArrayIteratorTemplate<MColorArray, MColor> Iterator;
	typedef MArrayConstIteratorTemplate<MColorArray, MColor> ConstIterator;

					MColorArray();
					MColorArray( const MColorArray& other );

BEGIN_NO_SCRIPT_SUPPORT:
	//!	NO SCRIPT SUPPORT
					MColorArray( const MColor src[], unsigned int count );
END_NO_SCRIPT_SUPPORT:

					MColorArray( const float src[][4], unsigned int count );
					MColorArray( const double src[][4], unsigned int count );
					MColorArray( unsigned int initialSize,
								 const MColor &initialValue = MColor::kOpaqueBlack );
					~MColorArray();
	const MColor&	operator[]( unsigned int index ) const;
 	MColorArray &   operator=( const MColorArray & other );
	MStatus			set( const MColor& element, unsigned int index );
	MStatus 		set( unsigned int index, float r, float g, float b, float a=1 );
	MStatus			set( float val[4], unsigned int index);
	MStatus			set( float elements[][4], unsigned int size);
	MStatus			set( double elements[][4], unsigned int size);
	MStatus			setLength( unsigned int length );
	unsigned int		length() const;
	MStatus			remove( unsigned int index );
	MStatus			insert( const MColor & element, unsigned int index );
	MStatus			append( const MColor & element );
	MStatus 		append( float r, float g, float b, float a=1 );
	MStatus			copy(const MColorArray& source);
	MStatus			clear();
	MStatus			get( float dest[][4] ) const;
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	MColor&	        operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MColorArray &array);
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
	MColorArray( void* );
	void * arr;
	struct api_data
	{
		float r;
		float g;
		float b;
		float a;
	};
	const api_data* debugPeekValue;
	bool   own;
	void syncDebugPeekValue();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MColorArray */
