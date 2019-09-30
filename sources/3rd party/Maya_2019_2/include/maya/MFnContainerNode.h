#ifndef _MFnContainerNode
#define _MFnContainerNode
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
// CLASS:    MFnContainerNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnContainerNode)

//! \ingroup OpenMaya MFn
//! \brief container function set 
/*!
  MFnContainerNode is the function set for creating, querying and
  editing containers.

  Maya uses container nodes to bundle sets of related nodes
  together with a published attribute list that defines the primary
  interface to those nodes. This class allows you to query information
  about container nodes in the Maya scene.
*/
class OPENMAYA_EXPORT MFnContainerNode : public MFnDependencyNode
{
	declareMFn(MFnContainerNode, MFnDependencyNode);

public:
    MStatus		getPublishedPlugs(MPlugArray& publishedPlugs,
								  MStringArray& publishedNames) const;
    MStatus		getPublishedNames(MStringArray& publishedNames,
								  bool unboundOnly) const;
    MStatus		getMembers(MObjectArray& members) const;
    MStatus		getSubcontainers(MObjectArray& members) const;
    MStatus		getParentContainer(MObject& parent) const;
    MStatus		getRootTransform(MObject& root) const;

	//! Specify which type of published node
    enum MPublishNodeType {
		kParentAnchor,	//!< published as parent anchor
		kChildAnchor,	//!< published as child anchor
		kGeneric		//!< published as node (non-anchor)
	};
	
    MStatus		getPublishedNodes(MPublishNodeType type,
								  MStringArray& publishedNames,
								  MObjectArray& nodes) const;

	MStatus 	clear();

	//  Set/clear the current container node to be the node managed by this function set.
	MStatus	makeCurrent(bool isCurrent = true);

	//  Return whether the container node managed by this function set is the current container.
	bool	isCurrent(MStatus *status = NULL);

	// Retrieve the current container node.
	static	MObject getCurrentAsMObject();

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnContainerNode, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnContainerNode */
