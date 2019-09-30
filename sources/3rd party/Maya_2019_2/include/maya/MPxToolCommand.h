#ifndef _MPxToolCommand
#define _MPxToolCommand
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
// CLASS:    MPxToolCommand
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MPxCommand.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxToolCommand)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for interactive tool commands
/*!
This is the base class for interactive tool commands.

An interactive tool command is a command that can be invoked as a MEL command
or from within a user defined context (see MPxContext).

Tool commands have the same functionality as MPxCommands, but include several
additional methods for use in interactive contexts: <i>setUpInteractive</i>,
<i>cancel</i>, <i>finalize</i>, and <i>doFinalize</i>.
*/
class OPENMAYAUI_EXPORT MPxToolCommand : public MPxCommand
{
public:
	MPxToolCommand(); // Called within a context

	virtual ~MPxToolCommand();

	//
	virtual MStatus   	doIt( const MArgList& args ) = 0;

	// The following are the methods for interactive use of a
	// MPxToolCommand object
	virtual MStatus 	cancel();
	virtual MStatus 	finalize();

	// SCRIPT USE ONLY
	MStatus _doFinalize( MArgList & command )
		{ return doFinalize( command ); }
	//

	static	const char*	className();

protected:
	//!	USE _doFinalize() IN SCRIPT
	MStatus				doFinalize( MArgList & command );
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxToolCommand */
