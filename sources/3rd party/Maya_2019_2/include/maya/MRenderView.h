#ifndef _MRenderView
#define _MRenderView
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
// CLASS:    MRenderView
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MDagPath.h>
#include <maya/MString.h>

// ****************************************************************************
// DECLARATIONS

//! Pixel data type.  Each channel must be a floating point
//! value in the range 0.0 to 255.0.
typedef struct RV_PIXEL {
    float    r;		//!< red
    float    g;		//!< green
    float    b;		//!< blue
    float    a;		//!< alpha
} RV_PIXEL;

//! Arbitrary Output Variable (AOV) data type.
typedef struct RV_AOV {
    unsigned int    numberOfChannels;
    MString         name;
    float*          pPixels;
} RV_AOV;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRenderView)

//! \ingroup OpenMayaRender
//! \brief Static class providing Render View API functions
/*!

  This class provides access to the Maya Render View.  The class
  allows plugins to send image data to the Render View in the same way
  that the Maya renderer does.  Either a "full render" or a "region
  render" can be performed.  In a full render, the Render View expects
  to receive pixel data that fills the entire image, while a region
  render expects only updates to a specified image region.

<b>Usage</b>

To send an image to the Render View, use the following sequence of calls:

  <ol>
	<li> Call doesRenderEditorExist() to make sure that a Render View
		exists.  If this returns false, then Maya is operating in batch
		mode.
	<li> Call setCurrentCamera() to specify the name of the camera
		that is being rendered.
	<li> Call startRender() or startRegionRender() to inform
		the Render View that you are about to start sending either a
		full image or a region update.  When region rendering, the
		getRenderRegion() method can be used to retrieve the currently
		selected marquee region in the Render View.
	<li> The image or image region is sent to the Render
		View as a series of one or more tiles.  For each tile, send
		the tile's image data using the updatePixels() method.  Call
		the refresh() method to refresh the Render View after each
		tile has been sent.
	<li> Call endRender() to signal the Render View that all
		image data has been sent.
  </ol>
*/
class OPENMAYARENDER_EXPORT MRenderView
{
public:

	static bool doesRenderEditorExist();

	static MStatus setCurrentCamera( MDagPath& camera );

	static MStatus getRenderRegion( unsigned int& left,
 									unsigned int& right,
									unsigned int& bottom,
									unsigned int& top );

	static MStatus startRender( unsigned int width,
								unsigned int height,
								bool doNotClearBackground = false,
								bool immediateFeedback = false );

	static MStatus startRegionRender( unsigned int imageWidth,
									  unsigned int imageHeight,
									  unsigned int regionLeft,
									  unsigned int regionRight,
									  unsigned int regionBottom,
									  unsigned int regionTop,
									  bool doNotClearBackground = false,
									  bool immediateFeedback = false );

	static MStatus updatePixels( unsigned int left,
								 unsigned int right,
								 unsigned int bottom,
								 unsigned int top,
								 RV_PIXEL* pPixels,
                                 bool isHdr = false,
                                 const unsigned int numberOfAOVs = 0,
                                 const RV_AOV* const pAOVs = NULL );

	static MStatus refresh( unsigned int left,
							unsigned int right,
							unsigned int bottom,
							unsigned int top );

	static MStatus endRender();
    
    static MStatus setDrawTileBoundary( bool drawTileBoundary );

	/*!
		Returns the name of this class.

		\return
		The name of this class.
	*/
	static const char *className() { return "MRenderView"; };

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderView */
