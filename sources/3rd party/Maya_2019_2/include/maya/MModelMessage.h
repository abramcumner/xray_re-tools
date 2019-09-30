#ifndef _MModelMessage
#define _MModelMessage
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
// CLASS:    MModelMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MModelMessage)

//! \ingroup OpenMaya
//! \brief Scene messages. 
/*!
	This class is used to register callbacks for model related messages.

    The addCallback method registers a function that will be executed
    whenever the specified message occurs. An id is returned and is used
	to remove the callback.

    To remove a callback use MMessage::removeCallback.
	All callbacks that are registered by a plug-in must be removed by that
	plug-in when it is unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYA_EXPORT MModelMessage : public MMessage
{
public:
	//! Message types supported by the addCallback method.
	enum Message {
		kActiveListModified	//!< active selection changes
   };

public:
	static MCallbackId	addCallback( Message, MMessage::MBasicFunction,
									 void * clientData = NULL,
									 MStatus * ReturnStatus = NULL );


	//!	Before duplicate callback
	static MCallbackId	addBeforeDuplicateCallback(
								MMessage::MBasicFunction,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//!	After duplicate callback
	static MCallbackId	addAfterDuplicateCallback(
								MMessage::MBasicFunction,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Node added to model message
	static MCallbackId	addNodeAddedToModelCallback(
								const MObject &dagNode,
								MMessage::MNodeFunction,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Node removed from model message
	static MCallbackId	addNodeRemovedFromModelCallback(
								const MObject &dagNode,
								MMessage::MNodeFunction,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static const char* className();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MModelMessage */
