#ifndef _MRenderCallback
#define _MRenderCallback
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
// CLASS:    MRenderCallback
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MMessage.h>
#include <maya/MTypes.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMayaRender
//! \brief Rendering Callbacks
/*!
This class is used to register callbacks to gain access to Maya's
rendering information during software rendering.  You can modify Maya's
shadow maps, RGB pixmap, and depth map to composite your own rendering
effects into Maya's rendering.

To register callbacks, inherit from this class and override
renderCallback(), shadowCastCallback(), or postProcessCallback(). Any
number of these methods can be overridden by the callback. Then
register the callbacks by calling the addCallback() method.

Each of the callback method gets passed a MRenderData which contains
the information.  MRenderData also provides utility methods for
converting between world space and screen space.  In the case of
shadowCastCallback, MRenderShadowData is provided and it also has
utility methods for converting between world space and shadow map
space.

If there are callbacks registered, prior to shadow maps being written out,
shadowCastCallback() will be invoked with light information and a pointer
to the shadow map passed in. Then immediately after software rendering
completes, renderCallback() will be invoked with the rendering's dimension
info and image passed in.  Lastly, during post-processing, postProcessCallback()
will be invoked with the rendering's dimension and pointers to the rgb pixmap
and depthmap passed in.

If multiple callbacks need to be registered, the order of invocation
can be set by adding each callback with a priority number, 0 being the
highest priority.
*/
class OPENMAYARENDER_EXPORT MRenderCallback {

public:
                    MRenderCallback();
	virtual			~MRenderCallback();

    // Methods to override, return true on success
    virtual bool    shadowCastCallback(const MRenderShadowData& data);
    virtual bool    renderCallback(const MRenderData& data);
    virtual bool    postProcessCallback(const MRenderData& data);

    static void     addCallback(MRenderCallback*, int priority = 0);
    static void     removeCallback(MRenderCallback*);

    static MCallbackId     addRenderTileCallback( MMessage::MRenderTileFunction func, MStatus* ReturnStatus );

    static const char* 		className();

private:
    const void*     internalData;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderCallback */

