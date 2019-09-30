#ifndef _MRichSelection
#define _MRichSelection
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
// CLASS:    MRichSelection
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRichSelection)

//! \ingroup OpenMaya
//! \brief Selection list supporting soft selection and symmetry. 
/*!
This class implements a selection list that support soft selection and symmetry.

The rich selection is split into two halves: the "normal" side, and an optional
symmetric component. Components on both sides can include weight data which is
used to specify both the amount of influence and the proximity to the centre of
symmetry. 

In addition to the selected objects, the rich selection also includes information
about the axis of symmetry so that operations can determine how to process any
symmetric selection (e.g. reflect transformations, etc).
*/
class OPENMAYA_EXPORT MRichSelection
{
public:

	MRichSelection();
	MRichSelection( const MRichSelection & src );

	virtual ~MRichSelection();

	MStatus		getSelection( MSelectionList& selection) const;
	MStatus		getSymmetry( MSelectionList& symmetry) const;
	MStatus		getSymmetryMatrix( MMatrix& symmetryMatrix, MSpace::Space& space) const;
	MStatus		getSymmetryMatrix( const MDagPath& path, MSpace::Space space, MMatrix& symmetryMatrix) const;
	MStatus		getSymmetryPlane( const MDagPath& path, MSpace::Space space, MPlane& plane) const;

	MStatus		clear();

	MStatus		setSelection( const MSelectionList& selection );

	static const char* className();

protected:
// No protected members

private:
	friend class MGlobal;
	MRichSelection( void * );
	void		setData( void * );
	void *		selection_data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRichSelection */
