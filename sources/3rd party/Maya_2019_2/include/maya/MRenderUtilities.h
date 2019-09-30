#ifndef _MRenderUtilities
#define _MRenderUtilities
//
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
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MHWGeometry.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{
    
// ****************************************************************************
// CLASS DECLARATION (MRenderUtilities)
//! \ingroup OpenMayaRender
//! \brief Utilities for Viewport 2.0
/*!
This class is a utility class for rendering in Viewport 2.0
*/
class OPENMAYARENDER_EXPORT MRenderUtilities
{
public:

	static MDrawContext* acquireSwatchDrawContext(const MRenderTarget* colorTarget = NULL, const MRenderTarget* depthTarget = NULL);

	static MDrawContext* acquireUVTextureDrawContext(const MRenderTarget* colorTarget = NULL, const MRenderTarget* depthTarget = NULL);

	static void releaseDrawContext(MDrawContext* context, bool releaseTargets = true);

	static void swatchBackgroundColor(float& r, float& g, float& b, float& a);

	static MStatus blitTargetToImage(const MRenderTarget* target, MImage &image);

	static MStatus blitTargetToGL(const MRenderTarget* target, const float region[2][2], bool unfiltered);

	static MStatus drawSimpleMesh(MDrawContext& context, const MVertexBuffer& vertexBuffer, const MIndexBuffer& indexBuffer, MGeometry::Primitive primitiveType, int start, int count);

	//! Camera positions supported by the Material Viewer.
	enum MaterialViewerCamera {
		//! Perspective camera with Material Viewer settings.
		kPerspectiveCamera = 0,
		//! Orthogonal camera with margins as wide as perspective camera for planar swatches.
		kOrthogonalCameraWithMargin,
		//! Orthogonal camera set up to fully capture a MaterialViewerShape::kPlaneShape for UV editor images.
		kOrthogonalCameraCloseUp
	};

	//! Light rigs supported by the Material Viewer.
	enum MaterialViewerLights {
		//! Material Viewer default light rig.
		kDefaultLights = 0,
		//! Single directional swatch light.
		kSwatchLight,
		//! Ambient light scheme.
		kAmbientLight
	};

	static MStatus renderMaterialViewerGeometry(const MString& shape, 
												MObject shaderNode, 
												MImage &image, 
												MaterialViewerCamera cameraMode=kPerspectiveCamera,
												MaterialViewerLights lightRig=kDefaultLights);

	static const char* className();

private:
    ~MRenderUtilities();
#ifdef __GNUC__
	friend class shutUpAboutPrivateDestructors;
#endif
};

} // namespace MHWRender

// ****************************************************************************

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderUtilities */
