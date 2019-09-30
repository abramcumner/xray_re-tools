#ifndef _MConditionMessage
#define _MConditionMessage
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
// CLASS:    MConditionMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MConditionMessage)

//! \ingroup OpenMaya
//! \brief Condition change messages.
/*!
	This class is used to register callbacks for changes to specific
    conditions.

	The addConditionCallback method will add callback a callback for
    condition changed messages.

	The first parameter passed to the addConditionCallback method is
	the name of the condition that will trigger the callback.  The
	list of available condition names can be retrieved by calling the
	getConditionNames method or by using the -listConditions flag on
	the \b scriptJob command.

    Callbacks that are registered for conditions will be passed a
    bool value as a parameter.  This value indicates the new state of
    the condition.

	The addConditionCallback method returns an id which is used to
	remove the callback.

    To remove a callback use MMessage::removeCallback.

	All callbacks that are registered by a plug-in must be removed by
	that plug-in when it is unloaded. Failure to do so will result in
	a fatal error.

	The getConditionState method is used to return the current state
	of the specified condition.
*/
class OPENMAYA_EXPORT MConditionMessage : public MMessage
{
public:
	static MCallbackId	addConditionCallback(
								const MString& conditionName,
								MMessage::MStateFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	static MStatus		getConditionNames( MStringArray & names );

	static bool			getConditionState( const MString& name,
										   MStatus * ReturnStatus = NULL );

	static const char* className();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MConditionMessage */
