#ifndef _MPaintMessage
#define _MPaintMessage
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
// CLASS:    MPaintMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPaintMessage)

//! \ingroup OpenMaya
//! \brief Scene messages. 
/*!
  This class is used to register callbacks for paint related messages.

  The addCallback method registers a function that will be executed
  whenever the specified message occurs. An id is returned and is used
  to remove the callback.

  To remove a callback use MMessage::removeCallback.  All callbacks
  that are registered by a plug-in must be removed by that plug-in
  when it is unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYAUI_EXPORT MPaintMessage : public MMessage
{
public:
	//! \brief Pointer to a message callback which takes a DAG path, an MObject, and a plug.
	/*!
	\param[in] path A DAG path.
	\param[in] object An object, the type of which will depend on the callback using the function.
	\param[in] plug A plug.
	\param[in] colors An array of colors.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MPathObjectPlugColorsFunction)( MDagPath& path, MObject& object, MPlug& plug, MColorArray& colors, void* clientData );

	// Register a callback on vertex paint.
	//
	static MCallbackId	addVertexColorCallback( MPathObjectPlugColorsFunction func,
												 void * clientData = NULL,
												 MStatus * ReturnStatus = NULL );

	static const char* className();

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPaintMessage */
