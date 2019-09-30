#ifndef _MCommandMessage
#define _MCommandMessage
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
// CLASS:    MCommandMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MCommandMessage)

//! \ingroup OpenMaya
//! \brief Helper class to handle MEL output. 
/*!
  This class is used to register for a callback for when certain MEL
  command related messages are generated.

  The add callback method returns an id which is used to remove the
  callback.

  To remove a callback use MMessage::removeCallback.

  All callbacks that are registered by a plug-in must be removed by
  that plug-in when it is unloaded. Failure to do so will result in a
  fatal error.

  \note 
  Setting up a callback using the addCommandCallback() method
  may degrade the performance of Maya since the installed callback
  will be invoked repeatedly as MEL operations are processed.
*/
class OPENMAYA_EXPORT MCommandMessage : public MMessage
{
public:
	//! Types of output messages
	enum MessageType {
		kHistory,		//!< Command history
		kDisplay,		//!< String to display unmodified
		kInfo,			//!< General information
		kWarning,		//!< Warning message
		kError,			//!< Error message
		kResult,		//!< Result from a command execution in the command window
		kStackTrace		//!< Stack trace
	};

	enum MProcType {
		kMELProc = 0,
		kMELCommand
	};

	//! \brief Pointer to a message function.
	/*!
	  \param[in] message	The command that was executed.
	  \param[in] messageType	The type of message.
	  \param[in,out] clientData	User defined data passed to the callback function.
	 */
	typedef void (*MMessageFunction)( 
		const MString & message,
		MCommandMessage::MessageType messageType,
		void* clientData );

	//! \brief Pointer to a message filter function.
	/*!
	  \param[in] message		The command that was executed.
	  \param[in] messageType	The type of message.
	  \param[out] filter		Bool that causes Maya to filter this
		message if set to true.
	  \param[in,out] clientData	User defined data passed to the callback function.
	 */
	typedef void (*MMessageFilterFunction)( 
		const MString & message,
		MCommandMessage::MessageType messageType,
		bool& filterOutput,
		void* clientData );

	static MCallbackId	addCommandCallback(
								MMessage::MStringFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addCommandOutputCallback(
								MCommandMessage::MMessageFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MCallbackId	addCommandOutputFilterCallback(
								MCommandMessage::MMessageFilterFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static const char* className();

	static MCallbackId addProcCallback( MMessage::MStringIntBoolIntFunction func,
										void * clientData,
										MStatus * ReturnStatus );
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MCommandMessage */
