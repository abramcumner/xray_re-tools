#ifndef _MHwTextureManager
#define _MHwTextureManager
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
// CLASS:    MHwTextureManager
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MString.h>
#include <maya/MTypes.h>
#include <maya/MImageFileInfo.h>

//! \brief Unique identifier for a texture entry. Returned by MHwTextureManager::registerTextureFile.
typedef int MHwTextureFileHandle;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MHwTextureManager)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
  \deprecated
  Use MHWRender::MTextureManager instead.

  The MHwTextureManager provides an interface for loading and using hardware
  textures.
*/

class OPENMAYARENDER_EXPORT MHwTextureManager
{
public:
	static MStatus	glBind( const MObject& fileTextureObject,
							MImageFileInfo::MHwTextureType& targetType,
							MImageFileInfo::MImageType imageType = MImageFileInfo::kImageTypeUnknown);

	static MStatus	glBind( const MPlug& fileTextureConnection,
							MImageFileInfo::MHwTextureType& targetType,
							MImageFileInfo::MImageType imageType = MImageFileInfo::kImageTypeUnknown);

	static MStatus	registerTextureFile( const MString& fileName,
										 MHwTextureFileHandle& hTexture);

	static MStatus	deregisterTextureFile( const MHwTextureFileHandle& hTexture );

	static MStatus	textureFile( const MHwTextureFileHandle& hTexture,
								MString& fileName );

	static MStatus	glBind( const MHwTextureFileHandle& hTexture,
							MImageFileInfo::MHwTextureType& targetType,
							MImageFileInfo::MImageType imageType = MImageFileInfo::kImageTypeUnknown);

	static const char* className();

private:
	~MHwTextureManager();
#ifdef __GNUC__
	friend class shutUpAboutPrivateDestructors;
#endif
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MHwTextureManager */
