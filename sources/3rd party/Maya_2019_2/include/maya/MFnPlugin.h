#ifndef _MFnPlugin
#define _MFnPlugin
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
// CLASS:    MFnPlugin
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MApiVersion.h>
#include <maya/MPxNode.h>
#include <maya/MPxData.h>
#include <map>
#include <maya/MDeprecate.h>

#if !defined(MNoPluginEntry)
#ifdef NT_PLUGIN
#include <maya/MTypes.h>
HINSTANCE MhInstPlugin;

extern "C" int __stdcall
DllMain(HINSTANCE hInstance, DWORD /*dwReason*/, void* /*lpReserved*/)
{
	MhInstPlugin = hInstance;
	return 1;
}
#endif // NT_PLUGIN
#endif // MNoPluginEntry

// ****************************************************************************
// DECLARATIONS

#define	PLUGIN_COMPANY "Autodesk"
struct _object;
//! \brief Opaque type used by the Python API to pass Python objects.
typedef struct _object PyObject;


class MPxAttributePluginFactory;


//! \brief Pointer to a function which returns a new instance of an MPxMaterialInformation node.
/*!
 \param[in,out] object Material node (e.g. shader)
*/
typedef MPxMaterialInformation* (*MMaterialInfoFactoryFnPtr) (MObject& object);

//! \brief Pointer to a function which returns a new instance of an MPxBakeEngine.
typedef MPxBakeEngine*	        (*MBakeEngineCreatorFnPtr)();

#ifndef SWIG
//	Make sure those functions have the proper export/visibility
    PLUGIN_EXPORT MStatus initializePlugin(MObject obj);
    PLUGIN_EXPORT MStatus uninitializePlugin(MObject obj);
#endif

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnPlugin)

//! \ingroup OpenMaya MFn
//! \brief Register and deregister plug-in services with Maya. 
/*!
This class is used in the initializePlugin and uninitializePlugin functions
of a Maya plug-in to respectively register and deregister the plug-in's
services (commands, node types, etc) with Maya.  The constructor for this
class must be passed the MObject that Maya provides as an argument to
initializePlugin and uninitializePlugin.

MFnPlugin provides various registration methods for use inside
initializePlugin, when the plug-in is being loaded, and deregistration
methods for use inside uninitializePlugin, when the plug-in is being
unloaded.  In general it is good practice to have uninitializePlugin
deregister the plug-in's services in the reverse order in which they were
registered in initializePlugin. This ensures, for example, that a custom
datatype is not deregistered before the node types which use it.

A plug-in's uninitializePlugin function is only called when the plug-in
is explicitly unloaded. It is <i>not</i> called 
when Maya exits. Normally this is not a problem because system resources
such as memory and open file handles are automatically released by the
operating system when Maya exits. However, if there are cleanup
tasks which a plug-in must perform even when Maya exits, for example
deleting a temporary file, then the plug-in's initializePlugin can use the
MSceneMessage::addCallback method with a message of "kMayaExiting"
to register a callback function that will be executed when Maya is about
to exit. The callback function can then handle any cleanup activities which
the operating system won't handle automatically on exit.

A side effect of including MFnPlugin.h in a source file is to embed an API
version string into the corresponding compiled object file. Because of
this, including MFnPlugin.h in more than one source file in the same plug-in
will lead to conflicts when the plug-in is linked. If it is necessary to
include MFnPlugin.h in more than one of a plug-in's source files the
preprocessor macro <i>MNoVersionString</i> should be defined in all but one
of those files prior to the inclusion of MFnPlugin.h. Normally, this issue
will not arise as only the file that contains the <i>initializePlugin</i>
and <i>uninitializePlugin</i> routines should need to include MFnPlugin.h.

It is unusual, but possible, to instantiate several MFnPlugin objects within
a single plug-in binary. In this case the vendor and version information
that is set for the plug-in is taken from the first instance as this
information works per binary rather than per command/node etc.
*/
class OPENMAYA_EXPORT MFnPlugin : public MFnBase
{
public:
					MFnPlugin();
					MFnPlugin( MObject& object,
							   const char* vendor = "Unknown",
							   const char* version = "Unknown",
							   const char* requiredApiVersion = "Any",
							   MStatus* ReturnStatus = 0L );
	virtual			~MFnPlugin();
	virtual			MFn::Type type() const;

	MString			vendor( MStatus* ReturnStatus=NULL ) const;
	MString			version( MStatus* ReturnStatus=NULL ) const;
	MString			apiVersion( MStatus* ReturnStatus=NULL ) const;
	MString			name( MStatus* ReturnStatus=NULL ) const;
	MString			loadPath( MStatus* ReturnStatus=NULL ) const;
	MStatus			setName( const MString& newName,
							 bool allowRename = true );

	MStatus			setVersion( const MString& newVersion );

	MStatus			registerCommand(const MString& commandName,
									MCreatorFunction creatorFunction,
									MCreateSyntaxFunction
									    createSyntaxFunction = NULL);
	MStatus			deregisterCommand(	const MString& commandName );
	MStatus 		registerControlCommand(const MString& commandName,
										   MCreatorFunction creatorFunction
										   );
	MStatus			deregisterControlCommand(const MString& commandName);
	MStatus 		registerModelEditorCommand(const MString& commandName,
								   		MCreatorFunction creatorFunction,
								   		MCreatorFunction paneCreatorFunction);
	MStatus			deregisterModelEditorCommand(const MString& commandName);
	MStatus 		registerConstraintCommand(const MString& commandName,
								   		MCreatorFunction creatorFunction );
	MStatus			deregisterConstraintCommand(const MString& commandName);
    MStatus         registerContextCommand( const MString& commandName,
											MCreatorFunction creatorFunction );

    MStatus         registerContextCommand( const MString& commandName,
											MCreatorFunction creatorFunction,
											const MString& toolCmdName,
											MCreatorFunction toolCmdCreator,
											MCreateSyntaxFunction
												toolCmdSyntax = NULL
											);

    MStatus         deregisterContextCommand( const MString& commandName );
    MStatus         deregisterContextCommand( const MString& commandName,
											  const MString& toolCmdName );
	MStatus			registerNode(	const MString& typeName,
									const MTypeId& typeId,
									MCreatorFunction creatorFunction,
									MInitializeFunction initFunction,
									MPxNode::Type type = MPxNode::kDependNode,
									const MString* classification = NULL);
	MStatus			deregisterNode(	const MTypeId& typeId );

	MStatus			registerEvaluator(const MString& evaluatorName, 
                                      unsigned int uniquePriority, 
									  MCreatorFunction creatorFunction);
	MStatus			deregisterEvaluator(const MString& evaluatorName);

	MStatus			registerTopologyEvaluator(const MString& evaluatorName, 
                                      unsigned int uniquePriority, 
									  MCreatorFunction creatorFunction);
	MStatus			deregisterTopologyEvaluator(const MString& evaluatorName);

	MStatus			registerShape(	const MString& typeName,
									const MTypeId& typeId,
									MCreatorFunction creatorFunction,
									MInitializeFunction initFunction,
									MCreatorFunction uiCreatorFunction,
									const MString* classification = NULL);
	MStatus			registerTransform(	const MString& typeName,
										const MTypeId& typeId,
										MCreatorFunction creatorFunction,
										MInitializeFunction initFunction,
										MCreateXformMatrixFunction xformCreatorFunction,
										const MTypeId& xformId,
										const MString* classification = NULL);
	MStatus			registerData(	const MString& typeName,
									const MTypeId& typeId,
									MCreatorFunction creatorFunction,
									MPxData::Type type = MPxData::kData );
	MStatus			deregisterData(	const MTypeId& typeId );
	MStatus         registerDevice( const MString& deviceName,
									MCreatorFunction creatorFunction );
	MStatus         deregisterDevice( const MString& deviceName );
	MStatus			registerFileTranslator( const MString& translatorName, 
											const char* pixmapName,
											MCreatorFunction creatorFunction,
											const char* optionsScriptName = NULL,
											const char* defaultOptionsString = NULL,
											bool requiresFullMel = false,
											MString dataStorageLocation = MFnPlugin::kDefaultDataLocation);
	MStatus			deregisterFileTranslator( const MString& translatorName );
	MStatus			registerURIFileResolver( const MString& fileResolverName,
											 const MString& uriScheme,
											 MCreatorFunction creatorFunction );
	MStatus			deregisterURIFileResolver( const MString& fileResolverName );
	MStatus			registerIkSolver( const MString& ikSolverName,
										MCreatorFunction creatorFunction );
	MStatus			deregisterIkSolver( const MString& ikSolverName );

	MStatus			registerCacheFormat( const MString& cacheFormatName,
										MCreatorFunction creatorFunction );
	MStatus			deregisterCacheFormat( const MString& cacheFormatName );

	MStatus			registerUIStrings(MInitializeFunction registerMStringResources,
									  const MString &pluginStringsProc);

	MStatus			registerUI(PyObject * creationProc,
							   PyObject * deletionProc,
							   PyObject * creationBatchProc = NULL,
							   PyObject * deletionBatchProc = NULL);
	MStatus			registerDragAndDropBehavior( const MString& behaviorName,
												 MCreatorFunction creatorFunction);

	MStatus         deregisterDragAndDropBehavior( const MString& behaviorName );

	MStatus			registerImageFile( const MString& imageFormatName,
									   MCreatorFunction creatorFunction,
									const MStringArray& imageFileExtensions);
	MStatus			deregisterImageFile( const MString& imageFormatName);

	MStatus			registerRenderPassImpl( const MString& passImplId,
											MRenderPassDef* passDef,
											MCreatorFunction creatorFunction,
											bool overload=false);
	MStatus			deregisterRenderPassImpl( const MString& passImplId);
	
	MStatus			registerAttributePatternFactory	 (const MString &typeName, MCreatorFunction fnPtr );
	MStatus			deregisterAttributePatternFactory(const MString &typeName);

	MStatus			registerAnimCurveInterpolator( const MString& typeName,
									int typeId,
									MCreatorFunction creatorFunction,
									int flags = 0 );
	MStatus			deregisterAnimCurveInterpolator( const MString& typeName );

	MStatus			registerDisplayFilter(const MString& name, const MString& label, const MString& classification);
	MStatus			deregisterDisplayFilter(const MString& name);

	MStatus			registerRenderer(const MString& name, MCreatorFunction creatorFunction);
	MStatus			deregisterRenderer(const MString& name);


	static MObject  findPlugin( const MString& pluginName );

	static bool		isNodeRegistered(	const MString& typeName);

	MTypeId			matrixTypeIdFromXformId(const MTypeId& xformTypeId, MStatus* ReturnStatus=NULL);

	MStringArray	addMenuItem(
							const MString& menuItemName,
							const MString& parentName,
							const MString& commandName,
							const MString& commandParams,
							bool needOptionBox = false,
							const MString *optBoxFunction = NULL,
							MStatus *retStatus = NULL,
                            const MString *extraMenuItemParams = NULL
							);
	MStatus			removeMenuItem(MStringArray& menuItemNames);
	MStatus			registerMaterialInfo(const MString& type, MMaterialInfoFactoryFnPtr fnPtr );
	MStatus			unregisterMaterialInfo(const MString &typeName);
	MStatus			registerBakeEngine(const MString &typeName, MBakeEngineCreatorFnPtr fnPtr );
	MStatus			unregisterBakeEngine(const MString &typeName);

	static void			setRegisteringCallableScript();
	static bool			registeringCallableScript();

	//	Deprecated Methods

BEGIN_NO_SCRIPT_SUPPORT:
	//!	Obsolete
    OPENMAYA_DEPRECATED(2019, "Use the other version of registerTransform instead.")
	MStatus			registerTransform(	const MString& typeName,
										const MTypeId& typeId,
										MCreatorFunction creatorFunction,
										MInitializeFunction initFunction,
										MCreatorFunction xformCreatorFunction,
										const MTypeId& xformId,
										const MString* classification = NULL);
                                        
	//!     NO SCRIPT SUPPORT
	MStatus			registerUI(const MString & creationProc,
							   const MString & deletionProc,
							   const MString & creationBatchProc = "",
							   const MString & deletionBatchProc = "");
END_NO_SCRIPT_SUPPORT:
	static const char*	className();
    //! Specifies the default storage location for registerFileTranslator, the default value is "data"
    static const MString kDefaultDataLocation;
private:
					MFnPlugin( const MObject& object,
							   const char* vendor = "Unknown",
							   const char* version = "Unknown",
							   const char* requiredApiVersion = "Any",
							   MStatus* ReturnStatus = 0L );
	MFnPlugin&		operator=( const MFnPlugin & );
	MFnPlugin*		operator& () const;
	MFnPlugin*		operator& ();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnPlugin */
