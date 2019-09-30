#ifndef _MMutexLock
#define _MMutexLock
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
// CLASS:    MMutexLock
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MMutexLock)
//
// The MMutexLock class provides a heavyweight locking primitive to use with threads
// created by the MThreadPool class.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MMutexLock)

//! \ingroup OpenMaya
//! \brief Methods for managing locks on threads created with MThreadPool and MThreadAsync. 
/*!
 Mutex lock class to be used with threads created by MThreadPool and
 MThreadAsync.
*/
class OPENMAYA_EXPORT MMutexLock
{

public:
	MMutexLock();

	~MMutexLock();

	void lock();

	void unlock();

	bool tryLock();

private:
	MMutexLock(const MMutexLock&) {}; // disallow this
	MMutexLock & operator=(const MMutexLock&); // disallow this
	void* f_spinLockRef;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MMutexLock */
