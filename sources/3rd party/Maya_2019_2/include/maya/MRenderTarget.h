#ifndef _MRenderTarget
#define _MRenderTarget
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
// CLASS:    MRenderTargetLegacy
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRenderTargetLegacy)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
	\deprecated
	Use MHWRender::MRenderTargetManager and MHWRender::MRenderTarget instead.

	MRenderTargetLegacy is a class contains information about a given
	hardware render target.
*/

class OPENMAYARENDER_EXPORT MRenderTargetLegacy
{
public:
	//! Width of render target in pixels
	unsigned int	width() const;

	//! Height of render target in pixels
	unsigned int	height() const;

	void			makeTargetCurrent() const;
	MStatus			writeColorBuffer( const MImage &image, signed short x = 0, signed short y = 0,
										bool writeDepth = false) const;

protected:
	// No protected data

private:
	MRenderTargetLegacy();
	~MRenderTargetLegacy();
	void			set( void * );
	void			setWidth( unsigned int w ) { fWidth = w; }
	void			setHeight( unsigned int h ) { fHeight = h; }

	friend class MHardwareRenderer;
	friend class MRenderingInfo;

	// Render target dimensions
	unsigned int	fWidth;
	unsigned int	fHeight;

	void			*fRenderTarget;
};
OPENMAYA_NAMESPACE_CLOSE

#endif // _MRenderTarget
#endif
