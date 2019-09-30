#ifndef _MAnimMessage
#define _MAnimMessage
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
// CLASS:    MAnimMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAnimMessage)

//! \ingroup OpenMayaAnim
//! \brief Animation messages 
/*!
	This class is used to register callbacks for animation messages.

    To remove a callback use MMessage::removeCallback.
	All callbacks that are registered by a plug-in must be removed by that
	plug-in when it is unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYAANIM_EXPORT MAnimMessage : public MMessage
{
public:
	//! AnimCurve edited callback
	static MCallbackId	addAnimCurveEditedCallback (
								MMessage::MObjArray func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);

	//! AnimCurve keyframe edited callback.
	static MCallbackId	addAnimKeyframeEditedCallback (
								MMessage::MObjArray func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! AnimCurve keyframe edited callback.
	static MCallbackId  addNodeAnimKeyframeEditedCallback (
			                    MObject &animNode,
								MMessage::MNodeObjArray func,
								void *clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! AnimCurve keyframe edit check callback.
	static MCallbackId  addAnimKeyframeEditCheckCallback (
								MMessage::MCheckPlugFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Pre Bake Simulation callback.
	static MCallbackId  addPreBakeResultsCallback (
								MMessage::MPlugsDGModFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Post Bake Simulation callback.
	static MCallbackId  addPostBakeResultsCallback (
								MMessage::MPlugsDGModFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//! Disable Implicit Control callback.
	static MCallbackId  addDisableImplicitControlCallback (
								MMessage::MPlugsDGModFunction func,
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
BEGIN_NO_SCRIPT_SUPPORT:

	// Obsolete, no script support
	static MCallbackId  addAnimKeyframeEditedCallback (
			                    MObject &animNode,
								MMessage::MNodeObjArray func,
								void *clientData = NULL,
								MStatus * ReturnStatus = NULL );

END_NO_SCRIPT_SUPPORT:

	//! AnimCurve keyframe edited callback flush.
	static void			flushAnimKeyframeEditedCallbacks();

	static const char* className();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAnimMessage */
