#ifndef _MObjectSetMessage
#define _MObjectSetMessage
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
// ****************************************************************************
//
// CLASS:    MObjectSetMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MObjectSetMessage)

//! \ingroup OpenMaya
//! \brief Object Set Messages. 
/*!
	This class is used to register callbacks for object set messages received
	by specific sets.

	There is 1 add callback method which will add callbacks for the
	following message

		\li <b>Set Members Modified</b>


	The first parameter passed to each of the add callback methods is the
	object set depenency node that will trigger the callback. The second
	parameter specifies the callback function and the optional third parameter
	specifies the callback argument.

	Each method returns an id which is used to remove the callback.

    To remove a callback use MMessage::removeCallback(). All callbacks that
	are registered by a plug-in must be removed by that plug-in when it is
	unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYA_EXPORT MObjectSetMessage : public MMessage
{
public:
	static MCallbackId	addSetMembersModifiedCallback(
								MObject& node,
								MMessage::MNodeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static const char* className();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MObjectSetMessage */
