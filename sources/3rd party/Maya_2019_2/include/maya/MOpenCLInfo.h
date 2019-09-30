#ifndef _MOpenCLInfo
#define _MOpenCLInfo
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

//
// CLASS:    MOpenCLInfo
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MOpenCLInfo)
//
//  MOpenCLInfo allows the user to access globally shared information about
//  OpenCL.  This includes the chosen device, the context and the command
//  queue.  It also includes helper methods for basic kernel loading and error
//  status checking.
//

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MString.h>
#include <maya/MOpenCLAutoPtr.h>
#include <clew/clew_cl.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MOpenCLInfo)

//! \ingroup OpenMaya
//! \brief  Global OpenCL information used by Maya
/*!
    This class provides access to all the global information Maya has about
    OpenCL.  This information includes the chosen device ID, context, and command queue.
    MOpenCLInfo also includes helper methods to handle basic kernel compilation
    and error status checking.

    You can only access MOpenCLInfo after VP2 is initialized.  If you access it
    <i>before</i> VP2 initialization, the behavior of MOpenCLInfo is undefined.
*/

// TODO: make all these return MStatus and return the value through a reference parameter?
class OPENMAYA_EXPORT MOpenCLInfo
{
public:
    static cl_context       getOpenCLContext();
    static cl_command_queue getMayaDefaultOpenCLCommandQueue();
    static cl_command_queue createOpenCLCommandQueue();
    static cl_device_id     getOpenCLDeviceId();
    static MAutoCLKernel    getOpenCLKernel(const MString& sourceFilePath, const MString& kernelName);
	static MAutoCLKernel    getOpenCLKernelFromString(const MString& programSource, const MString& programName, const MString& kernelName);
    static bool             releaseOpenCLKernel(const MAutoCLKernel& kernel);
    static void             checkCLErrorStatus(cl_int err);
    static void             appendMessage(MStringArray* messages, const char* format, ...);
	static void				appendMessage(MStringArray* messages, const MStringResourceId* formatResourceId, ...);
	static bool				supportsDoublePrecision();
};

OPENMAYA_NAMESPACE_CLOSE


#endif /* __cplusplus */
#endif /* _MOpenCLInfo */
