#ifndef __MPxUIControl_h
#define __MPxUIControl_h
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
// CLASS:    MPxUIControl
//
// ****************************************************************************

#if defined __cplusplus

#include <maya/MApiNamespace.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxUIControl)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for control creation
/*!
MPxUIControl is the base class for user defined UI.  This class should
never be extended.  Extend the derived classes of this class.
*/
class OPENMAYAUI_EXPORT MPxUIControl
{
public:
	virtual ~MPxUIControl		();

	static const char*	className();

protected:
	MPxUIControl				();
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* _cplusplus */
#endif /* MPxUIControl */
