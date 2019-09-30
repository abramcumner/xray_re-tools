#ifndef __MTemplateCommand_h
#define __MTemplateCommand_h
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
// CLASS:    MTemplateCommand
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MPxCommand.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>
#include <maya/MSyntax.h>
#include <maya/MDagModifier.h>
#include <maya/MObjectHandle.h>
#include <maya/MArgDatabase.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS

// ****************************************************************************
// CLASS DECLARATION (MTemplateCommand)

//! \ingroup OpenMaya
//! \brief Template class for non undoable actions.
/*!
  Helper template class which make writing not undoable actions easier.

  \par Parameters:
  \li ActionClass : derived from MPxCommand and used in the creator() method
  \li CommandName : name of the command
  \li CommandSyntax : function pointer to syntax creator
*/
template <class ActionClass, const char* CommandName, MCreateSyntaxFunction CommandSyntax >
class MTemplateAction : public MPxCommand
{
public:
	//! Default constructor
	MTemplateAction()
	{
	}

	//! Execution of the action. Default behaviour is failure.
	virtual MStatus	doIt ( const MArgList& )
	{
		return MStatus::kFailure;
	}

	//! Method called by Maya to create an instance of this class.
	static void* creator()
	{
		return new ActionClass;
	}

	/*! Plug-in registration method.

	\param[in] obj Plug-in object taken from the initializePlugin()
	entry point.
 */
	static MStatus registerCommand( MObject obj )
	{
		MFnPlugin plugin( obj );
		return plugin.registerCommand( CommandName, creator, CommandSyntax );
	}

	/*! Plug-in dregistration method.

	\param[in] obj Plug-in object taken from the initializePlugin()
	entry point.
	*/
	static MStatus deregisterCommand( MObject obj )
	{
		MFnPlugin plugin( obj );
		return plugin.deregisterCommand( CommandName );
	}
};
//! \ingroup OpenMaya
//! \brief Template class for undoable commands.
/*!
  Helper template class which make writing undoable commands easier.

  \par Parameters:
  \li CommandClass : derived from MPxCommand and used in the creator() method
  \li CommandName : name of the command
  \li CommandSyntax : function pointer to syntax creator
*/
template <class CommandClass, const char* CommandName, MCreateSyntaxFunction CommandSyntax>
class MTemplateCommand : public MTemplateAction<CommandClass,CommandName, CommandSyntax>
{
public:
	//! Define the command as undoable
	virtual bool isUndoable() const
	{
		return true;
	}
};

OPENMAYA_NAMESPACE_CLOSE

//
//	Utility strings + functions that template classes
//	may need.
//

// Strings we need for the command
const char *CreateCommandFlag = "-c";
const char *CreateCommandFlagLong = "-create";
const char *DeleteCommandFlag = "-d";
const char *DeleteCommandFlagLong = "-delete";

// Syntax for the command
MSyntax MTemplateCreateNodeCommand_newSyntax()
{
	MSyntax syntax;
	syntax.addFlag( CreateCommandFlag, CreateCommandFlagLong );
	syntax.addFlag( DeleteCommandFlag, DeleteCommandFlagLong );
	return syntax;
}

// Null syntax
MSyntax MTemplateCommand_nullSyntax()
{
	MSyntax syntax;
	return syntax;
}

// Should get duplicate error if used more than once in
// a plug-in binary
MObjectHandle MTemplateCreateNodeCommand_nodeHandle;

OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMaya
//! \brief Template class for undoable commands to create/delete a node.
/*!
  Helper template class which make it easier to write commands to
  create/delete a node.

  The command is tied into an MDGModifier to create a node with undo support.

  \note Only one such command per plug-in binary is allowed because
  MTemplateCreateNodeCommand_nodeHandle is a global variable.

  \par Parameters:
  \li CommandClass : derived from MPxCommand and used in the creator() method
  \li CommandName : name of the command
  \li NodeName : node that should be created
*/
template <class CommandClass, const char *CommandName, const char* NodeName>
class MTemplateCreateNodeCommand : public MTemplateCommand<CommandClass,CommandName,MTemplateCreateNodeCommand_newSyntax/*specialization*/>
{
public:

	/*! Execution of the command.

	\param[in] args command arguments
	*/
	virtual MStatus doIt( const MArgList& args )
	{
		MArgDatabase argData( this->syntax(), args );

		creating = true;
		if ( argData.isFlagSet( CreateCommandFlag ) )
			creating = true;
		else if ( argData.isFlagSet( DeleteCommandFlag ) )
			creating = false;
		else
			return MStatus::kFailure;

		MObject newNode;
		MStatus status;
		if ( creating )
		{
			if ( MTemplateCreateNodeCommand_nodeHandle.isValid() )
			{
				this->displayError("Must match -create/-delete calls");
				return MStatus::kFailure;
			}
			newNode = modifier.createNode( NodeName, MObject::kNullObj, &status );
			MTemplateCreateNodeCommand_nodeHandle = newNode;
		}
		else
		{
			if ( MTemplateCreateNodeCommand_nodeHandle.isValid() )
				status = modifier.deleteNode( MTemplateCreateNodeCommand_nodeHandle.object() );
			 MTemplateCreateNodeCommand_nodeHandle = MObject::kNullObj;
		}

		return redoIt();
	}

	//! Redo the create/delete
	virtual MStatus redoIt()
	{
		MStatus status;
		status = modifier.doIt();
		if ( MStatus::kSuccess == status )
			return MStatus::kSuccess;

		return MStatus::kFailure;
	}

	//! Undo the create/delete
	virtual MStatus undoIt()
	{
		return modifier.undoIt();
	}

private:
	bool creating;
	MDagModifier modifier;
};

OPENMAYA_NAMESPACE_CLOSE
using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTemplateAction;
using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTemplateCommand;
using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTemplateCreateNodeCommand;

#endif /* _cplusplus */
#endif /* __MTemplateCommand_h */

