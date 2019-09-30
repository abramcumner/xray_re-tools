#ifndef _MOpenCLBuffer
#define _MOpenCLBuffer
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

//
// CLASS:    MOpenCLBuffer
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MOpenCLBuffer)
//
//  MOpenCLBuffer is a convenience class which can replace calls to clCreateBuffer,
//	clEnqueueWriteBuffer and clReleaseBuffer.  MOpenCLBuffer provides a number of
//	features which make creating, updating and releasing an OpenCL buffer simple.
// 

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MOpenCLAutoPtr.h>
#include <clew/clew_cl.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MOpenCLBuffer)

//! \ingroup OpenMaya
//! \brief  Convenience class which makes allocating, updating and releasing OpenCL buffers easy.
/*! 
	MOpenCLBuffer is a convenience class which can replace calls to clCreateBuffer,
	clEnqueueWriteBuffer and clReleaseBuffer.  MOpenCLBuffer provides a number of
	features which make creating, updating and releasing an OpenCL buffer simple.

	When MOpenCLBuffer allocates a cl_mem object MOpenCLBuffer automatically
	notifies Maya of the increased GPU memory usage (see MRenderer::holdGPUMemory()).

	When MOpenCLBuffer updates a cl_mem object it will automatically allocate a
	larger buffer if the new data size is larger than the old data size.

	When MOpenCLBuffer deallocates a cl_mem object MOpenCLBuffer automatically
	notifies Maya of the decreased GPU memory usage.
*/
class OPENMAYA_EXPORT MOpenCLBuffer
{
public:
	// Create an MOpenCLBuffer without any GPU backing
	MOpenCLBuffer(cl_context gpuContext, cl_mem_flags flags);
	// Create an MOpenCLBuffer with a GPU buffer.  Fill the buffer with data from host_ptr
	MOpenCLBuffer(cl_context gpuContext, cl_mem_flags flags, size_t size, void* host_ptr, cl_int* err);
	// Create an MOpenCLBuffer without any GPU backing.
	MOpenCLBuffer();
	// Move constructor.
	MOpenCLBuffer(MOpenCLBuffer &&);
	~MOpenCLBuffer();

	MOpenCLBuffer& operator=(MOpenCLBuffer &&);

	// release existing GPU buffer & events, reset size to zero.
	void reset();
	// send new data to the GPU.  allocate a new buffer if the size of the upload is larger than the current size.
	cl_int uploadDataToGPU(cl_command_queue gpuQueue, cl_bool blocking_write, size_t offset, size_t size, void* host_ptr, cl_uint numEvents, const cl_event* events);
	// allocate a new buffer is size is larger than the current buffer's size.
	cl_int ensureBufferSize(size_t size);

	// get the OpenCL buffer
	MAutoCLMem buffer();
	const MAutoCLMem buffer() const;
	// get the event which must complete before it is safe to read buffer.  If there is no event, it is safe to read buffer (if buffer exists)
	MAutoCLEvent bufferReadyEvent();

private:
	MOpenCLBuffer(MOpenCLBuffer& other);
	MOpenCLBuffer operator=(MOpenCLBuffer& rhs);

	void* api_ptr;
};

OPENMAYA_NAMESPACE_CLOSE


#endif /* __cplusplus */
#endif /* _MOpenCLBuffer */
