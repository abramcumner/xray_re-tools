#ifndef _MDrawInfo
#define _MDrawInfo
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
// CLASS:    MDrawInfo
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
// CLASS DECLARATION (MDrawInfo)

//! \ingroup OpenMayaUI
//! \brief \obsolete
/*!
\deprecated
Use MHWRender::MRenderItem and MHWRender::MPxGeometryOverride instead 
for Viewport 2.0 support.

This class is used by the getDrawRequests method of MPxSurfaceShapeUI
to specify the current object drawing state for a user defined shape.

This getPrototype method is used to construct a draw request object
based on the current draw state for the object.

See MDrawRequest for more information.
*/
class OPENMAYAUI_EXPORT MDrawInfo
{
public:
	MDrawInfo();
	MDrawInfo( const MDrawInfo& in );
	virtual ~MDrawInfo();


	// This method returns a partially filled in MDrawRequest
	MDrawRequest			getPrototype(
								const MPxSurfaceShapeUI& drawHandler ) const;

	// Only really need camera path here
	M3dView  				view() const;

	// path to object to draw
	const MDagPath 			multiPath () const;
	
	// set the path to the object to be drawn. 
	void					setMultiPath( const MDagPath & );

	// projection (camera) matrix
	const MMatrix 			projectionMatrix() const;

	// world space inclusive matrix
	const MMatrix 			inclusiveMatrix() const;

	// display appearance (this may change during traversal)
	M3dView::DisplayStyle	displayStyle() const;

	// display status of object to draw
	M3dView::DisplayStatus	displayStatus() const;

	// display objects
	bool objectDisplayStatus( M3dView::DisplayObjects ) const;
	bool pluginObjectDisplayStatus( const MString & pluginDisplayFilter) const;

	bool					inSelect() const;
	bool					inUserInteraction() const;
	bool					userChangingViewContext() const;
	bool					completelyInside() const;

	// convenience to test if component can be drawn
	bool					canDrawComponent( bool isDisplayOn,
									const MSelectionMask & compMask ) const;

	static const char*		className();

protected:
	void*	 fData;

private:
    MDrawInfo( void* in );
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDrawInfo */
