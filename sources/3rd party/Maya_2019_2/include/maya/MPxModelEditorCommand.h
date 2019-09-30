#ifndef _MPxModelEditorCommand
#define _MPxModelEditorCommand
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
// CLASS:    MPxModelEditorCommand
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

class MPxModelEditor;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxModelEditorCommand)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for editor creation commands 
/*!

  MPxModelEditorCommand is the base class for user defined model
  editor commands.  This command gives all of the flags and options of
  the modelEditor command in addition to any user defined flags or
  behaviours. When registering this command, use the
  MFnPlugin::MFnPlugin::registerModelEditorCommand() method.  A
  MPx3dModelView is also required to be used with
  MPxModelEditorCommand and is specified when the
  MPxModelEditorCommand is registered.
*/
class OPENMAYAUI_EXPORT MPxModelEditorCommand
{
public:
							MPxModelEditorCommand	();
	virtual					~MPxModelEditorCommand	();
	virtual MStatus			doEditFlags			();
	virtual MStatus			doQueryFlags		();
	virtual MStatus			appendSyntax		();
	virtual bool			skipFlagForCreate	(const MString &longFlag);
	MStatus					setResult			(bool result);
	MStatus					setResult			(int result);
	MStatus					setResult			(double result);
	MStatus					setResult			(const MString &result);
	MStatus					setResult			(const MStringArray &result);
	MStatus					setResult			(const MDoubleArray &result);
	MStatus					setResult			(const MIntArray &result);

	virtual MString			editorCommandName() const;	
	virtual	MString			editorMenuScriptName() const;

	virtual MPx3dModelView	*makeModelView		( MStatus *ReturnStatus = NULL );
			MPx3dModelView	*modelView( MStatus *ReturnStatus = NULL ) const;


	// SCRIPT USE ONLY
	MSyntax	_syntax(MStatus *ReturnStatus = NULL) const
		{ return syntax( ReturnStatus ); }
	MArgParser _parser(MStatus *ReturnStatus = NULL) const
		{return parser( ReturnStatus );}
	//

	static const char*		className();

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
#endif /* _MPxModelEditorCommand */
