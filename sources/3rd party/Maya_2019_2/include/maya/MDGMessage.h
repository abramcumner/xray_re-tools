#ifndef _MDGMessage
#define _MDGMessage
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
// CLASS:    MDGMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

#define kDefaultNodeType "dependNode"

// ****************************************************************************
// CLASS DECLARATION (MDGMessage)

//! \ingroup OpenMaya
//! \brief Dependency graph messages. 
/*!
	This class is used to register callbacks for dependency graph messages.

	There are 4 add callback methods which will add callbacks for the
	following messages

		\li <b>Time change</b>
		\li <b>Node Added</b>
		\li <b>Node Removed</b>
		\li <b>Connection made or broken</b>


	A filter can be specified for node added/removed messages. The default
    node type is "dependNode" which matches all nodes.
	Each method returns an id which is used to remove the callback.

    To remove a callback use MMessage::removeCallback.
	All callbacks that are registered by a plug-in must be removed by that
	plug-in when it is unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYA_EXPORT MDGMessage : public MMessage
{
public:
	static MCallbackId	addTimeChangeCallback(
								MMessage::MTimeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
	static MCallbackId	addDelayedTimeChangeCallback(
								MMessage::MTimeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
	static MCallbackId	addDelayedTimeChangeRunupCallback(
								MMessage::MTimeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId  addForceUpdateCallback(
								MMessage::MTimeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addNodeAddedCallback(
								MMessage::MNodeFunction func,
								const MString& nodeType = kDefaultNodeType,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addNodeRemovedCallback(
								MMessage::MNodeFunction func,
								const MString& nodeType = kDefaultNodeType,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addConnectionCallback(
								MMessage::MPlugFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addPreConnectionCallback(
								MMessage::MPlugFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addNodeChangeUuidCheckCallback(
								MMessage::MCheckNodeUuidFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

private:
	static const char* className();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDGMessage */
