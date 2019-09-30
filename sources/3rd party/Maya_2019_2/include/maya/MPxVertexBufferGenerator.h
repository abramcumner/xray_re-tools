#ifndef _MPxVertexBufferGenerator
#define _MPxVertexBufferGenerator
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
// CLASS: MPxVertexBufferGenerator
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
// CLASS DECLARATION (MPxVertexBufferGenerator)

//! \ingroup OpenMayaRender MPx
//! \brief Base class for user defined vertex buffer generators.
/*!

Implementations of MPxVertexBufferGenerator must be registered with Maya through
MDrawRegistry.

MPxVertexGenerators are registered with a unique buffer name.  
The name signifies the name of the custom stream the generator can produce.  
When an MPxShaderOverride or custom renderer uses an effect with custom input 
streams they can provide a unique name in the MVertexBufferDescriptor and 
a registered MPxVertexBufferGenerator will be used to produce the custom stream
for a given shape using the incoming requirements held in the MVertexBuffer.

createVertexStream() is called to allow the generator to fill in the missing vertex data
Access the requirements for the requested stream through vertexBuffer.descriptor().
Populate the stream using vertexBuffer.acquire() and vertexBuffer.commit().
*/
 // rename MPxVertexBufferTranslator
class OPENMAYARENDER_EXPORT MPxVertexBufferGenerator
{
public:

    MPxVertexBufferGenerator();
    virtual ~MPxVertexBufferGenerator();

    virtual bool getSourceIndexing(const MObject& object, 
        MComponentDataIndexing& sourceIndexing) const=0;

	virtual bool getSourceStreams(const MObject& object,
		MStringArray &sourceStreams) const=0;

    virtual void createVertexStream(const MObject& object,
        MVertexBuffer& vertexBuffer, const MComponentDataIndexing& targetIndexing, const MComponentDataIndexing& sharedIndexing, const MVertexBufferArray& sourceStreams) const=0;

    static	const char*	className();
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxVertexBufferGenerator */
