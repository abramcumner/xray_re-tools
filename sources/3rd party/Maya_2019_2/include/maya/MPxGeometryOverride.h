#ifndef _MPxGeometryOverride
#define _MPxGeometryOverride
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
// CLASS:    MPxGeometryOverride
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MPxGeometryOverride)
//
//  MPxGeometryOverride allows the user to create an override to prepare
//  vertex data for a Maya DAG object for drawing with an arbitrary shader in
//  Viewport 2.0.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MUintArray.h>
#include <maya/MFloatArray.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MUIDrawManager.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// CLASS DECLARATION (MPxGeometryOverride)

//! \ingroup OpenMayaRender MPx
//! \brief Base for user-defined classes to prepare geometry for drawing
/*!
MPxGeometryOverride allows the user to create an override to prepare vertex
data that will be used to draw a specific Maya DAG object type with an
arbitrary shader (standard Maya or custom) in Viewport 2.0.

This class is designed to be a high level data-translator independent of any
specific hardware draw API. Once registered through MDrawRegistry, an instance
will be created for each node with a matching classification string. That
instance will be used to generate the vertex streams needed by the assigned
shaders in order to draw the object. The intent of this class is that it be
used to provide data for plugin shape types (MPxSurfaceShape) however it can
also be used to override the geometry translation for any Maya geometry type.

If a more low-level interface to the Viewport 2.0 draw loop is needed, look
at either MPxDrawOverride or MPxShaderOverride.

The override is only triggered when the associated DAG object has changed and
that object is about to be drawn. So it is not invoked if the user is simply
tumbling the scene, or if the object is not within the current view frustum.

Implementations of MPxGeometryOverride must be registered with Maya through
MDrawRegistry.

Users of this interface must implement several virtual methods which will be
called at specific times during the draw-preparation phase.

1) updateDG() : In the updateDG() call, all data needed to compute the indexing
and geometry data must be pulled from Maya and cached. It is invalid to query
attribute values from Maya nodes in any later stage and doing so may result in
instability.

2) updateRenderItems() : For each shader assigned to the instance of the object
Maya will assign a render item. A render item is a single atomic renderable
entity containing a shader and some geometry. In updateRenderItems(),
implementations of this class may enable or disable the automatic shader-based
render items and they may add or remove custom user defined render items in
order to cause additional things to be drawn. Look at the MRenderItem interface
for more details.

3) addUIDrawables() : For each instance of the object, besides the render
items updated in 'updateRenderItems()' for the geometry rendering, there is also
a render item list there for render the simple UI elements. 'addUIDrawables()'
happens just after normal geometry item updating, The original design for this stage
is to allow user accessing 'MUIDrawManager' which helps drawing the simple geometry
easily like line, circle, rectangle, text, etc.
Overriding this methods is not always necessary, but if you want to override it, please also
override 'hasUIDrawables()' to make it return true or the overrided method will not be called.

4) populateGeometry() : In this method the implementation is expected to fill
the MGeometry data structure with the vertex and index buffers required to draw
the object as indicated by the data in the geometry requirements instance
passed to this method. Failure to fulfill the geometry requirements may result
in incorrect drawing or possibly complete failure to draw the object.

5) cleanUp() : Delete any cached data generated in the earlier phases that is
no longer needed.

Additionally, this interface includes some optional virtual methods which may be
implemented to extend the functionality of MPxGeometryOverride.

1) requiresGeometryUpdate() : If this method returns true then all of the other 
methods will be called for this draw-preparation phase.  This allows your
MPxGeometryOverride to depend on data outside of associated DAG object.
By default this method returns false.

2) requiresUpdateRenderItems() : If this method returns false for an MDagPath instance
of the associated DAG object then updateRenderItems() will not be called for that MDagPath
draw-preparation phase.  If requiresUpdateRenderItems() returns false for all MDagPaths
then updateRenderItems() will not be called at all for the draw-preparation phase.

3) supportsEvaluationManagerParallelUpdate() : If this method returns true then the
MPxGeometryOverride will be considered for Evaluation Manager Parallel Update.

If the MPxGeometryOverride object returns false from hasUIDrawables and requiresUpdateRenderItems
during the draw-preparation phase then the MPxGeometryOverride is eligible for 
Evaluation Manager Parallel Update during that draw-preparation phase.

Evaluation Manager Parallel Update is a performance optimization which allows the
updateDG() and populateGeometry() step of the draw update to occur during Evaluation
Graph Execution rather than during VP2 scene rendering.  The improves performance by
allowing updateDG() and populateGeometry() calls of several plug-ins to run in parallel
with each other and with Evaluation Manager dependency graph evaluation.

Using Evaluation Manager Parallel Update incurs some additional overhead updating each
object.  For drawable objects where updateDG() and populateGeometry() calls are short 
(such as for UI objects) it may be faster to not support Evaluation Manager Parallel Update.

Enabling Evaluation Manager Parallel Update for MPxGeometryOverride does add some 
additional restrictions to the implementation of updateDG() and populateGeometry().

When Evaluation Manager Parallel Update is enabled, updateDG() and populateGeometry() 
may be called from a worker thread which may not safely access the graphics API.  To
populate geometry you must use the methods on MIndexBuffer and MVertexBuffer which 
explicitly state they are safe to use on a worker thread.  Furthermore, updateDG() 
and populateGeometry() may only access values on the MObject associated with the 
MPxGeometryOverride.  Accessing values on other objects will result in undefined behavior.

Using Evaluation Manager Parallel Update with Evaluation Caching enabled adds additional
limitations on Evaluation Manager Parallel Update.  During Evaluation Manager Parallel Update
it is only safe to read attributes which are not animated, or attributes which are animated
and cached.  For more information on how to ensure attributes are cached with Evaluation
Caching please refer to the Evaluation Caching documentation.

MPxGeometryOverride automatically supports Evaluation Caching.  No changes to MPxGeometryOverride
are necessary to enable Evaluation Caching support.  If MPxGeometryOverride is attached to an
user-defined node then some changes to the user-defined node may be necessary for Evaluation
Caching to work correct with the user-defined node and MPxGeometryOverride.  Specifically, all
values read by MPxGeometryOverride must be cached by the user-defined node.

MPxGeometryOverride includes optional support for VP2 Custom Caching.  To support VP2 Custom Caching
a node must also support Evaluation Manager Parallel Update.  When VP2 Custom Caching
is supported three new potential behaviors will occur.

1) During evaluation when the VP2 Custom Cache is not ready the MPxGeometryOverride will be
invoked as normal.  After populateGeometry() the VP2 Custom Cache will save references to
the MVertexBuffers and MIndexBuffers used to draw the object for the current time.  It is
not safe to hold references to these buffers after returning from populateGometry().

2) During evaluation when the VP2 Custom Cache is ready a subset of the MPxGeomtryOverride
methods will be invoked.  The plug-in will have a chance to choose which render items draw
and to add UI render items (hasUIDrawable(), updateRenderItems(), addUIDrawables() will all
be called).  However, no updateDG() or populateGeometry() call will be made.  The pre-existing
buffers from an earlier evaluation will be used to draw.

3) When Maya is idle Maya will fill the VP2 Custom Cache using background evaluation.  
updateDG(), populateGeometry() and cleanUp() will be invoked in a worker thread which does not have
the normal context as the current context.  During background evaluation there are additional
restrictions on updateDG(), populateGeometry() and cleanUp(): it is not safe to access the graphics API
and it is only safe to access values on the MObject associated with the MPxGeometryOverride.
After the populateGeometry() call the MVertexBuffers and MIndexBuffers prepared will be
stored.  It is not safe to hold references to these buffers after returning from populateGeometry().

When MPxGeometryOverride is invoked from background evaluation for VP2 Custom Caching the
current context will already be set correctly for the frame being background evaluated.

When VP2 Custom Caching is enabled and the desired content of the MVertexBuffers used to draw
the MPxGeometryOverride is changing then requiresGeometryUpdate() must return true.
*/
class OPENMAYARENDER_EXPORT MPxGeometryOverride
{
public:

	MPxGeometryOverride(const MObject& obj);
	virtual ~MPxGeometryOverride();

	virtual ::MHWRender::DrawAPI supportedDrawAPIs() const;
	virtual bool hasUIDrawables() const;
    virtual bool requiresGeometryUpdate() const;
    virtual bool requiresUpdateRenderItems(
        const MDagPath& path) const;
    virtual bool supportsEvaluationManagerParallelUpdate() const;
    virtual bool supportsVP2CustomCaching() const;

	virtual void updateDG() = 0;
	virtual bool isIndexingDirty(
		const MRenderItem& item);
	virtual bool isStreamDirty(
		const MVertexBufferDescriptor& desc);
	virtual void updateRenderItems(
		const MDagPath& path,
		MRenderItemList& list) = 0;
	virtual void addUIDrawables(
		const MDagPath& path,
		MUIDrawManager& drawManager,
		const MFrameContext& frameContext);
	virtual void populateGeometry(
		const MGeometryRequirements& requirements,
		const MRenderItemList& renderItems,
		MGeometry& data) = 0;
	virtual void cleanUp() = 0;

	virtual bool refineSelectionPath(const MSelectionInfo& selectInfo,
									 const MRenderItem& hitItem, 
									 MDagPath& multipath,
									 MObject& geomComponents,
									 MSelectionMask& objectMask);

	virtual void updateSelectionGranularity(
		const MDagPath& path,
		MSelectionContext& selectionContext);

	// Call trace methods
	virtual bool traceCallSequence() const;
	virtual void handleTraceMessage( const MString &message ) const;

	// Frame context
	const MFrameContext* getFrameContext() const;

	static bool pointSnappingActive();
	static	const char*	className();

private:
	MPxGeometryOverride() {}
	MFrameContext mFrameContext;
	bool mOutsideSetupCleanup;
};

} // namespace MHWRender
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxGeometryOverride */
