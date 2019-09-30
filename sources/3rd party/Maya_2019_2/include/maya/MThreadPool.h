#ifndef _MThreadPool
#define _MThreadPool
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
// CLASS:    MThreadPool
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>


#ifdef _WIN32
typedef long MThreadRetVal;
#else
typedef void * MThreadRetVal;
#endif

class MThreadRootTask;


//! \brief Pointer to a function to be executed within threads.
/*!
 \param[in] data Pointer to user-defined supplied when the thread task was created.

 \return Result of the function call.
*/
typedef MThreadRetVal (*MThreadFunc)(void *data);

//! \brief Pointer to a callback function to be executed within threads.
/*!
 \param[in] data Pointer to user-defined data supplied when the thread task was created.
 \param[in] root Pointer to the root task.
*/
typedef void (*MThreadCallbackFunc)(void *data, MThreadRootTask *);

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MThreadPool)

//! \ingroup OpenMaya
//! \brief Methods for creating and managing a thread pool.
/*!
 MThreadPool class. The thread pool is created with a number of
 threads equal to one less than the number of logical processors.
*/
class OPENMAYA_EXPORT MThreadPool
{

public:
	static MStatus init();

	static MStatus newParallelRegion(MThreadCallbackFunc func, void* data);

	static MStatus createTask(MThreadFunc func, void* data, MThreadRootTask* root);

	static MStatus executeAndJoin(MThreadRootTask* root);

	static void release();

	static const char* 	className();

private:
	MThreadPool() {};
	MThreadPool(const MThreadPool&) {}; // disallow this
	~MThreadPool() {};
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MThreadPool */
