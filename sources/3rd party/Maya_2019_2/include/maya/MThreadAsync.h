#ifndef _MThreadAsync
#define _MThreadAsync
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
// CLASS:    MThreadAsync
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
*/
typedef void (*MThreadAsyncCallbackFunc)(void *data);

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MThreadAsync)

//! \ingroup OpenMaya
//! \brief Methods for creating threads. 
/*!
 MThreadAsync class. Allows creation of asynchronous threads.
*/
class OPENMAYA_EXPORT MThreadAsync
{
public:
	static MStatus init();

	static MStatus createTask(MThreadFunc func, void* data, MThreadAsyncCallbackFunc afunc, MThreadRootTask* root);

	static void release();

	static const char* 	className();

private:
	MThreadAsync() {};
	MThreadAsync(const MThreadAsync&) {}; // disallow this
	~MThreadAsync() {};
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MThreadAsync */
