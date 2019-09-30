#ifndef _MTrimBoundaryArray
#define _MTrimBoundaryArray
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
// CLASS:    MTrimBoundaryArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MArrayIteratorTemplate.h>
#include <vector>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MTrimBoundaryArray)

//! \ingroup OpenMaya
//! \brief  An array class for trim boundaries. 
/*!

  This class implements an array of MObjectArray objects. Each
  MObjectArray is a collection of curves (edges, in fact) that
  represent a trim boundary.

  This class owns all of the MObjectArrays in the class. It doesn't
  hold a reference to any other MObjectArray.
*/
class OPENMAYA_EXPORT MTrimBoundaryArray
{
public:
	typedef MArrayIteratorTemplate<MTrimBoundaryArray, MObjectArray> Iterator;
	typedef MArrayConstIteratorTemplate<MTrimBoundaryArray, MObjectArray> ConstIterator;

	MTrimBoundaryArray();
	MTrimBoundaryArray(const MTrimBoundaryArray& other);
	~MTrimBoundaryArray();


	MStatus			reserve(unsigned int length);
 	unsigned int		length() const;
 	unsigned int		size() const;

 	const MObjectArray&	operator[](unsigned int index) const;
	const MObjectArray&	get( unsigned int index ) const;

 	MStatus	set(const MObjectArray& boundary, unsigned int index);
 	MStatus	insert(const MObjectArray& boundary, unsigned int index);
 	MStatus	append(const MObjectArray& boundary);
 	MStatus	remove(unsigned int index);
 	MStatus	clear();

	MObject	getMergedBoundary(unsigned int index, MStatus *status=NULL) const;

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
 	MTrimBoundaryArray& operator = (const MTrimBoundaryArray&);

	bool		fIsUVBoundary;
	void*		fBoundaryArray;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MTrimBoundaryArray */
