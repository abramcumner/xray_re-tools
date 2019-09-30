#ifndef _MPxGlBuffer
#define _MPxGlBuffer
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
// CLASS:    MPxGlBuffer
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MPxGlBuffer)
//
//  MPxGlBuffer allows the user to create OpenGL buffers that Maya
//	can draw into.  The base class as is defined will create a hardware
//	accellerated off-screen buffer.
//
//  To create a custom buffer, derive from this class and override the
//  beginBufferNotify and endBufferNotify methods.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxGlBuffer)

//! \ingroup OpenMayaUI MPx
//! \brief \obsolete
/*!
\deprecated
Use MHWRender::MRenderOverride and MHWRender::MRenderTarget instead.

Historically this class was used to created offscreen buffers on
Linux.  This class is now using FBO. Invoke openFbo() method to 
create a Frame Buffer Object. The contents
of the frame buffer object (FBO) can be read back by using the bindFbo()
method and OpenGl calls to read pixels. After rendering and reading
pixels, the frame buffer object can be destroyed by calling
closeFbo().

*/
class OPENMAYAUI_EXPORT MPxGlBuffer
{
public:
	MPxGlBuffer();
	MPxGlBuffer( M3dView &view );
	virtual ~MPxGlBuffer();

	MStatus		openFbo( short width, short height, M3dView & );
	MStatus		closeFbo( M3dView & ); 
	MStatus		bindFbo(); 
	MStatus		unbindFbo(); 
	
	virtual void			beginBufferNotify( );
	virtual void			endBufferNotify( );

	static	const char*		className();

protected:
	bool					hasColorIndex;
	bool					hasAlphaBuffer;
	bool					hasDepthBuffer;
	bool					hasAccumulationBuffer;
	bool					hasDoubleBuffer;
	bool					hasStencilBuffer;

private:
	void   setData( void* );
	void * 	data;

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxGlBuffer */
