#ifndef _MSyntax
#define _MSyntax
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
// CLASS:    MSyntax
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MSyntax)

//! \ingroup OpenMaya
//! \brief Syntax for Commands. 
/*!
This class is used to specify flags and arguments passed to commands.
Once the syntax of the command has been defined by calls to the methods
in this class, Maya will be able to automatically parse invocations
of the command and reject syntactically invalid ones.  When the <i>doIt</i>
method of the command is invoked, the provided argument list can be
parsed automatically into an instance of either the MArgDatabase class
(for commands) or MArgParser class (for context commands) by
passing the constructor for that class both the argument list and the
syntax object for the command.

There are three type of arguments that can be specified via these syntax
methods:

	\li <b>flags arguments</b> preceded by a '-' character.
	\li <b>command arguments</b> required parameters that follow the flags
	\li <b>objects</b> an optional list of Maya objects or the contents
						  of the selection list.


<b>Note:</b> <i>command arguments</i> and <i>objects</i> are incompatible
and must not be combined in the syntax definition of a command.  If an
attempt is made to do so, neither command arguments or objects will be
accessible via the MArgDatabase or MArgParser classes.

The <i>addFlag</i> method is used to add a flag argument to a command.
Flags need to have a <i>type</i> specified for them from the list of
available types in the MArgType enum and can take up to six parameters.
Flags can be set as being allowed multiple times on the command line
be calling the <i>makeFlagMultiUse</i> method.

The <i>addArg</i> method is used to add a command argument to a command.
Such arguments must also have a <i>type</i> specified for them from the list of
available types in the MArgType enum.  If you specify an argument via
this call, Maya will reject an invocation of the command as being
syntactically invalid if an argument of the correct type is not provided.
If the type <i>kSelectionItem</i> is provided, Maya will include the
contents of the selection list as the argument to the command.

The <i>setObjectType</i> method is used to specify that the command requires
an object list.  Parameters to this method allow you to specify minimum
and maximum numbers of objects.  As well, if the <i>useSelectionAsDefault</i>
method is used in conjunction with this, Maya will use the contents of the
selection list as the objects for the command if none are specified.

The documentation for the MArgDatabase and MArgParser classes describe how
to access the parsed information for a command inside the doIt method of
that command.
*/
class OPENMAYA_EXPORT MSyntax {
public:

	//! Argument types.
	enum MArgType {
		kInvalidArgType,	//!< \nop
		kNoArg,			//!< \nop
		kBoolean,		//!< \nop
		kLong,			//!< \nop
		kDouble,		//!< \nop
		kString,		//!< \nop
		kUnsigned,		//!< \nop
		kDistance,		//!< \nop
		kAngle,			//!< \nop
		kTime,			//!< \nop
		kSelectionItem,		//!< \nop
		kLastArgType		//!< \nop
	};

	//! Object list types.
	enum MObjectFormat {
		kInvalidObjectFormat,	//!< \nop
		kNone,			//!< \nop
		kStringObjects,		//!< \nop
		kSelectionList,		//!< \nop
		kLastObjectFormat	//!< \nop
	};

		    MSyntax ();
		    MSyntax ( const MSyntax& other );
	virtual ~MSyntax();
	MSyntax &operator=(const MSyntax &rhs);

	MStatus	addFlag					(const char *shortName,
									 const char *longName,
									 MArgType argType1 = kNoArg,
									 MArgType argType2 = kNoArg,
									 MArgType argType3 = kNoArg,
									 MArgType argType4 = kNoArg,
									 MArgType argType5 = kNoArg,
									 MArgType argType6 = kNoArg);

	MStatus	makeFlagMultiUse		(const char *flag);

	MStatus	makeFlagQueryWithFullArgs(const char *flag,
									  bool queryArgsAreOptional );

	MStatus	addArg					(MArgType arg);

	void	useSelectionAsDefault	(bool useSelectionList = false);

	MStatus	setObjectType			(MObjectFormat objectFormat,
									 unsigned int minimumObjects = 0);
	MStatus	setObjectType			(MObjectFormat objectFormat,
									 unsigned int minimumObjects,
									 unsigned int maximumObjects);

	void	setMinObjects			(unsigned int minimumObjectCount);
	void	setMaxObjects			(unsigned int maximumObjectCount);
	void	enableQuery				(bool supportsQuery = true);
	void	enableEdit				(bool supportsEdit = true);

	unsigned int	minObjects		() const;
	unsigned int	maxObjects		() const;
	bool		canQuery			() const;
	bool		canEdit				() const;

	static const char*	className	();

protected:
// No protected members

private:
	MSyntax(void *);
	MSyntax(const void *);
	bool fOwn;

	void * apiData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MSyntax */
