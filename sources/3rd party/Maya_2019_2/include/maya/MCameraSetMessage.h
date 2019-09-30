#ifndef _MCameraSetMessage
#define _MCameraSetMessage
//
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
// CLASS:    MCameraSetMessage
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// *****************************************************************************

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

// *****************************************************************************
// CLASS DESCRIPTION (MCameraSetMessage)

//! \ingroup OpenMaya
//! \brief CameraSet messages.
/*!
	This class is used to register callbacks with cameraSet specific event
	types.

	Using the addCallback() methods, a function can be registered that will
	be executed whenever the specific message/event occurs.
	
	To remove a callback, use MMessage::removeCallback(). All callbacks that
	are registered by a plug-in must be removed by that plug-in when it is
	unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYA_EXPORT MCameraSetMessage : public MMessage
{ 
public:
	static MCallbackId	addCameraLayerCallback(
								MMessage::MCameraLayerFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addCameraChangedCallback(
								MMessage::MCameraLayerCameraFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MCameraSetMessage */
