#ifndef _MHardwareRenderer
#define _MHardwareRenderer
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
// CLASS:    MHardwareRenderer
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MString.h>
#include <maya/MColor.h>
#include <maya/MDeprecate.h>

#define kDefaultQuadColor  MColor(0.5f, 0.5f, 0.5f, 1.0f)

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MHardwareRenderer)

//! \ingroup OpenMayaRender
//! \brief Static hardware renderer interface class
/*!
 MHardwareRenderer is an interface class which wraps the hardware
 renderer.
*/
class OPENMAYARENDER_EXPORT MHardwareRenderer
{
public:
	//! Draw Procedure status codes
	enum DrawProcedureStatusCode {
		//! Success
		kSuccess = 0,
		//! Failure
		kFailure,
		//! Item already exists
		kItemExists,
		//! Item is not found
		kItemNotFound,
		//! Location not found
		kLocationNotFound
	};

	//! Specify the pixel format of the current buffer
	enum BufferPixelFormat {
		//! 8 bit Red, Green, Blue, and Alpha channel
		kRGBA_Fix8,
		//! 16 bit Red, Green, Blue, and Alpha channel
		kRGBA_Float16,
		//! 32 bit floating point depth buffer
		kDepth_Float32
	};

	//! Draw Procedure call locations
	enum CallLocation {
		//! Before rendering one frame. No model or view matrices
		// can be assumed to have been set up at this time,
		// nor an assumption on the background.
		kPreRendering = 0,
		//! Before rendering one "exposure". If multiple exposures are
		//! are required to render one frame. After the frame buffer is cleared
		//! and model and view matrices are set up for the current exposure.
		kPreExposure = 1,
		//! After rendering one "exposure".
		kPostExposure = 2,
		//! After rendering one frame. Before a possible fame buffer swap.
		kPostRendering = 3
	};

	//! Default geometry shapes
	enum GeometricShape {
		//! Sphere with radius 1, centered at 0,0,0.
		kDefaultSphere = 0,
		//! Plane with width and height of 1, centered at 0,0,0.
		//! Assuming "Y-Up" orientation: width = x-axis, and height = y-axis.
		kDefaultPlane,
		//! Cube with width, height and depth of 1, centered at 0,0,0.
		kDefaultCube
	};

	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use the Viewport 2.0 device independent MHWRender interfaces instead.")
	static MHardwareRenderer*	theRenderer();

	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use the Viewport 2.0 backend independent MHWRender interfaces instead.")
	const MString &				backEndString() const;

	// Draw procedure methods
	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use MHWRender::MRenderOverride instead.")
	DrawProcedureStatusCode		addDrawProcedure( const MString & backEndString,
									MDrawProcedureBase *drawProcedureptr,
									CallLocation location);

	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use MHWRender::MRenderOverride instead.")
	DrawProcedureStatusCode		removeDrawProcedure( const MString & backEndString,
									MDrawProcedureBase *drawProcedureptr,
									CallLocation location );
	DrawProcedureStatusCode		insertDrawProcedure( const MString & backEndString,
									   MDrawProcedureBase *drawProcedurePtr,
									   CallLocation location,
									   unsigned int listIndex,
									   bool moveExistingItem = false);
	DrawProcedureStatusCode		getDrawProcedureCount( const MString & backEndString,
									  CallLocation location,
									  unsigned int &count ) const;
	DrawProcedureStatusCode		getDrawProcedureListNames( const MString & backEndString,
												CallLocation location,
												MStringArray &namesOfProcs ) const;
	MDrawProcedureBase * findDrawProcedure( const MString & backEndString,
									CallLocation location,
									const MString &procName ) const;

	// Context handling methods
	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use the Viewport 2.0 device independent MHWRender interfaces instead.")
	MStatus						makeResourceContextCurrent(const MString &backEndString);

	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use MHWRender::MRenderTarget and MHWRender::MRenderTargetDescription instead.")
	MStatus						getBufferSize( const MString& backEndString,
												unsigned int &width,
												unsigned int &height ) const;

	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use MHWRender::MRenderTarget and MHWRender::MRenderTargetDescription instead.")
	MStatus						getColorBufferPixelFormat( const MString& backEndString,
														BufferPixelFormat &fmt ) const;

	MStatus						getDepthBufferPixelFormat( const MString& backEndString,
														BufferPixelFormat &fmt ) const;

	MStatus						getCurrentExposureNumber( const MString& backEndString,
														  unsigned int &number ) const;
	MStatus						getTotalExposureCount( const MString& backEndString,
														  unsigned int &number ) const;

	MStatus restoreCurrent(const MString& backEndString) const;


	// Swatch rendering methods
	MStatus						makeSwatchContextCurrent( const MString& backEndString,
														  unsigned int & width,
														  unsigned int & height );

	MStatus						readSwatchContextPixels( const MString& backEndString,
												   MImage & image );

	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use MHWRender::MGeometryUtilities::acquireReferenceGeometry() instead.")
	MGeometryData *				referenceDefaultGeometry(
									MHardwareRenderer::GeometricShape geomShape,
									unsigned int & numberOfData,
									unsigned int* & pIndexing,
									unsigned int & indexLength);

	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use MHWRender::MGeometryUtilities::releaseReferenceGeometry() instead.")
	MStatus						dereferenceGeometry(MGeometryData * pGeomData,
											unsigned int numberOfData);

	void getSwatchOrthoCameraSetting( double& l, double& r,
									  double& b, double& t,
									  double& n, double& f ) const;

	void getSwatchPerspectiveCameraSetting( double& l, double& r,
									  double& b, double& t,
									  double& n, double& f ) const;
	void getSwatchPerspectiveCameraTranslation( float& x,  float& y, float& z, float& w ) const;

	void getSwatchLightDirection( float& x,  float& y, float& z, float& w ) const;

	void drawSwatchBackGroundQuads( const MColor & quadColor = kDefaultQuadColor,
									bool textured = false,
									unsigned int numberOfRepeats = 8) const;

	OPENMAYA_DEPRECATED(2019, "This function is obsolete, Please use the Viewport 2.0 device independent MHWRender interfaces instead.")
	MGLFunctionTable *			glFunctionTable() const;

protected:
	// Destructor and constructor are protected
	MHardwareRenderer();
	~MHardwareRenderer();

	// Static global instance of wrapper class
	//
	static	MHardwareRenderer *	fsHardwareRenderer;
	static  MGLFunctionTable *	fsGLFunctionTable;

private:
// No private members

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MHardwareRenderer */


