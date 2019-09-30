#ifndef _MPxShaderOverride
#define _MPxShaderOverride
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
// CLASS: MPxShaderOverride
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MPxShaderOverride)
//
//  MPxShaderOverride allows the user to create a custom draw override
//  for associating a full shading effect with a shading node (custom or
//  standard) in Maya. Its primary use is for associating hardware effects
//  with pre-existing plugin software shaders.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MHWGeometry.h>
#include <maya/MDagPath.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// CLASS DECLARATION (MPxShaderOverride)

//! \ingroup OpenMayaRender MPx
//! \brief Base class for user defined shading effect draw overrides.
/*!
MPxShaderOverride allows the user to create a custom override for
associating a "full shading effect" with a shading node (custom or standard)
in Maya. Its primary use is for associating hardware effects with pre-existing
plugin software shaders.

A "full shading effect" defines the complete shading and lighting involved
to render a given object. Input resources for shading such as geometry,
textures, and lights are defined and bound to the shading effect via the
override as required. The override is fully responsible for these tasks.

As an example, for hardware shading, this can be thought of as implementing
a CgFx or HLSL effect file renderer which can use the resources defined within
a Maya scene.

There are three main phases that the override must implement:

1) Initialization Phase : This phase occurs when Maya determines that the
hardware shader generated through this override needs to be rebuilt. This may
be due to a new object using the shader, or because something has changed on
the shader that requires a complete rebuild. For example a value change with
rebuildAlways() returning true will require a complete rebuild. 
During the initialization phase the
geometric stream requirements can be specified. If an MShaderInstance is
used as the shader for rendering then setGeometryRequirements() should be
called to extract the requirements from the MShaderInstance.
Otherwise, addGeometryRequirement() or addGeometryRequirements() should be used.
The requirements will determine which geometric streams are required from
objects to which the given shading effect is assigned. If no requirements are
specified then a single position stream requirement will be used.

The initialize() method must return a string representing the shader
key.  It often happens that different instances of the
MPxShaderOverride represent essentially the same shader, but with
different shader parameters. The shader key is used to identify the
MPxShaderOverride instances representing the same shader. To optimize
rendering, the Viewport 2.0 will make an effort to regroup the
rendering of objects with the same MPxShaderOverride shader key. This
allows the plug-in to perform its setup only once for the entire
sequence. It is up to each plug-in to decide what the meaning of
representing the same shader is.

There are two possible versions of initialize() to implement (however it is
only necessary to implement one, the default implementation of the second
calls the first). The first simply receives the shader node that is being
initialized. The second receives additional context (such as the DAG path
for the object bound to the shader) and also allows the implementation to
send back custom user data to attach to the MRenderItem representing the
DAG-object/shader association. The custom user data must be derived from
MUserData. If the deleteAfterUse() method returns true for the MUserData
instance then the data object will automatically be deleted when the render
item is deleted. Otherwise the lifetime of the user data object is the
responsibility of the caller. This user data may be accessed in the draw phase
from the render item being drawn. Note that Maya objects which have render
items that have custom user data can only be consolidated when the user data
pointer on each item references the same user data instance.  Draw performance
will suffer without consolidation, however if the user data object is shared
then memory management must be handled manually (ie. deleteAfterUse() should
return false).

During initialization, if the current display mode is non-textured
display mode, an internally defined static shader instance is used for all
render items which use the shading node associated with a given shader override. 
This is a performance optimization to avoid any additional node monitoring as
well as allow for render items which share this shader instance to be
consolidated.

If it is desirable to override this behavior, the nonTexturedShaderInstance() method can be
overridden to return a custom shader instance to be specified
instead of the default shared instance. If this shader instance needs to be
updated on node attribute changes, then a return parameter can be set to indicate this.
If no monitoring is required Maya will attempt to skip the
update phase while in non-textured mode. It is still possible that update is
required for the shader which used for textured mode display. For example if rebuildAlways()
returns true then the update phase would be called regardless of the options set in this method.

The handling of properties related to the non-textured shader instance 
such as transparency are not directly tied to the update phases isTransparent() method. 
That method is for the status of textured mode display shader. The
implementation can control transparency by directly calling
the MShaderInstance::setIsTransparent() with the appropriate value. If this
needs to be done at shader initialization time then the call should
be made within the nonTexturedShaderInstance() method before returning
the shader instance.

2) Data Update Phase : In this phase, updating of all data values required for
shading is performed. The interface has an explicit split of when the dependency
graph can be accessed (updateDG()), and when the draw API (e.g. OpenGL) can be
accessed (updateDevice()). Any intermediate data can be cleaned up when
endUpdate() is called.

As an example the override may require input from an attribute on a given node.
- updateDG() would evaluate the plug for the attribute and cache its value
temporarily on the override.
- updateDevice() would take the temporarily cached value and bind it to a
shader stored on the graphic device. 
- endUpdate() would reset the temporary data on the override.

If an MShaderInstance is being used, then the parameters on that instance
should be updated during updateDevice().

Note that the override can provide a draw hint as to whether shading will
involve semi-transparency. This hint can be provided by overriding the
isTransparent() method which gets called between updateDevice() and
endUpdate().

3) Drawing Phase : The actual drawing using the shader is performed in the
pure virtual draw() method. The callback method should return true if it was
able to draw successfully. If false is returned then drawing will be done
using the default shader used for unsupported materials.

Drawing is explicitly not intermixed with the data update on purpose. By the
time draw is being called all evaluation should have been completed. If there
is user data that needs to be passed between the update and drawing phases
the override must cache that data itself. It is an error to access the Maya
dependency graph during draw and attempts to do so may result in instability.

Although it is possible for implementations of the draw() method to handle all
shader setup and geometry draw, the expected use of the draw() method is to do
shader setup only. Then drawGeometry() can be called to allow Maya to handle
the actual geometry drawing. If manual geometry binding is required however, it
is possible to query the hardware resource handles through the geometry on each
render item in the render item list passed into the draw() method.

The handlesDraw() method will be invoked at the beginning of the draw phase
to allow the plugin to indicate whether it will handle drawing
based on the current draw context information. If this method
returns false then shaderInstance(), activateKey(), draw() and terminateKey() 
will not be called. The default implementation will return true if the
shader is being used for a color pass and there is no
per-frame shader override specified. An example of when
such a shader override is used is when all objects in the scene 
are set to draw with the "default material".

The activateKey() and terminateKey() method will also be invoked in
the draw phase each time a render item is drawn with a different
shader key (see the discussion above about shader keys). The
activateKey() and terminateKey() methods can be used to optimize
rendering by configuring the rendering state only once for a batch of
draw() calls that are sharing the same shader key.

The sequence of invocations will look like:
  - shaderOverrideA->activateKey(...)
  - shaderOverrideA->draw(...)
  - shaderOverrideB->draw(...)
  - shaderOverrideC->draw(...)
  - ...
  - shaderOverrideA->terminateKey(...)
  - shaderOverrideX->activateKey(...)

Note that the terminateKey() callback is always invoked on the same
MPxShaderOverride instance as the one used to invoked the
activateKey() callback.

If an MShaderInstance is being used for drawing, in order to take advatange
of the batch drawing optimization, the shaderInstance() method should return
the shader instance to use. The shader instance should then be bound 
during activateKey() and unbound during terminateKey().
An MShaderInstance can be bound and unbound from 
within the draw() method but will not be optimized for batch drawing.

The sequence using an MShaderInstance and the batch optimization will look like:
  - shaderOverrideA->shaderInstance(...)
  - shaderOverrideA->activateKey(...)
  - shaderOverrideA->draw(...)
  - shaderOverrideB->draw(...)
  - shaderOverrideC->draw(...)
  - ...
  - shaderOverrideA->terminateKey(...)
  - shaderOverrideX->shaderInstance(...)
  - shaderOverrideX->activateKey(...)

Note if full draw control is desired, the proxy class MPxDrawOverride may be
more appropriate.

Implementations of MPxShaderOverride must be registered with Maya through
MDrawRegistry.
*/
class OPENMAYARENDER_EXPORT MPxShaderOverride
{
public:
	MPxShaderOverride(const MObject& obj);
	virtual ~MPxShaderOverride();

	//! Initialization context used by advanced initalization method
	struct MInitContext
	{
		MObject shader; //! The Maya shading node this override is used for
		MDagPath dagPath; //! A path to the instance of the Maya DAG object for which the shader is being initialized
	};
	//! Data to pass back to Maya after initialization
	struct MInitFeedback
	{
		MUserData* customData; //! Optional user data to be associated with the render item for the shader assignment
	};

	// 1) Initialize phase
	virtual MString initialize(
		MObject shader);
	virtual MString initialize(
		const MInitContext& initContext,
		MInitFeedback& initFeedback);

	// 2) Update phase
	virtual void updateDG(MObject object);
	virtual void updateDevice();
	virtual void endUpdate();

	// 3) Draw phase
	virtual bool handlesDraw(MDrawContext& context);
    virtual void activateKey(MDrawContext& context, const MString& key);
	virtual bool draw(
		MDrawContext& context,
		const MRenderItemList& renderItemList) const = 0;
    virtual void terminateKey(MDrawContext& context, const MString& key);

	// Override properties
	virtual ::MHWRender::DrawAPI supportedDrawAPIs() const;
	virtual bool isTransparent();
	virtual bool supportsAdvancedTransparency() const;
	virtual bool overridesDrawState();
	virtual bool rebuildAlways();
	virtual double boundingBoxExtraScale() const;
	virtual bool overridesNonMaterialItems() const;
	virtual bool handlesConsolidatedGeometry() const;
	virtual bool requiresUnsharedGeometricStreams() const;
    virtual bool supportsMultiDraw() const;
	virtual MHWRender::MShaderInstance* shaderInstance(MDrawContext& context) const;
	virtual MHWRender::MShaderInstance* nonTexturedShaderInstance(bool &monitorNode) const;
	virtual MHWRender::MShaderInstance* multiDrawNonTexturedShaderInstance(bool &monitorNode) const;

	static const char* className();

protected:
	// Protected helper methods for use by derived classes
	MStatus addGeometryRequirement(const MVertexBufferDescriptor& desc);
	MStatus addGeometryRequirements(const MVertexBufferDescriptorList& list);
	MStatus setGeometryRequirements(const MShaderInstance &shaderInstance );
	MStatus addShaderSignature(void* signature, size_t signatureSize);
	MStatus addShaderSignature(const MShaderInstance &shaderInsance );
    MStatus addIndexingRequirement(const MIndexBufferDescriptor& desc);
	void drawGeometry(const MDrawContext& context) const;

private:
	// Internal data
	void setInternalData( void * data ) { fData = data; }
	void *fData;
	bool fInInit;


public:
	// Deprecated
    OPENMAYA_DEPRECATED(2019, "Use the activateKey(MDrawContext&, const MString&) version instead.")
	virtual void activateKey(MDrawContext& context);
	virtual void terminateKey(MDrawContext& context);
    OPENMAYA_DEPRECATED(2019, "Use the shaderInstance(MDrawContext&) version instead.")
	virtual MHWRender::MShaderInstance* shaderInstance() const;
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxShaderOverride */
