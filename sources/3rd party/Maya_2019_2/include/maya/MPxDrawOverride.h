#ifndef _MPxDrawOverride
#define _MPxDrawOverride
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
// CLASS:    MPxDrawOverride
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MPxDrawOverride)
//
//  MPxDrawOverride allows the user to define custom draw code to be
//  used to draw all instances of a specific DAG object type in Maya when using
//  Viewport 2.0.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MBoundingBox.h>
#include <maya/MMatrix.h>
#include <maya/MUserData.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>
#include <maya/MDeprecate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// CLASS DECLARATION (MPxDrawOverride)

//! \ingroup OpenMayaRender MPx
//! \brief Base class for user defined drawing of nodes.
/*!
MPxDrawOverride allows the user to define custom draw code to be used
to draw all instances of a specific DAG object type in Maya when using
Viewport 2.0.

transform() should always return the world space transformation matrix for the
object to be drawn in the custom draw code. The default implementation simply
returns the transformation defined by the parent transform nodes.

isBounded() can return true or false to indicate whether the object is bounded
or not respectively. If a false value is returned
then camera frustum culling will not be performed against the bounding box.
In this situation the boundingBox() method will not be called since the bounding box 
is not required.

boundingBox() should always return the object space bounding box for whatever
is to be drawn in the custom draw code. If the bounding box is incorrect the
node may be culled at the wrong time and the custom draw method might not be
called.

When the object associated with the draw override changes, prepareForDraw() is
called which allows the user to pull data from Maya to be used in the draw
phase. It is invalid to query attribute values from Maya nodes during the draw
callback and doing so may result in instability.

The method excludedFromPostEffects() allows for the plug-in to indicate
whether to contribute to passes which are required for post effects. If so, 
the plug-in's draw() may be called multiple times, once for each pass. 
Note that even without overriding this method that the plug-in's draw can be called 
more than once (e.g. for a shadow map pass).

The method isTransparent() allows for the plug-in to indicate
whether the object should be drawn as transparency. If so,
the plug-in's draw() will be called two times, one for front-culling pass and 
one for back-culling pass. It will never be called into weighted average or depth peeling.
Note that without overriding this method or make the method return false means 
the object will only be drawn once in opaque pass.

addUIDrawables() provides access to the MUIDrawManager, which can be used
to queue up operations for drawing simple UI elements such as lines, circles and
text. It is called just after prepareForDraw() and carries the same restrictions on
the sorts of operations it can perform. To enable addUIDrawables(), override
hasUIDrawables() and make it return true. UI elements are not affected
by post effects, and hence are not affected by the excludedFromPostEffects()
return value.


At draw time, the user defined callback will be invoked at which point any
custom OpenGL drawing may occur. Data needed from the Maya dependency graph
must have been cached in the prepareForDraw() stage as it is invalid to query
such data during the draw callback.

Implementations of MPxDrawOverride must be registered with Maya through
MDrawRegistry. If the registration contains "/includePostEffects"
this will override the value returned from excludedFromPostEffects().
If the registration contains "/isTransparent"
this will override the value returned from isTransparent().
For example, a classification starts with "/drawdb/geometry/testNode/includePostEffects/isTransparent"
means testNode should contribute to passes required for post effect 
and it is a transparent object for draw.

When using the MHWRender::MRenderer::setGeometryDrawDirty() interface to mark a
DAG object associated with an MPxDrawOverride object dirty, the optional 
topologyChanged parameter must be set to true.
*/
class OPENMAYARENDER_EXPORT MPxDrawOverride
{
public:
	//! User draw callback definition, draw context and blind user data are parameters
	typedef void (*GeometryDrawOverrideCb)(const MDrawContext&, const MUserData*);

	MPxDrawOverride(
		const MObject& obj,
		GeometryDrawOverrideCb callback,
		bool isAlwaysDirty = true);

	virtual ~MPxDrawOverride();

	virtual ::MHWRender::DrawAPI supportedDrawAPIs() const;
	virtual bool hasUIDrawables() const;

	virtual MMatrix transform(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const;

	virtual MBoundingBox boundingBox(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const;

	virtual bool isBounded(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const;

	virtual bool disableInternalBoundingBoxDraw() const;
	virtual bool excludedFromPostEffects() const;
	virtual bool isTransparent() const;

	virtual MUserData* prepareForDraw(
		const MDagPath& objPath,
		const MDagPath& cameraPath,
		const MFrameContext& frameContext,
		MUserData* oldData) = 0;

	virtual void addUIDrawables(
		const MDagPath& objPath,
		MUIDrawManager& drawManager,
		const MFrameContext& frameContext,
		const MUserData* data);

	virtual bool refineSelectionPath(const MSelectionInfo& selectInfo,
									 const MRenderItem& hitItem, 
									 MDagPath& path,
									 MObject& geomComponents,
									 MSelectionMask& objectMask);

	virtual void updateSelectionGranularity(
		const MDagPath& path,
		MSelectionContext& selectionContext);

	// Call trace methods
	virtual bool traceCallSequence() const;
	virtual void handleTraceMessage( const MString &message ) const;

	// Customized selection behavior
	virtual bool wantUserSelection() const;
	virtual bool userSelect(
		const MSelectionInfo& selectInfo,
		const MDrawContext& context,
		const MDagPath& objPath,
		const MUserData* data,
		MSelectionList& selectionList,
		MPointArray& worldSpaceHitPts);

	OPENMAYA_DEPRECATED(2019, "Use the other userSelect() method instead.")
	virtual bool userSelect(
		MSelectionInfo& selectInfo,
		const MDrawContext& context,
		MPoint& hitPoint,
		const MUserData* data);

	static const char* className();

protected:
	MStatus setGeometryForRenderItem(
		MRenderItem& renderItem,
		const MVertexBufferArray& vertexBuffers,
		const MIndexBuffer* indexBuffer,
		const MBoundingBox* objectBox) const;

	static MStatus drawRenderItem(
		const MHWRender::MDrawContext& context, MRenderItem& item);

	static bool pointSnappingActive();

private:
	GeometryDrawOverrideCb fCallback;
	bool fIsAlwaysDirty;
	 
};

} // namespace MHWRender
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxDrawOverride */
