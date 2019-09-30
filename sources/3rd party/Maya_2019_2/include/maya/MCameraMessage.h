#ifndef _MCameraMessage
#define _MCameraMessage
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
// CLASS:    MCameraMessage
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MCameraMessage)
//
//	This class is used to register callbacks for camera interaction.
//  These callbacks allow you to execute commands at the beginning
//  and end of camera interaction.  	
//  
//  There are 2 add callback methods which will add callbacks for the
//  following messages
//  
//      \li <b>Begin Camera Manipulation</b>
//      \li <b>End Camera Manipulation</b>
//  
//  Camera manipulation is triggered by:
//      
//      \li <b>Dolly</b>
//      \li <b>Orbit</b>
//      \li <b>PanZoom</b>
//      \li <b>Roll</b>
//      \li <b>Track</b>
//      \li <b>Tumble</b>
//	
//  The first parameter passed to each of the add callback methods is the
//  cameraShape node that will trigger the callback.
//  
//  Each method returns an id which is used to remove the callback.
//
//  To remove a callback use MMessage::removeCallback.  All callbacks
//  that are registered by a plug-in must be removed by that plug-in
//  when it is unloaded. Failure to do so will result in a fatal error.
//
// ****************************************************************************
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>

// ****************************************************************************
// CLASS DECLARATION (MCameraMessage)

//! \ingroup OpenMaya
//! \brief Dependency node messages. 
/*!
	This class is used to register callbacks for dependency node messages
	of specific dependency nodes.

	There are 2 add callback methods which will add callbacks for the
	following messages

		\li <b>Begin Camera Manipulation</b>
		\li <b>End Camera Manipulation</b>

	Camera manipulation is triggered by:
	    
		\li <b>Dolly</b>
		\li <b>Orbit</b>
		\li <b>PanZoom</b>
		\li <b>Roll</b>
		\li <b>Track</b>
		\li <b>Tumble</b>
		
	The first parameter passed to each of the add callback methods is the
	cameraShape node that will trigger the callback.

    Each method returns an id which is used to remove the callback.

    To remove a callback use MMessage::removeCallback.
	All callbacks that are registered by a plug-in must be removed by that
	plug-in when it is unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYA_EXPORT MCameraMessage : public MMessage
{
public:
	static MCallbackId	addBeginManipulationCallback(
								const MObject& node,
								MMessage::MNodeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );


	static MCallbackId	addEndManipulationCallback(
								const MObject& node,
								MMessage::MNodeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static const char* className();

};

#endif /* __cplusplus */
#endif /* _MCameraMessage */
