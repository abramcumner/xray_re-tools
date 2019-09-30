#ifndef _MArgDatabase
#define _MArgDatabase
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
// CLASS:    MArgDatabase
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MArgParser.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MArgDatabase)

//! \ingroup OpenMaya
//! \brief Command argument list parser.
/*!
  This class extends MArgParser by providing the ability to retrieve flag
  parameters, command arguments and command objects as selection lists.

  See MArgParser for more details on how command argument parsing works
  in general.
*/
class OPENMAYA_EXPORT MArgDatabase : public MArgParser
{
public:

			MArgDatabase		();

			MArgDatabase		(const MSyntax &syntax,
								 const MArgList &argList,
								 MStatus *ReturnStatus = NULL);

	virtual ~MArgDatabase();


	MStatus getFlagArgument		(const char *flag, unsigned int index,
								 MSelectionList &result) const;

	MStatus	getCommandArgument	(unsigned int index, MSelectionList &result) const;

	MStatus	getObjects			(MSelectionList &result) const;

BEGIN_NO_SCRIPT_SUPPORT:
	// Redeclarations of overloaded methods from MArgParser
	//! \noscript
	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 bool &result) const;
	//! \noscript
	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 int &result) const;
	//! \noscript
	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 double &result) const;
	//! \noscript
	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MString &result) const;
	//! \noscript
	MStatus getFlagArgument		(const char *flag, unsigned int index,
								 unsigned int &result) const;
	//! \noscript
	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MDistance &result) const;
	//! \noscript
	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MAngle &result) const;
	//! \noscript
	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MTime &result) const;
	//! \noscript
	MStatus	getCommandArgument	(unsigned int index, bool &result) const;
	//! \noscript
	MStatus	getCommandArgument	(unsigned int index, int &result) const;
	//! \noscript
	MStatus	getCommandArgument	(unsigned int index, double &result) const;
	//! \noscript
	MStatus	getCommandArgument	(unsigned int index, MString &result) const;
	//! \noscript
	MStatus	getCommandArgument	(unsigned int index, MDistance &result) const;
	//! \noscript
	MStatus	getCommandArgument	(unsigned int index, MAngle &result) const;
	//! \noscript
	MStatus	getCommandArgument	(unsigned int index, MTime &result) const;
END_NO_SCRIPT_SUPPORT:

	MStatus	getObjects			(MStringArray &result) const;

	static	const char*			className();

protected:
// No protected members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MArgDatabase */
