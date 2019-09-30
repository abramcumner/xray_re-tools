#ifndef _MOpenCLAutoPtr
#define _MOpenCLAutoPtr

// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

//
// CLASS:    MAutoCLKernel
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MAutoCLKernel)
//
//  MAutoCLKernel is an auto pointer to a cl_kernel object which helps manage
//  the lifetime of cl_kernel.
//

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <clew/clew_cl.h>

#include <maya/MApiNamespace.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAutoCLKernel)

//! \ingroup OpenMaya
//! \brief  Auto pointer for cl_kernel objects 
/*! cl_kernel objects in openCL are reference-counted objects.  When created, a
	cl_kernel has a preexisting reference count of 1. An MAutoCLKernel object <b>does not</b>
	need to increment the reference count when it takes over ownership of a cl_kernel
	from OpenCL.  An MAutoCLKernel object <b>does</b> need to increment the reference count
	of a cl_kernel object when the cl_kernel is shared between MAutoCLKernel objects.

	Always use MAutoCLKernel objects in user code.  Only use raw cl_kernel handle when
	you use the OpenCL API.

	Assignment or construction with a raw cl_kernel <b>DOES NOT</b> increment the reference count.
	Assignment or construction from another MAutoCLKernel <b>DOES</b> increment the reference count.
*/

class OPENMAYA_EXPORT MAutoCLKernel
{
public:
	MAutoCLKernel();
	~MAutoCLKernel();

	// Assignment or construction from another MAutoCLKernel WILL increment the ref count
	MAutoCLKernel( const MAutoCLKernel& other );
	MAutoCLKernel& operator=(const MAutoCLKernel& other);

	// Construction from a cl_kernel WILL NOT increment the ref count
	// Dummy parameter helps avoid accidental use
	class NoRef{};
	MAutoCLKernel(cl_kernel value, NoRef dummy);

	// Attach or detach to a cl_kernel without modifying the ref count
	void attach(cl_kernel value);
	cl_kernel detach();

	// Reset to NULL and decrement the reference count
	void reset();

	// Return the value without modifying.  Use to pass the value into OpenCL.
	// Do not use elsewhere to avoid dangers of messing up the ref count!
	cl_kernel get() const;

	// Check if the current handle is empty and not referring to an actual cl_kernel.
	bool isNull() const;

	// Overriding operator& is dangerous to the reference count.  Provide
	// getReadOnlyRef() as a stand-in for const-returning operator&.
	// this can be passed into OpenCL in places where it expects an array.
	const cl_kernel* getReadOnlyRef() const;

	// Release the ref count for the current handle and return a pointer
	// to our internal storage for cl_kernel.  It is not safe to dereference
	// the returned pointer because the handle may already be deleted.
	// Use this when you want to re-use an existing MAutoCLKernel for a new
	// kernel.
	cl_kernel* getReferenceForAssignment();

	bool operator==(const MAutoCLKernel& other) const;
	bool operator==(cl_kernel other) const; // TODO: should it be a const cl_kernel?
	bool operator!() const;

	void swap(MAutoCLKernel& other);
	bool operator<(const MAutoCLKernel& other) const;
private:
 	void* fAutoCLKernel;
};

// ****************************************************************************
// CLASS DECLARATION (MAutoCLMem)

//! \ingroup OpenMaya
//! \brief  Auto pointer for cl_mem objects 
/*! cl_mem objects in openCL are reference-counted objects.  When created, a
	cl_mem has a preexisting reference count of 1. An MAutoCLMem object <b>does not</b>
	need to increment the reference count when it takes over ownership of a cl_mem
	from OpenCL.  An MAutoCLMem object <b>does</b> need to increment the reference count
	of a cl_mem object when the cl_mem is shared between MAutoCLMem objects.

	Always use MAutoCLMem objects in user code.  Only use the raw cl_mem handle when you
	use the OpenCL API.

	Assignment or construction with a raw cl_mem <b>DOES NOT</b> increment the reference count.
	Assignment or construction from another MAutoCLMem <b>DOES</b> increment the reference count.
*/
class OPENMAYA_EXPORT MAutoCLMem
{
public:
	MAutoCLMem();
	~MAutoCLMem();

	// Assignment or construction from another MAutoCLMem WILL increment the ref count
	MAutoCLMem( const MAutoCLMem& other );
	MAutoCLMem& operator=(const MAutoCLMem& other);

	// Construction from a cl_mem WILL NOT increment the ref count
	// Dummy parameter helps avoid accidental use
	class NoRef{};
	MAutoCLMem(cl_mem value, NoRef dummy);

	// Attach or detach to a cl_mem without modifying the ref count
	void attach(cl_mem value);
	cl_mem detach();

	// Reset to NULL and decrement the reference count
	void reset();

	// Return the value without modifying.  Use to pass the value into OpenCL.
	// Do not use elsewhere to avoid dangers of messing up the ref count!
	cl_mem get() const;

	// Check if the current handle is empty and not referring to an actual cl_mem.
	bool isNull() const;

	// Overriding operator& is dangerous to the reference count.  Provide
	// getReadOnlyRef() as a stand-in for const-returning operator&.
	// this can be passed into OpenCL in places where it expects an array.
	const cl_mem* getReadOnlyRef() const;

	// Release the ref count for the current handle and return a pointer
	// to our internal storage for cl_kernel.  It is not safe to dereference
	// the returned pointer because the handle may already be deleted.
	// Use this when you want to re-use an existing MAutoCLMem for a new
	// cl_mem.
	cl_mem* getReferenceForAssignment();

	bool operator==(const MAutoCLMem& other) const;
	bool operator==(cl_mem other) const; // TODO: should it be a const cl_mem?
	bool operator!() const;

	void swap(MAutoCLMem& other);
	bool operator<(const MAutoCLMem& other) const;
private:
 	void* fAutoCLMem;
};

// ****************************************************************************
// CLASS DECLARATION (MAutoCLEvent)

//! \ingroup OpenMaya
//! \brief  AutoPtr for cl_event objects 
/*! cl_event objects in openCL are reference-counted objects.  When created, a
	cl_event has a preexisting reference count of 1. An MAutoCLEvent object <b>does not</b>
	need to increment the reference count when it takes over ownership of a cl_event
	from OpenCL.  An MAutoCLEvent object <b>does</b> need to increment the reference count
	of a cl_event object when the cl_event is shared between MAutoCLEvent objects.

	Always use MAutoCLEvent objects in user code.  Only use the raw cl_event handle when you
	use the OpenCL API.

	Assignment or construction with a raw cl_event <b>DOES NOT</b> increment the reference count.
	Assignment or construction from another MAutoCLEvent <b>DOES</b> increment the reference count.
*/
class OPENMAYA_EXPORT MAutoCLEvent
{
public:
	MAutoCLEvent();
	~MAutoCLEvent();

	// Assignment or construction from another MAutoCLEvent WILL increment the ref count
	MAutoCLEvent( const MAutoCLEvent& other );
	MAutoCLEvent& operator=(const MAutoCLEvent& other);

	// Construction from a cl_event WILL NOT increment the ref count
	// Dummy parameter helps avoid accidental use
	class NoRef{};
	MAutoCLEvent(cl_event value, NoRef dummy);

	// Attach or detach to a cl_event without modifying the ref count
	void attach(cl_event value);
	cl_event detach();

	// Reset to NULL and decrement the reference count
	void reset();

	// Return the value without modifying.  Use to pass the value into OpenCL.
	// Do not use elsewhere to avoid dangers of messing up the ref count!
	cl_event get() const;

	// Check if the current handle is empty and not referring to an actual cl_event.
	bool isNull() const;

	// Overriding operator& is dangerous to the reference count.  Provide
	// getReadOnlyRef() as a stand-in for const-returning operator&.
	// this can be passed into OpenCL in places where it expects an array.
	const cl_event* getReadOnlyRef() const;

	// Release the ref count for the current handle and return a pointer
	// to our internal storage for cl_event.  It is not safe to dereference
	// the returned pointer because the handle may already be deleted.
	// Use this when you want to re-use an existing MAutoCLEvent for a new
	// event.
	cl_event* getReferenceForAssignment();

	bool operator==(const MAutoCLEvent& other) const;
	bool operator==(cl_event other) const; // TODO: should it be a const cl_event?
	bool operator!() const;

	void swap(MAutoCLEvent& other);
	bool operator<(const MAutoCLEvent& other) const;
private:
 	void* fAutoCLEvent;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MObjectArray */
