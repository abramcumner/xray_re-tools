#ifndef _MD3D9Renderer
#define _MD3D9Renderer
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
// CLASS:    MD3D9Renderer
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MString.h>
#include <maya/MHardwareRenderer.h>

// Forward declarations
//
struct IDirect3DDevice9;

OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
  \deprecated
  Use the DirectX 11 device from MHWRender::MRenderer::GPUDeviceHandle() instead.

  MD3D9Renderer is a wrapper class to provide access to Maya's DirectX
  resources.
*/
class OPENMAYARENDER_EXPORT MD3D9Renderer /* public MPxHardwareRender */ {

public:

	// Swatch rendering methods
	MStatus			makeSwatchContextCurrent(unsigned int& width,  unsigned int& height);

	MStatus			setBackgroundColor(const MColor& color);

	MStatus			readSwatchContextPixels(MImage & image);

	IDirect3DDevice9*	getD3D9Device();

	static MD3D9Renderer*	theRenderer();

protected:
// No protected members

private:

	static MD3D9Renderer* fsHardwareRenderer;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MD3D9Renderer */


