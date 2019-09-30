#ifndef _MPxRenderer
#define _MPxRenderer
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+

#if defined __cplusplus


#include <maya/MObject.h>
#include <maya/MString.h>
#include <maya/MMatrix.h>
#include <maya/MUuid.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxRenderer)

//! \ingroup OpenMayaRender
//! \brief Base class for plugin renderers
/*!
A base class providing an interface through which a plugin can implement a renderer
to provide rendered images of a scene.

LIMITATION: Currently this class is only used for rendering into the Material Viewer in the Hypershade editor.
For renderer integration into the Render View, please refer to the API class MRenderView and the MEL 
command "renderer". For rendering of shader swatches, please refer to the API class MSwatchRenderBase.

The derived class needs to be registered and deregistered with Maya using calls to 
MFnPlugin::registerRenderer() and MFnPlugin::deregisterRenderer().

Translation of the scene data to render is done by Maya calling the corresponding translate and set methods.
The translate methods (translateMesh(), translateLightSource(), translateCamera(), translateTransform(), translateShader(), translateEnvironment)
should implement translation of a Maya object to a corresponding renderer specific object. 
When Maya calls translate or set methods it is always done in a thread-safe context. This means that the Maya handle
sent in (and associated data) is valid and thread-safe for the duration of the call, but not guaranteed to be 
valid or thread-safe after the call, so the renderer should not save the Maya handle. If it needs to save the 
handle, it should do so using an MObjectHandle and never save the MObject directly. If the handle is saved it is up 
to the renderer to make sure the data is accessed in a thread-safe way.

When Maya translates a shape object (mesh, light, camera), the call to the translate method is always 
followed by one or more calls to translateTransform(), which instantiates the shape and assigns it as a
child to one or more transform nodes.

Calls to all translate and set methods are always preceded by a call to beginSceneUpdate(). This is to let 
the renderer know that scene updates are about to happen, to let it cancel the current rendering or otherwise 
prepare for scene updates. The update is then followed by a call to endSceneUpdate(), to let the renderer know 
that no more updates are comming (until next beginSceneUpdate()). The renderer can then do any scene data 
post-processing and restart the rendering.

Calls to translate and set methods can occur before a render job is started, for initial scene translation. But it 
can also occur during rendering for scene changes. So the renderer must support these methods being called 
at any time. However they are always encapsulated by calls to beginSceneUpdate() and endSceneUpdate().

The translate methods are also used to update already existing objects. So if a translate method is called with an
uuid pointing to an already translated object, that means the object should be updated.

A render session is started by Maya calling startAsync(). This call should start a render control thread running 
asynchronously from Maya's main thread. The control thread is in turn allowed to spawn as many render worker threads 
as given by the JobParams sent in.
	
During rendering the refresh() method should be called to send new image data back to Maya. The progress() method 
could be called to report the progress of the current image.

While the job is running it is up to the renderer to decide when the rendered image is finished. When finished 
the render worker threads should terminate or sleep, and not cause unnecessary CPU usage. If scene updates occur 
the control thread should restart the rendering to produce a new image with the new scene data. The job should be 
running in this manner until a call to stopAsync() is received. At that point both worker threads and the control 
thread should terminate.

A call to destroyScene() should clear out the scene and destroy all allocated scene data.

The steps below outlines the events for a render job session and which calls Maya sends to the renderer. A scene update 
consists of a block of calls, named a Scene Update Block below:

beginSceneUpdate()
	translateMesh()
	translateTransform()
	setProperty()
	setShader()
	...
endSceneUpdate()

1. Maya will start the render session with 0 or more Scene Update Blocks. The renderer should use them to build its internal representation for the scene graph.
2. Maya will call startAsync(). The renderer should start a separate thread.
3. The renderer should start rendering the internal scene graph that it now has, in the separate thread or by starting other worker threads.
4. The renderer should call progress() to indicate to Maya how far along it is. A value between 0.0 and 1.0 indicates the rendering is in progress. A value of 1.0 or higher indicates the rendering is completed. A negative value indicates the task is done and scene graph destroyed (see point 12).
5. The renderer should call refresh() during rendering to present successive image results that Maya can display.
6. When the rendering is completed the renderer should drop into a waiting state. Notification of this is done by calling progress() with a value of 1.0 or higher.
7. Maya may modify the scene graph through additional Scene Update Blocks. If these occur while the renderer is in its waiting state then it must start up a new render. If these occur while the renderer is already rendering, it must incorporate the changes into the current render, typically by restarting it.
8. Steps 3 through 7 may be repeated multiple times.
9. Maya calls stopAsync(). The renderer should terminate its separate threads but retains its internal scene graph.
10. Steps 1 through 9 may be repeated to produce renderings of variations of the same scene.
11. Maya calls destroyScene(). The renderer should destroy its internal scene graph.
12. The renderer should then finish by calling progress() with a negative value, to let Maya know the scene graph is destroyed and the renderer is shut down.
*/
class OPENMAYARENDER_EXPORT MPxRenderer
{
public:
	//! Identifier for environment types
	enum EnvironmentType
	{
		IBL_ENVIRONMENT, //!< Environment using an HDR image file (Image Based Lighting).
	};

	//! Identifier for job types
	enum JobType
	{
		SWATCH_RENDER_JOB, //!< Rendering of shader swatch
	};

	//! Parameters for starting a new job
	class JobParams
	{
	public:
		JobType type;            //!< Type of job to start
		MString description;     //!< Job description
		unsigned int maxThreads; //!< Maximum number of threads allowed for the job
		unsigned int priority;   //!< Hint of the priority for this job
		MUuid cameraId;          //!< Id of camera transform node, for the render camera.
	};

	//! Parameters for doing refresh of the view when new image data is available
	class RefreshParams
	{
	public:
		unsigned int width;           //!< Width of the image frame
		unsigned int height;          //!< Height of the image frame
		unsigned int left;            //!< Left edge of the image tile
		unsigned int right;           //!< Right edge of the image tile
		unsigned int bottom;          //!< Bottom edge of the image tile
		unsigned int top;             //!< Top edge of the image tile
		unsigned int channels;        //!< Number of channels in the image data
		unsigned int bytesPerChannel; //!< Size of each channel in bytes
		void* data;                   //!< The image data
	};

	//! Parameters for reporting progress of a task
	class ProgressParams
	{
	public:
		MString description; //!< Description of current task
		float progress;      //!< Progess of current task. See progress() method for more information.
	};

public:
	virtual	~MPxRenderer();

	virtual MStatus startAsync(const JobParams& params);
	virtual MStatus stopAsync();
	virtual bool isRunningAsync();

	virtual MStatus beginSceneUpdate();

	virtual MStatus translateMesh(const MUuid& id, const MObject& node);
	virtual MStatus translateLightSource(const MUuid& id, const MObject& node);
	virtual MStatus translateCamera(const MUuid& id, const MObject& node);
	virtual MStatus translateEnvironment(const MUuid& id, EnvironmentType type);
	virtual MStatus translateTransform(const MUuid& id, const MUuid& childId, const MMatrix& matrix);
	virtual MStatus translateShader(const MUuid& id, const MObject& node);

	virtual MStatus setProperty(const MUuid& id, const MString& name, bool value);
	virtual MStatus setProperty(const MUuid& id, const MString& name, int value);
	virtual MStatus setProperty(const MUuid& id, const MString& name, float value);
	virtual MStatus setProperty(const MUuid& id, const MString& name, const MString& value);

	virtual MStatus setShader(const MUuid& id, const MUuid& shaderId);
	virtual MStatus setResolution(unsigned int width, unsigned int height);

	virtual MStatus endSceneUpdate();

	virtual MStatus destroyScene();

	virtual bool isSafeToUnload() = 0;

protected:
	MPxRenderer();

	void refresh(const RefreshParams& params);
	void progress(const ProgressParams& params);

	void* instance;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxRenderer */
