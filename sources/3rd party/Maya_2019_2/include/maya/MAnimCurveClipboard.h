#ifndef _MAnimCurveClipboard
#define _MAnimCurveClipboard
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
// CLASS:    MAnimCurveClipboard
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MAnimCurveClipboardItemArray.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAnimCurveClipboard)

//! \ingroup OpenMayaAnim
//! \brief Control over the animation clipboard 
/*!
	The clipboard is a list of MAnimCurveClipboardItems (i.e. an
	MAnimCurveClipboardItemArray).  All of the data stored on a clipboard
	remains static; that is, it will persist as long as the application
	remains running.

	The items on the clipboard must be ordered.  In the case where the
	clipboard info represents animation of a hierarchy, the order in which
	items appear in the clipboard is reliant on a depth-first-iteration from
	the root or the hierarchy.  This ordering, is essential to properly match
	up hierarchies of objects

	As an example, consider that animation from the following hierarchy
	is placed into the clipboard. Slanted Dag Objects are animated.

	\image html animHier.gif

	In this example, the object labelled "C" has translate{X,Y,Z} animated,
	while all the others only have one animated attribute (excluding objects
	B and F, which have no animated attributes).

	Using notation where A(r,c,a) represents the animCurve driving object
	"A", which is at row "r" in its subhierarchy, has "c" children, and
	"a" animated attributes these items would appear in the clipboard in this
	order (i.e. depth-first with each object's attributes explicitly indexed
	before continuing down the hierarchy):

\code
	A(0,3,0) , B(1,1,0) , C(2,1,0) , C(2,1,1) , C(2,1,2) , D(3,0,0) ,
	E(1,0,0) , F(1,2,0) , G(2,0,0) , H(2,0,0)
\endcode

	For example C(2,1,2) would mean that the object C resides on the second
	level of the subhierarchy and has one child.  The last "2" is simply
	used as an index to count the number of animated attributes on this
	object.

	Multiple objects can be represented on the clipboard in this manner.  In
	the example above, if we had a separate second object with no children,
	"J", it would appear at the end of the array as J(0,0,0).

	Note that although B and F contain no animation data themselves, they
	must still be placed on the clipboard as placeholders to preserve the
	hierarchy information.  A placeholder object is defined by a NULL value
	for the MAnimCurveClipboardItem's animCurve.

	There is a special clipboard that remains static.  It can be accessed by
	MAnimCurveClipboard::theAPIClipboard().
*/
class OPENMAYAANIM_EXPORT MAnimCurveClipboard
{
public:
										MAnimCurveClipboard();
										~MAnimCurveClipboard();

	// For accessing the static API clipboard
	static MAnimCurveClipboard &		theAPIClipboard();
	MStatus		set( const MAnimCurveClipboard &cb );
	MStatus		set( const MAnimCurveClipboardItemArray &clipboardItemArray );
	MStatus		set( const MAnimCurveClipboardItemArray &clipboardItemArray,
					 const MTime &startTime, const MTime &endTime,
					 const float &startUnitlessInput,
					 const float &endUnitlessInput,
					 bool strictValidation = true);
	MStatus		clear ();

	bool		isEmpty( MStatus * ReturnStatus = NULL ) const;
	const MAnimCurveClipboardItemArray 	clipboardItems( MStatus *
												ReturnStatus = NULL) const;
	MTime		startTime( MStatus * ReturnStatus = NULL ) const;
	MTime		endTime( MStatus * ReturnStatus = NULL ) const;
	float		startUnitlessInput( MStatus * ReturnStatus = NULL ) const;
	float		endUnitlessInput( MStatus * ReturnStatus = NULL ) const;


protected:
// No protected members

private:
	void *							fClipboard;

	static MAnimCurveClipboard		fsAPIClipboard;

	MAnimCurveClipboard& operator = (const MAnimCurveClipboard&);
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAnimCurveClipboard */
