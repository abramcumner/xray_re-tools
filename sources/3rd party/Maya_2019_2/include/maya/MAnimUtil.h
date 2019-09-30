#ifndef _MAnimUtil
#define _MAnimUtil
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
// CLASS:    MAnimUtil
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAnimUtil)

//! \ingroup OpenMayaAnim
//! \brief Static class providing common animation helper methods 
/*!
	MAnimUtil is a static class which provides methods which determine if an
	object is being animated, which attributes are animated for a given object
	and which animation curves are used to animate a given attribute.

	Note: for purposes of this helper class, "animation" refers to attributes
	animated by animCurves (i.e. by setting keys).  It does not include any
	animation through expressions or dynamics or timer node connections.

	Some helper methods may examine different types of objects.  Depending
	upon how the object is represented, the same object may be examined
	differently.

	If the object is specified as an <b>MObject</b>, then all of its attributes
	are examined.  If the MObject is a hierarchical object (such as a dag node)
	and you specify that the objects parents should be examined, then all
	the parents of an instanced object will be examined.

	If the object is specified as an <b>MDagPath</b>, then all of its attributes
	are examined.  If you specify that the objects parents should be examined,
	then only the specified path of the instanced object will be examined.

	If the object is specifed as an <b>MPlug</b> and no attribute has been set, then
	the method will behave as if it was called with an MObject.  Otherwise
	only the specified attribute will be examined.  For example, a compound
	attribute will not have its child attributes examined (i.e.  if you
	specify node.translate, node.translateX will not be examined).

	If the object is specified through an <b>MSelectionList</b>, it can also have
	multiple representations.  If the object was added to the MSelectionList
	as an MObject, then it will be examined as an MObject.  If the object
	was added to the MSelectionList as an MDagPath, then it will be examined
	as an MDagPath.  If the object was added to the MSelectionList as
	either a component or an MPlug, it will be examined as an MPlug with
	additional expansion of compound attributes into their child attributes
	(for example,  if specify node.translate is specified, node.translateX will be
	examined.  More importantly, if you specify poly.f[4] then xValue, yValue
	and zValue will be examined for each vertex).

	Below is a summary of how the same object will be examined by
	MAnimUtil::isAnimated depending upon how it has been represented.  If
	we have the following hierarchical layout:

	<ul>
	<li> <b>group1</b> (translateX is animated)
	    <ul>
		<li> <b>pTorus1</b> (instanced and not animated)
	        <ul>
			<li> <b>pTorusShape1</b> (face[4] is animated)
	        </ul>
	    </ul>

	<li> <b>group2</b> (not animated)
	    <ul>
		<li> <b>pTorus1</b> (another instance of pTorus1 as above)
	    </ul>
	</ul>


	The MAnimUtil::isAnimated will respond as follows:

	<b>isAnimated ( MObject(pTorus1), false ) </b> => false

	<b>isAnimated ( MObject(pTorus1), true ) </b> => true

	<b>isAnimated ( MDagPath(group2|pTorus1), false ) </b> => false

	<b>isAnimated ( MDagPath(group2|pTorus1), true ) </b> => false

	<b>isAnimated ( MPlug(pTorus1), false ) </b> => false

	<b>isAnimated ( MPlug(pTorus1), true ) </b> => true

	<b>isAnimated ( MPlug(group1.translate) ) </b> => false

	<b>isAnimated ( MPlug(group1.translateX) ) </b> => true

	<b>isAnimated ( MPlug(pTorusShape1.pnts[4]) ) </b> => false

	<b>isAnimated ( MPlug(pTorusShape1.pnts[4].xValue) ) </b> => true

	The following list assumes that MAnimUtil is called with an MSelectionList
	named list, and shows the response depending upon how the object has been
	added to list:

	<b>isAnimated ( list.add(MObject(pTorus1)), false ) </b> => false

	<b>isAnimated ( list.add(MObject(pTorus1)), true ) </b> => true

	<b>isAnimated ( list.add(MDagPath(group2|pTorus1)), false ) </b> => false

	<b>isAnimated ( list.add(MDagPath(group2|pTorus1)), true ) </b> => false

	<b>isAnimated ( list.add(MPlug(pTorus1)), false ) </b> => false

	<b>isAnimated ( list.add(MPlug(pTorus1)), true ) </b> => true

	<b>isAnimated ( list.add(MPlug(group1.translate)) ) </b> => true

	<b>isAnimated ( list.add(MPlug(group1.translateX) ) </b> => true

	<b>isAnimated ( list.add(MPlug(pTorusShape1.pnts[4])) ) </b> => true

	<b>isAnimated ( list.add(MPlug(pTorusShape1.pnts[4].xValue)) ) </b> => true
*/
class OPENMAYAANIM_EXPORT MAnimUtil
{
public:
	static bool		isAnimated(	const MObject &node,
								bool checkParent = false,
								MStatus * ReturnStatus = NULL );
	static bool		isAnimated(	const MDagPath &path,
								bool checkParent = false,
								MStatus * ReturnStatus = NULL );
	static bool		isAnimated(	const MPlug &plug,
								bool checkParent = false,
								MStatus * ReturnStatus = NULL );
	static bool		isAnimated(	const MSelectionList &selectionList,
								bool checkParent = false,
								MStatus * ReturnStatus = NULL );
	static bool		findAnimatedPlugs(	const MObject &node,
										MPlugArray &animatedPlugs,
										bool checkParent = false,
										MStatus * ReturnStatus = NULL );
	static bool		findAnimatedPlugs(	const MDagPath &path,
										MPlugArray &animatedPlugs,
										bool checkParent = false,
										MStatus * ReturnStatus = NULL );
	static bool		findAnimatedPlugs(	const MSelectionList &selectionList,
										MPlugArray &animatedPlugs,
										bool checkParent = false,
										MStatus * ReturnStatus = NULL );
	static bool		findAnimation(	const MPlug &plug,
									MObjectArray &animation,
									MStatus * ReturnStatus = NULL );
	static bool		findSetDrivenKeyAnimation(	const MPlug &plug,
												MObjectArray &animation,
												MPlugArray &drivers,
												MStatus * ReturnStatus = NULL );
	static bool		findConstraint(	const MPlug &plug,
									MObject &constraint,
									MObjectArray& targets,
									MStatus * ReturnStatus = NULL );
	static bool		findAnimatablePlugs(	const MSelectionList &selectionList,
										MPlugArray &animatablePlugs);


	static bool		findAnimationLayers(	const MPlug &plug,
											MObjectArray& layers,
											MPlugArray &plugs,
											MStatus * ReturnStatus = NULL );
	static const char* className();

private:
	~MAnimUtil();
#ifdef __GNUC__
	friend class shutUpAboutPrivateDestructors;
#endif
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAnimUtil */
