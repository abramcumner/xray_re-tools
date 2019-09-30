#ifndef _MArgParser
#define _MArgParser
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
// CLASS:    MArgParser
//
// ***************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MArgParser)

//! \ingroup OpenMaya
//! \brief Command argument list parser.
/*!
  This class is used to parse and store all of the flags, arguments
  and objects which are passed to a command.  The MArgParser constructor
  accepts an MSyntax object, which describes the format for a command, and
  uses it to parse the command arguments into a form which is easy to
  query.  The documentation for MSyntax describes three types of arguments to
  commands:

	\li	<b>flag arguments</b> preceded by a '-' character. Each flag can be
		followed by a fixed number of parameters.
	\li	<b>command arguments</b> a fixed number of typed arguments
	\li	<b>objects</b> a range of Maya objects or the contents
	of Maya's current selection.

  A command may accept either <b>command arguments</b> or <b>objects</b>, but
  not both.

  The flag names, numbers and types of parameters accepted by each flag,
  number and types of command arguments, range of objects accepted and
  whether Maya's current selection can be used in place of missing objects,
  are all determined by the MSyntax object passed to MArgParser's constructor.

  In normal usage a command's <i>doIt</i> method would create an
  MArgParser object by passing it an MSyntax object which describes the
  command's syntax and an MArgList containing the command line arguments
  specified in the command's invocation. A successful return status from the
  constructor would mean that command invocation was syntactically correct and
  the other MArgList methods could be used to extract the values of the
  arguments to the command. A failure return status would mean that the command
  invocation was syntactically incorrect and the command should return without 
  attempting any further processing of its arguments. Note that in the case
  of a failure MArgParser will automatically display an appropriate error
  message in the Script Editor so the command is freed from having to do that
  itself.

  The <i>isFlagSet</i> method can be used to find out if a particular
  flag argument was provided in the command invocation.

  If a flag accepts one or more parameters, the <i>getFlagArgument</i> and
  <i>flagArgument*</i> methods can be used to return its parameters.

  If the same flag appears multiple times in the command line (i.e. it was
  specified with MSyntax::makeFlagMultiUse() to allow multiple uses per command
  invocation) the <i>getFlagArgument</i> and <i>flagArgument*</i> methods will
  only be able to return parameters for the first use of the flag. 
  However, getFlagArgumentList() provides the ability to access any of a
  multi-use flag's instances.

  If the command requires <i>command arguments</i> (i.e. the
  <i>addArg</i> method was used to construct the syntax object), then
  <i>getCommandArgument</i> and <i>commandArgument*</i> methods can be used to
  return the values of those arguments.

  If the command requires <i>objects</i> then the <i>getObjects</i>
  method can be used to return those objects as an array of strings.
*/
class OPENMAYA_EXPORT MArgParser
{
public:
			MArgParser();

			MArgParser			(const MSyntax &syntax,
								 const MArgList &argList,
								 MStatus *ReturnStatus = NULL);

	virtual ~MArgParser();

	bool	isFlagSet			(const char *flag,
								 MStatus *ReturnStatus = NULL) const;

	bool	isEdit				(MStatus *ReturnStatus = NULL) const;

	bool	isQuery				(MStatus *ReturnStatus = NULL) const;


	unsigned int numberOfFlagsUsed	() const;

	unsigned int numberOfFlagUses	(const char *flag) const;

BEGIN_NO_SCRIPT_SUPPORT:

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 bool &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 int &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 double &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MString &result) const;

	MStatus getFlagArgument		(const char *flag, unsigned int index,
								 unsigned int &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MDistance &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MAngle &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MTime &result) const;

END_NO_SCRIPT_SUPPORT:

	bool	flagArgumentBool		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL ) const;

	int		flagArgumentInt			(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL ) const;

	double	flagArgumentDouble		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;

	MString	flagArgumentString		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;

	MDistance flagArgumentMDistance	(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;

	MAngle	flagArgumentMAngle		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;

	MTime	flagArgumentMTime		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;


	MStatus getFlagArgumentPosition	(const char *flag, unsigned int i,
								 	 unsigned int &position) const;

	MStatus getFlagArgumentList		(const char *flag, unsigned int i,
								 	 MArgList& args) const;

BEGIN_NO_SCRIPT_SUPPORT:

	MStatus	getCommandArgument	(unsigned int index, bool &result) const;

	MStatus	getCommandArgument	(unsigned int index, int &result) const;

	MStatus	getCommandArgument	(unsigned int index, double &result) const;

	MStatus	getCommandArgument	(unsigned int index, MString &result) const;

	MStatus	getCommandArgument	(unsigned int index, MDistance &result) const;

	MStatus	getCommandArgument	(unsigned int index, MAngle &result) const;

	MStatus	getCommandArgument	(unsigned int index, MTime &result) const;

END_NO_SCRIPT_SUPPORT:

	bool commandArgumentBool	(unsigned int index, MStatus *ReturnStatus = NULL ) const;

	int	commandArgumentInt	(unsigned int index, MStatus *ReturnStatus = NULL ) const;

	double commandArgumentDouble (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MString	commandArgumentString (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MDistance commandArgumentMDistance (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MAngle commandArgumentMAngle (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MTime commandArgumentMTime (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MStatus	getObjects			(MStringArray &result) const;

	static	const char*			className();

protected:
	bool fOwn;
	void *apiData;

private:
	MArgParser (void *);
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MArgParser */
