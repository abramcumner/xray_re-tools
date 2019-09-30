#ifndef _MFnPfxGeometry
#define _MFnPfxGeometry
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
// CLASS:    MFnPfxGeometry
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MFnDagNode.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnPfxGeometry)

//! \ingroup OpenMayaFX MFn
//! \brief PfxGeometry node function set
/*!
  This is the function set for paint effects objects.

  PfxGeometry is the parent class for the stroke and pfxHair node.
  The output geometry for pfxHair and stroke nodes may be accessed
  through this class.
*/
class OPENMAYAFX_EXPORT MFnPfxGeometry : public MFnDagNode
{
	declareDagMFn(MFnPfxGeometry, MFnDagNode);

	MStatus getLineData(
			MRenderLineArray &mainLines,
			MRenderLineArray &leafLines,
			MRenderLineArray &flowerLines,
			bool doLines, bool doTwist,
			bool doWidth, bool doFlatness,
			bool doParameter, bool doColor,
			bool doIncandescence, bool doTransparency,
			bool worldSpace );

	MStatus getBoundingBox( double* min, double* max );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnPfxGeometry, MFnDagNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnPfxGeometry */
