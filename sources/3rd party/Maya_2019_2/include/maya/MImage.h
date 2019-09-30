#ifndef _MImage
#define _MImage
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
// CLASS:    MImage
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MImage)
//
// MImage is a class which provides access to some of Maya's image manipulation
// functionality.
//
// MImage provides methods for loading and resizing image files in any
// Maya-supported format, including but not limited to IFF, SGI, Softimage,
// TIFF, Alias PIX, GIF, RLA, JPEG, EPS.
//
// MImage stores the image as an uncompressed array of pixels, that can be read
// and manipulate directly. That format is either RGBA (depth=4), RGB (depth=3),
// or Luminance (depth=1).
//
// (NOTE: At the moment, all files are directly converted to RGBA regardless
// of their original format. This restriction might get lifted in the future if
// there is enough interest.)
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MString.h>
#include <maya/MTypes.h>

#define kIffStringDefault MString("iff")


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MImage)

//! \ingroup OpenMaya
//! \brief Image manipulation 
/*!
  This class provides methods for reading file images stored on disk.

  MImage is a class which provides access to some of Maya's image
  manipulation functionality. It has methods for loading and resizing
  image files in any Maya-supported raster format, including IFF, SGI,
  Softimage (pic), TIFF (tif), Alias PIX (als), GIF, RLA, JPEG (jpg).
  The image is stored as an uncompressed array of pixels, that can be
  read and manipulated directly. For simplicity, the pixels are stored
  in a RGBA format (4 bytes per pixel).
*/
class OPENMAYA_EXPORT MImage
{
public:
	//! Filter formats.
	enum MImageFilterFormat {
		kNoFormat,		//!< \nop
		kHeightFieldBumpFormat,	//!< \nop
		kNormalMapBumpFormat,	//!< \nop
		kUnknownFormat		//!< \nop
	};

	//! Pixel formats.
	enum MPixelType {
		kUnknown,	//!< Format not known or invalid
		kByte,		//!< One byte per channel, ranging from 0 to 255
		kFloat		//!< One float per channel, ranging from 0.0 to 1.0
	};

	MImage();
	~MImage();
	MStatus create(unsigned int width, unsigned int height, unsigned int channels = 4, MPixelType type = kByte);
	MStatus readFromFile(MString pathname, MPixelType type = kByte);
	MStatus readFromTextureNode(const MObject& fileTextureObject, MPixelType type = kByte);
	MStatus getSize(unsigned int &width, unsigned int &height) const;
	MPixelType pixelType() const;
	unsigned char* pixels() const;
	float* floatPixels() const;
	void setPixels(unsigned char* pixels, unsigned int width, unsigned int height);
	void setFloatPixels(float* pixels, unsigned int width, unsigned int height, unsigned int channels = 4);
	unsigned int depth() const;
	MStatus getDepthMapSize( unsigned int &width, unsigned int &height ) const;
	MStatus getDepthMapRange( float &minValue, float &maxValue ) const;
	MStatus setDepthMap(float *depth, unsigned width, unsigned height );
	MStatus setDepthMap(const MFloatArray & depth, unsigned width, unsigned height );
	float *depthMap( MStatus *ReturnStatus = NULL ) const;
	MStatus readDepthMap(MString pathname);
	MStatus resize(int width, int height, bool preserveAspectRatio = true);
	static bool filterExists(MImageFilterFormat sourceFormat, MImageFilterFormat targetFormat);
	MStatus filter(MImageFilterFormat sourceFormat, MImageFilterFormat targetFormat,
		double scale = 1.0, double offset = 0.0);

	MStatus writeToFile(MString pathname, MString outputFormat = kIffStringDefault ) const;
	MStatus writeToFileWithDepth(MString pathname, MString outputFormat = kIffStringDefault, bool writeDepth = false ) const;

	MStatus release();
	void verticalFlip();
	void setRGBA(bool rgbaFormat);
	bool isRGBA() const;
	bool haveDepth() const;
	MStatus convertPixelFormat( MPixelType type, double scale = 1.0, double offset = 0.0);

	static const char *className();

protected:
	//
	MStatus			readFromFileInternalFixed(const MString & pathname, MPixelType pixelType, void* exposureControl = NULL);
	MStatus			readFromFileInternalFloat(const MString & pathname, MPixelType pixelType, void* exposureControl = NULL);
	MStatus			readFromFileInternal(const MString & pathname, MPixelType pixelType, void* exposureControl = NULL);

private:
	friend class MHWRender::MTextureManager;
	void set( void * );
	void setFloat( void * );


	void *_pPixelMap;
	void *_pFloatMap;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MImage */
