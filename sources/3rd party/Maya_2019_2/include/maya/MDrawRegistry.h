#ifndef _MDrawRegistry
#define _MDrawRegistry
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
// CLASS:    MDrawRegistry
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MString.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{


// ****************************************************************************
// CLASS DECLARATION (MDrawRegistry)

//! \ingroup OpenMayaRender
//! \brief Access the registry associating node types with custom implementations
/*!
This class provides a means to register custom implementations to be used by Viewport 2.0.

* Custom draw overrides :
Used by Viewport 2.0 to draw dependency nodes.
The registry is based on classification strings. Each draw override must be
registered with a classification string and any node type with a matching
classification string will be drawn using the registered draw override.

* Custom stream buffer generators/mutators :
Used by Viewport 2.0 to generate custom streams.
The registry is based on identification name. Each generator/mutator must be
registered with a identification name and any stream with a matching name
will be filled using the registered generator or altered using the registered mutator.

* Custom index buffer generators/mutators :
Used by Viewport 2.0 to generate custom index buffers.
The registry is based on primitive type. Each generator/mutator must be
registered with a primitive type and any index buffer with a matching type
will be filled using the registered generator or altered using the registered mutator.

* Custom selection component converters :
Used by Viewport 2.0 to perform custom component selection.
The registry is based on identification name. Each convert must be
registered with a identification name and any render item with a matching name
will be processed by the converter during a viewport 2.0 selection.

*/
class OPENMAYARENDER_EXPORT MDrawRegistry
{
public:
	//////////////////////////////////////////////
	// Custom draw overrides
	typedef MPxShadingNodeOverride* (*ShadingNodeOverrideCreator)(const MObject&);
	static MStatus registerShadingNodeOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					ShadingNodeOverrideCreator creator);
	static MStatus deregisterShadingNodeOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxSurfaceShadingNodeOverride* (*SurfaceShadingNodeOverrideCreator)(const MObject&);
	static MStatus registerSurfaceShadingNodeOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					SurfaceShadingNodeOverrideCreator creator);
	static MStatus deregisterSurfaceShadingNodeOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxShaderOverride* (*ShaderOverrideCreator)(const MObject&);
	static MStatus registerShaderOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					ShaderOverrideCreator creator);
	static MStatus deregisterShaderOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxGeometryOverride* (*GeometryOverrideCreator)(const MObject&);
	static MStatus registerGeometryOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					GeometryOverrideCreator creator);
	static MStatus deregisterGeometryOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxDrawOverride* (*DrawOverrideCreator)(const MObject&);
	static MStatus registerDrawOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					DrawOverrideCreator creator);
	static MStatus deregisterDrawOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxSubSceneOverride* (*SubSceneOverrideCreator)(const MObject&);
	static MStatus registerSubSceneOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					SubSceneOverrideCreator creator);
	static MStatus deregisterSubSceneOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxImagePlaneOverride* (*ImagePlaneOverrideCreator)(const MObject&);
	static MStatus registerImagePlaneOverrideCreator(
		const MString& drawClassification,
		const MString& registrantId,
		ImagePlaneOverrideCreator creator);
	static MStatus deregisterImagePlaneOverrideCreator(
		const MString& drawClassification,
		const MString& registrantId);

	//////////////////////////////////////////////
	// Custom stream buffer generators/mutators
    typedef MPxVertexBufferGenerator* (*VertexBufferGeneratorCreator)();
    static MStatus registerVertexBufferGenerator(
        const MString& bufferName, VertexBufferGeneratorCreator creator);
    static MStatus deregisterVertexBufferGenerator(
        const MString& bufferName);

    typedef MPxVertexBufferMutator* (*VertexBufferMutatorCreator)();
    static MStatus registerVertexBufferMutator(
        const MString& bufferName, VertexBufferMutatorCreator creator);
    static MStatus deregisterVertexBufferMutator(
        const MString& bufferName);

	//////////////////////////////////////////////
	// Custom index buffer generators/mutators
    typedef MPxPrimitiveGenerator* (*PrimitiveGeneratorCreator)();
    static MStatus registerPrimitiveGenerator(
        const MString& primitiveType, PrimitiveGeneratorCreator creator);
    static MStatus deregisterPrimitiveGenerator(
        const MString& primitiveType);

    typedef MPxIndexBufferMutator* (*IndexBufferMutatorCreator)();
    static MStatus registerIndexBufferMutator(
        const MString& primitiveType, IndexBufferMutatorCreator creator);
    static MStatus deregisterIndexBufferMutator(
        const MString& primitiveType);

	//////////////////////////////////////////////
	// Custom selection component converters
    typedef MPxComponentConverter* (*ComponentConverterCreator)();
	static MStatus registerComponentConverter(
		const MString& renderItemName, ComponentConverterCreator creator);
	static MStatus deregisterComponentConverter(
		const MString& renderItemName);


	static void setRegisteringCallableScript();
	static bool registeringCallableScript();

	static const char* className();

private:
	MDrawRegistry();
	~MDrawRegistry();

	static MStatus deregisterOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					const char* methodName);

	static bool fRegisteringCallableScript;
};

} // namespace MHWRender

// ****************************************************************************

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDrawRegistry */

