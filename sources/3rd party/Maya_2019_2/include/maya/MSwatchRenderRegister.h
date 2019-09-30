#ifndef _MSwatchRenderRegister
#define _MSwatchRenderRegister
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
// CLASS:    MSwatchRenderRegister
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

//! \brief Pointer to a function which returns a swatch generator object.
/*
 \param[in] dependNode Node for which the swatch is being generated.
 \param[in] renderNode Shader to be used in rendering the node's swatch.
 \param[in] imageResolution Desired resolution, in pixels, of the swatch.
*/
typedef MSwatchRenderBase* (*MSwatchRenderCreatorFnPtr) (MObject dependNode, MObject renderNode, int imageResolution);

OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMayaRender
//! \brief Manages swatch generators
/*!
  Provides an interface for plugins to register/unregister swatch
  generator classes (derived from MSwatchRenderBase) with Maya.
  Whenever a swatch needs to be generated for a node, it checks the
  classfication string of the node for the preferred swatch generator.
  If a match is found, it creates and manages the swatch generator
  object (that is it deletes the swatch generator object once the
  image has been generated). The doIteration function is called for
  generating the swatch.  The doIteration function is called
  repeatedly (during idle events) till it returns true. This allows
  for generation of the swatch in stages.
*/
class OPENMAYARENDER_EXPORT MSwatchRenderRegister {
public:
	//! registers a new swatch generator creation function by name.
	static MStatus	registerSwatchRender(MString swatchGenName, MSwatchRenderCreatorFnPtr fnPtr);

	//! removes the previously registered swatch generator
	static MStatus	unregisterSwatchRender(MString swatchGenName);

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MSwatchRenderRegister */

