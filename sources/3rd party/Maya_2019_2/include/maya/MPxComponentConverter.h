#ifndef _MPxComponentConverter
#define _MPxComponentConverter
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

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MApiNamespace.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// CLASS DECLARATION (MPxComponentConverter)
//! \ingroup OpenMayaRender MPx
//! \brief Base class for user defined component converter.
/*!

Implementations of MPxComponentConverter must be registered with Maya through
MDrawRegistry.

MPxComponentConverters are registered with a unique render item name.  
A registered MPxComponentConverter will be used to perform custom viewport 2.0 selection.

They are used to convert selection intersection data to a list of object components (MFnComponent),
by matching the index buffer positions (MIntersection::index()) to valid component ids.

The same converter can be registered against different render item name,
meaning that different render items can share the same component converter.
*/
class OPENMAYARENDER_EXPORT MPxComponentConverter
{
public:
	MPxComponentConverter();
	virtual ~MPxComponentConverter();

	virtual void initialize(const MRenderItem& renderItem) = 0;
	virtual void addIntersection(MIntersection& intersection) = 0;
	virtual MObject component() = 0;
	virtual MSelectionMask selectionMask() const = 0;

	static const char* className();
}; // MPxComponentConverter


} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxComponentConverter */

