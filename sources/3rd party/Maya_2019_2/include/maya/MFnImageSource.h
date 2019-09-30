#ifndef _MFnImageSource
#define _MFnImageSource
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
// CLASS:    MFnImageSource
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MTypes.h>
#include <maya/MFnDependencyNode.h>
// Unfortunate include for MCommonRenderSettingsData::MpathType
#include <maya/MCommonRenderSettingsData.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnImageSource)

//! \ingroup OpenMayaRender MFn
//! \brief Function set for image sources.
/*!
An image source is a node that is used in render graphs, e.g. compositing 
graphs, shading  networks.  It provides access to the images produced by 3D 
scene renderers, or 2D image processing tasks.  This class provides 
functionality for working with image sources, e.g. retrieving the path of
rendered image source files.
*/

class OPENMAYARENDER_EXPORT MFnImageSource : public MFnDependencyNode
{
	declareMFn(MFnImageSource, MFnDependencyNode);

public:

 	MString	getImageName(
           MCommonRenderSettingsData::MpathType pathType,
           double                               frameNumber,
           const MString&                       sceneName,
           const MObject                        camera,
           const MObject                        renderLayer,
           MString*                             bufferName = NULL,
           MStatus*                             ReturnStatus = NULL
        ) const;   

    MObject sourceLayer(const MObject& currentLayer, MStatus* ReturnStatus = NULL);

    MObject sourceCamera(const MObject& currentCamera, MStatus* ReturnStatus = NULL);

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnImageSource, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnImageSource */
