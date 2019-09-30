#ifndef _MPxDragAndDropBehavior
#define _MPxDragAndDropBehavior
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
// CLASS:    MPxDragAndDropBehavior
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxDragAndDropBehavior)

//! \ingroup OpenMayaUI MPx
//! \brief Drag and Drop Behavior
/*!

  This is the base class for user defined drag and drop behaviors.
  This class allows a plugin to override the behavior of drag and drop
  connections from nodes in the hyperGraph/hyperShade to other nodes
  or other UI. These behaviors are defined by the type of relationship
  that is trying to be resolved. These are connectAttrToAttr(),
  connectAttrToNode(), connectNodeToAttr(), connectNodeToNode().  In
  order for the overridden methods to be executed the
  shouldBeUsedFor() method must be overridden in order to decide which
  behavior to use given the source and destination node.
*/
class OPENMAYAUI_EXPORT MPxDragAndDropBehavior
{
public:
	MPxDragAndDropBehavior();
	virtual ~MPxDragAndDropBehavior();

	MPxDragAndDropBehavior( void * init );

	virtual bool shouldBeUsedFor( MObject &sourceNode, MObject &destinationNode,
								  MPlug &sourcePlug, MPlug &destinationPlug);

	virtual MStatus connectAttrToAttr( MPlug &sourcePlug, MPlug &destinationPlug, bool force );

	virtual MStatus connectAttrToNode( MPlug &sourcePlug, MObject &destinationNode, bool force );

	virtual MStatus connectNodeToAttr( MObject &sourceNode, MPlug &destinationPlug, bool force );

	virtual MStatus connectNodeToNode( MObject &sourceNode, MObject &destinationNode, bool force );

	static	const char*	className();

private:
	void   setData( void* );
	void * 	data;

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxDragAndDropBehavior */
