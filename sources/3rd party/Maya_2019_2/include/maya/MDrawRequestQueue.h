#ifndef _MDrawRequestQueue
#define _MDrawRequestQueue
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
// CLASS:    MDrawRequestQueue
//
// ****************************************************************************
//
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MDrawRequest.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDrawRequestQueue)

//! \ingroup OpenMayaUI
//! \brief \obsolete
/*!
\deprecated
Use MHWRender::MRenderItem and MHWRender::MPxGeometryOverride instead for Viewport 2.0 support.

This class defines a simple interface for a collection of
MDrawRequest objects.

An MDrawRequestQueue object is passed to the getDrawRequests method
of a user defined shape's UI class (MPxSurfaceShapeUI).
This queue keeps track of all the things that need to get draw
when a refresh of the view occurs.

Maya will call the getDrawRequest methods of all the visible DAG objects
before a refresh happens. Then as the refresh happens, each draw request
will be processed and the corresponding draw method for each DAG
object will get called. For user defined shapes MPxSurfaceShapeUI::draw
will get called.
*/
class OPENMAYAUI_EXPORT MDrawRequestQueue
{
public:
	MDrawRequestQueue();
	~MDrawRequestQueue();

public:
	bool			isEmpty() const;
	void			add( MDrawRequest & );
	MDrawRequest	remove();

	MDrawRequestQueue( const MDrawRequestQueue & );
	MDrawRequestQueue & operator=( const MDrawRequestQueue & );

	static const char*	className();

protected:
// No protected members

private:
	bool			isInternalQueue() const; 
	void			cleanup();
    MDrawRequestQueue( void* in );
	void*			fDrawRequestQueue;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDrawRequestQueue */
