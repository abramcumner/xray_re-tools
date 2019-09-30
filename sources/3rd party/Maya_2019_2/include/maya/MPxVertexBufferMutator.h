#ifndef _MPxVertexBufferMutator
#define _MPxVertexBufferMutator
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
// CLASS: MPxVertexBufferMutator
//
// ****************************************************************************


#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MHWGeometry.h>
#include <maya/MDagPath.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MComponentDataIndexing.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{
// ****************************************************************************
// CLASS DECLARATION (MPxVertexBufferMutator)

//! \ingroup OpenMayaRender MPx
//! \brief Base class for user defined vertex buffer generators.
/*!

Implementations of MPxVertexBufferMutator must be registered with Maya through
MDrawRegistry.

MPxVertexBufferMutators are registered with a unique buffer name.  
The name signifies the name of the custom stream the mutator can alter.  
When an MPxShaderOverride or custom renderer uses an effect with custom input 
streams they can provide a unique name in the MVertexBufferDescriptor and 
a registered MPxVertexBufferMutator will be used to alter the custom stream
for a given shape using the incoming requirements held in the MVertexBuffer.

modifyVertexStream() is called to allow the mutator to alter the vertex data
Access the requirements for the requested stream through vertexBuffer.descriptor().
Populate the stream using vertexBuffer.acquire() and vertexBuffer.commit().
*/

class OPENMAYARENDER_EXPORT MPxVertexBufferMutator
{
public:

    MPxVertexBufferMutator();
    virtual ~MPxVertexBufferMutator();

    virtual void modifyVertexStream(const MObject& object,
		MVertexBuffer& vertexBuffer, const MComponentDataIndexing& targetIndexing) const=0;

    static	const char*	className();
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxVertexBufferMutator */
