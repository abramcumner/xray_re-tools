#ifndef _MEventMessage
#define _MEventMessage
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
// CLASS:    MEventMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MStringArray.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MEventMessage)

//! \ingroup OpenMaya
//! \brief Event messages.
/*!
	This class is used to register callbacks for specific events.

	The first parameter passed to the add callback method is the name
	of the event that will trigger the callback.  The list of
	available event names can be retrieved by calling the
	getEventNames method or by using the -listEvents flag on the
	\b scriptJob command.

	The add callback method returns an id which is used to remove the
	callback.

    To remove a callback use MMessage::removeCallback.

	All callbacks that are registered by a plug-in must be removed by
	that plug-in when it is unloaded.  Failure to do so will result in
	a fatal error.
*/
class OPENMAYA_EXPORT MEventMessage : public MMessage
{
public:
	static MCallbackId	addEventCallback(
								const MString& event,
								MMessage::MBasicFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MStatus		getEventNames( MStringArray & names );

	static const char* className();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MEventMessage */
