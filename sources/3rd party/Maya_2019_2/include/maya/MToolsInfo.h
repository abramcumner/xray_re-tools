#ifndef _MToolsInfo
#define _MToolsInfo
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
// CLASS:    MToolsInfo
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// CLASS DECLARATION (MToolsInfo)

//! \ingroup OpenMayaUI
//! \brief Tool information
/*!
  MToolsInfo is a caretaker class used to keep track of the state of
  the current tool property sheet. The tool writer should make sure
  to call the setDirtyFlag method when any of the values are modified.
  The dirty flag is used to indicate that the UI needs to be updated
  when the value of a tool property sheet option has changed.
*/
class OPENMAYAUI_EXPORT MToolsInfo {
public:
	static void		setDirtyFlag(const MPxContext &context);
	static void		resetDirtyFlag();
	static bool		isDirty();

	static const char *className();
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MToolsInfo */
