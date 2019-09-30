#ifndef _MPxImageFile
#define _MPxImageFile
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
// CLASS:    MPxImageFile
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
// CLASS DECLARATION (MPxImageFile)

//! \ingroup OpenMaya MPx
//! \brief Image manipulation. 
/*!
  This class provides methods for reading file images stored on disk.

  The MPxImageFile class can be used to implement support for new image
  file formats in Maya. The implementation is able to support both fixed
  and floating point image formats.

  MPxImageFile allows custom image formats to be recognized by Maya and any 
  plug-ins that use Maya's API for accessing images. However be aware that 
  some third party plugins bypass Maya's API and use 
  their own internal facilities for accessing images, which means that they
  will not recognize MPxImageFile-based image formats. 
*/
class OPENMAYA_EXPORT MPxImageFile
{
public:
					MPxImageFile();
					virtual	~MPxImageFile();
					virtual MStatus open( MString pathname, MImageFileInfo* info);
					virtual MStatus load( MImage& image, unsigned int imageNumber);
					virtual MStatus glLoad( const MImageFileInfo& info, unsigned int imageNumber);
					virtual MStatus close();

protected:
	// No protected members

private:

	void*			_instance;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxImageFile */
