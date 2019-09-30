#ifndef _MRenderer
#define _MRenderer
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
// CLASS:    MHWRender::MRenderer
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MHWRender::MRenderer)
//
//  MHWRender::MRenderer is the main interface class to the renderer
//	which is used for rendering interactive viewports in "Viewport 2.0" mode
//	as well as for rendering with the "Maya Harware 2.0" batch renderer.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MMatrix.h>
#include <maya/MFloatPoint.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>
#include <maya/MStringArray.h>
#include <maya/MDeprecate.h>


// ****************************************************************************
// NAMESPACE
namespace MHWRender
{

//! Draw API identifiers
enum DrawAPI
{
	kNone = 0,							//!< Uninitialized device
	kOpenGL = 1 << 0,					//!< OpenGL
	kDirectX11 = 1 << 1,				//!< Direct X 11
	kOpenGLCoreProfile = 1 << 2,		//!< Core Profile OpenGL
	kAllDevices = kOpenGL | kDirectX11 | kOpenGLCoreProfile	//!< All : OpenGL and Direct X 11
};

/*!
	Type-safe bitwise 'or' operator for GPU device type flags

	\param[in] a First device flag
	\param[in] b Second device flag

	\return Bitwise 'or' of input flags
*/
inline MHWRender::DrawAPI operator|(
	MHWRender::DrawAPI a, MHWRender::DrawAPI b)
{
	return static_cast<MHWRender::DrawAPI>(
		static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

/*! Pixel / raster formats

The following short form notation is used for
channel specification:

- R = red channel
- G = green channel
- B = blue channel
- A = alpha channel
- E = exponent channel
- L = luminence channel
- X = channel is not used
- DXT1, DXT2, DXT3, DXT4, and DXT5 are S3 compression formats
- BC6H, BC7 are extended compression formats only for DirectX 11
- Numbers after the channel gives the bit depth
- Channel order is data storage order.

The following short form notation is used for
data format specification:

- UNORM means unsigned values which have been normalized to the  0 to 1 range.
- SNORM means signed values which have been normalized to the -1 to 1 range.
- UINT means unsigned integer values
- SINT means signed integer values
- FLOAT means floating point
- If normalization is not explicitly specified then the values are unnormalized
*/
enum MRasterFormat {
	// Depth formats
	kD24S8 = 0,		//!< Depth: 24-bit, Stencil 8-bit
	kD24X8,			//!< Depth: 24-bit
	kD32_FLOAT,		//!< Depth 32-bit

	kR24G8,			//!< Red 24-bit, Green 8-bit
	kR24X8,			//!< Red 24-bit

	// Compressed formats
	kDXT1_UNORM,			//!< DXT1 : unsigned
	kDXT1_UNORM_SRGB,		//!< DXT1 : unsigned, sRGB format
	kDXT2_UNORM,			//!< DXT2 : unsigned
	kDXT2_UNORM_SRGB,		//!< DXT2 : sRGB format
	kDXT2_UNORM_PREALPHA,	//!< DXT2, pre-multiplied alpha
	kDXT3_UNORM,			//!< DXT3 : unsigned
	kDXT3_UNORM_SRGB,		//!< DXT3 : unsigned, sRGB format
	kDXT3_UNORM_PREALPHA,	//!< DXT3, pre-multiplied alpha
	kDXT4_UNORM,			//!< DXT4 : unsigned
	kDXT4_SNORM,			//!< DXT4 : signed
	kDXT5_UNORM,			//!< DXT5 : unsigned
	kDXT5_SNORM,			//!< DXT5 : signed
	kBC6H_UF16,				//!< BC6H : no sign bit, 16-bit floating point
	kBC6H_SF16,				//!< BC6H : with sign bit, 16-bit floating point
	kBC7_UNORM,				//!< BC7 : unsigned
	kBC7_UNORM_SRGB,		//!< BC7 : unsigned, sRGB format

	kR9G9B9E5_FLOAT, //!< HDR format : 9 bits for each of RGB, no alpha, 5 bit shared exponent

	// 1-bit formats
	kR1_UNORM,		//!< Red: 1-bit

	// 8-bit formats
	kA8,			//!< Alpha: 8-bit
	kR8_UNORM,		//!< Red: 8-bit
	kR8_SNORM,      //!< Red: 8-bit signed
	kR8_UINT,       //!< Red: 8-bit unsigned integer
	kR8_SINT,		//!< Red: 8-bit signed integer
	kL8,				//!< Luminence: 8-bit

	// 16-bit formats
	kR16_FLOAT,		//!< Red: 16-bit float
	kR16_UNORM,		//!< Red: 16-bit unsigned
	kR16_SNORM,		//!< Red: 16-bit signed
	kR16_UINT,		//!< Red: 16-bit unsigned integer
	kR16_SINT,		//!< Red: 16-bit signed integer
	kL16,			//!< Luminence, 16-bit
	kR8G8_UNORM,	//!< Red: 8-bit, Green : 8-bit, unsigned
	kR8G8_SNORM,	//!< Red: 8-bit, Green : 8-bit, signed
	kR8G8_UINT,		//!< Red: 8-bit, Green : 8-bit, unsigned integer
	kR8G8_SINT,		//!< Red: 8-bit, Green : 8-bit, signed integer
	kB5G5R5A1,		//!< RGB : 5-bits each, Alpha : 1-bit
	kB5G6R5,		//!< RGB : 5-bits each, Alpha : 1-bit

	// 32-bit formats
	kR32_FLOAT,		//!< Red : 32-bit float
	kR32_UINT,		//!< Red : 32-bit unsigned integer
	kR32_SINT,		//!< Red : 32-bit signed integer
	kR16G16_FLOAT,	//!< Red and green : 16-bit float each
	kR16G16_UNORM,	//!< Red and green : 16-bit unsigned
	kR16G16_SNORM,	//!< Red and green : 16-bit signed
	kR16G16_UINT,	//!< Red and green : 16-bit unsigned
	kR16G16_SINT,	//!< Red and green : 16-bit signed
	kR8G8B8A8_UNORM, //!< RGBA : 8-bits unsigned each
	kR8G8B8A8_SNORM, //!< RGBA : 8-bits signed each
	kR8G8B8A8_UINT,	//!< RGBA : 8-bits unsigned integer each
	kR8G8B8A8_SINT,	//!< RGBA : 8-bits signed integer each
	kR10G10B10A2_UNORM, //!< 2 bit alpha, 10 bits for each of RGB
	kR10G10B10A2_UINT, //!< 2 bit alpha, 10 bits for each of RGB, unsigned integer
	kB8G8R8A8,		//!< BGRA : 8-bits each
	kB8G8R8X8,		//!< BGR : 8-bits each. No alpha
	kR8G8B8X8,      //!< RGB : 8-bits each
	kA8B8G8R8,		//< ABGR : 8-bits each

	// 64 bit formats
	kR32G32_FLOAT,	//!< RG : 32-bits float each
	kR32G32_UINT,	//!< RG : 32-bits unsigned each
	kR32G32_SINT,	//!< RG : 32-bits signed each
	kR16G16B16A16_FLOAT, //!< RGBA : 16-bits float each
	kR16G16B16A16_UNORM, //!< RGBA : 16-bits unsigned each
	kR16G16B16A16_SNORM, //!< RGBA : 16-bits signed each
	kR16G16B16A16_UINT, //!< RGBA : 16-bits unsigned integer each
	kR16G16B16A16_SINT, //!< RGBA : 16-bits unsigned integer each

	// 96-bit formats
	kR32G32B32_FLOAT, //!< RGB : 32-bits float each
	kR32G32B32_UINT, //!< RGB : 32-bits unsigned integer each
	kR32G32B32_SINT, //!< RGB : 32-bits signed integer each

	// 128-bit formats
	kR32G32B32A32_FLOAT, //!< RGBA : 32-bits float each
	kR32G32B32A32_UINT, //!< RGBA : 32-bits unsigned integer each
	kR32G32B32A32_SINT, //!< RGBA : 32-bits signed integer each

	kNumberOfRasterFormats //!< Not to be used to describe a raster. This is the number of rasters formats
};

} // namespace MHWRender


OPENMAYA_MAJOR_NAMESPACE_OPEN

namespace MHWRender
{


// ****************************************************************************
/*! Camera override description. Provides information for specifying
	a camera override for a render operation.
*/
class OPENMAYARENDER_EXPORT MCameraOverride
{
public:
	MCameraOverride()
	{
		mUseProjectionMatrix = false;
		mUseViewMatrix = false;
		mUseNearClippingPlane = false;
		mUseFarClippingPlane = false;
		mUseHiddenCameraList = false;
	}
	MDagPath mCameraPath;		//!< Camera path override
	MDagPathArray mHiddenCameraList; //!< List of cameras that should not be made visible when rendering
	bool mUseHiddenCameraList;	//!< Whether to use hidden camera list override
	bool mUseProjectionMatrix;	//!< Whether to use the projection matrix override
	MMatrix mProjectionMatrix;	//!< Camera projection matrix override
	bool mUseViewMatrix;		//!< Whether to use the view matrix override
	MMatrix mViewMatrix;		//!< Camera view matrix override
	bool mUseNearClippingPlane; //!< Whether to use the near clipping plane override
	double mNearClippingPlane;  //!< Near clipping plane override
	bool mUseFarClippingPlane;  //!< Whether to use the far clipping plane override
	double mFarClippingPlane;   //!< Far clipping plane override
};

// ****************************************************************************
// CLASS DECLARATION (MRenderOperation)
//! \ingroup OpenMayaRender
//! \brief Class which defines a rendering operation
//
class OPENMAYARENDER_EXPORT MRenderOperation
{
public:

    // Standard operation names
    static const MString kStandardBackgroundName;  
    static const MString kStandardSceneName;    
    static const MString kStandardHUDName;
    static const MString kStandardPresentName;

    // Standard target names
    static const MString kColorTargetName;  
    static const MString kDepthTargetName;
    static const MString kAuxiliaryTargetName;
    static const MString kAuxiliaryDepthTargetName;

	virtual ~MRenderOperation();

	// sRGB write enable for render targets
	virtual bool enableSRGBWrite();

	// Viewport rectangle override
	virtual const MFloatPoint * viewportRectangleOverride();

	// Identifier query
	virtual const MString & name() const;

	// target related overrides
    virtual bool getInputTargetDescription(const MString& name, MRenderTargetDescription& description);
    virtual int writableTargets(unsigned int& count);
	virtual MRenderTarget* const* targetOverrideList(unsigned int &listSize);

    bool enabled() { return mEnabled; }
    void setEnabled(bool enable) { mEnabled = enable; }

    // Target handling
    const MStringArray& inputTargets();
    const MStringArray& outputTargets();
    void renameOutputTarget(const MString& name, const MString& newName);

	/*!
		Supported render operation types
	*/
	enum MRenderOperationType
	{
		kClear, //!< Clear background operation
		kSceneRender, //!< Render a 3d scene
		kQuadRender, //!< Render a 2d quad
		kUserDefined, //!< User defined operation
		kHUDRender, //!< 2D HUD draw operation
		kPresentTarget //!< Present target for viewing
	};

	// Type identifier query
	MRenderOperationType operationType() const;

protected:
	MRenderOperation( const MString & name );
	MRenderOperation();

    MRenderTarget* getInputTarget(const MString& name);

	MRenderOperationType mOperationType; //!< Operation type

	MString mName; 	//!< Identifier for a sub render

    MStringArray mInputTargetNames;
    MStringArray mOutputTargetNames;

private:
    void initializeTargetArray();
    bool setOutputTarget(const MString& name, MRenderTarget* target);

    MHWRender::MRenderTarget **mRenderTargets;
    bool mEnabled;

};

// ****************************************************************************
// CLASS DECLARATION (MRenderOperationList)
//! \ingroup OpenMayaRender
//! \brief Base class for holding a list of MRenderOperations.  The list is the owner of the operations.
//
class OPENMAYARENDER_EXPORT MRenderOperationList
{
public:
    MRenderOperationList();
    ~MRenderOperationList();

    int	length() const;
    const MRenderOperation* operator[]( int index ) const;
    MRenderOperation* operator[]( int index );

    bool replace(const MString& name, MRenderOperation* operation);

    bool append(MRenderOperation* operation);
    bool insertBefore(const MString& name, MRenderOperation* operation);
    bool insertAfter(const MString& name, MRenderOperation* operation);
    bool remove(const MString& name);
    bool removeAt(int index);
    MRenderOperation* take(const MString& name);
    MRenderOperation* takeAt(int index);
    int indexOf(const MString& name);
    void clear();

    static const char* className();

private:

    MRenderOperationList(const MRenderOperationList&);
    MRenderOperationList& operator=(const MRenderOperationList&);

    void* fData;
};

// ****************************************************************************
// CLASS DECLARATION (MUserRenderOperation)
//! \ingroup OpenMayaRender
//! \brief Class which defines a user defined rendering operation
//
class OPENMAYARENDER_EXPORT MUserRenderOperation : public MRenderOperation
{
public:
	MUserRenderOperation(const MString &name);
	virtual ~MUserRenderOperation();

	// Camera override
	virtual const MCameraOverride * cameraOverride();

	// Derived classes define the what the operation does
	virtual MStatus execute(const MDrawContext & drawContext ) = 0;

	// If it has some UI drawables to add
	virtual bool hasUIDrawables() const ;

	// Derived class to override this method for adding some UI drawables
	virtual void addUIDrawables( MUIDrawManager& drawManager, const MFrameContext& frameContext );

	// Requires access to light data
	virtual bool requiresLightData() const;
};

// ****************************************************************************
// CLASS DECLARATION (MHUDRender)
//! \ingroup OpenMayaRender
//! \brief Class which defines rendering the 2D heads-up-display
//
class OPENMAYARENDER_EXPORT MHUDRender : public MRenderOperation
{
public:
	MHUDRender();
	virtual ~MHUDRender();

	const MString & name() const;

	// If it has some UI drawables to add
	virtual bool hasUIDrawables() const ;

	// Derived class to override this method for adding some 2D UI drawables
	virtual void addUIDrawables( MUIDrawManager& drawManager2D, const MFrameContext& frameContext );
};

// ****************************************************************************
// CLASS DECLARATION (MPresentTarget)
//! \ingroup OpenMayaRender
//! \brief Class which defines the operation of
//!  presenting a target for final output.
//
class OPENMAYARENDER_EXPORT MPresentTarget : public MRenderOperation
{
public:
	/*!
		Supported output target back-buffer options.

		If the final output target is an  on-screen OpenGL context which
		supports active stereo rendering then it is possible to send
		the output to either the left or right back-buffer.

		If the final output target is an offscreen target or if the on-screen OpenGL
		context being used does not support active stereo, then only the 'center'
		option can be used. 'Center' is the default back-buffer associated
		with an OpenGL context. If 'left' or 'right' options are specified
		they will be ignored in these cases.

		This option is currently ignored if the active rendering API is not OpenGL.
	*/
	enum MTargetBackBuffer
	{
		kCenterBuffer,	//!< Default or 'center' buffer
		kLeftBuffer,	//!< Left back-buffer
		kRightBuffer	//!< Right back-buffer
	};

	MPresentTarget(const MString &name);
	virtual ~MPresentTarget();

	bool presentDepth() const;
	void setPresentDepth(bool val);

    bool enableSRGBWrite();

	MTargetBackBuffer targetBackBuffer() const;
	void setTargetBackBuffer( MTargetBackBuffer backBuffer );

protected:
	MTargetBackBuffer mTargetBackBuffer; //!< Back-buffer of output target to render to
	bool mPresentDepth; //!< Present depth
};

// ****************************************************************************
// CLASS DECLARATION (MClearOperation)
//! \ingroup OpenMayaRender
//! \brief Class which defines the operation of
//!  clearing render target channels.
//
class OPENMAYARENDER_EXPORT MClearOperation : public MRenderOperation
{
public:

	/*!
		ClearMask describes the set of channels to clear
		If the mask value is set then that given channel will
		be cleared.
	*/
	enum ClearMask
	{
		kClearNone = 0, //!< Clear nothing
		kClearColor = 1, //!< Clear color
		kClearDepth = 1 << 1 , //!< Clear depth
		kClearStencil = 1 << 2, //!< Clear stencil
		kClearAll = ~0 //!< Clear all
	};

    MClearOperation(const MString &name);
    virtual ~MClearOperation();

    bool overridesColors() const;
	unsigned int mask() const;
	const float* clearColor() const;
	bool clearGradient() const;
	const float* clearColor2() const;
	int clearStencil() const;
	float clearDepth() const;

    void setOverridesColors(bool override);
	void setMask( unsigned int mask );
	void setClearColor( float value[4] );
	void setClearGradient( bool value );
	void setClearColor2( float value[4] );
	void setClearStencil( int value );
	void setClearDepth( float value );

protected:
	unsigned int mClearMask;	//!< Clear mask
	float mClearColor[4];		//!< Clear color value
	float mClearColor2[4];		//!< Secondary clear color value. Used when gradient background drawing enabled
	bool mClearGradient;		//!< Flag to indicate whether to clear the gradient
	int mClearStencil;			//!< Clear stencil value
	float mClearDepth;			//!< Clear depth value
    bool mOverridesColors;

private:
	MClearOperation();

	friend class MSceneRender;
	friend class MQuadRender;
};

// ****************************************************************************
// CLASS DECLARATION (MSceneRender)
//! \ingroup OpenMayaRender
//! \brief Class which defines a scene render
//
class OPENMAYARENDER_EXPORT MSceneRender : public MRenderOperation
{
public:
	//! Object type exclusions
	//! This enumeration is marked to be deprecated. Use the MFramentContext static
	//! member equivalents instead which is a superset of this enumeration.
	enum MObjectTypeExclusions
	{
		kExcludeNone				= 0,		//!< Exclude no object types
		kExcludeNurbsCurves			= 1<<(0),	//!< Exclude NURBS curves
		kExcludeNurbsSurfaces		= 1<<(1),	//!< Exclude NURBS surface
		kExcludeMeshes				= 1<<(2),	//!< Exclude polygonal meshes
		kExcludePlanes				= 1<<(3),	//!< Exclude planes
		kExcludeLights				= 1<<(4),	//!< Exclude lights
		kExcludeCameras				= 1<<(5),	//!< Exclude camera
		kExcludeJoints				= 1<<(6),	//!< Exclude joints
		kExcludeIkHandles			= 1<<(7),	//!< Exclude IK handles
		kExcludeDeformers			= 1<<(8),	//!< Exclude all deformations
		kExcludeDynamics			= 1<<(9),	//!< Exclude all dynamics objects (emitters, cloth)
		kExcludeParticleInstancers	= 1<<(10),	//!< Exclude all Particle Instancers
		kExcludeLocators			= 1<<(11),	//!< Exclude locators
		kExcludeDimensions			= 1<<(12),	//!< Exclude all measurement objects
		kExcludeSelectHandles		= 1<<(13),	//!< Exclude selection handles
		kExcludePivots				= 1<<(14),	//!< Exclude pivots
		kExcludeTextures			= 1<<(15),	//!< Exclude texture placement objects
		kExcludeGrid				= 1<<(16),	//!< Exclude grid drawing
		kExcludeCVs					= 1<<(17),	//!< Exclude NURBS control vertices
		kExcludeHulls				= 1<<(18),	//!< Exclude NURBS hulls
		kExcludeStrokes				= 1<<(19),	//!< Exclude PaintFX strokes
		kExcludeSubdivSurfaces		= 1<<(20),	//!< Exclude subdivision surfaces
		kExcludeFluids				= 1<<(21),	//!< Exclude fluid objects
		kExcludeFollicles			= 1<<(22),	//!< Exclude hair follicles
		kExcludeHairSystems			= 1<<(23),	//!< Exclude hair system
		kExcludeImagePlane			= 1<<(24),	//!< Exclude image planes
		kExcludeNCloths				= 1<<(25),	//!< Exclude N-cloth objects
		kExcludeNRigids				= 1<<(26),	//!< Exclude rigid-body objects
		kExcludeDynamicConstraints	= 1<<(27),	//!< Exclude rigid-body constraints
		kExcludeManipulators		= 1<<(28),	//!< Exclude manipulators
		kExcludeNParticles			= 1<<(29),	//!< Exclude N-particle objects
		kExcludeMotionTrails        = 1<<(30),	//!< Exclude motion trails
		kExcludeHoldOuts            = 1<<(31),	//!< Exclude Hold-Outs		
		kExcludeAll					= ~0		//!< Exclude all listed object types
	} ;

	MSceneRender(const MString &name);
	MSceneRender(const MString &name, const MString &fragmentName);
	virtual ~MSceneRender();

	// Operation pre and post render callbacks
	virtual void preRender();
	virtual void postRender();
	virtual void preSceneRender(const MDrawContext & context);
	virtual void postSceneRender(const MDrawContext & context);

	//! Render filter options. Refer to the renderFilterOverride() method
	//! for details on usage.
	enum MSceneFilterOption
	{
		kNoSceneFilterOverride 				= 0,
		kRenderPreSceneUIItems				= 1 << (0),	//!< Render UI items before scene render like grid or user added pre-scene UI items
		kRenderOpaqueShadedItems			= 1 << (1),	//!< Render only opaque shaded objects but not their wireframe or components
		kRenderTransparentShadedItems	    = 1 << (2),	//!< Render only transparent shaded objects but not their wireframe or components
		kRenderShadedItems					= (kRenderOpaqueShadedItems | kRenderTransparentShadedItems),	//!< Render only shaded (opaque and transparent) objects but not their wireframe or components
		kRenderPostSceneUIItems				= 1 << (3),	//!< Render UI items after scene render like wireframe and components for surfaces as well as non-surface objects.
		kRenderUIItems						= (kRenderPreSceneUIItems | kRenderPostSceneUIItems),
		kRenderNonShadedItems				= kRenderUIItems, //! This flag has same meaning as kRenderUIItems and will be deprecated in the future.
		kRenderAllItems						= ~0		//!< Render all items.
	};
	virtual MSceneFilterOption renderFilterOverride();

	// Camera override
	virtual const MCameraOverride * cameraOverride();

	// Object set filter
	virtual const MSelectionList * objectSetOverride();

	// Shader override
	virtual const MShaderInstance* shaderOverride();

	// Object type exclusions
	virtual MUint64 getObjectTypeExclusions();

	//! Display modes
    enum MDisplayMode {
		kNoDisplayModeOverride = 0,	//!< No display mode override
        kWireFrame = 1<<(0),		//!< Display wireframe
		kShaded = 1<<(1),			//!< Display smooth shaded
		kFlatShaded = 1<<(2),		//!< Display flat shaded
		kShadeActiveOnly = 1<<(3),	//!< Shade active objects. Only applicable if kShaded or kFlatShaded is enabled. 
		kBoundingBox = 1<<(4),		//!< Display bounding boxes
		kDefaultMaterial = 1<<(5),	//!< Use default material. Only applicable if kShaded or kFlatShaded is enabled. 
		kTextured = 1<<(6),			//!< Display textured. Only applicable if kShaded or kFlatShaded is enabled. 		
    };
	virtual MDisplayMode displayModeOverride();

	//! Lighting mode
	enum MLightingMode {
		kNoLightingModeOverride = 0, //!< No lighting mode override
		kNoLight,			//!< Use no light
		kAmbientLight,		//!< Use global ambient light
		kLightDefault,		//!< Use default light
		kSelectedLights,	//!< Use lights which are selected
		kSceneLights		//!< Use all lights in the scene
	};
	virtual MLightingMode lightModeOverride();
	virtual const bool* shadowEnableOverride();

	//! Post effect override
	enum MPostEffectsOverride
	{
		kPostEffectDisableNone = 0,		//!< Use current render settings options
		kPostEffectDisableSSAO = 1 << (0),	//!< Disable SSAO post effect
		kPostEffectDisableMotionBlur = 1 << (1), //!< Disable motion blur post effect
		kPostEffectDisableDOF = 1 << (2),	//!< Disable depth-of-field post effect
		kPostEffectDisableAll = ~0	//!< Disable all post effects
	};
	virtual MPostEffectsOverride postEffectsOverride();

	//! Culling option
	enum MCullingOption {
		kNoCullingOverride = 0,	//!< No culling override
		kCullNone,				//!< Don't perform culling
		kCullBackFaces,			//!< Cull back faces
		kCullFrontFaces,		//!< Cull front faces
	};
	virtual MCullingOption cullingOverride();

	// Clear operation helper
	virtual MClearOperation & clearOperation();

	// If it has some UI drawables to add
	virtual bool hasUIDrawables() const ;

	// Derived class to override this method for adding some pre-scene UI drawables
	virtual void addPreUIDrawables( MUIDrawManager& drawManager, const MFrameContext& frameContext );

	// Derived class to override this method for adding some post-scene UI drawables
	virtual void addPostUIDrawables( MUIDrawManager& drawManager, const MFrameContext& frameContext );
    
    // Get an edit access to the parameters driving this operation
    MRenderParameters* getParameters();

    // Optional name of the fragment to use when rendering the scene.
    MString fragmentName();

	// Deprecated methods
    OPENMAYA_DEPRECATED(2019, "Please use getObjectTypeExclusions() instead.")
	virtual MObjectTypeExclusions objectTypeExclusions();

protected:

    void* mFragment;
    MRenderParameters* mParameters;
	MClearOperation mClearOperation; //!< Clear operation

};

// ****************************************************************************
// CLASS DECLARATION (MQuadRender)
//! \ingroup OpenMayaRender
//! \brief Class which defines a 2d geometry quad render
//
class OPENMAYARENDER_EXPORT MQuadRender : public MRenderOperation
{
public:
	MQuadRender(const MString &name);
	virtual ~MQuadRender();

	// Shader
	virtual const MShaderInstance* shader();

	// Clear operation helper
	virtual MClearOperation & clearOperation();

	// State overrides
	virtual const MDepthStencilState* depthStencilStateOverride();
	virtual const MRasterizerState* rasterizerStateOverride();
	virtual const MBlendState* blendStateOverride();
protected:
	MClearOperation mClearOperation; //!< Clear operation
};

// ****************************************************************************
// CLASS DECLARATION (MRenderOverride)
//! \ingroup OpenMayaRender
//! \brief Base class for defining a rendering override
//
class OPENMAYARENDER_EXPORT MRenderOverride
{
public:
	MRenderOverride( const MString & name );
	virtual ~MRenderOverride();

	// Indicates which draw APIs are supported
	virtual ::MHWRender::DrawAPI supportedDrawAPIs() const;

	// Methods to iterate through operations
	// on an override
	virtual bool startOperationIterator();
	virtual MRenderOperation * renderOperation();
	virtual bool nextRenderOperation();

	// Identifier
	const MString & name() const;

	// Label to use in the user interface
	virtual MString uiName() const;

	// Pre / post setup and cleanup methods
	virtual MStatus setup( const MString & destination );
	virtual MStatus cleanup();

	// Frame context
	const MFrameContext* getFrameContext() const;

	// Unique identifier for the override
	MString mName;

protected:

    MHWRender::MRenderOperationList mOperations;

private:
	MFrameContext mFrameContext;
	bool mOutsideSetupCleanup;

    int mCurrentOperationIndex;

	friend class M3dView;
	MRenderOverride();
};

// ****************************************************************************
// CLASS DECLARATION (MRenderScriptCallback)
//! \ingroup OpenMayaRender
//! \brief Class which defines a callback that can be executed from a script fragment.
//!
//! An MRenderScriptCallback is a class that can be implemented and executed from a script fragment.
//! Derived instances of this class can be registered and deregistered with the MRenderer class.
//! Once registered they can be called by name using the "Call" script command in script fragments used by an MSceneRender.
//! The renderer's parameter set plus up to three additional script arguments can be passed into the callback from the script.
//
class OPENMAYARENDER_EXPORT MRenderScriptCallback
{
public:
    MRenderScriptCallback() {}
    virtual ~MRenderScriptCallback() {}

	virtual MString name() const=0;
    virtual bool execute(MRenderParameters* params, MString arg1, MString arg2, MString arg3) = 0;
};


// ****************************************************************************
// CLASS DECLARATION (MRenderer)
//! \ingroup OpenMayaRender
//! \brief Main interface class to the Viewport 2.0 renderer.
//
class OPENMAYARENDER_EXPORT MRenderer
{
public:
	// The renderer
	static MRenderer *theRenderer(bool initializeRenderer = true);

	//////////////////////////////////////////////////////////////////
	// Drawing API information
	//
	::MHWRender::DrawAPI drawAPI() const;
	bool drawAPIIsOpenGL() const;
	unsigned int drawAPIVersion() const;
	void * GPUDeviceHandle() const;
	unsigned int GPUmaximumVertexBufferSize() const;
	unsigned int GPUmaximumPrimitiveCount() const;
	void GPUmaximumOutputTargetSize(unsigned int& w, unsigned int& h) const;
	MInt64 GPUtotalMemorySize() const;

    //! GPU memory type
    enum MGPUMemType {
        kMemTexture,    //!< GPU texture used memory
        kMemAll         //!< GPU total used memory
    };
    MInt64 GPUUsedMemorySize(MGPUMemType type) const;

	//////////////////////////////////////////////////////////////////
	// Fragment manager
	//
	MFragmentManager* getFragmentManager() const;

	//////////////////////////////////////////////////////////////////
	// Shader manager
	//
	const MShaderManager* getShaderManager() const;

	//////////////////////////////////////////////////////////////////
	// Target manager
	//
	const MRenderTargetManager* getRenderTargetManager() const;

	bool copyTargetToScreen(const MRenderTarget* renderTarget);

	bool render(const MString& sourceName, MRenderTarget** targets, unsigned int count);

	//////////////////////////////////////////////////////////////////
	// Texture manager
	//
	MTextureManager* getTextureManager() const;

	//////////////////////////////////////////////////////////////////
	// Render override methods
	//
	MStatus registerOverride(const MRenderOverride *roverride);
	MStatus deregisterOverride(const MRenderOverride *roverride);
	const MRenderOverride * findRenderOverride( const MString &name );
	const MString activeRenderOverride() const;
	unsigned int renderOverrideCount();
    MStatus setRenderOverrideName( const MString & name );
    MString renderOverrideName( MStatus *ReturnStatus = NULL ) const;
	MStatus registerScriptCallback(MRenderScriptCallback *callback);
	MStatus deregisterScriptCallback(MRenderScriptCallback *callback);

    //////////////////////////////////////////////////////////////////
    // Viewport 2 Co-ordination methods
    //
    MStatus holdGPUMemory(MInt64 sizeInBytes, MInt64* evictedGPUMemSize = NULL);
    MStatus releaseGPUMemory(MInt64 sizeInBytes);

    void getStandardViewportOperations(MRenderOperationList& operations) const;

    // Output target size methods
	void setOutputTargetOverrideSize(unsigned int w, unsigned int h);
	void getOutputTargetOverrideSize(int & w, int & h);
	void unsetOutputTargetOverrideSize();
    MStatus outputTargetSize(unsigned int& w, unsigned int& h) const;

	static void disableChangeManagementUntilNextRefresh();

	static void setGeometryDrawDirty(MObject obj, bool topologyChanged = true);

	static void needEvaluateAllLights();
	static void setLightsAndShadowsDirty();
	bool setLightRequiresShadows( MObject obj, bool flag );

	// Pre and post render management
	//
	// Notification interface
	//! Definition for callback function for addNotification()
	typedef void (*NotificationCallback)(MDrawContext& context, void* clientData);
	MStatus addNotification(NotificationCallback notification, const MString& name, const MString semanticLocation,
							void* clientData); 
	MStatus removeNotification(const MString& name, const MString& semanticLocation); 
	unsigned int notificationCount(const MString& semanticLocation) const;
	bool presentOnScreen() const;
	void setPresentOnScreen(bool val);

	// Color methods
	bool  useGradient() const;
	MColor clearColor() const;
	MColor clearColor2() const;

protected:
    unsigned int fRasterMap[::MHWRender::kNumberOfRasterFormats];

private:
	static MRenderer *fsTheRenderer;

	//////////////////////////////////////////////////////////////////
	// Pass Information
	//
	void setPId(const MString& val) { fPId = val; };
	void setPSem(const MStringArray& val) { fPSem = val; };
	MString& pId() { return fPId; }
	MStringArray& pSem() { return fPSem; }
	MString fPId;
	MStringArray fPSem;

	MFragmentManager* fFragmentManager;
	MShaderManager* fShaderManager;
	MRenderTargetManager* fRenderTargetManager;
	MTextureManager* fTextureManager;
	bool fInitialized;

	MRenderer();
	~MRenderer();

	void initialize(bool initializeRenderer);

	// Left unimplemented on purpose to prevent methods
	// from being called.
	MRenderer( const MRenderer &other );
	MRenderer& operator=( const MRenderer &rhs );

	friend class MDrawContext;
    friend class MRenderParameters;
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderer */
