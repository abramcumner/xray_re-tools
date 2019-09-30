#ifndef _MRenderingInfo
#define _MRenderingInfo
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
// CLASS:    MRenderingInfo
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MDagPath.h>
#include <maya/MViewportRenderer.h>
#include <maya/MRenderTarget.h>
#include <maya/MMatrix.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRenderingInfo)

//! \ingroup OpenMayaRender
//! \brief Information to perform rendering into a hardware render target.
/*!
MRenderingInfo is a class which holds information about rendering into
hardware render targets.
*/
class OPENMAYARENDER_EXPORT MRenderingInfo
{
public:
	//! Origin (x) of region to render to in pixels
	unsigned int	originX() const;

	//! Origin (y) of region to render to in pixels
	unsigned int	originY() const;

	//! Width of region to render in pixels
	unsigned int	width() const;

	//! Height of region to render in pixels
	unsigned int	height() const;

	//! Native target rendering API
	MViewportRenderer::RenderingAPI renderingAPI() const;

	//! Native target rendering version
	float renderingVersion() const;

	//! Current camera being used to render with
    const MDagPath & cameraPath() const;

	//! Current render target
	const MRenderTargetLegacy	& renderTarget() const;

	//! Current view matrix
	const MMatrix & viewMatrix() const;

	//! Current projection matrix
	const MMatrix & projectionMatrix() const;

protected:
	// No protected data

private:
	MRenderingInfo();
	~MRenderingInfo();
	void			set( void * );
	const void *	get() const;

	friend class MViewportRenderer;
	friend class MHardwareRenderer;

	unsigned int	fOriginX;
	unsigned int	fOriginY;
	unsigned int	fWidth;
	unsigned int	fHeight;
	MViewportRenderer::RenderingAPI fRenderingAPI;
	MDagPath		fCameraPath;

	MMatrix			fViewMatrix;
	MMatrix			fProjectionMatrix;

	float			fRenderingVersion;

	MRenderTargetLegacy	fRenderTarget;

	const void*	 _info;
};

OPENMAYA_NAMESPACE_CLOSE

#endif // _MRenderingInfo
#endif


