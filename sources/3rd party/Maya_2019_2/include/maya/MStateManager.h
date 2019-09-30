#ifndef _MStateManager
#define _MStateManager
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
// CLASS:	MStateManager
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MStateManager)
//
//  MStateManager allows efficient access to GPU state information
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// DECLARATIONS


// ****************************************************************************
// CLASS DECLARATION (MStateManager)
//! \ingroup OpenMayaRender
//! \brief Class to allow efficient access to GPU state information
/*!
MStateManager can be used to provide efficient access to GPU state information
when drawing in MPxShaderOverride or MPxDrawOverride. It cannot be created by
the user, only accessed through MDrawContext.

MStateManager works with GPU state in blocks for efficiency. These blocks are:
blend state, rasterizer state, depth stencil state and sampler state. Each
state block is represented by a different class.

Users may acquire immutable instances of such classes by calling the relevant
acquire methods on the state manager using a descriptor object to indicate what
parameters the state block should have. Once acquired a state block may be held
throughout the session and used to set the state where needed. Acquiring state
blocks is relatively slow and expensive so it is recommended to avoid acquiring
them during draw. State blocks are not large and so holding the memory should
not be overly burdensome.

For more details about the specific state block types, see MBlendState,
MRasterizerState, MDepthStencilState and MSamplerState.
*/
class OPENMAYARENDER_EXPORT MStateManager
{
public:
	//! Indicates how to perform comparisons for buffers.
	enum CompareMode
	{
		kCompareNever = 1,			//!< never True.
		kCompareLess = 2,			//!< True if less.
		kCompareEqual = 3,			//!< True if equal.
		kCompareLessEqual = 4,		//!< True if less or equal.
		kCompareGreater = 5,		//!< True if greater.
		kCompareNotEqual = 6,		//!< True if not equal.
		kCompareGreaterEqual = 7,	//!< True if greater or equal.
		kCompareAlways = 8			//!< Always true.
	};

	//! Indicates which shader the sampler applies to.
	enum ShaderType
	{
		kNoShader,			//!< Shader type is unknown at this point
		kVertexShader,		//!< The vertex shader
		kGeometryShader,	//!< The geometry shader
		kPixelShader,		//!< The pixel or fragment shader
		kHullShader,		//!< The hull or pre-tesselation shader
		kDomainShader		//!< The domain or post-tesselation shader
	};

	static const MBlendState* acquireBlendState( const MBlendStateDesc& blendStateDesc, MStatus* ReturnStatus=NULL );
	static void releaseBlendState(const MBlendState* state);
	MStatus setBlendState( const MBlendState* blendState );
	const MBlendState* getBlendState( MStatus* ReturnStatus=NULL ) const;

	static const MRasterizerState* acquireRasterizerState( const MRasterizerStateDesc& rasterizerStateDesc, MStatus* ReturnStatus=NULL );
	static void releaseRasterizerState(const MRasterizerState* state);
	MStatus setRasterizerState( const MRasterizerState* rasterizerState );
	const MRasterizerState* getRasterizerState( MStatus* ReturnStatus=NULL ) const;

	static const MDepthStencilState* acquireDepthStencilState( const MDepthStencilStateDesc& desc, MStatus* ReturnStatus=NULL );
	static void releaseDepthStencilState(const MDepthStencilState* state);
	MStatus setDepthStencilState( const MDepthStencilState* depthStencilState );
	const MDepthStencilState* getDepthStencilState( MStatus* ReturnStatus=NULL ) const;

	static int getMaxSamplerCount();
	static const MSamplerState* acquireSamplerState( const MSamplerStateDesc& samplerStateDesc, MStatus* ReturnStatus=NULL );
	static void releaseSamplerState(const MSamplerState* state);
	MStatus	setSamplerState( ShaderType shader, int samplerIndex, const MSamplerState* samplerState );
	const MSamplerState* getSamplerState( ShaderType shader, int samplerIndex, MStatus* ReturnStatus=NULL ) const;

	static const char* className();

private:
	MStateManager(void* data);
	~MStateManager();

	static void samplerDescriptionFromState( void *state, MSamplerStateDesc & samplerDesc );

	void setData(void* data);
	void* fData;

	friend class MDrawContext;
	friend class MLightParameterInformation;
    friend class MRenderParameters;
};




// ****************************************************************************
// CLASS DECLARATION (MBlendState)
//! \ingroup OpenMayaRender
//! \brief Container class for an acquired GPU blend state.
/*!
MBlendState wraps an acquired composite GPU blend state. This blend state can
be used to set the current GPU blend state using
MStateManager::setBlendState(myBlendState).

Instances of MBlendState cannot be created or modified by users. You can read
the descriptor for an MBlendState using MBlendState::desc(), but you cannot set
or alter the descriptor directly.

MBlendState can only be obtained via MStateManager::acquireBlendState(). To use
MStateManager::acquireBlendState(), create and fill an MBlendStateDesc
instance with the requested state and pass it to
MStateManager::acquireBlendState() which will return the cached unique blend
state of that description. If the unique blend state did not previously exist
in the cache, it is created.

The information included in the blend state includes alpha to coverage enable,
independent blend enable, a global float4 blend factor, and an array of
blending descriptors, one for each target.

The advantages of using this class rather than setting blend states explicitly
are:

1. Since the states are cached internally, state setting is much faster than
setting individual states.
2. Since state is set through the Maya viewport API, Maya can maintain accurate
knowledge of the current GPU state, even in the presence of arbitrary plug-ins.
This also speeds state setup by reducing redundant state setting and OGL
queries.
3. The information is device aware, meaning that it will return the correct
results based on the current active device. For example it will return the
appropriate values for DirectX versus an OpenGL device.
*/
class OPENMAYARENDER_EXPORT MBlendState
{
public:
	//! Blend operations for hardware target blending.
	enum BlendOperation {
		kAdd = 0x1,				//!< Add the source and destination together.
		kSubtract = 0x2,		//!< Destination minus source.
		kReverseSubtract = 0x3,	//!< Source minus destination.
		kMin = 0x4,				//!< The minimum of the source and destination.
		kMax = 0x5				//!< The maximum of the source and destination.
	};

	//! Specifies what value to use as blend factor with an optional pre-operation.
	enum BlendOption
	{
		kZero = 1, 					//!< Blend factor is 0.
		kOne = 2, 					//!< Blend factor is 1.
		kSourceColor = 3, 			//!< Blend factor is source color, src.rgb.
		kInvSourceColor = 4, 		//!< Blend factor is the complement of the source color, (1 - src.rgb).
		kSourceAlpha = 5, 			//!< Blend factor is source alpha, src.a.
		kInvSourceAlpha = 6, 		//!< Blend factor is the complement of the source alpha, (1 - src.a).
		kSourceAlphaSat = 11,		//!< Blend factor is (f, f, f, 1); where f = saturate(src.a) between 0 and 1.
		kDestinationColor = 9,		//!< Blend factor is destination color, dst.rgb.
		kInvDestinationColor = 10,	//!< Blend factor is the complement of the destination color, (1 - dst.rgb).
		kDestinationAlpha = 7,		//!< Blend factor is destination alpha, dst.a.
		kInvDestinationAlpha = 8,	//!< Blend factor is the complement of the destination alpha, (1 - dst.a).
		kBothSourceAlpha = 12,		//!< Source blend factor is src.a, destination blend factor is (1- src.a).
		kBothInvSourceAlpha = 13,	//!< Source blend factor is (1- src.a), destination blend factor is src.a.
		kBlendFactor = 14, 			//!< Blend factor is the global blend factor specified by users.
		kInvBlendFactor = 15 		//!< Blend factor is the complement of the global blend factor, (1 - blendFactor.rgba).
	};

	//! A bitfield that indicates which color components(red, green, blue, alpha) are writable.
	enum ChannelMask
	{
		kNoChannels   = 0,		//!< \nop
		kRedChannel   = 0x1,	//!< \nop
		kGreenChannel = 0x2,	//!< \nop
		kBlueChannel  = 0x4,	//!< \nop
		kAlphaChannel = 0x8,	//!< \nop
		kRGBChannels  = 0x7,	//!< \nop
		kRGBAChannels = 0xf 	//!< \nop
	};

	//! Maximum number of simulatanious render targets.
	static const int kMaxTargets = 8;

	const MBlendStateDesc& desc( MStatus* ReturnStatus=NULL ) const;

	static const char* className();

	void * resourceHandle() const;

private:
	MBlendState( void *GPUBlendState );
	MBlendState();
	~MBlendState();

	void *fData;

	friend class MStateManager;
	static MBlendStateDesc *fErrorDesc;
};

// ****************************************************************************
// CLASS DECLARATION (MTargetBlendDesc)
//! \ingroup OpenMayaRender
//! \brief Descriptor for a blend state for a single render target
/*!
This class holds the complete blend state for a single render target. The
blended result will be: result = !BlendEnable ? source :
blendOperation(sourceBlendOption(source), destinationBlendOption(destination));

MBlendStateDesc maintains an array of MTargetBlendDesc of size
MBlendState::kMaxTargets.
*/
class OPENMAYARENDER_EXPORT MTargetBlendDesc
{
public :
	bool						blendEnable;			//!< Enable blending on this target, default is false

	// Color blending controls
	MBlendState::BlendOption	sourceBlend;			//!< The blend factor for the source color, default is one
	MBlendState::BlendOption	destinationBlend;		//!< The blend factor for the destination color, default is zero
	MBlendState::BlendOperation	blendOperation;			//!< The blend operation, default is add

	// Alpha blending controls
	MBlendState::BlendOption	alphaSourceBlend;		//!< The blend factor for the source alpha, default is one
	MBlendState::BlendOption	alphaDestinationBlend;	//!< The blend factor for the destination alpha, default is zero
	MBlendState::BlendOperation	alphaBlendOperation;	//!< The blend operation for alpha, default is add

	MBlendState::ChannelMask	targetWriteMask;		//!< Indicates what color components(red, green, blue, alpha) are writable, the default is RGBA.

	MTargetBlendDesc();
	~MTargetBlendDesc();

	void setDefaults();

BEGIN_NO_SCRIPT_SUPPORT:
	//! NO SCRIPT SUPPORT
	MTargetBlendDesc(const MTargetBlendDesc& rDesc);
	//! NO SCRIPT SUPPORT
	MTargetBlendDesc& operator=(const MTargetBlendDesc& rDesc);
END_NO_SCRIPT_SUPPORT:

	static const char* className();
};

// ****************************************************************************
// CLASS DECLARATION (MBlendStateDesc)
//! \ingroup OpenMayaRender
//! \brief Descriptor for a complete blend state
/*!
This class describes a complete blending state, including the blending state
for all targets.
*/
class OPENMAYARENDER_EXPORT MBlendStateDesc
{
public:
	bool				alphaToCoverageEnable;		//!< Enable alpha to coverage. Default false.
	bool				independentBlendEnable;		//!< Use TargetBlends[0] if false or individual blend states if true. Default false.
	float				blendFactor[4];				//!< Global blend factor, default (1,1,1,1).
	unsigned int		multiSampleMask;			/*!< Each bit in this mask, starting at the least significant bit (LSB),
														 controls modification of one of the samples in a multisample render
														 target. Thus, for an 8-sample render target, the low byte contains the eight
														 write enables for each of the eight samples. The actual location of the
														 samples is determined by the GPU sample pattern, which is not in general known
														 or controlled directly by applications. Default 0xffffffff.
														*/
	MTargetBlendDesc	targetBlends[ MBlendState::kMaxTargets ]; //!< An array of 8 MTargetBlendDesc, one for each target.

	MBlendStateDesc();
	~MBlendStateDesc();

	void setDefaults();

BEGIN_NO_SCRIPT_SUPPORT:
	//! NO SCRIPT SUPPORT
	MBlendStateDesc(const MBlendStateDesc& rDesc);
	//! NO SCRIPT SUPPORT
	MBlendStateDesc& operator=(const MBlendStateDesc& rDesc);
END_NO_SCRIPT_SUPPORT:

	static const char* className();
};




// ****************************************************************************
// CLASS DECLARATION (MRasterizerState)
//! \ingroup OpenMayaRender
//! \brief Container class for an acquired complete GPU rasterizer state.
/*!
MRasterizerState wraps an acquired composite GPU rasterizer state. This
rasterizer state can be used to set the current GPU rasterizer state using
MStateManager::setRaserizerState(myRaserizerState).

Instances of MRasterizerState cannot be created or modified by users. You can
read the descriptor for an MRasterizerState using MRasterizerState::desc(),
but you cannot set or alter the descriptor directly.

MRasterizerState can only be obtained via
MStateManager::acquireRaserizerState(). To use
MStateManager::acquireRaserizerState(), create and fill an MRasterizerStateDesc
instance with the requested state and pass it to
MStateManager::acquireRaserizerState() which will return the cached unique
rasterizer state of that description. If the unique rasterizer state did not
previously exist in the cache, it is created.

The information included in the rasterizer state includes fill mode, culling
mode, anti-aliasing enables, depth bias information, depth clipping enable,
scissor clip enable, and whether CW or CCW winding is "front".

The advantages of using this class rather than setting rasterizer states
explicitly are:

1. Since the states are cached internally, state setting is much faster than
setting individual states.
2. Since state is set through the Maya viewport API, Maya can maintain accurate
knowledge of the current GPU state, even in the presence of arbitrary plug-ins.
This also speeds state setup by reducing redundant state setting and OGL
queries.
3. The information is device aware, meaning that it will return the correct
results based on the current active device. For example it will return the
appropriate values for DirectX versus an OpenGL device.
*/
class OPENMAYARENDER_EXPORT MRasterizerState
{
public:
	//! Indicates how to fill various primitives.
	enum FillMode
	{
		kFillSolid = 3,		//!< Primitives are solid filled.
		kFillWireFrame = 2	//!< Primitives have only the wireframe edges drawn.
	};

	//! Indicates how to cull primitive faces during drawing.
	enum CullMode
	{
		kCullNone = 1,		//!< Disables frontface/backface culling.
		kCullFront = 2,		//!< Culls front facing facets.
		kCullBack = 3		//!< Culls back facing facets.
	};

	const MRasterizerStateDesc& desc( MStatus* ReturnStatus=NULL ) const;

	static const char* className();

	void * resourceHandle() const;

private:
	MRasterizerState( void *GPURasterizerState );
	MRasterizerState();
	~MRasterizerState();

	void *fData;

	friend class MStateManager;

	static MRasterizerStateDesc* fErrorDesc;
};

// ****************************************************************************
// CLASS DECLARATION (MRasterizerStateDesc)
//! \ingroup OpenMayaRender
//! \brief Descriptor for a complete rasterizer state
/*!
This class describes a complete rasterizer state, like fill mode, cull mode,
depth bias, multisampling, point size, etc.
*/
class OPENMAYARENDER_EXPORT MRasterizerStateDesc
{
public:
	MRasterizerState::FillMode	fillMode;	//!< Select the primitive fill mode, default kFillSolid.
	MRasterizerState::CullMode	cullMode;	//!< Select the face culling mode, default kCullNone.
	bool		frontCounterClockwise;		//!< Select whether CW or CCW winding is used for "front" face, default false.
	bool		depthBiasIsFloat;			//!< Indicates that DepthBias is a float value, default false.
	float 		depthBias;					//!< DepthBias adds the given bias value to the rasterizer z value prior to depth testing.
	float		depthBiasClamp;				//!< Maximum value scaled depth bias can attain, default 0.
	float		slopeScaledDepthBias;		//!< Slope scaled depth bias value, default 0.
	bool		depthClipEnable;			//!< Enables HW automatic depth clipping, default true.
	bool		scissorEnable;				//!< Enables HW scissor clip rectangle, default false.
	bool		multiSampleEnable;			//!< Enables HW full screen multi-sample anti-aliasing, default false.
	bool		antialiasedLineEnable;  	//!< Enables HW anti-aliased lines, auto disabled by multi-sample AA, default false.

	MRasterizerStateDesc();
	~MRasterizerStateDesc();

	void setDefaults();

BEGIN_NO_SCRIPT_SUPPORT:
	//! NO SCRIPT SUPPORT
	MRasterizerStateDesc(const MRasterizerStateDesc& rDesc);
	//! NO SCRIPT SUPPORT
	MRasterizerStateDesc& operator =(const MRasterizerStateDesc& rDesc);
END_NO_SCRIPT_SUPPORT:

