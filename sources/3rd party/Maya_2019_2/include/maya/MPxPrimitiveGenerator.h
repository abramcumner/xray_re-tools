#ifndef _MPxPrimitiveGenerator
#define _MPxPrimitiveGenerator
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
// CLASS DECLARATION (MPxPrimitiveGenerator)

//! \ingroup OpenMayaRender MPx
//! \brief Base class for user defined primitive generators.
/*!

Implementations of MPxPrimitiveGenerator must be registered with Maya through
MDrawRegistry.

MPxPrimitiveGenerators are registered with a unique primitive type name.  
The name signifies the name of the custom primitive the generator can produce.  
When an MPxShaderOverride or custom renderer uses an effect with custom input 
primitive they can provide a MIndexBufferDescriptor with the desired custom primitive name.
A registered MPxPrimitiveGenerator will be used to produce the custom primitive.
*/

class OPENMAYARENDER_EXPORT MPxPrimitiveGenerator
{
public:

    MPxPrimitiveGenerator();
    virtual ~MPxPrimitiveGenerator();

    virtual unsigned int computeIndexCount(const MObject& object, const MObject& component) const=0;

    virtual MGeometry::Primitive generateIndexing(const MObject& object, const MObject& component,
        const MComponentDataIndexingList& sourceIndexing,
        const MComponentDataIndexingList& targetIndexing,
        MIndexBuffer& indexBuffer,
		int& primitiveStride) const=0;

    static	const char*	className();
};

} // namespace MHWRender
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxPrimitiveGenerator */
