#ifndef _MDrawRequest
#define _MDrawRequest
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
// CLASS:    MDrawRequest
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

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDrawRequest)

//! \ingroup OpenMayaUI
//! \brief \obsolete
/*!
\deprecated
Use MHWRender::MRenderItem and MHWRender::MPxGeometryOverride 
instead for Viewport 2.0 support.

This class encapsulates all the information needed to fulfill
a request to draw an object or part of an object.
This class is used by the draw methods of MPxSurfaceShapeUI
derived objects.

The draw request should be created in the overridden
MPxSurfaceShapeUI::getDrawRequests method. Once created the
appropriate "set" methods of this class should be used to define
what is being requested. Then the request should be placed on the
draw reqeust queue using MDrawRequestQueue::add.

When your request gets processed by maya, your overriden
MPxSurfaceShape::draw method will get called with your request.
Use the query methods of this class to determine what to draw.

You create a draw request using the method MDrawInfo::getPrototype.
A draw request automatically picks up certain information (listed
below) upon its creation. So you don't have to set any of this
information unless you want to change it.

Information automatically set by MDrawInfo::getPrototype :

    \li <b>path</b>                path to object to be drawn
    \li <b>view</b>                view to draw in
    \li <b>matrix</b>              world matrix for object
    \li <b>display appearance</b>  how object should be drawn
    \li <b>display status</b>      active, dormant etc.


The draw token is an integer value which you can use to specify
what is to be drawn. This is object specific and so you should
define an enum with the information you require to decide what
is being drawn in your MPxSurfaceShapeUI::draw method.

Here is an example of draw token values for a polygonal mesh
object as defined in an MPxSurfaceShapeUI derived class.

\code
    // Draw Tokens
    //
    enum {
        kDrawVertices, // component token
        kDrawWireframe,
        kDrawWireframeOnShaded,
        kDrawSmoothShaded,
        kDrawFlatShaded,
        kLastToken
    };
\endcode
*/
class OPENMAYAUI_EXPORT MDrawRequest
{
public:
	MDrawRequest();
	MDrawRequest( const MDrawRequest& in );
	~MDrawRequest();

public:

	// the view to draw to
	M3dView					view() const;
	void					setView( M3dView & );
	const MDagPath			multiPath() const;
	void					setMultiPath( const MDagPath & );
	MObject 				component() const;
	void					setComponent( MObject & );
	MDrawData 				drawData() const;
	void					setDrawData( MDrawData & );
	M3dView::DisplayStatus	displayStatus() const;
	void					setDisplayStatus( M3dView::DisplayStatus );
	bool					displayCulling() const;
	void					setDisplayCulling( bool );
	bool					displayCullOpposite() const;
	void					setDisplayCullOpposite( bool );
	M3dView::DisplayStyle	displayStyle() const;
	void					setDisplayStyle( M3dView::DisplayStyle );
	int						color( M3dView::ColorTable table ) const;
	void					setColor( int, M3dView::ColorTable table );
	MColor					color() const;
	void					setColor( const MColor & color );
	MMaterial 				material() const;
	void					setMaterial( MMaterial& );
	bool					isTransparent() const;
	void					setIsTransparent( bool );
	bool					drawLast() const;
	void					setDrawLast( bool );
	int						token() const;
	void					setToken( int );
	const MMatrix &			matrix() const;
	void					setMatrix( const MMatrix & );

	MDrawRequest&	operator = ( const MDrawRequest& other );

	static const char*		className();

protected:
// No protected members

private:
    MDrawRequest( void* in, bool own );
	void*	fDrawRequest;
	bool    fOwn;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDrawRequest */
