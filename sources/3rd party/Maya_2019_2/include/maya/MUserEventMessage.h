#ifndef _MUserEventMessage
#define _MUserEventMessage
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
// CLASS:    MUserEventMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MUserEventMessage)

//! \ingroup OpenMaya
//! \brief User defined Message Types 
/*!
	This class is used to register user-defined event types, register
	callbacks with the user-defined event types, and to post user-defined
	messages.

	The registerUserEvent and deregisterUserEvent methods allow user event
	types to be created and destroyed.  User events are identified by a unique
	string identifier.

	The addCallback method registers a function that will be executed
	whenever the specified message occurs. An id is returned and is used
	to remove the callback.

	The postUserEvent notifies all registered callbacks of the occurence of the
	user-defined event.

	To remove a callback use MMessage::removeCallback.
	All callbacks that are registered by a plug-in must be removed by that
	plug-in when it is unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYA_EXPORT MUserEventMessage : public MMessage
{
public:

	static MStatus		registerUserEvent( const MString& event );
	static bool			isUserEvent( const MString& event );
	static MStatus		deregisterUserEvent( const MString& event );
	static MCallbackId	addUserEventCallback(
								const MString& event,
								MMessage::MBasicFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
	static MStatus		postUserEvent(
								const MString& event,
								void* clientData = NULL );

	static const char* className();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MUserEventMessage */
