#ifndef _MExternalDropCallback
#define _MExternalDropCallback
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
// CLASS:    MExternalDropCallback
//
// ****************************************************************************
//
#if defined __cplusplus

// ****************************************************************************


#include <maya/MStatus.h>
#include <maya/MTypes.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************

//! \ingroup OpenMayaUI
//! \brief Provides a callback hook into Maya's drag-and-drop mechanism.
/*!
This class is used to register callbacks to gain access to Maya's drag-and-drop information during dropping an external object to Maya. You can replace or augment Maya's drop behavior for external drag-and-drop operations. 

To register callbacks, inherit from this class and override externalDropCallback(). The method can be overridden by the callback. Then register the callbacks by calling the addCallback() method.

Each of the callback methods gets passed the name of the drop site control and an MExternalDropData instance which contains additional information provided by the system (for example, text or URL information associated with the drop).

If multiple callbacks need to be registered, the order of invocation can be set by adding each callback with a priority number, 0 being the highest priority.
Callbacks are then invoked in priority order. Note that the first callback that does not return kMayaDefault will cause the invocations to stop; lower priority callbacks will then not be invoked.
*/
class OPENMAYAUI_EXPORT MExternalDropCallback
{
public:
	//! Possible return values from externalDropCallback(), used to inform Maya of what further action to take, if any.
	enum MExternalDropStatus
	{
		//! Run Maya default action
		kMayaDefault = 0,
		//! Skip Maya default action and accept the drop
		kNoMayaDefaultAndAccept,
		//! Skip Maya default action and do not accept the drop
		kNoMayaDefaultAndNoAccept,
	};

					MExternalDropCallback();
	virtual			~MExternalDropCallback();

    virtual MExternalDropStatus	externalDropCallback( bool doDrop, const MString& controlName, const MExternalDropData& data ) = 0;

    static MStatus	addCallback( MExternalDropCallback*, int priority = 0 );
    static MStatus	removeCallback( MExternalDropCallback* );

protected:

private:
    void*	fInternalData;

	// prevent copying
	MExternalDropCallback( const MExternalDropCallback& );
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MExternalDropCallback */
