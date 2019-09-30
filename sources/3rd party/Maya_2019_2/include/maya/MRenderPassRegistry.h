#ifndef _MRenderPassRegistry
#define _MRenderPassRegistry

//
//-
// ===========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MRenderPassRegistry
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MString.h>
#include <maya/MStatus.h>
#include <maya/MRenderPassDef.h>
#include <maya/MPxRenderPassImpl.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRenderPassRegistry)

//! \ingroup OpenMayaRender
//! \brief Access the render pass registry
/*!
  This class provides a means to register custom render pass definitions with
  Maya's internal render pass registry. It can also be used to query pass
  definitions.
*/

class OPENMAYA_EXPORT MRenderPassRegistry {
public:
	static MRenderPassDef getRenderPassDefinition(const MString& passID,
									MStatus* ReturnStatus=NULL);

	static MRenderPassDef registerRenderPassDefinition(
									const MString &passID,
									const MString &passName,
									const MString &passGroup,
									const MString &description,
									bool overload=false,
									MStatus* ReturnStatus=NULL);

	static const char* className();

private:
	MRenderPassRegistry();
	~MRenderPassRegistry();
};

// ****************************************************************************

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderPassRegistry */

