#ifndef __MTemplateManipulator_h
#define __MTemplateManipulator_h
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
// Writing manipulator or their contains in some cases require additional
// classes for interacting with Maya. Below are some template classes
// for making the writing of manipulators or their containers easier.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxSelectionContext.h>
#include <maya/MPxContextCommand.h>
#include <maya/MModelMessage.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MTemplateManipulator)

//! \ingroup OpenMayaUI
//! \brief Template class for context commands.
/*!
  Template helper to build command that is used to create context.

  \param[in] ContextCommandName : string name of context command
  \param[in] ContextCommandClass : context command derived from MPxContextCommand
  \param[in] ContextClass : context derived from MPxSelectionContext
*/
template <const char *ContextCommandName, class ContextCommandClass, class ContextClass>
class MTemplateContextCommand : public MPxContextCommand
{
public:
	//! Constructor
	MTemplateContextCommand() {}

	//! Command function for make the context
	virtual MPxContext* makeObj()
	{
		return new ContextClass();
	}

	//! Creator of the context command
	static void* creator()
	{
		return new ContextCommandClass;
	}

	/*!
	  Plug-in registration method. 

	  \param[in] obj plugin object taken from the initializePlugin()
	  entry point.
	*/
	static MStatus registerContextCommand( MObject obj )
	{
		MFnPlugin plugin( obj );
		return plugin.registerContextCommand(ContextCommandName,
										   creator);
	}

	/*! 
	  Plug-in deregistration method.

	  \param[in] obj plugin object taken from the initializePlugin()
	  entry point.
	*/
	static MStatus deregisterContextCommand( MObject obj )
	{
		MFnPlugin plugin( obj );
		return plugin.deregisterContextCommand(ContextCommandName);
	}
};

//! \ingroup OpenMayaUI
//! \brief Template class for selection context.
/*!
  Template helper to build selection contexts.

  \par Parameters:
  \li ContextNameString : string name of context command
  \li ContextClass : context command derived from MPxContextCommand
  \li NodeType : MFn type that selection context should operate on
  \li ManipulatorClass : derived from MPxManipulatorNode
  \li ManipulatorNodeName : name of the manipulator node
*/
template <const char *ContextNameString, class ContextClass, MFn::Type NodeType, class ManipulatorClass, const char *ManipulatorNodeName>
class MTemplateSelectionContext : public MPxSelectionContext
{
public:
	//! Constructor
	MTemplateSelectionContext()
	{
		MString str("Plug-in manipulator: ");
		str+= ContextNameString;
		setTitleString(str);
		manipulatorClassPtr = 0;
	}

	//! Destructor
	virtual ~MTemplateSelectionContext() {}

	/*! 
	  Sets up a active list modified message which will trigger
	  manipulator create/delete operations.

	  \param[in] event The event hat triggered it.
	*/
	virtual void toolOnSetup(MEvent &event)
	{
		MString str("Move the object using the manipulator");
		setHelpString(str);

		updateManipulators(this);
		MStatus status;
		activeListModifiedMsgId = MModelMessage::addCallback(MModelMessage::kActiveListModified,
										 updateManipulators,
										 this, &status);
		if (!status)
		{
			MGlobal::displayError("Model addCallback failed");
		}
	}

	//! Removes the callback
	virtual void toolOffCleanup()
	{
		MStatus status;
		status = MModelMessage::removeCallback(activeListModifiedMsgId);
		if (!status)
		{
			MGlobal::displayError("Model remove callback failed");
		}
		MPxContext::toolOffCleanup();
	}

	/*!
	  Override to specify which attributes are required on the
	  selected geometry.

	  \param[in] namesOfAttributes Array of attribute names.
	*/
	virtual void namesOfAttributes(MStringArray& namesOfAttributes)
	{
		namesOfAttributes.clear();
	}

	/*! Override to set manipulator initial state.

	\note manipulatorClassPtr and firstObjectSelected will be set on
	entry. manipulatorClassPtr is the manipulator created and
	firstObjectSelected can be used to position the manipulator in the
	correct position.
	*/
	virtual void setInitialState()
	{
		// No-op in default state
	}

	//! Ensure that valid geometry is selected
	bool validGeometrySelected()
	{
		MStatus status = MStatus::kSuccess;

		MSelectionList list;
		status = MGlobal::getActiveSelectionList(list);
		MItSelectionList iter(list, MFn::kInvalid, &status);

		if (MStatus::kSuccess == status)
		{
			for (; !iter.isDone(); iter.next())
			{
				MObject dependNode;
				iter.getDependNode(dependNode);
				if (dependNode.isNull() || !dependNode.hasFn(NodeType))
				{
					MGlobal::displayWarning("Object in selection list is not right type of node");
					return false;
				}

				MFnDependencyNode dependNodeFn(dependNode);
				MStringArray attributeNames;
				namesOfAttributes( attributeNames );
				unsigned int i;
				for ( i = 0; i < attributeNames.length(); i++ )
				{
					MPlug plug = dependNodeFn.findPlug(attributeNames[i], true, &status);
					if ( plug.isNull() )
					{
						MGlobal::displayWarning("Object cannot be manipulated: " +
							dependNodeFn.name());
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}

	/*!
	  Callback that creates the manipulator if valid geometry is
	  selected. Also removes the manipulator if no geometry is
	  selected. Handles connecting the manipulator to multiply
	  selected nodes.

	  \param[in] data Pointer to the current context class.
	*/
	static void updateManipulators(void * data)
	{
		MStatus status = MStatus::kSuccess;

		ContextClass * ctxPtr = (ContextClass *) data;
		ctxPtr->deleteManipulators();

		if ( ! ctxPtr->validGeometrySelected() )
			return;

		// Clear info
		ctxPtr->manipulatorClassPtr = 0;
		ctxPtr->firstObjectSelected = MObject::kNullObj;

		MSelectionList list;
		status = MGlobal::getActiveSelectionList(list);
		MItSelectionList iter(list, MFn::kInvalid, &status);

		if (MStatus::kSuccess == status)
		{
			MString manipName (ManipulatorNodeName);
			MObject manipObject;
			ManipulatorClass* manipulator =
				(ManipulatorClass *) ManipulatorClass::newManipulator(manipName, manipObject);

			if (NULL != manipulator)
			{
				// Save state
				ctxPtr->manipulatorClassPtr = manipulator;
				// Add the manipulator
				ctxPtr->addManipulator(manipObject);
				//
				for (; !iter.isDone(); iter.next())
				{
					MObject dependNode;
					iter.getDependNode(dependNode);
					MFnDependencyNode dependNodeFn(dependNode);
					// Connect the manipulator to the object in the selection list.
					if (!manipulator->connectToDependNode(dependNode))
					{
						MGlobal::displayWarning("Error connecting manipulator to"
							" object: " + dependNodeFn.name());
						continue;
					}
					//
					if ( MObject::kNullObj == ctxPtr->firstObjectSelected )
						ctxPtr->firstObjectSelected = dependNode;
				}

				// Allow the manipulator to set initial state
				ctxPtr->setInitialState();
			}
		}
	}

protected:
	MObject firstObjectSelected;
	ManipulatorClass *manipulatorClassPtr;
private:
	MCallbackId activeListModifiedMsgId;
};
OPENMAYA_NAMESPACE_CLOSE

using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTemplateContextCommand;
using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTemplateSelectionContext;

#endif /* _cplusplus */
#endif /* __MTemplateManipulator_h */

