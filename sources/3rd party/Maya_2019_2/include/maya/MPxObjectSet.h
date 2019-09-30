#ifndef _MPxObjectSet
#define _MPxObjectSet
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
// CLASS:    MPxObjectSet
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MPxObjectSet)
//
//  MPxObjectSet is the parent class of all user defined object sets.
//  User defined sets are dependency nodes.
//
//  This class can be used to implement new kinds of sets within maya that
//  can have selectable/manipulatable components and behave in a similar
//  manner to the objectSet node included in maya.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MPxNode.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxObjectSet)

//! \ingroup OpenMaya MPx
//! \brief Parent class of all user defined object sets. 
/*!
  MPxObjectSet is the parent class of all user defined sets.
*/
class OPENMAYA_EXPORT MPxObjectSet : public MPxNode
{
public:

	MPxObjectSet();
	virtual ~MPxObjectSet();
	virtual MPxNode::Type type() const;
	virtual bool canBeDeleted( bool isSrcNode );

	//
    // Inherited attributes
	//

	//! Connections to this attribute specify the dagNodes or
	//! parts (components) of the same that are members of this set.
	static MObject dagSetMembers;

	//! References to dependNode attributes which defines set membership.
	static MObject DNSetMembers;

	//! The index of a user defined color in which the dag object component
	//! members should appear. A value of -1 disables use of the color.
	//! Values outside the range [-1,7] may give unpredictable results.
	static MObject memberWireframeColor;

	//! A text string to annotate the set.
	static MObject annotation;

	//! This attribute is obsolete and should no longer be used
	static MObject isLayer;

	//! Is set membership restricted to objects with vertices?
	//! This attribute should not be explicitly changed by the user.
	static MObject verticesOnlySet;

	//! Is set membership restricted to objects with edges?
	//! This attribute should not be explicitly changed by the user.
	static MObject edgesOnlySet;

	//! Is set membership restricted to objects with facets?
	//! This attribute should not be explicitly changed by the user.
	static MObject facetsOnlySet;

	//! Is set membership restricted to objects with edit points?
	//! This attribute should not be explicitly changed by the user.
	static MObject editPointsOnlySet;

	//! Is set membership restricted to renderable objects only?
	//! This attribute should not be explicitly changed by the user.
	static MObject renderableOnlySet;

	//! References to the partition nodes this set is a member of.
	static MObject partition;

	//! When parts (components) of dagNodes are in the set, connections
	//! are made to this attribute to hold references to groupId nodes,
	//! with the id uniquely identifying the group in the dagNode.
	static MObject groupNodes;

	//! References to nodes that operate upon this set, such as deformers.
	static MObject usedByNodes;

	static const char*	    className();

private:
	static void				initialSetup();

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxObjectSet */
