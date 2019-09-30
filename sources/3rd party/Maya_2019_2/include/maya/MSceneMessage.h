#ifndef _MSceneMessage
#define _MSceneMessage
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
// CLASS:    MSceneMessage
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MStringArray.h>
#include <maya/MDeprecate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MSceneMessage)

//! \ingroup OpenMaya
//! \brief Scene messages. 
/*!
  This class is used to register callbacks for scene related messages.

  The addCallback method registers a function that will be executed
  whenever the specified message occurs. An id is returned and is used
  to remove the callback.

  To remove a callback use MMessage::removeCallback.  All callbacks
  that are registered by a plug-in must be removed by that plug-in
  when it is unloaded. Failure to do so will result in a fatal error.
*/
class OPENMAYA_EXPORT MSceneMessage : public MMessage
{
public:
	//! Events to which messages can be attached.
	enum Message {
		//! Called after any operation that changes which files are loaded.
		kSceneUpdate,
		//! Called before a File > New operation.
		kBeforeNew,
		//! Called after a File > New operation.
		kAfterNew,
		//! Called before a File > Import operation.
		kBeforeImport,
		//! Called after a File > Import operation.
		kAfterImport,
		//! Called before a File > Open operation.
		kBeforeOpen,
		//! Called after a File > Open operation.
		kAfterOpen,
		//! Called immediately before any file read operation (open, import or reference) and while reading state (MFileIO::isReadingFile) is true.
		kBeforeFileRead,
		//! Called immediately after any file read operation (open, import or reference) and while reading state (MFileIO::isReadingFile) is true.
		kAfterFileRead,
		//! Call once after any file operation (open, import or reference) with reference edits recorded during the execution of the callback
		//! Note, isReadingFile is false during this callback.
		kAfterSceneReadAndRecordEdits,
		//! Called before a File > Export operation.
		kBeforeExport,
		//! Called after a File > Export operation.
		kAfterExport,
		//! Called before a File > Save (or SaveAs) operation.
		kBeforeSave,
		//! Called after a File > Save (or SaveAs) operation.
		kAfterSave,
		//! Called before a File > Reference operation. Deprecated. Use kBeforeCreateReference/kBeforeLoadReference
		kBeforeReference,
		//! Called after a File > Reference operation. Deprecated. Use kAfterCreateReference/kAfterLoadReference
		kAfterReference,
		//! Called before a File > RemoveReference operation.
		kBeforeRemoveReference,
		//! Called after a File > RemoveReference operation.
		kAfterRemoveReference,
		//! Called before a File > ImportReference operation.
		kBeforeImportReference,
		//! Called after a File > ImportReference operation.
		kAfterImportReference,
		//! Called before a File > ExportReference operation.
		kBeforeExportReference,
		//! Called after a File > ExportReference operation.
		kAfterExportReference,
		//! Called before a File > UnloadReference operation.
		kBeforeUnloadReference,
		//! Called after a File > UnloadReference operation.
		kAfterUnloadReference,

		//! Called before a Software Render begins.
		kBeforeSoftwareRender,
		//! Called after a Software Render ends.
		kAfterSoftwareRender,
		//! Called before each frame of a Software Render.
		kBeforeSoftwareFrameRender,
		//! Called after each frame of a Software Render.
		kAfterSoftwareFrameRender,
		//! Called when an interactive render is interrupted by the user.
		kSoftwareRenderInterrupted,

		//! Called on interactive or batch startup after initialization.
		kMayaInitialized,
		//! Called just before Maya exits.
		kMayaExiting,

		//! Called prior to File > New operation, allows user to cancel action.
		kBeforeNewCheck,
		//! Called prior to File > Open operation, allows user to cancel action.
		kBeforeOpenCheck,
		//! Called prior to File > Save operation, allows user to cancel action.
		kBeforeSaveCheck,
		//! Called prior to File > Import operation, allows user to cancel action.
		kBeforeImportCheck,
		//! Called prior to File > Export operation, allows user to cancel action.
		kBeforeExportCheck,

		//! Called before a File > LoadReference operation.
		kBeforeLoadReference,
		//! Called after a File > LoadReference operation.
		kAfterLoadReference,
		//! Called before a File > LoadReference operation, allows user to cancel action.
		kBeforeLoadReferenceCheck,
		//! Called prior to a File > CreateReference operation, allows user to cancel action. Deprecated. Use kBeforeCreateReferenceCheck
		kBeforeReferenceCheck,
		//! Called prior to a File > CreateReference operation, allows user to cancel action.
		kBeforeCreateReferenceCheck = kBeforeReferenceCheck,

		//! Called prior to a plugin being loaded.
		kBeforePluginLoad,
		//! Called after a plugin is loaded.
		kAfterPluginLoad,
		//! Called prior to a plugin being unloaded.
		kBeforePluginUnload,
		//! Called after a plugin is unloaded.
		kAfterPluginUnload,

		//! Called before a File > CreateReference operation
		kBeforeCreateReference,
		//! Called after a File > CreateReference operation
		kAfterCreateReference,

		//! Called at the start of a File > Export operation, after the export file has become the active file.
		kExportStarted,

		//! Same as kBeforeLoadReference except reference edits are recorded during the execution of the callback
		kBeforeLoadReferenceAndRecordEdits,
		//! Same as kAfterLoadReference except reference edits are recorded during the execution of the callback
		kAfterLoadReferenceAndRecordEdits,
		//! Same as kBeforeCreateReference except reference edits are recorded during the execution of the callback
		kBeforeCreateReferenceAndRecordEdits,
		//! Same as kAfterCreateReference except reference edits are recorded during the execution of the callback
		kAfterCreateReferenceAndRecordEdits,

		//! Last value of the enum.
		kLast
		
   };

public:
	// Register a callback which takes no arguments other than clientData.
	//
	static MCallbackId	addCallback( Message msg,
									 MMessage::MBasicFunction func,
									 void * clientData = NULL,
									 MStatus * ReturnStatus = NULL );

	// Register a callback, which has the option to abort the current
	// operation. This is done by returning a false value in the supplied
	// return code parameter
	//
	static MCallbackId	addCheckCallback( Message msg,
									 MMessage::MCheckFunction func,
									 void * clientData = NULL,
									 MStatus * ReturnStatus = NULL );

	// Register a callback which has the option to abort the current file IO
	// or modify the file being acted on. By returning a false value in the
	// supplied return code parameter the operation can be aborted. By
	// setting the file path stored in the file parameter, the file being
	// acted on can be modified.
	//
	static MCallbackId	addCheckFileCallback( Message msg,
									 MMessage::MCheckFileFunction func,
									 void * clientData = NULL,
									 MStatus * ReturnStatus = NULL );

	// Register a callback which takes a string array argument, in addition
	// to the usual clientData.
	static MCallbackId addStringArrayCallback( Message msg,
									MMessage::MStringArrayFunction func,
									void * clientData = NULL,
									MStatus * ReturnStatus = NULL );

	// Register a callback which takes an MObject and a MFileObject argument, 
	// in addition to the usual clientData.
	//
	static MCallbackId addReferenceCallback( Message msg,
									MMessage::MObjectFileFunction func,
									void * clientData = NULL,
									MStatus * ReturnStatus = NULL );

	// Register a callback, which has the option to abort the current referencing
	// operation. This is done by returning a false value in the supplied
	// return code parameter
	//
	static MCallbackId addCheckReferenceCallback( Message msg,
									MMessage::MCheckObjectFileFunction func,
									void * clientData = NULL,
									MStatus * ReturnStatus = NULL );

	// Register a callback which takes a MConnFailFunction
	//
	static MCallbackId addConnectionFailedCallback(MMessage::MConnFailFunction func,
												   void * clientData = NULL,
												   MStatus * ReturnStatus = NULL );
	
	// Register a callback which notifies about namespace change
	//
	static MCallbackId	addNamespaceRenamedCallback( MMessage::MTwoStringFunction func,
													 void * clientData = NULL,
													 MStatus * ReturnStatus = NULL );

	static const char* className();


BEGIN_NO_SCRIPT_SUPPORT:

	//	Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use the MSceneMessage::addCheckCallback() method instead.")
	static MCallbackId	addCallback( Message, void (*func)( bool* retCode, void* clientData ),
									 void * clientData = NULL,
									 MStatus * ReturnStatus = NULL );

	//	Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use the MSceneMessage::addCheckFileCallback() method instead.")
	static MCallbackId	addCallback( Message, void (*func)( bool* retCode, MFileObject& file, void* clientData ),
									 void * clientData = NULL,
									 MStatus * ReturnStatus = NULL );

	//	Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use the MSceneMessage::addStringArrayCallback() method instead.")
	static MCallbackId addCallback( Message,
									void (*func)( const MStringArray &, void* ),
									void * clientData,
									MStatus * ReturnStatus );

END_NO_SCRIPT_SUPPORT:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MSceneMessage */
