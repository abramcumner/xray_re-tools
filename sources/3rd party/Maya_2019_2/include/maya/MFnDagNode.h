#ifndef _MFnDagNode
#define _MFnDagNode

// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

// ****************************************************************************
//
// CLASS:    MFnDagNode
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <stdlib.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnDagNode)

//! \ingroup OpenMaya MFn
//! \brief DAG Node Function Set.
/*!
Provides methods for attaching Function Sets to, querying, and adding
children to DAG Nodes. Particularly useful when used in conjunction
with the DAG Iterator class (MItDag).

The MFnDagNode function set is used to query and set the attributes of
nodes in the DAG (Directed Acyclic Graph).  The DAG is a graph that
describes the hierarchy of the objects in the model.  At each level of
the hierarchy there is a four by four transformation matrix that
affects all of the objects beneath it in the DAG.

In the DAG, geometry objects (also known as shapes) do not have
transformation information associated with them.  Only transform
nodes have actual transform matrices (see MFnTransform).

Each node that exists in the DAG is also a dependency node that
exists in the dependency graph.  This makes it possible to control
the attributes of a DAG node based on calculations performed by
dependency nodes.

There are two ways to specify a DAG node in Maya.  The first is
to use an MObject handle, which acts as a pointer to a specific
node in the DAG.  Given only an MObject, it is not possible to do
world space operations on a DAG node because there may be more
than one path through the DAG to any given node.  In other words,
it is not possible to identify a particular instance only given
an MObject.

In many cases it is preferable to use a DAG path (MDagPath) to specify
a DAG node.  A DAG path always refers to a specific instance of an
object.  This makes it possible to perform unambiguous world space
transformations.

It is also possible to iterate over the nodes in the DAG using a
DAG iterator (MItDag).
*/
class OPENMAYA_EXPORT MFnDagNode : public MFnDependencyNode
{
	declareMinimalMFn( MFnDagNode );

public:

	//! Anonymous enum to store constant values.
	enum {
		//! used by addChild to indicate the next empty position in the list.
		kNextPos = 0xff
	};

	//! Enum to set how an object is colored
	enum MObjectColorType {
		kUseDefaultColor = 0,	//!< Use preference color
		kUseIndexColor,			//!< Use index color
		kUseRGBColor			//!< Use RGB color
	};

	MFnDagNode();
	MFnDagNode( MObject & object, MStatus * ret = NULL );
	MFnDagNode( const MDagPath & object, MStatus * ret = NULL );

    MObject         create( const MTypeId &typeId,
							MObject &parent = MObject::kNullObj,
							MStatus* ReturnStatus = NULL );
    MObject         create( const MTypeId &typeId,
							const MString &name,
							MObject &parent = MObject::kNullObj,
							MStatus* ReturnStatus = NULL );

    MObject         create( const MString &type,
							MObject &parent = MObject::kNullObj,
							MStatus* ReturnStatus = NULL );
    MObject         create( const MString &type,
							const MString &name,
							MObject &parent = MObject::kNullObj,
							MStatus* ReturnStatus = NULL );

	unsigned int	parentCount( MStatus * ReturnStatus = NULL ) const;
	MObject 		parent( unsigned int i,
							MStatus * ReturnStatus = NULL ) const;
	MStatus	        addChild( MObject & child, unsigned int index = kNextPos,
							  bool keepExistingParents = false );
	MStatus			removeChild( MObject & child );
	MStatus			removeChildAt( unsigned int index );
	unsigned int    childCount(  MStatus * ReturnStatus = NULL ) const;
	MObject 	    child( unsigned int i,
						   MStatus * ReturnStatus = NULL ) const;
	MObject  		dagRoot( MStatus * ReturnStatus = NULL );
	bool			hasParent( const MObject & node,
							   MStatus * ReturnStatus = NULL ) const;
	bool			hasChild (const MObject& node,
							  MStatus * ReturnStatus = NULL ) const;
	bool			isChildOf (const MObject& node,
							   MStatus * ReturnStatus = NULL ) const;
	bool			isParentOf (const MObject& node,
								MStatus * ReturnStatus = NULL ) const;
	bool			inUnderWorld ( MStatus * ReturnStatus = NULL ) const;
	bool			inModel ( MStatus * ReturnStatus = NULL ) const;
	bool			isInstanceable( MStatus* ReturnStatus=NULL ) const;
	MStatus			setInstanceable( const bool how );
	bool			isInstanced( bool indirect = true,
						           MStatus * ReturnStatus = NULL ) const;
	bool			isInstancedAttribute( const MObject& attr,
									MStatus * ReturnStatus = NULL ) const;
	unsigned int	instanceCount( bool total,
						           MStatus * ReturnStatus = NULL ) const;
	MObject			duplicate( bool instance = false,
					           bool instanceLeaf = false,
					           MStatus * ReturnStatus = NULL ) const;

	// BUG 301927: Making path methods const --RJ
	MStatus		    getPath( MDagPath& path ) const;
	MStatus		    getAllPaths( MDagPathArray& paths ) const;
	MString         fullPathName(MStatus *ReturnStatus = NULL) const;
    MString         partialPathName(MStatus *ReturnStatus = NULL) const;

	MMatrix			transformationMatrix( MStatus * ReturnStatus = NULL ) const;

	bool            isIntermediateObject( MStatus * ReturnStatus = NULL ) const;
	MStatus         setIntermediateObject( bool isIntermediate );

	OPENMAYA_DEPRECATED(2019, "Use MFnDagNode::objectColorIndex() and MFnDagNode::objectColorRGB() instead")
	int				objectColor( MStatus * ReturnStatus = NULL ) const;
	MStatus			setObjectColor( int color );
	bool			usingObjectColor ( MStatus * ReturnStatus = NULL ) const;
    OPENMAYA_DEPRECATED(2019, "Use MFnDagNode::setObjectColorType() instead")
	MStatus			setUseObjectColor( bool useObjectColor );

	MObjectColorType objectColorType ( MStatus * ReturnStatus = NULL ) const;
	MStatus			 setObjectColorType ( MObjectColorType type );


	// RGB wireframe functions
	MStatus			setObjectColor( const MColor& color );
	MColor			objectColorRGB( MStatus * ReturnStatus = NULL ) const;
	int				objectColorIndex( MStatus * ReturnStatus = NULL ) const;

	MColor			hiliteColor( MStatus * ReturnStatus = NULL ) const;
	bool			usingHiliteColor( MStatus * ReturnStatus = NULL ) const;

	MColor			dormantColor( MStatus * ReturnStatus = NULL ) const;
	MColor			activeColor( MStatus * ReturnStatus = NULL ) const;

	bool			drawOverrideEnabled( MStatus * ReturnStatus = NULL ) const;
	bool			drawOverrideIsReference( MStatus * ReturnStatus = NULL ) const;
	bool			drawOverrideIsTemplate( MStatus * ReturnStatus = NULL ) const;
	bool			drawOverrideColor( MColor& color, MStatus * ReturnStatus = NULL ) const;

	MStatus			getConnectedSetsAndMembers(
							unsigned int instanceNumber,
							MObjectArray & sets,
							MObjectArray & comps,
							bool renderableSetsOnly ) const;

	MBoundingBox	boundingBox( MStatus * ReturnStatus = NULL ) const;

	MDagPath		dagPath( MStatus * ReturnStatus = NULL ) const;
	virtual MStatus setObject( const MDagPath & path );
 	virtual MStatus setObject( MObject & object ) /* override */;

	// Obsolete
    OPENMAYA_DEPRECATED(2019, "The model returned is an unknown type to the API and now the method always returns MObject::kNullObj.")
	MObject         model( MStatus * ReturnStatus = NULL ) const;

	// Internal
	MStatus			objectGroupComponent( unsigned int groupId,

										  MStringArray & );

BEGIN_NO_SCRIPT_SUPPORT:

    //!	No script support
	MFnDagNode( const MObject & object, MStatus * ret = NULL );

	//!	No script support
 	virtual MStatus setObject( const MObject & object );

END_NO_SCRIPT_SUPPORT:

protected:

	void * f_path;
	void * f_xform;

	void * f_data1;
	void * f_data2;

private:
// No private members
};

#define declareDagMFn( MFnClass, MFnParentClass )			  	 	\
	declareMinimalMFn( MFnClass );								 	\
	public:	        											 	\
		MFnClass();											     	\
		MFnClass( MObject & object, MStatus * ret = NULL );	   	 	\
		MFnClass( const MDagPath & object, MStatus * ret = NULL )

#define declareDagMFnConstConstructor( MFnClass, MFnParentClass )	\
		MFnClass( const MObject & object, MStatus * ret = NULL )

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MFnDagNode */
