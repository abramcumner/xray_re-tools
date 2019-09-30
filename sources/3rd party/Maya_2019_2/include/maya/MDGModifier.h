#ifndef _MDGModifier
#define _MDGModifier
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
// CLASS:    MDGModifier
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MDGModifier)
//
// An MDGModifier is used to change the structure of the dependency
// graph.  This includes adding nodes, making new connections, and
// removing existing connections.  An MDGModifier keeps track of all
// operations that it is used for, so that they can be undone or
// redone.  Calling undo causes an MDGModifier to undo all of the
// dependency graph changes that it has done.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDeprecate.h>


// ****************************************************************************
// DECLARATIONS

class TdependNode;
class Tattribute;
class TattributeList;
class Tstring;
class Tmetaclass;
namespace adsk {
	namespace Data {
		class Associations;
	}
}


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDGModifier)

//! \ingroup OpenMaya
//! \brief Dependency graph modifier. 
/*!
 An MDGModifier is used to change the structure of the dependency
 graph.  This includes adding nodes, making new connections, and
 removing existing connections.  To  perform operations using an
 MDGModifier, register all of the changes that are to be made and then
 call the doIt method to make the changes.  Undo is provided through
 the undoIt method.

 Under some situations, a doIt() call may be required to separate
 some of the operations. The specific case of interest involves
 performing a disconnect() and a deleteNode() on the same node
 within the stack of operations.

 Assuming that the Maya model has a connected node that was going
 to be deleted, do the following:

 \code
	MStatus cmd::redoIt()
	{
		MStatus result = dgModifier.disconnect(plugA, plugB);
		if (!result)
			...
		result = dgModifier.doIt();
		if (!result)
			...

		result = dgModifier.deleteNode(plugBNode);
		if (!result)
			...

		result = dgModifier.doIt();
		if (!result)
			...

		return result;
	}
 \endcode

 Then undoIt() can be implemented as:

 \code
	MStatus cmd::undoIt()
	{
		MStatus result = dgModifier.undoIt();
		if (!result)
			...
		return result;
	}
  \endcode

MDGModifier does not understanding the hierarchical relationship between DAG nodes. When performing DG operations on DAG nodes MDagModifier should be used instead. In some cases MDagModifier may even be required when operating on non-DAG nodes. For example, a container node is a non-DAG node but it may contain DAG nodes. When the container is deleted its members will be deleted as well. Since some of those members may be DAG nodes an MDagModifier should be used to delete the container node.
*/
class OPENMAYA_EXPORT MDGModifier
{
public:
	MDGModifier();
	virtual ~MDGModifier();

    MObject     createNode( const MTypeId &typeId,
                            MStatus* ReturnStatus = NULL );
    MObject     createNode( const MString &type,
                            MStatus* ReturnStatus = NULL );
    MStatus     deleteNode( const MObject & node );

    MStatus     renameNode( const MObject & node, const MString &newName );

	MStatus		setNodeLockState( const MObject & node, bool newState );

	MStatus		connect(	const MObject & sourceNode,
							const MObject & sourceAttr,
							const MObject & destNode,
							const MObject & destAttr );
	MStatus		disconnect(	const MObject & sourceNode,
							const MObject & sourceAttr,
							const MObject & destNode,
							const MObject & destAttr );
	MStatus		connect(	const MPlug& source, const MPlug& dest );
	MStatus		disconnect(	const MPlug& source, const MPlug& dest );

    // Dynamic attributes
    MStatus     addAttribute	( const MObject& node, const MObject& attribute );
    MStatus     removeAttribute	( const MObject& node, const MObject& attribute );
    MStatus     renameAttribute	( const MObject& node, const MObject& attribute,
                                  const MString& shortName, const MString& longName );

    // Extension attributes
    MStatus     addExtensionAttribute	( const MNodeClass& nodeClass,
										  const MObject& attribute );
    MStatus     removeExtensionAttribute( const MNodeClass& nodeClass,
										  const MObject& attribute );
    MStatus     removeExtensionAttributeIfUnset( const MNodeClass& nodeClass,
												 const MObject& attribute );
	MStatus		linkExtensionAttributeToPlugin(
										  const MObject& plugin,
										  const MObject& attribute );
	MStatus		unlinkExtensionAttributeFromPlugin(
										  const MObject& mPlugin,
										  const MObject& mAttribute );

	MStatus		commandToExecute	  ( const MString& command );
	MStatus		pythonCommandToExecute( const MString& command );
    
	MStatus		newPlugValue		 ( const MPlug& plug, MObject& plugValue );
	MStatus		newPlugValueBool	 ( const MPlug& plug, bool plugValue );
	MStatus		newPlugValueChar	 ( const MPlug& plug, char plugValue );
	MStatus		newPlugValueDouble	 ( const MPlug& plug, double plugValue );
	MStatus		newPlugValueFloat	 ( const MPlug& plug, float plugValue );
	MStatus		newPlugValueInt		 ( const MPlug& plug, int plugValue );
	MStatus		newPlugValueInt64	 ( const MPlug& plug, MInt64 plugValue );
	MStatus		newPlugValueMAngle	 ( const MPlug& plug, const MAngle& plugValue );
	MStatus		newPlugValueMDistance( const MPlug& plug, const MDistance& plugValue );
	MStatus		newPlugValueMTime	 ( const MPlug& plug, const MTime& plugValue );
	MStatus		newPlugValueShort	 ( const MPlug& plug, short plugValue );
	MStatus		newPlugValueString	 ( const MPlug& plug, const MString& plugValue );

	MStatus		removeMultiInstance	 ( const MPlug& plug, bool breakConnections );

	MStatus		setMetadata		 	 (const MObject& node,
									  const adsk::Data::Associations& metadata );
	MStatus		deleteMetadata		 (const MObject& node);
    
	MStatus		doIt();
	MStatus		undoIt();

	static const  char*	className();

	// Obsolete
    OPENMAYA_DEPRECATED(2019, "Use the method MDGModifier::addAttribute( const MObject&, const MObject& ) instead")
    MStatus     addAttribute	( const MObject& node, const MObject& attribute,
							  	  MFnDependencyNode::MAttrClass type );
	// Obsolete
    OPENMAYA_DEPRECATED(2019, "Use MDGModifier::removeAttribute( const MObject&, const MObject& ) instead")
    MStatus     removeAttribute	( const MObject& node, const MObject& attribute,
							  	  MFnDependencyNode::MAttrClass type );

protected:
    MDGModifier( void * );
	MDGModifier( const MDGModifier & other );
	MDGModifier&	operator =( const MDGModifier & rhs );
	bool		checkCompoundAttrs(const TattributeList&, const Tstring&, const Tstring&);
	bool		attrExistsAtNode(TdependNode*, Tattribute*);
	void*		  data;
	bool		  fOwn;

private:
	MObject		createNodeFromMetaClass( Tmetaclass* typid, MStatus* ReturnStatus = NULL );
	MStatus     pythonCallableToExecute(void * pyObject);
public:
	struct __PythonCallableToExecute {
		//! \brief	Workaround for internal issue.
		//! 		Use pythonCommandToExecute in scripts to invoke python callable.
		inline static MStatus invoke(MDGModifier* modifer, void* callable)
		{
			return modifer->pythonCallableToExecute(callable);
		}
	};
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDGModifier */
