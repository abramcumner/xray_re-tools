#ifndef _MTextureManager
#define _MTextureManager
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
// CLASS:    MHWRender::MTextureManager
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MHWRender::MTextureManager)
//
//  MHWRender::MTextureManager is the main interface class to get texture
//	which is used for rendering interactive viewports in "Viewport 2.0" mode
//	as well as for rendering with the "Maya Hardware 2.0" batch renderer.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MViewport2Renderer.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MPlug.h>
#include <maya/MFloatArray.h>


namespace MHWRender
{
// ****************************************************************************
// DECLARATIONS

//! Texture types
enum MTextureType {
	kImage1D = 0,	//!< 1D image
	kImage1DArray,	//!< Array of 1D images
	kImage2D,		//!< 2D image
	kImage2DArray,	//!< Array of 2D images
	kCubeMap,		//!< Cube map
	kVolumeTexture,	//!< Volume texture
	kDepthTexture,	//!< Depth texture

	kNumberOfTextureTypes //! Not to be used to describe a raster. This is the number of texture types.
};

/*! Environment mapping types for textures. Describes the algorithm for
how the data in the texture should be looked up.
*/
enum MEnvironmentMapType {
	kEnvNone = 0,	//!< Direct mapping
	kEnvSphere,		//!< Sphere style environment mapping
	kEnvHemiSphere,	//!< Hemi-sphere style environment mapping
	kEnvLatLong,	//!< Latitude/longitude style environment map
	kEnvCrossVert,	//!< Vertical cross style environment map
	kEnvCrossHoriz,	//!< Horizontal cross style environment map
	kEnvCubemap,	//!< 6 faces cube map style environment map

	kNumberOfEnvMapTypes //! Not to be used to describe a raster. This is the number of environment map types.
};

} // namespace MHWRender

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

/*! Texture description. Provides sufficient information to describe how
	a block of data can be interpreted as a texture.
*/
class MTextureDescription
{
public:
	//! Utility to set texture description to describe a 0 size 2-dimensional texture
	void setToDefault2DTexture()
	{
		fWidth = 0;
		fHeight = 0;
		fDepth = 1;				// depth of 1
		fBytesPerRow = 0;
		fBytesPerSlice = 0;
		fMipmaps = 1;			// No mip-maps
		fArraySlices = 1;		// 1 slice
		fFormat = ::MHWRender::kR8G8B8A8_UNORM; // "standard" fixed-point RGBA8
		fTextureType =::MHWRender::kImage2D; // 2D texture
		fEnvMapType = ::MHWRender::kEnvNone; // No special lookup
	}

	unsigned int fWidth;		//!< Width in pixels
	unsigned int fHeight;		//!< Height in pixels
	unsigned int fDepth;		//!< Depth in pixels. A 2D texture has depth of 1.
	unsigned int fBytesPerRow;	//!< Number of bytes in a row of pixels
	unsigned int fBytesPerSlice;//!< Number of bytes in a slice (if an array)
	unsigned int fMipmaps;		//!< Number of mipmap levels. 0 means the entire mipmap chain.
	unsigned int fArraySlices;	//!< Number of array slices. e.g. 6 would be required for a cube-map
	::MHWRender::MRasterFormat fFormat;		//!< Pixel / raster format
	::MHWRender::MTextureType	fTextureType;  //!< Type of texture
	::MHWRender::MEnvironmentMapType fEnvMapType;	//!< Type of environment mapping
};

/*! Information required to perform normalization of values
	stored in the depth buffer of an MImage with respect to clipping
	plane range.

	The near and far clip plane values should not be less than
	the minimum clip plane value for a Maya camera. Any values which do
	not pass this condition will be set to the camera minimum
	before any normalization is performed.
*/
class MDepthNormalizationDescription
{
public:
	//! Create with some default values
	MDepthNormalizationDescription()
	: fNearClipDistance(0.1f)
	, fFarClipDistance(10000.0f)
	, fDepthScale(1.0f)
	, fDepthBias(0.0f)
	{
	}
	float fNearClipDistance; //!< Near clip plane of a camera.
	float fFarClipDistance;  //!< Far clip plane of a camera
	float fDepthScale;		//!< Scale to apply to depth values
	float fDepthBias;		//!< Bias to apply to depth value
};

/*!	Structure to represent an update region for a texture

	A 2D texture slice is is defined in X and Y.
	Z defines slices within a 3D texture
*/
class MTextureUpdateRegion
{
public:
	//! Create with default region size of 1 pixel
	MTextureUpdateRegion()
	{
		fXRangeMin = fYRangeMin = fZRangeMin = 0;
		fXRangeMax = fYRangeMax = fZRangeMax = 1;
	}
	unsigned int fXRangeMin;
	unsigned int fXRangeMax;
	unsigned int fYRangeMin;
	unsigned int fYRangeMax;
	unsigned int fZRangeMin;
	unsigned int fZRangeMax;
};

// ****************************************************************************
// CLASS DECLARATION (MTexture)
//! \ingroup OpenMayaRender
//!\brief Class which includes texture data
//
class OPENMAYARENDER_EXPORT MTexture
{
public:
	// Access to draw API dependent texture data
	void* resourceHandle() const;
	const MString& name() const;
	void textureDescription(MTextureDescription& desc) const;

	bool hasAlpha() const;
	MStatus setHasAlpha(bool value);
	bool hasZeroAlpha() const;
	MStatus setHasZeroAlpha(bool value);
	bool hasTransparentAlpha() const;
	MStatus setHasTransparentAlpha(bool value);

	// Access methods
	unsigned int bytesPerPixel() const;

	void * rawData(int &rowPitch, size_t &slicePitch) const;
	MStatus	update(const void* pixelData, bool generateMipMaps, unsigned int rowPitch=0, MHWRender::MTextureUpdateRegion *region=NULL);
	MStatus update(const MImage &image, bool generateMipMaps);
	MStatus update(const MObject& textureNode);

	static const char* className();
	static void freeRawData(void *);

private:
	MTexture(const MString& name, void* data);
	~MTexture();
	MString fName;
	void* fData;

	friend class MRenderer;
	friend class MTextureManager;
	friend class MShaderInstance;
	friend class MLightParameterInformation;
	friend class MDrawContext;
	friend class MUIDrawManager;
    friend class MRenderParameters;
};

// ****************************************************************************
// CLASS DECLARATION (MTextureArguments)
//! \ingroup OpenMayaRender
//!\brief Class which includes texture arguments
/*! This class encapsulates the information needed by the MTextureManager to acquire a texture from a file,
	such as the file path and mipmap levels. Encapsulating the texture information in an object allows you
	to store the information for easy re-use, and can simplify your texture loading code.

	MTextureArgments also accepts a file texture node, which is required if you want to activate background
	loading for the texture. You can set the file texture node through the MTextureArguments() constructor, or
	by calling MTextureArguments::setFileTextureNode().
	
	To use this class, call the constructor to create an instance and set it up with the necessary parameters.
	When you need to get the texture, pass the object in a call to
	MTextureManager::acquireTexture(const MTextureArgments& args).
*/
class OPENMAYARENDER_EXPORT MTextureArguments
{
public:
	MTextureArguments(const MString& filePath,
		int mipmapLevels = 0,
		bool useExposureControl = true,
		const MString& layerName = MString(),
		int alphaChannelIdx = -1);

	void setContextNodeFullName(const MString& contextNodeFullName);

	void setFileTextureNode(MObject node);
	
	~MTextureArguments() {}

private:
	MString		fFilePath;
	int			fMipmapLevels;
	bool		fUseExposureControl;
	MString		fLayerName;
	int			fAlphaChannelIndex;
	MString		fContextNodeFullName;
	MObject		fNode;

	friend class MRenderer;
	friend class MTextureManager;
};

// ****************************************************************************
// CLASS DECLARATION (MTextureManager)
//! \ingroup OpenMayaRender
//! \brief Class which manages texture
//
class OPENMAYARENDER_EXPORT MTextureManager
{
public:
	//////////////////////////////////////////////////////////////////
	// Image paths
	MStatus addImagePath( const MString & path );
	MStatus imagePaths(MStringArray & paths) const;

	// Texture management
	//! obsolete, use the one with contextNodeFullName param
	OPENMAYA_DEPRECATED(2019, "Use the one with the contextNodeFullName input parameter instead.")
	MTexture* acquireTexture(const MString& filePath, int mipmapLevels = 0, bool useExposureControl = true, const MString& layerName = MString(), int alphaChannelIdx = -1); // From disk
	MTexture* acquireTexture(const MTextureArguments& args); // From disk
	MTexture* acquireTexture(const MString& filePath, const MString& contextNodeFullName, int mipmapLevels = 0, bool useExposureControl = true, const MString& layerName = MString(), int alphaChannelIdx = -1); // From disk with context Node
	MTexture* acquireTexture(const MString& textureName, const MPlug& plug, int width, int height, bool generateMipMaps = true); // From plug
	MTexture* acquireTexture(const MObject& textureNode, bool allowBackgroundLoad = false); // From file texture node
	MTexture* acquireTexture(const MString& textureName, const MHWRender::MTextureDescription& textureDesc, const void* pixelData, bool generateMipMaps = true); // From memory
	MTexture* acquireTiledTexture(const MString textureName,
									const MStringArray& tilePaths,
									const MFloatArray& tilePositions,
									const MColor & undefinedColor,
									unsigned int width, unsigned int height,
									MStringArray & failedTilePaths,
									MFloatArray & uvScaleOffset);

	MTexture* acquireDepthTexture(const MString &textureName, const MImage &image,
										bool generateMipMaps,
										const MDepthNormalizationDescription *normalizationDesc);
	MTexture* acquireDepthTexture(const MString &textureName,
										float *pixelData,
										unsigned int width, unsigned int height,
										bool generateMipMaps,
										const MDepthNormalizationDescription *normalizationDesc);

	void releaseTexture(MTexture* texture) const;

	MStatus saveTexture(MTexture* texture, const MString& filePath); // To disk

	MTexture* findTexture(const MString& textureName);

	static const char* className();

private:
	unsigned int fTextureTypeMap[::MHWRender::kNumberOfTextureTypes];
	unsigned int fEnvMapTypeMap[::MHWRender::kNumberOfEnvMapTypes];

	unsigned int* fRasterMap;

	MTextureManager(unsigned int* rasterMap);
	~MTextureManager();

	friend class MTexture;
	friend class MRenderer;
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MTextureManager */
