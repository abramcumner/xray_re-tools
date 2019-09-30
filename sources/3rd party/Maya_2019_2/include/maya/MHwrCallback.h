#ifndef _MHwrCallback
#define _MHwrCallback
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
// CLASS:    MHwrCallback
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MTypes.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMayaRender
//! \brief Rendering Callbacks
/*!
This class is used to register callbacks to gain access to Maya's
Hardware Rendering device status. You can be notified of device creation, lost
reset and deletion.

To register callbacks, inherit from this class and override
deviceNew, deviceLost, deviceReset, deviceDeleted. Any
number of these methods can be overridden by the callback. Then
register the callbacks by calling the addCallback() method.

If multiple callbacks need to be registered, the order of invocation
can be set by adding each callback with a priority number, 0 being the
highest priority.
*/
class OPENMAYARENDER_EXPORT MHwrCallback {
public:
                    MHwrCallback();
	virtual			~MHwrCallback();

    // Methods to override, return true on success
    virtual void    deviceNew();
    virtual void	deviceLost();
    virtual void    deviceReset();
	virtual void	deviceDeleted();

    static void     addCallback(MHwrCallback*, int priority = 0);
    static void     removeCallback(MHwrCallback*);

private:
    const void*     internalData;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MHwrCallback */

