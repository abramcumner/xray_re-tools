#ifndef _MObjectHandle
#define _MObjectHandle
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
// ****************************************************************************
//
// CLASS:    MObjectHandle
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// CLASS DECLARATION (MObjectHandle)

//! \ingroup OpenMaya
//! \brief Generic Class for validating MObjects.
/*!
  MObjectHandle is a wrapper class for the MObject class. An MObjectHandle
  will provide a user with added information on the validity of an MObject.
  Each MObjectHandle that is created registers an entry into a table to
  maintain state tracking of the MObject that the handle was created for.
  This will help users track when an MObject is invalid and should be
  re-retrieved.
*/
class OPENMAYA_EXPORT MObjectHandle
{
public:
	MObjectHandle();
	MObjectHandle( const MObject& object );
	MObjectHandle( const MObjectHandle &handle );
	~MObjectHandle();
	MObject			object() const;
	const MObject & objectRef() const;
	unsigned int	hashCode() const;
	bool			isValid() const;
	bool			isAlive() const;

	bool			operator==(const MObject &object) const;
	bool			operator==(const MObjectHandle &object) const;
	bool			operator!=(const MObject &object) const;
	bool			operator!=(const MObjectHandle &object) const;

	MObjectHandle & operator=(const MObject &object );
	MObjectHandle & operator=(const MObjectHandle &other );

    static unsigned int	objectHashCode(const MObject&);
private:

	// Internal object registration methods
	void			registerObject();
	void			deregisterObject();

	// Internal destructor callback
	static void		registerDestroyedCallback();
	static void		deregisterDestroyedCallback();
	static bool		fIsCallbackInitialized;
	static void		destroyedCallback(void*);

	MObject			fObject;
	bool			fIsAlive;
	bool			fIsDependNode;
	static int		fHandleCount;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MObjectHandle */