	static const char* className();
};




// ****************************************************************************
// CLASS DECLARATION (MDepthStencilState)
//! \ingroup OpenMayaRender
//! \brief Container class for an acquired complete GPU depth stencil state.
/*!
MDepthStencilState wraps an acquired composite GPU depth stencil state. This
depth stencil state can be used to set the current GPU depth stencil state
using MStateManager::setDepthStencilState(myDepthState).

Instances of MDepthStencilState cannot be created or modified by users. You can
read the descriptor for an MDepthStencilState using MDepthStencilState::desc(),
but you cannot set or alter the descriptor directly.

MDepthStencilState can only be obtained via
MStateManager::acquireDepthStencilState(). To use
MStateManager::acquireDepthStencilState(), create and fill an
MDepthStencilStateDesc instance with the requested state and pass it to
MStateManager::acquireDepthStencilState() which will return the cached unique
depth stencil state of that description. If the unique depth stencil state did
not previously exist in the cache, it is created.

The information included in the depth stencil state includes depth buffer
enable, depth buffer write enable, stencil enable, and separate stencil
operations for front faces and back faces.

The advantages of using this class rather than setting depth stencil states
explicitly are:

1. Since the states are cached internally, state setting is much faster than
setting individual states.
2. Since state is set through the Maya viewport API, Maya can maintain accurate
knowledge of the current GPU state, even in the presence of arbitrary plug-ins.
This also speeds state setup by reducing redundant state setting and OGL
queries.
3. The information is device aware, meaning that it will return the correct
results based on the current active device. For example it will return the
appropriate values for DirectX versus an OpenGL device.
*/
class OPENMAYARENDER_EXPORT MDepthStencilState
{
public:
	//! Describes the choices for stencil buffer operations.
	enum StencilOperation
	{
		kKeepStencil = 1,			//!< Do not update the entry in the stencil buffer. This is the default value
		kZeroStencil = 2,			//!< Set the stencil-buffer entry to 0.
		kReplaceStencil = 3,		//!< Replace the stencil-buffer entry with a reference value.
		kIncrementStencilSat = 4,	//!< Increment the stencil buffer entry, clamping to the maximum value.
		kDecrementStencilSat = 5,	//!< Decrement the stencil buffer entry, clamping to zero.
		kInvertStencil = 6,			//!< Invert the bits in the stencil-buffer entry.
		kIncrementStencil = 7,		//!< Increment the stencil buffer entry, wrapping to zero if the new value exceeds the maximum value..
		kDecrementStencil = 8		//!< Decrement the stencil buffer entry, wrapping to the maximum value if the new value is less than zero
	};

	const MDepthStencilStateDesc& desc( MStatus* ReturnStatus=NULL ) const;

	static const char* className();

	void * resourceHandle() const;

private:
	MDepthStencilState( void *GPUDepthStencilState );
	MDepthStencilState();
	~MDepthStencilState();

	void *fData;

	friend class MStateManager;

	static MDepthStencilStateDesc*  fErrorDesc;
};

// ****************************************************************************
// CLASS DECLARATION (MStencilOpDesc)
//! \ingroup OpenMayaRender
//! \brief Descriptor for a depth-stencil operation
/*!
This class encapsulates a depth-stencil operation descriptor. There are
separate MStencilOpDesc stencil operation descriptors for front and back faces
in the depth stencil state.
*/
class OPENMAYARENDER_EXPORT MStencilOpDesc
{
public:
	MDepthStencilState::StencilOperation	stencilPassOp;		//!< Stencil op to use when the fragment passes the stencil test, default kKeepStencil.
	MDepthStencilState::StencilOperation	stencilFailOp;		//!< Stencil op to use when the fragment fails the stencil test, default kKeepStencil.
	MDepthStencilState::StencilOperation	stencilDepthFailOp;	//!< Stencil op to use when the fragment passes the depth test, default kKeepStencil.
	MStateManager::CompareMode				stencilFunc;		//!< Sets the stencil buffer comparison function, default kCompareAlways.

	MStencilOpDesc();
	~MStencilOpDesc();

	void setDefaults();

BEGIN_NO_SCRIPT_SUPPORT:
	//! NO SCRIPT SUPPORT
	MStencilOpDesc(const MStencilOpDesc& rDesc);
	//! NO SCRIPT SUPPORT
	MStencilOpDesc& operator=(const MStencilOpDesc& rDesc);
END_NO_SCRIPT_SUPPORT:

	static const char* className();
};

// ****************************************************************************
// CLASS DECLARATION (MStencilOpDesc)
//! \ingroup OpenMayaRender
//! \brief Descriptor for a complete depth-stencil state
/*!
This class encapsulates a complete depth-stencil state, like z buffer enable,
z write enable, z comparison function, stencil enable, stencil masks and
reference values. It also contains MStencilOpDesc stencil operation descriptors
for front and back faces.
*/
class OPENMAYARENDER_EXPORT MDepthStencilStateDesc
{
public:
	bool			depthEnable;		//!< Enables depth buffer reads and compares, default true.
	bool			depthWriteEnable;	//!< Enables depth buffer writes, default true.
	MStateManager::CompareMode depthFunc; //!< Sets the depth buffer comparison function, default less than.
	bool			stencilEnable;		//!< Enables stencil buffer operation.
	unsigned char	stencilReadMask;	//!< Sets a bitwise stencil buffer read mask, default 0xff.
	unsigned char	stencilWriteMask;	//!< Sets a bitwise stencil buffer write mask, default 0xff.
	int				stencilReferenceVal;//!< Sets the stencil reference value.
	MStencilOpDesc frontFace;	//!< Sets the stencil op for the front facing fragments.
	MStencilOpDesc backFace;	//!< Sets the stencil op for the back facing fragments.

	MDepthStencilStateDesc();
	~MDepthStencilStateDesc();

	void setDefaults();

BEGIN_NO_SCRIPT_SUPPORT:
	//! NO SCRIPT SUPPORT
	MDepthStencilStateDesc(const MDepthStencilStateDesc& rDesc);
	//! NO SCRIPT SUPPORT
	MDepthStencilStateDesc& operator =(const MDepthStencilStateDesc& rDesc);
END_NO_SCRIPT_SUPPORT:

	static const char* className();
};




// ****************************************************************************
// CLASS DECLARATION (MSamplerState)
//! \ingroup OpenMayaRender
//! \brief Container class for an acquired complete GPU sampler state.
/*!
MSamplerState wraps an acquired composite GPU sampler state. This sampler state
can be used to set the current GPU sampler state using
MStateManager::setSamplerState(myShader, 0, mySamplerState).

Instances of MSamplerState cannot be created or modified by users. You can read
the descriptor for an MSamplerState using MSamplerState::desc(), but you cannot
set or alter the descriptor directly.

MSamplerState can only be obtained via MStateManager::acquireSamplerState().
To use MStateManager::acquireSamplerState(), create and fill an
MSamplerStateDesc instance with the requested state and pass it to
MStateManager::acquireSamplerState() which will return the cached unique
sampler state of that description. If the unique sampler state did not
previously exist in the cache, it is created.

The information included in the sampler state includes texture filter
selection, texture address mode selection, mip LOD controls,
anisotropy control, and the border color.

Any single given sampler state can be bound to as many GPU sampler stages as
needed.

The advantages of using this class rather than setting sampler states
explicitly are:

1. Since the states are cached internally, state setting is much faster than
setting individual states.
2. Since state is set through the Maya viewport API, Maya can maintain accurate
knowledge of the current GPU state, even in the presence of arbitrary plug-ins.
This also speeds state setup by reducing redundant state setting and OGL
queries.
3. The information is device aware, meaning that it will return the correct
results based on the current active device. For example it will return the
appropriate values for DirectX versus an OpenGL device.
*/
class OPENMAYARENDER_EXPORT MSamplerState
{
public:
	//! Indicates how to filter a texture sample.
	enum TextureFilter
	{
		kMinMagMipPoint = 0,					//!< Use point sampling for minification, magnification, and mip-level sampling.
		kMinMagPoint_MipLinear = 1,				//!< Use point sampling for minification and magnification; use linear interpolation for mip-level sampling.
		kMinPoint_MagLinear_MipPoint = 0x4,		//!< Use point sampling for minification; use linear interpolation for magnification; use point sampling for mip-level sampling.
		kMinPoint_MagMipLinear = 0x5,			//!< Use point sampling for minification; use linear interpolation for magnification and mip-level sampling.
		kMinLinear_MagMipPoint = 0x10,			//!< Use linear interpolation for minification; use point sampling for magnification and mip-level sampling.
		kMinLinear_MagPoint_MipLinear = 0x11,	//!< Use linear interpolation for minification; use point sampling for magnification; use linear interpolation for mip-level sampling.
		kMinMagLinear_MipPoint = 0x14,			//!< Use linear interpolation for minification and magnification; use point sampling for mip-level sampling.
		kMinMagMipLinear = 0x15,				//!< Use linear interpolation for minification, magnification, and mip-level sampling.
		kAnisotropic = 0x55						//!< Use anisotropic filtering for minification, magnification, and mip-level sampling
	};

	//! Indicates how to handle texture addresses beyond the canonical (0..1, 0..1) range.
	enum TextureAddress
	{
		kTexWrap   = 1,	//!< Wrap addresses beyond the [0,1] tile back into the [0.1] range, repeating the texture over the surface.
		kTexMirror = 2,	//!< Similar to wrap, but with odd tiles mirrored in u and v.
		kTexClamp  = 3, //!< Clamp address in the [0, 1] range. Extends edge pixels beyond range.
		kTexBorder = 4  //!< Clamp samples out of [0, 1] range to border color.
	};

	const MSamplerStateDesc& desc( MStatus* ReturnStatus=NULL ) const;

	static const char* className();

	void * resourceHandle() const;

private:
	MSamplerState( void *GPUSamplerState );
	MSamplerState();
	~MSamplerState();

	void *fData;

	friend class MStateManager;
	friend class MShaderInstance;
	static MSamplerStateDesc*  fErrorDesc;
    friend class MRenderParameters;
};

// ****************************************************************************
// CLASS DECLARATION (MStencilOpDesc)
//! \ingroup OpenMayaRender
//! \brief Descriptor for a complete sampler state
/*!
This class encapsulates a complete Sampler state.
*/
class OPENMAYARENDER_EXPORT MSamplerStateDesc
{
public:
	MSamplerState::TextureFilter  filter;		//!< Selects how to filter a texture sample, default kMinMagMipPoint.
	MStateManager::CompareMode    comparisonFn;	//!< Selects the filter comparison function, default kCompareAlways.
	MSamplerState::TextureAddress addressU;		//!< Select the u coordinate addressing mode, default kTexWrap.
	MSamplerState::TextureAddress addressV;		//!< Select the v coordinate addressing mode, default kTexWrap.
	MSamplerState::TextureAddress addressW;		//!< Select the w coordinate addressing mode, default kTexWrap.
	float			borderColor[4];	//!< Set border color used for accesses beyond texture 0..1, default(0,0,0,0).
	float			mipLODBias;		//!< Set a float bias to be added to the computed mip LOD level, default 0.
	unsigned int	minLOD;			//!< Set minimum mip LOD level accessed, default 0.
	unsigned int	maxLOD;			//!< Set maximum mip LOD level accessed, default 16.
	unsigned int	maxAnisotropy;	//!< Set the maximum anisotropy permitted for anisotropic filters. The range is 1..16, default 1.
	unsigned int	coordCount;		//!< Set the number of texture coordinates, default 2.
	unsigned int	elementIndex;	//!< When using texture arrays, selects array element, default 0.

	MSamplerStateDesc();
	~MSamplerStateDesc();

	void setDefaults();

BEGIN_NO_SCRIPT_SUPPORT:
	//! NO SCRIPT SUPPORT
	MSamplerStateDesc(const MSamplerStateDesc& rDesc);
	//! NO SCRIPT SUPPORT
	MSamplerStateDesc& operator=(const MSamplerStateDesc& rDesc);
END_NO_SCRIPT_SUPPORT:

	static const char* className();
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MStateManager */
