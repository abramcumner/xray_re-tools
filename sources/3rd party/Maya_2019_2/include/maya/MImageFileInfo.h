#ifndef _MImageFileInfo
#define _MImageFileInfo
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
// CLASS:    MImageFileInfo
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MString.h>
#include <maya/MImage.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MImageFileInfo)

//! \ingroup OpenMaya
//! \brief Image manipulation 
/*!

  This class provides methods for reading file images stored on disk.

  MImageFileInfo is a utility class used to describe the characteristics
  of an image file, such as dimensions, channel count, and pixel format.
  This class is used in MPxImageFile.
*/
class OPENMAYA_EXPORT MImageFileInfo
{
public:
	//! Hardware texture types.
	enum MHwTextureType {
		kHwTextureUnknown,	//!< \nop
		kHwTexture1D,		//!< \nop
		kHwTexture2D,		//!< \nop
		kHwTexture3D,		//!< \nop
		kHwTextureRectangle,	//!< \nop
		kHwTextureCubeMap	//!< \nop
	};

	//! Image types.
	enum MImageType {
		kImageTypeUnknown,	//!< \nop
		kImageTypeColor,	//!< \nop
		kImageTypeNormal,	//!< \nop
		kImageTypeBump		//!< \nop
	};

					MImageFileInfo();
					void width( unsigned int value);
					void height( unsigned int value);
					void channels( unsigned int value);
					void numberOfImages( unsigned int value);
					void pixelType( MImage::MPixelType value);
					void imageType( MImageType value);
					void hardwareType( MHwTextureType value);
					void hasAlpha( bool value);
					void hasMipMaps( bool value);
					unsigned int width() const;
					unsigned int height() const;
					unsigned int channels() const;
					unsigned int numberOfImages() const;
					MImage::MPixelType pixelType() const;
					MImageType imageType() const;
					MHwTextureType hardwareType() const;
					bool hasAlpha() const;
					bool hasMipMaps() const;

protected:
	// No protected members

private:

	unsigned int	imageWidth;
	unsigned int	imageHeight;
	unsigned int	imageChannels;
	unsigned int	imageCount;
	MImage::MPixelType imagePixelType;
	MImageType		imageImageType;
	MHwTextureType	imageHardwareType;
	bool			imageHasAlpha;
	bool			imageHasMipMaps;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MImageFileInfo */
