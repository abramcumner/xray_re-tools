#ifndef _MProgressWindow
#define _MProgressWindow
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
// CLASS:    MProgressWindow
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES



#include <maya/MStatus.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MProgressWindow)

//! \ingroup OpenMayaUI
//! \brief Create and manipulate progress windows 
/*!
	The MProgressWindow class manages a window containing a status message, a
	graphical progress gauge, and optionally a "Hit ESC to Cancel" label for
	interruptable operations.

	Only a single progress window may be displayed at any time.  To reserve the
	use of the progress window, use the reserve() method in this class.  Any
	methods that change the state of the progress window will fail unless the
	progress window has first been successfully reserved.

	The startProgress() and endProgress() functions show and hide the progress
	window.  endProgress() also has the effect of unreserving the progress
	window, allowing it to be reserved for another use.

	The MEL command "progressWindow" provides equivalent functionality to this
	class.  Attempting to manipulate a progress window that is in use by MEL
	will cause the methods in this class to fail.
*/
class OPENMAYAUI_EXPORT MProgressWindow
{
public:

	static bool reserve();
	static MStatus startProgress();
	static MStatus endProgress();

	static MStatus setProgressRange(const int minValue, const int maxValue);
	static MStatus setProgressMin(const int minValue);
	static MStatus setProgressMax(const int maxValue);

	static int progressMin(MStatus* ReturnStatus = NULL);
	static int progressMax(MStatus* ReturnStatus = NULL);

	static MStatus setProgress(const int progress);
	static MStatus advanceProgress(const int amount);
	static int progress(MStatus* ReturnStatus = NULL);

	static MStatus setTitle(const MString &title);
	static MString title(MStatus* ReturnStatus = NULL);

	static MStatus setProgressStatus(const MString &progressStatus);
	static MString progressStatus(MStatus* ReturnStatus = NULL);

	static MStatus setInterruptable(const bool value);
	static bool isInterruptable(MStatus* ReturnStatus = NULL);
	static bool isCancelled(MStatus* ReturnStatus = NULL);

	static const char* 		className();

protected:
	// No protected members
private:
	static bool reserved;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MProgressWindow */
