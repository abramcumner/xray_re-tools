#ifndef _MNodeMessage
#define _MNodeMessage
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
// CLASS:    MNodeMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MNodeMessage)

//! \ingroup OpenMaya
//! \brief Dependency node messages. 
/*!
	This class is used to register callbacks for dependency node messages
	of specific dependency nodes.

	There are 5 add callback methods which will add callbacks for the
	following messages

		\li <b>Attribute Changed</b>
		\li <b>Attribute Added or Removed</b>
		\li <b>Node Dirty</b>
		\li <b>Name Changed</b>
		\li <b>UUID Changed</b>


	The first parameter passed to each of the add callback methods is the
	dependency node that will trigger the callback.

    Callbacks that are registered for attribute changed/addedOrRemoved messages
    will be passed an AttributeMessage value as a parameter. This value
    indicates the type of attribute message that has occurred. See the
    AttributeMessage enum for all available messages.

	Each method returns an id which is used to remove the callback.

    To remove a callback use MMessage::removeCallback.
	All callbacks that are registered by a plug-in must be removed by that
	plug-in when it is unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYA_EXPORT MNodeMessage : public MMessage
{
public:
	//! The type of attribute changed/addedOrRemoved messages that has occurred
	enum AttributeMessage {
		//! a connection has been made to an attribute of this node
		kConnectionMade			= 0x01,
		//! a connection has been broken for an attribute of this node
		kConnectionBroken		= 0x02,
		//! an attribute of this node has been evaluated
		kAttributeEval			= 0x04,
		//! an attribute value of this node has been set
		kAttributeSet			= 0x08,
		//! an attribute of this node has been locked
		kAttributeLocked		= 0x10,
		//! an attribute of this node has been unlocked
		kAttributeUnlocked 		= 0x20,
		//! an attribute has been added to this node
		kAttributeAdded			= 0x40,
		//! an attribute has been removed from this node
		kAttributeRemoved		= 0x80,
		//! an attribute of this node has had an alias added, removed, or renamed
		kAttributeRenamed		= 0x100,
		//! an attribute of this node has been marked keyable
		kAttributeKeyable		= 0x200,
		//! an attribute of this node has been marked unkeyable
		kAttributeUnkeyable		= 0x400,
		//! the connection was coming into the node
		kIncomingDirection		= 0x800,
		//! an array attribute has been added to this node
		kAttributeArrayAdded	= 0x1000,
		//! an array attribute has been removed from this node
		kAttributeArrayRemoved	= 0x2000,
		//! the otherPlug data has been set
		kOtherPlugSet			= 0x4000,
		//! last value of the enum
		kLast					= 0x8000
	};

	//! Allows you to prevent attributes from becoming (un)keyable.
	enum KeyableChangeMsg
	{
		kKeyChangeInvalid = 0,	//!< \nop
		kMakeKeyable,		//!< \nop
		kMakeUnkeyable,		//!< \nop
		kKeyChangeLast		//!< \nop
	};

	//! \brief Pointer to an AttributeMessage callback which takes two plugs.
	/*!
	\param[in] msg Type of message which caused the function to be called.
	\param[in,out] plug First plug. Meaning depends upon the specific message which invoked the callback.
	\param[in,out] otherPlug Second plug. Meaning depends upon the specific message which invoked the callback.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MAttr2PlugFunction)( MNodeMessage::AttributeMessage msg,MPlug & plug,MPlug & otherPlug,void* clientData );

	//! \brief Pointer to an AttributeMessage callback which takes a single plug.
	/*!
	\param[in] msg Type of message which caused the function to be called.
	\param[in,out] plug Meaning depends upon the specific message which invoked the callback.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MAttrPlugFunction)( MNodeMessage::AttributeMessage msg,MPlug & plug,void* clientData 	);

	//! \brief Pointer to a change in keyability callback function.
	/*!
	\param[in,out] plug The plug whose keyability is changing.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	\param[in] msg Specifies how the plug's keyability is changing.
	\param[out] decision Callback sets this true to accept the change, false to reject it.
	*/
	typedef void (*MKeyableFunction)( MPlug &plug,void *clientData, MNodeMessage::KeyableChangeMsg msg, bool &decision );

public:
	static MCallbackId	addAttributeChangedCallback(
								MObject& node,
								MNodeMessage::MAttr2PlugFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addAttributeAddedOrRemovedCallback(
								MObject& node,
								MNodeMessage::MAttrPlugFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addNodeDirtyCallback(
								MObject& node,
								MMessage::MNodeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addNodeDirtyPlugCallback(
								MObject& node,
								MMessage::MNodePlugFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addNameChangedCallback(
								MObject& node,
								MMessage::MNodeStringFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addUuidChangedCallback(
								MObject& node,
								MMessage::MNodeUuidFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addNodeAboutToDeleteCallback(
								MObject& node,
								MMessage::MNodeModifierFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addNodePreRemovalCallback(
								MObject& node,
								MMessage::MNodeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//!	NodeDestroyed callback
	static MCallbackId	addNodeDestroyedCallback(
								MObject& node,
								MMessage::MBasicFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Attribute keyable state change override
	static MCallbackId	addKeyableChangeOverride(
								MPlug& plug,
								MNodeMessage::MKeyableFunction func,
								void *clientData = NULL,
								MStatus *status = NULL );

	static const char* className();

BEGIN_NO_SCRIPT_SUPPORT:
	// Obsolete, no script support
    OPENMAYA_DEPRECATED(2019, "Use the version which takes a callback function of type MMessage::MNodeFunction instead.")
	static MCallbackId	addNodeDirtyCallback(
								MObject& node,
								MMessage::MBasicFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Obsolete, no script support
	OPENMAYA_DEPRECATED(2019, "Use the MNodeMessage::addNodeDirtyPlugCallback method instead.")
	static MCallbackId	addNodeDirtyCallback(
								MObject& node,
								MMessage::MNodePlugFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Obsolete, no script support
	OPENMAYA_DEPRECATED(2019, "Use the version which takes a callback function of type MMessage::MNodeStringFunction instead.")
	static MCallbackId	addNameChangedCallback(
								MObject& node,
								MMessage::MNodeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	// Obsolete, no script support
	OPENMAYA_DEPRECATED(2019, "Use the version which takes a callback function of type MMessage::MNodeModifierFunction instead.")
	static MCallbackId	addNodeAboutToDeleteCallback(
								MObject& node,
								MMessage::MModifierFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

END_NO_SCRIPT_SUPPORT:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MNodeMessage */
