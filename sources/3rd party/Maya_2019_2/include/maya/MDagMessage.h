#ifndef _MDagMessage
#define _MDagMessage
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
// CLASS:    MDagMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MString.h>
#include <maya/MDeprecate.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDagMessage)

//! \ingroup OpenMaya
//! \brief DAG messages. 
/*!
	This class is used to register callbacks for DAG messages.

	There are 6 types of add callback methods which will add callbacks for the
	following messages

		\li <b>Parent Added</b>
		\li <b>Parent Removed</b>
		\li <b>Child Added</b>
		\li <b>Child Removed</b>
		\li <b>Child Reordered</b>
		\li <b>Instance Added</b>
		\li <b>Instance Removed</b>


	Methods exist to register callbacks for every time any DAG node is
	affected as well as methods that work on specific nodes.
	Each method returns an id which is used to remove the callback.

    To remove a callback use MMessage::removeCallback.
	All callbacks that are registered by a plug-in must be removed by that
	plug-in when it is unloaded. Failure to do so will result in a fatal error.

	NOTE: It is possible to get Parent Added and Child Added messages before
	the node name has been set.  This can happen if the node is newly created.
	Additionally, the MDagPath string path names passed to the callback may
	not be set as yet if the node has not been added to the model. Accessing
	objects that are newly created or calling commands that access such 
	objects from a callback may produce unknown results.
*/
class OPENMAYA_EXPORT MDagMessage : public MMessage
{
public:
	//! The type of DAG changed messages that have occurred
	enum DagMessage {
		//! an invalid message was used.
		kInvalidMsg = -1,
		//! a dummy enum used for looping through the message types
		kParentAdded,
		//!	a parent was removed from a DAG node
		kParentRemoved,
		//!	a child was added to a DAG node
		kChildAdded,
		//!	a child was removed from a DAG node
		kChildRemoved,
		//!	a child of a DAG node was reordered
		kChildReordered,
		//!	a DAG node was instanced
		kInstanceAdded,
		//!	a DAG node instance was removed
		kInstanceRemoved,
		//!	last value of the enum
		kLast
	};

        enum MatrixModifiedFlags {
                //      Individual flags
                kScaleX        = 1<<0,  kScaleY        = 1<<1,  kScaleZ        = 1<<2,
                kShearXY       = 1<<3,  kShearXZ       = 1<<4,  kShearYZ       = 1<<5,
                kRotateX       = 1<<6,  kRotateY       = 1<<7,  kRotateZ       = 1<<8,
                kTranslateX    = 1<<9,  kTranslateY    = 1<<10, kTranslateZ    = 1<<11,
                kScalePivotX   = 1<<12, kScalePivotY   = 1<<13, kScalePivotZ   = 1<<14,
                kRotatePivotX  = 1<<15, kRotatePivotY  = 1<<16, kRotatePivotZ  = 1<<17,
                kScaleTransX   = 1<<18, kScaleTransY   = 1<<19, kScaleTransZ   = 1<<20,
                kRotateTransX  = 1<<21, kRotateTransY  = 1<<22, kRotateTransZ  = 1<<23,
                kRotateOrientX = 1<<24, kRotateOrientY = 1<<25, kRotateOrientZ = 1<<26,
                kRotateOrder   = 1<<27,
                //      Composite flags
                kAll           = (1<<28)-1,
                kScale         = kScaleX        | kScaleY        | kScaleZ,
                kShear         = kShearXY       | kShearXZ       | kShearYZ,
                kRotation      = kRotateX       | kRotateY       | kRotateZ,
                kTranslation   = kTranslateX    | kTranslateY    | kTranslateZ,
                kScalePivot    = kScalePivotX   | kScalePivotY   | kScalePivotZ,
                kRotatePivot   = kRotatePivotX  | kRotatePivotY  | kRotatePivotZ,
                kScalePivotTrans=kScaleTransX   | kScaleTransY   | kScaleTransZ,
                kRotatePivotTrans=kRotateTransX | kRotateTransY  | kRotateTransZ,
                kRotateOrient  = kRotateOrientX | kRotateOrientY | kRotateOrientZ
        };

public:

	//! \brief Pointer to a callback function which takes an MDagMessage, two MDagPaths and a clientData pointer.
	/*!
	\param[in] msgType Type of message which caused the function to be called.
	\param[in,out] child First DAG path, usually used to pass the child of a parent/child pair.
	\param[in,out] parent Second DAG path, usually use to pass the parent of a parent/child pair.
	\param[in,out] clientData User-defined data which was supplied when the callback was added.
	*/
	typedef void (*MMessageParentChildFunction)( MDagMessage::DagMessage msgType,MDagPath &child, MDagPath &parent, void * clientData );

	//! \brief Pointer to world matrix modified callback function.
	/*!
	\param[in] transformNode The node whose transformation has changed.
	\param[in] modified The flag which shows what has changed.	
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MWorldMatrixModifiedFunction)(MObject& transformNode, MDagMessage::MatrixModifiedFlags& modified, void *clientData);

	// Parent added callback for all nodes
	static MCallbackId	addParentAddedCallback(
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Parent added callback for a specified node
	static MCallbackId	addParentAddedDagPathCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Parent removed callback for all nodes
	static MCallbackId  addParentRemovedCallback(
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Parent removed callback for a specified node
	static MCallbackId  addParentRemovedDagPathCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Child added callback for all nodes
	static MCallbackId	addChildAddedCallback(
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Child added callback for a specified node
	static MCallbackId	addChildAddedDagPathCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Child removed callback for all nodes
	static MCallbackId	addChildRemovedCallback(
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Child removed callback for a specified node
	static MCallbackId	addChildRemovedDagPathCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Child reordered callback for all nodes
	static MCallbackId	addChildReorderedCallback(
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Child reordered callback for a specified node
	static MCallbackId	addChildReorderedDagPathCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );



	//	Any DAG change callback with the DagMessage for all nodes
	static MCallbackId	addDagCallback(
								DagMessage msgType,
								MDagMessage::MMessageParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//	Any Dag change callback with the DagMessage for a specified node
	static MCallbackId	addDagDagPathCallback(
								MDagPath &node,
								DagMessage msgType,
								MDagMessage::MMessageParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//	Adds a Dag change callback for all known Dag change messages for
	//	all nodes.
	static MCallbackId	addAllDagChangesCallback(
								MDagMessage::MMessageParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//	Adds a Dag change callback for all known Dag change messages for
	//	the specified node.
	static MCallbackId	addAllDagChangesDagPathCallback(
								MDagPath &node,
								MDagMessage::MMessageParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );



	//	Adds a Dag change callback for the instancing of
	//	all the nodes in the DAG
	static MCallbackId	addInstanceAddedCallback(
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );


	//	Adds a Dag change callback for the instancing of
	//	the specified node.
	static MCallbackId	addInstanceAddedDagPathCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//	Adds a Dag change callback for the instance removed/deleted of
	//	all the nodes in the DAG
	static MCallbackId	addInstanceRemovedCallback(
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//	Adds a Dag change callback for the instance removed of
	//	the specified node.
	static MCallbackId	addInstanceRemovedDagPathCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//	Adds a world matrix modified callback for the specified
	//	dag node instance.
	static MCallbackId	addWorldMatrixModifiedCallback(
								MDagPath &node,
								MDagMessage::MWorldMatrixModifiedFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static const char* className();

BEGIN_NO_SCRIPT_SUPPORT:

	//! Obsolete and no script support
    OPENMAYA_DEPRECATED(2019, "Use MDagMessage::addParentAddedDagPathCallback instead.")
	static MCallbackId	addParentAddedCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Obsolete and no script support
    OPENMAYA_DEPRECATED(2019, "Use MDagMessage::addParentRemovedDagPathCallback instead.")
	static MCallbackId  addParentRemovedCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! obsolete and no script support
	OPENMAYA_DEPRECATED(2019, "Use MDagMessage::addChildAddedDagPathCallback instead.")
	static MCallbackId	addChildAddedCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);

	//! Obsolete and no script support
    OPENMAYA_DEPRECATED(2019, "Use MDagMessage::addChildRemovedDagPathCallback instead.")
	static MCallbackId	addChildRemovedCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);

	//! Obsolete and no script support
    OPENMAYA_DEPRECATED(2019, "Use MDagMessage::addChildReorderedDagPathCallback instead.")
	static MCallbackId	addChildReorderedCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Obsolete and no script support
	OPENMAYA_DEPRECATED(2019, "Use MDagMessage::addDagCallback instead.")
	static MCallbackId	addInstanceAddedCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);

	//! Obsolete and no script support
	OPENMAYA_DEPRECATED(2019, "Use MDagMessage::addDagCallback instead.")
	static MCallbackId	addInstanceRemovedCallback(
								MDagPath &node,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);
	//! Obsolete and no script support
	static MCallbackId	addDagCallback(
								DagMessage msgType,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Obsolete and no script support
	static MCallbackId	addDagCallback(
								MDagPath &node,
								DagMessage msgType,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! obsolete and no script support
	OPENMAYA_DEPRECATED(2019, "Use MDagMessage::addDagDagPathCallback instead.")
	static MCallbackId	addDagCallback(
								MDagPath &node,
								DagMessage msgType,
								MDagMessage::MMessageParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Obsolete and no script support
	OPENMAYA_DEPRECATED(2019, "Use MDagMessage::addAllDagChangesDagPathCallback instead.")
	static MCallbackId	addAllDagChangesCallback(
								MDagPath &node,
								MDagMessage::MMessageParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);
END_NO_SCRIPT_SUPPORT:

private:
	static MCallbackId	addDagCallback(
								MDagPath *dagPath,
								DagMessage msgType,
								MMessage::MParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addDagCallback(
								MDagPath *dagPath,
								DagMessage msgType,
								MDagMessage::MMessageParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addDagCallback(
								MDagPath *dagPath,
								MDagMessage::MWorldMatrixModifiedFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addAllDagChangesCallback(
								MDagPath *dagPath,
								MDagMessage::MMessageParentChildFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDagMessage */
