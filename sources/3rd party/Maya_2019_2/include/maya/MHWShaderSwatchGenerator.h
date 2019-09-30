#ifndef _MHWShaderSwatchGenerator
#define _MHWShaderSwatchGenerator
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
// CLASS:    MHWShaderSwatchGenerator
//
// ****************************************************************************

#if defined __cplusplus

// INCLUDED HEADER FILES


#include <maya/MSwatchRenderBase.h>
#include <maya/MSwatchRenderRegister.h>
#include <maya/MSwatchRenderBase.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// CLASS DECLARATION (MHWShaderSwatchGenerator)

//! \ingroup OpenMayaUI
//! \brief Hardware shader swatch generator utility class 
/*!
  Derived from the MSwatchRenderBase class as a utility for generating
  a swatch for a plugin hardware shader class. This class supports
  hardware shaders derived from both MPxHardwareShader and the older
  MPxHwShaderNode base classes.
*/
class OPENMAYAUI_EXPORT MHWShaderSwatchGenerator : public MSwatchRenderBase
{
public:
	MHWShaderSwatchGenerator( MObject obj, MObject renderObj, int res );

	static MSwatchRenderBase* createObj(MObject obj, MObject renderObj, int res);

	static const MString& initialize();

	static void getSwatchBackgroundColor(float& r, float& g, float& b, float& a);

	virtual bool doIteration();

protected:
// No protected members

private:
// No private members
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MHWShaderSwatchGenerator */


