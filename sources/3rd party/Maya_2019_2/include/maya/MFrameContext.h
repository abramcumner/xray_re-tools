#ifndef _MFrameContext
#define _MFrameContext
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MStatus.h>
#include <maya/MMatrix.h>
#include <maya/MFloatArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MBoundingBox.h>
#include <maya/MDagPath.h>
#include <maya/MString.h>
#include <maya/MColor.h>

// ****************************************************************************
// CLASS FORWARDS


OPENMAYA_MAJOR_NAMESPACE_OPEN

namespace MHWRender
{

// ****************************************************************************
// DECLARATIONS
//
//! \ingroup OpenMayaRender
//! \brief This class contains some global information for the current render frame.
/*!
MFrameContext is designed to provide information which is available per frame
render. This includes information such as render targets, viewport size and
camera information.

In terms of relative scope, MFrameContext can be thought of as encompassing
the time period for a "pass" (MPassContext) and the time period for actual drawing
(MDrawContext).

MDrawContext is derived from MFrameContext and provides its own implementation
for all virtual methods. The values returned from these methods may differ
slightly between MFrameContext and MDrawContext as MFrameContext retrieves the
values from Maya and MDrawContext retrieves the values from the GPU device
state. Also, MFrameContext::getMatrix() is not able to return values for any
matrix type requiring the object-to-world matrix as that information is only
available at draw time.
	*/
class OPENMAYARENDER_EXPORT MFrameContext
{
public:
	//! Matrices that can be accessed
	enum MatrixType {
		kWorldMtx,                      //!< Object to world matrix
		kWorldTransposeMtx,             //!< Object to world matrix transpose
		kWorldInverseMtx,               //!< Object to world matrix inverse
		kWorldTranspInverseMtx,         //!< Object to world matrix transpose inverse (adjoint)
		kViewMtx,                       //!< World to view matrix
		kViewTransposeMtx,              //!< World to view matrix tranpose
		kViewInverseMtx,                //!< World to view matrix inverse
		kViewTranspInverseMtx,          //!< World to view matrix transpose inverse (adjoint)
		kProjectionMtx,                 //!< Projection matrix
		kProjectionTranposeMtx,         //!< Projection matrix tranpose
		kProjectionInverseMtx,          //!< Projection matrix inverse
		kProjectionTranspInverseMtx,    //!< Projection matrix tranpose inverse (adjoint)
		kViewProjMtx,                   //!< View * projection matrix
		kViewProjTranposeMtx,           //!< View * projection matrix tranpose
		kViewProjInverseMtx,            //!< View * projection matrix inverse
		kViewProjTranspInverseMtx,      //!< View * projection matrix tranpose inverse (adjoint)
		kWorldViewMtx,                  //!< World * view matrix
		kWorldViewTransposeMtx,         //!< World * view matrix transpose
		kWorldViewInverseMtx,           //!< World * view matrix inverse
		kWorldViewTranspInverseMtx,     //!< World * view matrix transpose inverse (adjoint)
		kWorldViewProjMtx,              //!< World * view * projection matrix
		kWorldViewProjTransposeMtx,     //!< World * view * projection matrix transpose
		kWorldViewProjInverseMtx,       //!< World * view * projection matrix inverse
		kWorldViewProjTranspInverseMtx  //!< World * view * projection matrix tranpose inverse (adjoint)
	};
	static MFrameContext::MatrixType semanticToMatrixType(const MString &value, MStatus* ReturnStatus=NULL);
	virtual MMatrix getMatrix(MFrameContext::MatrixType mtype, MStatus* ReturnStatus=NULL) const;

	//! Tuples that can be accessed. May be one or more values returned depending on the tuple requested.
	enum TupleType {
		kViewPosition,		//!< View position
		kViewDirection,		//!< View direction vector
		kViewUp,			//!< View up vector
		kViewRight,			//!< View right vector
		kViewportPixelSize,	//!< Viewport size in pixels (single value)
		kViewNearClipValue,	//!< Camera near clip value (single value)
		kViewFarClipValue,	//!< Camera far clip value (single value)
		kViewUnnormlizedNearClipValue,	//!< Unnormalized camera near clip value (single value)
		kViewUnnormalizedFarClipValue	//!< Unnormalized camera far clip value (single value)
	};
	static MFrameContext::TupleType semanticToTupleType(const MString &value, MStatus* ReturnStatus=NULL);
	virtual MDoubleArray getTuple(MFrameContext::TupleType ttype, MStatus* ReturnStatus=NULL) const;

	virtual MStatus getViewportDimensions(int &originX, int &originY, int &width, int &height) const;

	virtual float getGlobalLineWidth() const;

	MDagPath getCurrentCameraPath(MStatus* ReturnStatus=NULL) const;

	const MRenderTarget* getCurrentColorRenderTarget() const;
	const MRenderTarget* getCurrentDepthRenderTarget() const;

	//! Display styles
    enum DisplayStyle
    {
        kGouraudShaded = (0x1),        //!< Shaded display.
        kWireFrame = (0x1 << 1),       //!< Wire frame display.
		kBoundingBox = (0x1 << 2),     //!< Bounding box display.
		kTextured = (0x1 << 3),        //!< Textured display.
		kDefaultMaterial = (0x1 << 4), //!< Default material display.
		kXrayJoint = (0x1 << 5),       //!< X-ray joint display.
		kXray = (0x1 << 6),            //!< X-ray display.
		kTwoSidedLighting = (0x1 << 7), //!< Two-sided lighting enabled.
		kFlatShaded = (0x1 << 8),		//!< Flat shading display.
		kShadeActiveOnly = (0x1 << 9),	//!< Shade active object only.
		kXrayActiveComponents = (0x1 << 10), //!< X-ray active components.
		kBackfaceCulling = (0x1 << 11), //!< Backface culling enabled.
		kSmoothWireframe = (0x1 << 12) //!< Smooth wireframe enabled.
    };
	unsigned int getDisplayStyle() const;

	//! Lighting modes
	enum LightingMode
	{
		kNoLighting,	 //!< Use no light
		kAmbientLight,   //!< Use global ambient light
		kLightDefault,   //!< Use default light
		kSelectedLights, //!< Use lights which are selected
		kSceneLights,    //!< Use all lights in the scene
		kCustomLights	 //!< A custom set of lights which are not part of the scene. Currently this applies for use in the Hypershade Material Viewer panel
	};
	LightingMode getLightingMode() const;
	unsigned int getLightLimit() const;

	//! Struct for environment map parameters.
	struct EnvironmentParams
	{
		bool enabled;     //!< True if an environment is enabled.
		MString filePath; //!< Full path to the environment image file. The image is in lat-long format.
	};

	EnvironmentParams getEnvironmentParameters() const;

	//! Types of post effects which may be enabled during rendering
	enum PostEffectType {
		kAmbientOcclusion,	//!< Screen-space ambient occlusion
		kMotionBlur,		//!< 2D Motion blur
		kGammaCorrection,	//!< Gamma correction
        kViewColorTransformEnabled = kGammaCorrection, //!< Color managed viewing
		kDepthOfField,		//!< Depth of field
		kAntiAliasing		//!< Hardware multi-sampling
	};
	bool getPostEffectEnabled( PostEffectType postEffectType ) const;

	//! Types of the fog mode
	enum FogMode {
		kFogLinear,	//!< The linear fog
		kFogExp,		//!< The exponential fog
		kFogExp2	//!< The exponential squared fog
	};
	//! Struct for hardware fog parameters
	struct HwFogParams
	{
		bool	HwFogEnabled;	//!< If hardware fog is enabled
		FogMode HwFogMode; //!< Hardware fog mode, like Linear, Exponential, Exponential squared.
		float HwFogStart;		//!< The near distance used in the linear fog.
		float HwFogEnd;		//!< The far distance used in the linear fog.
		float HwFogDensity;		//!< The density of the exponential fog.
		MColor HwFogColor;		//!< The fog color includes (r, g, b, a).
	};

	HwFogParams getHwFogParameters() const;

	//! Options for transparency algorithm
	enum TransparencyAlgorithm {
		kUnsorted = 0,	//!< Unsorted transparent object drawing
		kObjectSorting,	//!< Object sorting of transparent objects
		kWeightedAverage, //!< Weight average transparency
		kDepthPeeling, //!< Depth-peel transparency
	};
	TransparencyAlgorithm getTransparencyAlgorithm() const;

	static bool inUserInteraction();
	static bool userChangingViewContext();
	//! Wireframe on shaded modes
	enum WireOnShadedMode
	{
		kWireframeOnShadedFull,		//!< Draw wireframe
		kWireFrameOnShadedReduced,	//!< Draw wireframe but with reduced quality
		kWireFrameOnShadedNone		//!< Do not draw wireframe
	};
	static WireOnShadedMode wireOnShadedMode();
	static bool shadeTemplates();

	//! Rendering destinations
	enum RenderingDestination
	{
		k3dViewport,				//!< Rendering to an interactive 3d viewport
		k2dViewport,				//!< Rendering to an interactive 2d viewport such as the render view
		kImage						//!< Rendering to an image
	};
	RenderingDestination renderingDestination(MString & destinationName) const;

	/*!
	Bit flags specifying types of objects excluded from displaying in the viewport.
	*/
	//@{
	static const MUint64 kExcludeNone				= MUint64(0);			//!< Exclude no object types
	static const MUint64 kExcludeNurbsCurves		= MUint64(1) << (0);	//!< Exclude NURBS curves
	static const MUint64 kExcludeNurbsSurfaces		= MUint64(1) << (1);	//!< Exclude NURBS surface
	static const MUint64 kExcludeMeshes				= MUint64(1) << (2);	//!< Exclude polygonal meshes
	static const MUint64 kExcludePlanes				= MUint64(1) << (3);	//!< Exclude planes
	static const MUint64 kExcludeLights				= MUint64(1) << (4);	//!< Exclude lights
	static const MUint64 kExcludeCameras			= MUint64(1) << (5);	//!< Exclude camera
	static const MUint64 kExcludeJoints				= MUint64(1) << (6);	//!< Exclude joints
	static const MUint64 kExcludeIkHandles			= MUint64(1) << (7);	//!< Exclude IK handles
	static const MUint64 kExcludeDeformers			= MUint64(1) << (8);	//!< Exclude all deformations
	static const MUint64 kExcludeDynamics			= MUint64(1) << (9);	//!< Exclude all dynamics objects (emiiters, cloth)
	static const MUint64 kExcludeParticleInstancers	= MUint64(1) << (10);	//!< Exclude all Particle Instancers
	static const MUint64 kExcludeLocators			= MUint64(1) << (11);	//!< Exclude locators
	static const MUint64 kExcludeDimensions			= MUint64(1) << (12);	//!< Exclude all measurement objects
	static const MUint64 kExcludeSelectHandles		= MUint64(1) << (13);	//!< Exclude selection handles
	static const MUint64 kExcludePivots				= MUint64(1) << (14);	//!< Exclude pivots
	static const MUint64 kExcludeTextures			= MUint64(1) << (15);	//!< Exclude texure placement objects
	static const MUint64 kExcludeGrid				= MUint64(1) << (16);	//!< Exclude grid drawing
	static const MUint64 kExcludeCVs				= MUint64(1) << (17);	//!< Exclude NURBS control vertices
	static const MUint64 kExcludeHulls				= MUint64(1) << (18);	//!< Exclude NURBS hulls
	static const MUint64 kExcludeStrokes			= MUint64(1) << (19);	//!< Exclude PaintFX strokes
	static const MUint64 kExcludeSubdivSurfaces		= MUint64(1) << (20);	//!< Exclude subdivision surfaces
	static const MUint64 kExcludeFluids				= MUint64(1) << (21);	//!< Exclude fluid objects
	static const MUint64 kExcludeFollicles			= MUint64(1) << (22);	//!< Exclude hair follicles
	static const MUint64 kExcludeHairSystems		= MUint64(1) << (23);	//!< Exclude hair system
	static const MUint64 kExcludeImagePlane			= MUint64(1) << (24);	//!< Exclude image planes
	static const MUint64 kExcludeNCloths			= MUint64(1) << (25);	//!< Exclude N-cloth objects
	static const MUint64 kExcludeNRigids			= MUint64(1) << (26);	//!< Exclude rigid-body objects
	static const MUint64 kExcludeDynamicConstraints	= MUint64(1) << (27);	//!< Exclude rigid-body constraints
	static const MUint64 kExcludeManipulators		= MUint64(1) << (28);	//!< Exclude manipulators
	static const MUint64 kExcludeNParticles			= MUint64(1) << (29);	//!< Exclude N-particle objects
	static const MUint64 kExcludeMotionTrails		= MUint64(1) << (30);	//!< Exclude motion trails
	static const MUint64 kExcludeHoldOuts			= MUint64(1) << (31);	//!< Exclude Hold-Outs
	static const MUint64 kExcludePluginShapes		= MUint64(1) << (32);	//!< Exclude plug-in shapes
	static const MUint64 kExcludeHUD				= MUint64(1) << (33);	//!< Exclude HUD
	static const MUint64 kExcludeClipGhosts			= MUint64(1) << (34);	//!< Exclude animation clip ghosts
	static const MUint64 kExcludeGreasePencils		= MUint64(1) << (35);	//!< Exclude grease-pencil draw
	static const MUint64 kExcludeControllers		= MUint64(1) << (36);	//!< Exclude Controllers
	static const MUint64 kExcludeAll				= ~MUint64(0);			//!< Exclude all listed object types
	//@}

	MUint64 objectTypeExclusions() const;
	void classificationExclusions(MStringArray &classification) const;

	//! Background parameters
	struct BackgroundParams
	{
		bool displayGradient;		//!< Display gradient
		bool clearColorFlag;		//!< Clear color flag
		bool clearDepthFlag;		//!< Clear depth flag
		bool clearStencilFlag;		//!< Clear stencil flag
		MColor clearColor1;			//!< Clear color 1
		MColor clearColor2;			//!< Clear color 2
		float clearDepthValue;		//!< Clear depth value
		int clearStencilValue;		//!< Clear stencil value
	};
	void getBackgroundParameters( MFrameContext::BackgroundParams & params ) const;

	//! Depth-of-field parameters
	struct DOFParams
	{
		bool enabled;			//!< True if DOF is enabled
		float focalDistance;	//!< Camera focus distance
		float alpha;			//!< Combination of camera paramaters: a = F*F/(A*(Zf-F))
	};
	void getDOFParameters(MFrameContext::DOFParams &params) const;

	//! Render override information
	struct RenderOverrideInformation
	{
		MString overrideName;	//!< Name of any active override. Empty string if none
	};
	void getRenderOverrideInformation(RenderOverrideInformation &info) const;

	void worldToViewport(const MPoint &inPoint, double &xPos, double &yPos) const;
	void viewportToWorld(double xPos, double yPos, MPoint &worldNearPt, MPoint &worldFarPt) const;

	static const char* className();

protected:
	MFrameContext();
	virtual ~MFrameContext();

	friend class MRenderOverride;
	friend class MPxGeometryOverride;
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFrameContext */
