#ifndef _MItKeyframe
#define _MItKeyframe
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
// CLASS:    MItKeyframe
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MTime.h>


// ****************************************************************************
// DECLARATIONS

class MPtrBase;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItKeyframe)

//! \ingroup OpenMayaAnim
//! \brief  Keyframe Iterator 
/*!
Iterate over the keyframes of a particular Anim Curve Node, and query
and edit the keyframe to which the iterator points.

Determine the time and value of the keyframe, as well as the x,y values
and type of the tangent to the curve entering (in tangent) and leaving
(out tangent) the keyframe.

Set the time and value of the keyframe, and the type of the tangents.

Anim Curves are implemented as Dependency Graph (DG) Nodes. Each Node has
multiple ordered and indexed keyframes.

Use the Keyframe Iterator to systematically visit, and query and/or edit the
keyframes of a Anim Curve Node.

Use the Keyframe Iterator in conjunction with the Anim Curve Function Set
(MFnAnimCurve) to edit Anim Curve Nodes.

On creation the iterator is attached to a Anim Curve Node.  The
iterator may be detached from its current Node and attached to another
Anim Curve Node using the reset() method.

Though keyframes within a Node are ordered and indexed, the iterator
maintains the index of the current Node internally and does not export
it.  Use the Anim Curve Function Set to access the index for a
keyframe.  The index is zero-based.  When an iterator is first
attached to a Node, either on creation or reset, the index is set to
zero.  There is an overloaded version of the reset() method which does
not cause the iterator to change Nodes, but merely resets the index.

Use the next(), reset() and isDone() methods to perform iterations.

Use the specific query methods to determine the time, value and
tangent information for the keyframe under the iterator.

Use specific edit methods to set the time and value , as well as the
incoming and outgoing tangent type (MItKeyframe::TangentType) of the
keyframe under the iterator.

There is no method for setting the x,y value of either of the
tangents.

Setting the time of a keyframe will fail if the new time would require
a re-ordering of the keyframes.  Use the Anim Curve Functiion Set
methods MFnAnimCurve::remove() and MFnAnimCurve::addKeyFrame() to
re-order the keyframes.
*/
class OPENMAYAANIM_EXPORT MItKeyframe {
public:
	//! Tangent types.
	enum TangentType {
		/*!
		Use the global tangent settings in Maya's 'Animation Preferences'.
		*/
		kTangentGlobal = 0,

		/*!
		Normally, if a key is moved tangent values will be adjusted
		to maintain their relationships to the surrounding
	        keys (linear, smooth, etc).

		If a tangent is set to be fixed then it will not change
		when keys are moved but will retain its current value.
		*/
		kTangentFixed,

		/*!
		For an in tangent this sets it to point directly at the
		previous key. For an out tangent it points directly to
		the next key. This is used to create straight lines
		between keys.
		*/
		kTangentLinear,

		/*!
		The tangent is horizontal.
		*/
		kTangentFlat,

		/*!
		The in and out tangents are colinear and set to provide a
		smooth transition from the key before to the key after.
		*/
		kTangentSmooth,

		/*!
		The key's value is maintained until the next key is reached
		whereupon it immediately jumps to the new key's value.
		*/
		kTangentStep,

		/*!
		Slow tangent.
		*/
		kTangentSlow,

		/*!
		Fast tangent.
		*/
		kTangentFast,

		/*!
		Same as smooth except when two keys are very close together
		in which case the tangents between them are treated as linear.
		This corrects for slippage which is sometimes seen when
		spline tangents are used on keyframes which are close together.
		*/
		kTangentClamped,

		/*!
		Similar to smooth except that minima and maxima between
		keyframes are flattened, if necessary, to prevent them
		overshooting their keys.
		*/
		kTangentPlateau,

		/*!
		Upon leaving the keyframe the value immediately jumps to that of
		the next keyframe and stays there.
		*/
		kTangentStepNext,

		/*!
 		The first and last keyframes will have flat tangents. Between,
		inner keyframes, the animation curve will not over shoot
		the keyframe values. 
		*/
		kTangentAuto
	};

    //! Defines the value type of the tangent
    typedef double TangentValue;

	MItKeyframe( MObject & animCurveNode, MStatus * ReturnStatus = NULL );
	~MItKeyframe();
	MStatus     reset( MObject & animCurveNode );
	MStatus     reset();
	MStatus     next();
	bool        isDone( MStatus * ReturnStatus = NULL );
	MTime       time( MStatus * ReturnStatus = NULL );
	MStatus     setTime( MTime time );
	double      value( MStatus * ReturnStatus = NULL );
	MStatus     setValue( double value );
	TangentType inTangentType( MStatus * ReturnStatus = NULL );
	TangentType outTangentType( MStatus * ReturnStatus = NULL );
	MStatus     setInTangentType( TangentType tangentType );
	MStatus     setOutTangentType( TangentType tangentType );
	MStatus     getTangentOut( TangentValue &x, TangentValue &y );
	MStatus     getTangentIn( TangentValue &x, TangentValue &y );
	bool		tangentsLocked( MStatus * ReturnStatus = NULL ) const;
	MStatus		setTangentsLocked( bool locked );

	static const char* className();

protected:
// No protected members

private:
    MPtrBase *        f_ptr;
	unsigned int          f_index;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItKeyframe */
