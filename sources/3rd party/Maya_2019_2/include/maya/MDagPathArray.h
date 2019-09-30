#ifndef _MDagPathArray
#define _MDagPathArray
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
// CLASS:    MDagPathArray
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MDagPathArray)
//
//	Methods are provided for obtaining the DAG Path elements by index, clearing
//	the array, determining the length of the array, and removing, inserting and
//	appending array elements.
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
// CLASS DECLARATION (MDagPathArray)

//! \ingroup OpenMaya
//! \brief Indexable Array of DAG Paths. 
/*!
Provides methods for manipulating arrays of DAG Paths.

Arrays of DAG Paths are useful for storing and manipluating multiple Paths
to a particular DAG Node.  The DAG Path method MDagPath::getAllPathsTo()
and DAG Node Function Set method MFnDagNode::getAllPaths() implicitly
return an array of DAG Paths.

These arrays may also be used to manage Paths for a number of different
Nodes.

DAG Path arrays are used in conjunction with DAG Paths (MDagPath) and
individual elements of the arrays can be parameters to some methods of the
DAG Node Function Set (MFnDagNode).

Use this DAG Path Array Class to create and manipulate arrays of DAG Paths,
for either a particular DAG Node or a number of different DAG Nodes.

The length of the array adjusts automatically.
*/
class OPENMAYA_EXPORT MDagPathArray
{
public:
	typedef MArrayIteratorTemplate<MDagPathArray, MDagPath> Iterator;
	typedef MArrayConstIteratorTemplate<MDagPathArray, MDagPath> ConstIterator;

					MDagPathArray();
					MDagPathArray(const MDagPathArray& other);
					MDagPathArray(
						unsigned int initializeSize, const MDagPath& initialValue
					);
					~MDagPathArray();
	const MDagPath&	operator[]( unsigned int index ) const;
	MDagPathArray& operator=(const MDagPathArray& other );
	MStatus			set( const MDagPath& element, unsigned int index);
	MStatus			setLength( unsigned int length );
	unsigned int		length() const;
	MStatus			remove( unsigned int index );
	MStatus			insert( const MDagPath & element, unsigned int index );
	MStatus			append( const MDagPath & element );
 	MStatus         copy( const MDagPathArray& source );
	MStatus			clear();
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int	sizeIncrement () const;
BEGIN_NO_SCRIPT_SUPPORT:

	MDagPath &		operator[]( unsigned int index );

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MDagPathArray &array);
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
	MDagPathArray ( void * );
	void * arr;
	bool   own;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDagPathArray */
