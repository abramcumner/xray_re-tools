#ifndef _MViewportRenderer
#define _MViewportRenderer
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
// CLASS:    MViewportRenderer
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MString.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MViewportRenderer)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
\deprecated
Use MHWRender::MRenderOverride and MHWRender::MSceneRender instead.

MViewportRenderer is a class which represents a hardware viewport renderer.
*/
class OPENMAYARENDER_EXPORT MViewportRenderer
{
public:
	//! API used for rendering
	typedef enum
	{
		//! Renderer uses hardware OpenGL for rendering
		kOpenGL,
		//! Renderer uses hardware Direct3D for rendering
		kDirect3D,
		//! Renderer renders using software
		kSoftware
	} RenderingAPI;

	//! Override status
	typedef enum {
		//! Override nothing
		kNoOverride,
		//! Override all drawing
		kOverrideAllDrawing,
		//! Override all drawing, then follow with a standard render pass
		kOverrideThenStandard,
		//! Override draw, then follow with a UI only render pass
		kOverrideThenUI,
	} RenderingOverride;

	//! Rendering exclusions when RenderingOverride is kOverrideThenStandard
	typedef enum {
		kExcludeNone				= 0,
		kExcludeNurbsCurves			= 1<<(0),
		kExcludeNurbsSurfaces		= 1<<(1),
		kExcludeMeshes				= 1<<(2),
		kExcludePlanes				= 1<<(3),
		kExcludeLights				= 1<<(4),
		kExcludeCameras				= 1<<(5),
		kExcludeJoints				= 1<<(6),
		kExcludeIkHandles			= 1<<(7),
		kExcludeDeformers			= 1<<(8),
		kExcludeDynamics			= 1<<(9),
		kExcludeParticleInstancers	= 1<<(10),
		kExcludeLocators			= 1<<(11),
		kExcludeDimensions			= 1<<(12),
		kExcludeSelectHandles		= 1<<(13),
		kExcludePivots				= 1<<(14),
		kExcludeTextures			= 1<<(15),
		kExcludeGrid				= 1<<(16),
		kExcludeCVs					= 1<<(17),
		kExcludeHulls				= 1<<(18),
		kExcludeStrokes				= 1<<(19),
		kExcludeSubdivSurfaces		= 1<<(20),
		kExcludeFluids				= 1<<(21),
		kExcludeFollicles			= 1<<(22),
		kExcludeHairSystems			= 1<<(23),
		kExcludeImagePlane			= 1<<(24),
		kExcludeNCloths				= 1<<(25),
		kExcludeNRigids				= 1<<(26),
		kExcludeDynamicConstraints	= 1<<(27),	
		kExcludeManipulators		= 1<<(28),
		kExcludeNParticles			= 1<<(29),
		kExcludeMotionTrails		= 1<<(30),
		kExcludePluginShapes		= 1<<(31),
		kExcludeAll					= ~0,
	} RenderingExclusion;

	//! Constructor
	MViewportRenderer(const MString & name);

	//! Destructor
	virtual ~MViewportRenderer();

	// Virtual overrides
	//
	//! Renderer initialization.
	virtual	MStatus	initialize() = 0;
	//! Renderer cleanup.
	virtual	MStatus	uninitialize() = 0;

	//! Method which does the actual rendering.
	virtual MStatus	render( const MRenderingInfo &info ) = 0;

	//! Query the native rendering API's supported by this renderer.
	virtual bool	nativelySupports( MViewportRenderer::RenderingAPI api,
										  float version ) = 0;
	//! Check if override exists
	virtual bool	override( MViewportRenderer::RenderingOverride override ) = 0;

	//! Rendering exclusion for standard pass of kOverrideThenStandard
	virtual unsigned int	overrideThenStandardExclusion() const;

	// Registration methods
	MStatus			registerRenderer() const;
	MStatus			deregisterRenderer() const;

	// Basic access methods
	//
	const MString &	name() const;

	const MString &	UIname() const;
	void			setUIName(const MString &name );

	MViewportRenderer::RenderingOverride  renderingOverride() const;
	void			setRenderingOverride( RenderingOverride override );

protected:
	// Default constructor is protected
	MViewportRenderer();

	friend class MHardwareRenderer;

	// Rendering identification
	MString			fName;
	MString			fUIName;

	// Rendering override information
	RenderingOverride	fRenderingOverride;

private:
// No private members

};
OPENMAYA_NAMESPACE_CLOSE

#endif
#endif // _MViewportRenderer
