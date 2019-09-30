#ifndef _MGlobal
#define _MGlobal
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
// CLASS:    MGlobal
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MSelectionMask.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MGlobal)

//! \ingroup OpenMaya
//! \brief Static class providing common API global functions. 
/*!
  Provide methods for selection, 3D-views, time, model manipulation and MEL
  commands.

  MGlobal is a static class which provides access to Maya's model (3d
  graphical database).

  MGlobal provides methods for selection, 3d-views, setting the global
  time, adding to the DAG, and executing MEL commands from within the
  API.  There are multiple methods provided in this class for
  executing MEL within the API.  Plug-ins requiring the execution of
  MEL from the non-main Maya thread must use the
  executeCommandOnIdle() method to avoid program errors.

  MGlobal also provides methods for controlling error logging in the API.
*/
class OPENMAYA_EXPORT MGlobal
{
public:
	static MString      mayaVersion ();

	static int          apiVersion ();
	static int          customVersion();

	static MString		getAbsolutePathToResources();

	//! State in which Maya has been started.
	enum MMayaState	{
		kInteractive,	//!< Running with a UI.
		kBatch,			//!< Running without a UI.
		kLibraryApp,	//!< Running as a standalone (MLibrary) application.
		kBaseUIMode		//!< Running with UI enabled but Maya's std UI scripts not run.
	};

	static MMayaState   mayaState( MStatus * ReturnStatus = NULL );
	static MStatus		getFunctionSetList( MObject forObject,
											MStringArray & array );
	static MStatus		getAssociatedSets( const MSelectionList & list,
											MObjectArray & setArray );
    static MStatus		getSelectionListByName( const MString& name,
												MSelectionList &list );
	//! Operations on selection lists
    enum ListAdjustment {
        kReplaceList,   //!< Totally replace the list with the given items.
		/*!
			Any of the items which are already on the list will be removed.
			Any which are not already on the list will be added to the end
			of the list.
		*/
		kXORWithList,   
        kAddToList,     //!< Add the items to the end of the list.
        kRemoveFromList, //!< Remove the items from the list.
		kAddToHeadOfList //!< Add the items to the beginning of the list.
    };

	//! Choices for types of selection methods used in the selectFromScreen functions.
	enum SelectionMethod {
		kSurfaceSelectMethod,	//!< Select based on the surface of the objects.
		kWireframeSelectMethod	//!< Select based on the wireframe of the objects.
	};

	static MStatus	 	getActiveSelectionList( MSelectionList & dest, bool orderedSelectionIfAvailable = false );
	static MStatus	 	getRichSelection( MRichSelection & dest, bool defaultToActiveSelection = true);
	static MStatus 		getLiveList( MSelectionList & dest );
	static MStatus 		getHiliteList( MSelectionList & dest );
	static MStatus 		setHiliteList( MSelectionList & src );
	static MStatus	 	setActiveSelectionList( const MSelectionList & src,
									   ListAdjustment=kReplaceList);
	static MStatus	 	setRichSelection( const MRichSelection & src );
	static MStatus 		setDisplayCVs( MSelectionList&, bool );
	//
	static MStatus		selectCommand( const MSelectionList & src,
									   ListAdjustment=kReplaceList);
    static MStatus		selectByName( const MString& name,
									  ListAdjustment=kAddToList );
    static MStatus		unselectByName( const MString& name );
    static MStatus		select( MObject& object, ListAdjustment=kAddToList );
    static MStatus		select( const MDagPath& object, const MObject& component,
								ListAdjustment=kAddToList );
    static MStatus		unselect( MObject& object );
    static MStatus		unselect( const MDagPath& object, const MObject& component );
    static MStatus		selectFromScreen( const short& x_pos,
										  const short& y_pos,
                                          ListAdjustment=kAddToList,
										  SelectionMethod=kWireframeSelectMethod);
    static MStatus		selectFromScreen( const short& start_x,
										  const	short& start_y,
                                          const short& end_x,
										  const short& end_y,
                                          ListAdjustment=kAddToList,
										  SelectionMethod=kWireframeSelectMethod);
	static SelectionMethod selectionMethod( MStatus * ReturnStatus = NULL );

	//! Global selection modes
	enum MSelectionMode {
		kSelectObjectMode,		//!< Select objects.
		kSelectComponentMode,	//!< Select components.
		kSelectRootMode,		//!< Select DAG roots.
		kSelectLeafMode,		//!< Select DAG leaves.
		kSelectTemplateMode		//!< Select templated objects.
	};

	static MSelectionMode selectionMode( MStatus * ReturnStatus = NULL );
	static MStatus		  setSelectionMode( MSelectionMode mode );

	static MSelectionMask objectSelectionMask( MStatus * ReturnStatus = NULL );
	static MStatus		  setObjectSelectionMask( const MSelectionMask& mask );
	static MSelectionMask componentSelectionMask( MStatus * ReturnStatus =
												  NULL );
	static MStatus		  setComponentSelectionMask( const MSelectionMask& mask );
	static MSelectionMask animSelectionMask( MStatus * ReturnStatus = NULL );
	static MStatus		  setAnimSelectionMask( const MSelectionMask& mask );
	static MSelectionMask miscSelectionMask( MStatus * ReturnStatus = NULL );
	static MStatus		  setMiscSelectionMask( const MSelectionMask& mask );

	static MStatus		  clearSelectionList();
	static bool			  isSelected( MObject& object,
							  		  MStatus * ReturnStatus = NULL );

	static bool			  trackSelectionOrderEnabled( MStatus * ReturnStatus = NULL );
	static MStatus		  setTrackSelectionOrderEnabled( bool enable );

	static MStatus		getPreselectionHiliteList( MSelectionList& list );
	static MStatus		setPreselectionHiliteList( const MSelectionList& list );

	static MObject		currentToolContext();

	static MStatus		viewFrame( const MTime & time );
	static MStatus      viewFrame( double time);

	static MStatus		sourceFile( const MString& fileName );
	static MStatus 		executeCommandOnIdle( const MString& command,
										bool displayEnabled = false);
	static MStatus 		executeCommand( const MString& command,
										bool displayEnabled = false,
										bool undoEnabled = false);
	static MStatus		executeCommand( const MString& command,
										MCommandResult& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	static MStatus		executeCommand( const MString& command,
										int& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	static MStatus		executeCommand( const MString& command,
										MIntArray& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	static MStatus		executeCommand( const MString& command,
										double& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	static MStatus		executeCommand( const MString& command,
										MDoubleArray& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	static MStatus		executeCommand( const MString& command,
										MStringArray& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	static MString		executeCommandStringResult( const MString& command,
													bool displayEnabled = false,
													bool undoEnabled = false,
													MStatus * ResultStatus = NULL);

BEGIN_NO_SCRIPT_SUPPORT:
	//! \noscript
	static MStatus		executeCommand( const MString& command,
										MString& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	
	
	//! Priority for idle task
	enum MIdleTaskPriority	{
		kHighIdlePriority,		//!< High priority
		kLowIdlePriority,		//!< Low priority
		kVeryLowIdlePriority	//!< Very low priority
	};
	//! Definition for callback function for executeTaskOnIdle()
	typedef void (*MIdleTaskCallback)(void* clientData);
	//! \noscript
	static MStatus		executeTaskOnIdle (	MIdleTaskCallback task,
											void* clientData = NULL,
											MIdleTaskPriority priority = kLowIdlePriority );

	//! \noscript
	static MStatus		executePythonCommand (	const MString& command,
												bool displayEnabled = false,
												bool undoEnabled = false );

	//! \noscript
	static MStatus		executePythonCommand (	const MString& command,
												int& result,
												bool displayEnabled = false,
												bool undoEnabled = false );

	//! \noscript
	static MStatus		executePythonCommand (	const MString& command,
												MIntArray& result,
												bool displayEnabled = false,
												bool undoEnabled = false );

	//! \noscript
	static MStatus		executePythonCommand (	const MString& command,
												double& result,
												bool displayEnabled = false,
												bool undoEnabled = false );

	//! \noscript
	static MStatus		executePythonCommand (	const MString& command,
												MDoubleArray& result,
												bool displayEnabled = false,
												bool undoEnabled = false );
	//! \noscript
	static MString		executePythonCommandStringResult (	const MString& command,
												bool displayEnabled = false,
												bool undoEnabled = false,
												MStatus * ReturnStatus = NULL);
	//! \noscript
	static MStatus		executePythonCommand (	const MString& command,
												MString& result,
												bool displayEnabled = false,
												bool undoEnabled = false );

	//! \noscript
	static MStatus		executePythonCommand (	const MString& command,
												MStringArray& result,
												bool displayEnabled = false,
												bool undoEnabled = false );
	//! \noscript
	static MStatus 		executePythonCommandOnIdle( const MString& command,
												bool displayEnabled = false);
END_NO_SCRIPT_SUPPORT:


    static MStatus      addToModel( MObject & object,
									  MObject & parent = MObject::kNullObj );
    static MStatus      addToModelAt( MObject&, const MVector& point,
								  const double scale[3]  = NULL,
								  const double rotation[3] = NULL,
								  MTransformationMatrix::RotationOrder order =
									  MTransformationMatrix::kXYZ );
    static MStatus      removeFromModel( MObject& );
    static MStatus      deleteNode( MObject& );
    static MStatus      setYAxisUp( bool rotateView = false );
    static MStatus      setZAxisUp( bool rotateView = false );
    static bool			isYAxisUp( MStatus* = NULL );
    static bool			isZAxisUp( MStatus* = NULL );
    static MVector      upAxis( MStatus* = NULL );

	static void			displayInfo( const MString & theMessage );
	static void			displayWarning( const MString & theWarning );
	static void			displayError( const MString & theError );
	static void			setDisableStow( bool state );
	static bool			disableStow( );

	// setting & getting option variable values
	static int			optionVarIntValue(const MString& name, bool *exists = NULL );
	static double		optionVarDoubleValue(const MString& name, bool *exists = NULL);
	static MString		optionVarStringValue(const MString& name, bool *exists = NULL);
	static bool			setOptionVarValue(const MString& name, int value);
	static bool			setOptionVarValue(const MString& name, double value);
	static bool			setOptionVarValue(const MString& name, MString value);

	static void			removeOptionVar(const MString& name);
	static bool			optionVarExists(const MString& name);

//	Support for DEBUG/NDEBUG API
//
	static MString		defaultErrorLogPathName( MStatus* ReturnStatus = NULL );
	static MStatus		resetToDefaultErrorLogPathName();
	static MStatus		setErrorLogPathName( const MString& logPathName );
	static MString		errorLogPathName( MStatus* ReturnStatus = NULL );
	static MStatus		startErrorLogging();
	static MStatus		startErrorLogging( const MString& logPathName );
	static bool			errorLoggingIsOn( MStatus* ReturnStatus = NULL );
	static MStatus		stopErrorLogging();
	static MStatus		closeErrorLog();
	static MStatus		doErrorLogEntry( const MString& errorLogEntry );

	static bool			isUndoing();
	static bool			isRedoing();

	static const char* className();

//	Deprecated methods
//
BEGIN_NO_SCRIPT_SUPPORT:
	//! \obsolete \noscript
	OPENMAYA_DEPRECATED(2019, "Use MGlobal::optionVarIntValue instead.")
	static bool			getOptionVarValue(const MString& name, int& value);
	//! \obsolete \noscript
	OPENMAYA_DEPRECATED(2019, "Use MGlobal::optionVarDoubleValue instead.")
	static bool			getOptionVarValue(const MString& name, double& value);
	//! \obsolete \noscript
	OPENMAYA_DEPRECATED(2019, "Use MGlobal::optionVarStringValue instead.")
	static bool			getOptionVarValue(const MString& name, MString& value);
	//! \obsolete \noscript
	OPENMAYA_DEPRECATED(2019, "	Use the version of MGlobal::select which takes a constant reference")
    static MStatus		select( MDagPath& object, const MObject& component,
								ListAdjustment=kAddToList );
	//! \obsolete \noscript
    OPENMAYA_DEPRECATED(2019, "Use the version of MGlobal::unselect which takes a constant reference to an MDagPath as its first parameter.")
    static MStatus		unselect( MDagPath& object, const MObject& component );
END_NO_SCRIPT_SUPPORT:

private:
	~MGlobal();
#ifdef __GNUC__
	friend class shutUpAboutPrivateDestructors;
#endif
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MGlobal */
