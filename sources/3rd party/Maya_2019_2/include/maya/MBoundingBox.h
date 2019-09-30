#ifndef _MBoundingBox
#define _MBoundingBox
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
// CLASS:    MBoundingBox
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MPoint.h>


// PDB
#if defined(_WIN32)
#undef min
#undef max
#endif // _WIN32

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MBoundingBox)

//! \ingroup OpenMaya
//! \brief Implementation of a 3D bounding box. 
/*!
	This class provides an implementation of a bounding box.  A bounding box
    describes a volume in space that bounds a piece of geometry.  The box is
    defined by two points which describe the minimum and maximum corners of
    the box.
*/
class OPENMAYA_EXPORT MBoundingBox
{
public:
    MBoundingBox();
	MBoundingBox( const MBoundingBox & src );
	MBoundingBox( const MPoint &corner1, const MPoint &corner2 );

	~MBoundingBox();

	void	clear();

    void	transformUsing ( const MMatrix &matrix );
	void	expand( const MPoint & point );
	void	expand( const MBoundingBox & box );

	bool	contains( const MPoint & point ) const;
	bool    intersects (const MBoundingBox & box, double tol = 0.0 ) const;
	double	width() const;
	double	height() const;
	double	depth() const;
	MPoint	center() const;
	MPoint	min() const;
	MPoint	max() const;

	MBoundingBox & operator=( const MBoundingBox & other );


protected:
// No protected members

private:
	double data[6];
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MBoundingBox */
