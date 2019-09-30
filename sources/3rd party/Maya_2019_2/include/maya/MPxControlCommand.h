#ifndef _MPxControlCommand
#define _MPxControlCommand
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
// CLASS:    MPxControlCommand
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MPxControlCommand)
//
//  This is the command class used for creating proxy UI control.
//
//  The following proxy classes can currently be constructed:
//		Table widget (using MPxUITableControl).
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
// CLASS DECLARATION (MPxControlCommand)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for control creation commands 
/*!

  MPxControlCommand is the base class for user defined UI control
  commands.  This command gives all of the flags and options of the
  base control command in addition to any user defined flags or
  behaviours. When registering this command, use the
  MFnPlugin::registerControlCommand() method. All control commands
  have a corresponding MPxUIControl. It is important to note that a
  given MPxControlCommand is reponsible for only ONE MPxUIControl.
*/
class OPENMAYAUI_EXPORT MPxControlCommand
{
public:
							MPxControlCommand	();
	virtual					~MPxControlCommand	();

	virtual MPxUIControl *	makeControl			() = 0;

	virtual MStatus			doEditFlags			();
	virtual MStatus			doQueryFlags		();
	virtual MStatus			appendSyntax		();
	virtual bool			skipFlagForCreate	(const MString &longFlag);
	void					clearResult			();
	MStatus					setResult			(bool result);
	MStatus					setResult			(int result);
	MStatus					setResult			(double result);
	MStatus					setResult			(const MString &result);
	MStatus					setResult			(const MStringArray &result);
	MStatus					setResult			(const MIntArray &result);

	// SCRIPT USE ONLY
	MPxUIControl *_control( MStatus *ReturnStatus = NULL )
		{ return control( ReturnStatus ); }
	MSyntax	_syntax(MStatus *ReturnStatus = NULL) const
		{ return syntax( ReturnStatus ); }
	MArgParser _parser(MStatus *ReturnStatus = NULL) const
		{ return parser( ReturnStatus ); }
	//

	static const char*		className			();

protected:
	//!	USE _control() IN SCRIPT
	MPxUIControl *control( MStatus *ReturnStatus = NULL );
	//!	USE _syntax() IN SCRIPT
	MSyntax syntax(MStatus *ReturnStatus = NULL) const;
	//!	USE _parser() IN SCRIPT
	MArgParser parser(MStatus *ReturnStatus = NULL) const;

private:
	void setData(void *ptr);

   	void *instance;

	friend class MPxUIControl;
	friend class MPxUITableControl;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxControlCommand */
