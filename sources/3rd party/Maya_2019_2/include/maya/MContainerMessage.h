#ifndef _MContainerMessage
#define _MContainerMessage
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
// CLASS:    MContainerMessage
//
// ****************************************************************************
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MMessage.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MContainerMessage)

//! \ingroup OpenMaya
//! \brief Container-related messages.
/*!
    The container node in Maya bundles together a group of related nodes.
	The user can then publish attribute names to the container, and bind
	attributes from nodes in the container to the published names.

	MContainerMessage class allows the API user to register callbacks that
	inform them about changes to published attributes on containers.

    To remove a callback use MMessage::removeCallback.  All callbacks
    that are registered by a plug-in must be removed by that plug-in
    when it is unloaded. Failure to do so will result in a fatal
    error.
*/
class OPENMAYA_EXPORT MContainerMessage : public MMessage
{
public:

	static MCallbackId	addPublishAttrCallback(
								MMessage::MNodeStringBoolFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addBoundAttrCallback(
								MMessage::MNodeStringBoolFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static const char* className();

protected:
	// No protected.
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MContainerMessage */
