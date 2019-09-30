#ifndef __MTextureEditorDrawInfo_h
#define __MTextureEditorDrawInfo_h
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
// CLASS:    MTextureEditorDrawInfo
//
// ****************************************************************************
//
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/M3dView.h>
#include <maya/MDrawRequest.h>

// ****************************************************************************
// DECLARATIONS

class MSelectionTypeSet;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MTextureEditorDrawInfo)

//! \ingroup OpenMayaUI
//! \brief Drawing state for drawing to the UV texture window with custom
//! shapes.
/*!
  This class is used by drawUV method of MPxSurfaceShapeUI to specify
  the current UV drawing state for a user defined shape. API users
  must override the canDrawUV method on MPxSurfaceShapeUI to recieve
  drawUV calls. The only situation where the drawing style can change
  is during a selection event. However, selection events are currently
  not passed onto the API user.  Therefore, most of the functionality
  in this class is place holder for future work.

  \see MPxSurfaceShapeUI
*/
class OPENMAYAUI_EXPORT MTextureEditorDrawInfo
{
public:
	MTextureEditorDrawInfo();
	MTextureEditorDrawInfo( const MTextureEditorDrawInfo& in );
	virtual ~MTextureEditorDrawInfo();

	//! Draw modes
	enum DrawingFunction {
		kDrawFunctionFirst = 1,	//!< Lowest possible enum value

		//! Draw wireframe only (default)
		kDrawWireframe = kDrawFunctionFirst, 
		kDrawEverything,			//!< Draw vertices, uvs, faces, and edges
		kDrawVertexForSelect,		//!< Draw vertices for selection
		kDrawEdgeForSelect,			//!< Draw edges for selection
		kDrawFacetForSelect,		//!< Draw faces for selection
		kDrawUVForSelect,			//!< Draw uvs for selection

		kDrawFunctionLast = kDrawUVForSelect //!< Highest possible enum value
	} ;

	DrawingFunction			drawingFunction() const;
	void					setDrawingFunction( DrawingFunction func );

	static const char*		className();

private:
	MTextureEditorDrawInfo( void * in );

	void *					fData;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MTextureEditorDrawInfo */
