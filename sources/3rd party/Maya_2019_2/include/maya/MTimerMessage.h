#ifndef _MTimerMessage
#define _MTimerMessage
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
// CLASS:    MTimerMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MTimerMessage)

//! \ingroup OpenMaya
//! \brief Register callbacks that are based on a fixed time interval. 
/*!

  This class is used to register callbacks that are called at fixed
  time intervals.

  To remove a callback use MMessage::removeCallback.

  All callbacks that are registered by a plug-in must be removed by
  that plug-in when it is unloaded.  Failure to do so will result in
  a fatal error.
*/
class OPENMAYA_EXPORT MTimerMessage : public MMessage
{
public:
	//! \brief Pointer to a sleep callback function.
	typedef void (*sleepCallbackPtr)();
	static MCallbackId	addTimerCallback(
								float period,
								MMessage::MElapsedTimeFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static const char* className();

	//! \obsolete
	static MStatus setSleepCallback( sleepCallbackPtr sleepFunc );
	//! \obsolete
	static MTimerMessage::sleepCallbackPtr sleepCallback( );
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MTimerMessage */
