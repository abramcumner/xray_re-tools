#ifndef _MAttributePatternArray
#define _MAttributePatternArray
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
// CLASS:    MAttributePatternArray
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MAttributePatternArray)
//
//	Methods are provided for obtaining the attribute pattern elements by
//	index, clearing the array, determining the length of the array, and
//	removing, inserting and appending array elements.
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
// CLASS DECLARATION (MAttributePatternArray)

//! \ingroup OpenMaya
//! \brief Indexable Array of attribute patterns. 
/*!
Provides methods for manipulating arrays of attribute patterns.

Arrays of attribute patterns are used when constructing pattern lists from a
string or file description. Since there's no way of knowing in advance how many
will be created from a given description this general purpose array was needed.

The length of the array adjusts automatically.
*/
class OPENMAYA_EXPORT MAttributePatternArray
{
public:
	typedef MArrayIteratorTemplate<MAttributePatternArray, MAttributePattern> Iterator;
	typedef MArrayConstIteratorTemplate<MAttributePatternArray, MAttributePattern> ConstIterator;

	MAttributePatternArray();
	MAttributePatternArray( const MAttributePatternArray& other );
	MAttributePatternArray( unsigned int			 initializeSize,
							const MAttributePattern& initialValue );
	~MAttributePatternArray();
	const MAttributePattern& operator[]	(unsigned int index) const;
	MAttributePatternArray&	operator=	(const MAttributePatternArray& rhs);
	MStatus			set					( const MAttributePattern& element, unsigned int index);
	MStatus			setLength			( unsigned int length );
	unsigned int	length				() const;
	MStatus			remove				( unsigned int index );
	MStatus			insert				( const MAttributePattern & element, unsigned int index );
	MStatus			append				( const MAttributePattern & element );
 	MStatus         copy				( const MAttributePatternArray& source );
	MStatus			clear				();
	void			setSizeIncrement	( unsigned int newIncrement );
	unsigned int	sizeIncrement		() const;
BEGIN_NO_SCRIPT_SUPPORT:

	MAttributePattern &		operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MAttributePatternArray &array);
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
	MAttributePatternArray ( void * );
	void * arr;
	bool   own;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAttributePatternArray */
