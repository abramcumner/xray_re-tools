#ifndef _MPolyMessage
#define _MPolyMessage
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
// CLASS:    MPolyMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPolyMessage)

//! \ingroup OpenMaya
//! \brief Polygon component id change messages. 
/*!
	This class is used to register callbacks for poly component
	id modification messages.

	There is 1 add callback method which will add callbacks for the
	following messages:

		\li <b>vertex component id modified</b>
		\li <b>edge component id modified</b>
		\li <b>face component id modified</b>

    To remove a callback use MMessage::removeCallback.
	All callbacks that are registered by a plug-in must be removed by that
	plug-in when it is unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYA_EXPORT MPolyMessage : public MMessage
{
public:
	//! Values passed in addPolyComponentIdChangeCallback's
	//! "wantIdModifications" array to indicate which component
	//! id changes should trigger the callback.
	enum
	{
		kVertexIndex = 0,	//!< vertex component id changed
		kEdgeIndex = 1,		//!< edge component id changed
		kFaceIndex = 2,		//!< face component id changed
		kLastErrorIndex = 3	//!< \nop
	};

	//! Types of UV set change messages
	enum MessageType {
		kUVSetAdded,			//!< new UV set added
		kUVSetDeleted,			//!< UV set deleted
		kCurrentUVSetChanged	//!< current UV set changed
	};

	//! Types of color set change messages
	enum ColorSetMessageType {
		kColorSetAdded,			//!< new Color set added
		kColorSetDeleted,		//!< Color set deleted
		kCurrentColorSetChanged	//!< current Color set changed
	};

	//! \brief Pointer to a callback function which takes a node, a string and message type.
	/*!
	\param[in,out] node			The node for which this callback was registered.
	\param[in] name				The meaning depends upon the specific message type for which the callback was registered.
	\param[in] type				The type of message.
	\param[in,out] clientData	Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MNodeStringTypeFunction)(
		MObject & node, 
		const MString & name, 
		MPolyMessage::MessageType type, 
		void* clientData );

	//! \brief Pointer to a callback function which takes a node, a string and message type.
	/*!
	\param[in,out] node			The node for which this callback was registered.
	\param[in] name				The meaning depends upon the specific message type for which the callback was registered.
	\param[in] type				The type of color set message.
	\param[in,out] clientData	Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MNodeStringTypeColorSetFunction)(
		MObject & node, 
		const MString & name, 
		MPolyMessage::ColorSetMessageType type, 
		void* clientData );

	static MCallbackId	addPolyComponentIdChangedCallback (
								MObject& node,
								bool wantIdModifications[],
								unsigned int count,
								MMessage::MComponentFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);

	static MCallbackId	addPolyTopologyChangedCallback (
								MObject& node,
								MMessage::MNodeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);

	static MCallbackId	addUVSetChangedCallback (
								MObject& node,
								MPolyMessage::MNodeStringTypeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);

	static MCallbackId	addColorSetChangedCallback (
								MObject& node,
								MPolyMessage::MNodeStringTypeColorSetFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);

	static unsigned int deletedId( void );

	static const char* className();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPolyMessage */
