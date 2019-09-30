#ifndef _MPxIndexBufferMutator
#define _MPxIndexBufferMutator
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
// CLASS: MPxVertexBufferProvider
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
// CLASS DECLARATION (MPxIndexBufferMutator)

//! \ingroup OpenMayaRender MPx
//! \brief Base class for user defined index buffer mutators.
/*!

Implementations of MPxIndexBufferMutator must be registered with Maya through
MDrawRegistry.

MPxIndexBufferMutators are registered with a unique mutator type name.  
The name signifies the name of the custom index buffer the mutator can produce.  
When an MPxShaderOverride or custom renderer uses an effect with custom index buffer 
mutator they can provide a MIndexBufferDescriptor with the desired mutator name.
A registered MPxIndexBufferMutator will be used to mutate the existing index buffer.
*/

class OPENMAYARENDER_EXPORT MPxIndexBufferMutator
{
public:

    MPxIndexBufferMutator();
    virtual ~MPxIndexBufferMutator();

    virtual MGeometry::Primitive mutateIndexing(const MHWRender::MComponentDataIndexingList& sourceIndexBuffers,
		const MVertexBufferArray &vertexBuffers,
        MIndexBuffer& indexBuffer,
		int& primitiveStride) const=0;

    static	const char*	className();
};

} // namespace MHWRender
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxIndexBufferMutator */
