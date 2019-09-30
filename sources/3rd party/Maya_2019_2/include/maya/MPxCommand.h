#ifndef _MPxCommand
#define _MPxCommand
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
// CLASS:    MPxCommand
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MString.h>
#include <maya/MDeprecate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxCommand)

//! \ingroup OpenMaya MPx
//! \brief Base class for user commands. 
/*!
	This is the proxy class for creating MEL commands through the API.

	Each command is derived from this one, and must have a <i>doIt</i>
	method, and optionally <i>undoIt</i>, and <i>redoIt</i> methods.

	The <i>doIt</i> method should collect whatever information is
	required to do the task, and store it in local class data.  It
	should finally call <i>redoIt</i> to make the command happen.  The
	<i>redoIt</i> method should do the actual work, using only the local
	class data.  The <i>undoIt</i> method should undo the actual work,
	again using only the local class data.

	Maya contains support for infinite levels of undo.  If a command
	written in a plug-in changes the state of anything in Maya, it
	should implement <i>undoIt</i> and <i>redoIt</i> methods.  As well,
	if the <i>doIt</i> method returns successfully, Maya will call the
	method's <i>isUndoable</i> method immediately afterwards.  If that
	method returns true, the instance of this class is retained
	and passed to Maya's undo manager so that the <i>undoIt</i> and
	<i>redoIt</i> methods can be called when appropriate.  If
	<i>isUndoable</i> returns false, the command instance is destroyed
	right away.

	So, for example, if a command supports both <i>query</i> and
	<i>edit</i> modes, in query mode the command should set a flag so
	that the <i>isUndoable</i> method returns false to prevent that command
	instance from being retained by the undo manager.  In edit mode, where
	the state of Maya is changed, <i>isUndoable</i> should return true
	to enable undo and redo.
*/
class OPENMAYA_EXPORT MPxCommand
{
public:
						MPxCommand();
	virtual 			~MPxCommand();
	virtual MStatus   	doIt( const MArgList& args );
	virtual MStatus   	undoIt( );
	virtual MStatus   	redoIt( );
	virtual bool		isUndoable() const;
	virtual bool		hasSyntax() const;
	MSyntax				syntax() const;
	bool                isHistoryOn() const;
	MString      		commandString() const;
	MStatus            	setHistoryOn( bool state );
	MStatus            	setCommandString( const MString & );

	static void			displayInfo( const MString & theInfo );

	static void			displayWarning( const MString & theWarning, bool showLineNumber = false );
	static void			displayError( const MString & theError, bool showLineNumber = false );

	//! Types of results from commands.
	enum MResultType {
		kLong,		//!< \nop
		kDouble,	//!< \nop
		kString,	//!< \nop
		kNoArg		//!< \nop
	};

	static void         clearResult();

	static void			setResult( unsigned int val ); 
	static void         setResult( int val );
	static void         setResult( double val );
	static void         setResult( bool val );
BEGIN_NO_SCRIPT_SUPPORT:
	static void         setResult( const char* val );
END_NO_SCRIPT_SUPPORT:
	static void         setResult( const MString& val );
	static void         setResult( const MIntArray& val );
	static void         setResult( const MDoubleArray& val );
	static void         setResult( const MStringArray& val );

	static void         appendToResult( int val );
	static void         appendToResult( double val );
	static void         appendToResult( bool val );
BEGIN_NO_SCRIPT_SUPPORT:
	static void         appendToResult( const char* val );
END_NO_SCRIPT_SUPPORT:
	static void         appendToResult( const MString& val );
	static void         appendToResult( const MStringArray& val );

	static bool isCurrentResultArray();
	static MResultType  currentResultType();
	
BEGIN_NO_SCRIPT_SUPPORT:
	static MStatus      getCurrentResult( int& val );
	static MStatus      getCurrentResult( double& val );
	static MStatus      getCurrentResult( MString& val );
END_NO_SCRIPT_SUPPORT:
	static MStatus      getCurrentResult( MIntArray& val );
	static MStatus      getCurrentResult( MDoubleArray& val );
	static MStatus      getCurrentResult( MStringArray& val );
	static int			currentIntResult( MStatus * ReturnStatus = NULL );
	static double		currentDoubleResult( MStatus * ReturnStatus = NULL );
	static MString		currentStringResult( MStatus * ReturnStatus = NULL );

	static const char*	className();

	// Obsolete
	OPENMAYA_DEPRECATED(2019, "This method has no effect any more on the undoability of a command.")
	MStatus            	setUndoable( bool state );

protected:
	void setData( void * ptr );
	void*				instance;
private:
	MString command;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxCommand */
