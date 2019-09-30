#ifndef _MPxContextCommand
#define _MPxContextCommand
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
// CLASS:    MPxContextCommand
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MSyntax.h>
#include <maya/MArgParser.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxContextCommand)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for context creation commands 
/*!
This is the base class for context creation commands.

The purpose of this command class is to create instances of
user contexts derived from <b>MPxContext</b>,
and to allow the MEL programmer to edit and query
various properties of the context related to this command.

The user will derive off of this class and override the <b>makeObj</b> method
which will be called when this command is invoked in Maya.

The context command is registered in Maya
using MFnPlugin::registerContextCommand
*/
class OPENMAYAUI_EXPORT MPxContextCommand
{
public:
							MPxContextCommand	();
	virtual					~MPxContextCommand	();
	virtual MStatus			doEditFlags			();
	virtual MStatus			doQueryFlags		();
	virtual MPxContext *	makeObj				();
	virtual MStatus			appendSyntax		();
	MStatus					setResult			(bool result);
	MStatus					setResult			(int result);
	MStatus					setResult			(double result);
	MStatus					setResult			(const MString &result);

	// SCRIPT USE ONLY
	MSyntax	_syntax(MStatus *ReturnStatus = NULL) const
		{ return syntax(ReturnStatus); }
	MArgParser _parser(MStatus *ReturnStatus = NULL) const
		{ return parser(ReturnStatus); }
	//

	static const char*	className();

protected:
	//!	USE _syntax() IN SCRIPT
	MSyntax	syntax(MStatus *ReturnStatus = NULL) const;
	//!	USE _parser() IN SCRIPT
	MArgParser parser(MStatus *ReturnStatus = NULL) const;

private:
	void setData(void *ptr);

	void *instance;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxContextCommand */

