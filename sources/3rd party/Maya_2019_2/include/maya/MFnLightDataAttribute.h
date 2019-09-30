#ifndef _MFnLightDataAttribute
#define _MFnLightDataAttribute
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
// CLASS:    MFnLightDataAttribute
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnLightDataAttribute)

//! \ingroup OpenMaya MFn
//! \brief Light data attribute function set.
/*!

  Function object for LightData attributes.  A LightData attribute
  describes a single light source, giving its direction and intensity,
  as well as specifying whether or not it contributes to the ambient,
  diffuse, and specular components of the shading model.
*/
class OPENMAYA_EXPORT MFnLightDataAttribute : public MFnAttribute
{
	declareMFn(MFnLightDataAttribute, MFnAttribute);

public:
	MObject    create( const MString& fullName,
					   const MString& briefName,
					   const MObject & direction,
					   const MObject & intensity,
					   const MObject & ambient,
					   const MObject & diffuse,
					   const MObject & specular,
					   const MObject & shadowFraciton,
					   const MObject & preShadowIntensity,
					   const MObject & blindData,
					   MStatus* ReturnStatus = NULL );

	MStatus     getDefault( float & defDirectionX,
							float & defDirectionY,
							float & defDirectionZ,
							float & defIntensityR,
							float & defIntensityG,
							float & defIntensityB,
							bool  & defAmbient,
							bool  & defDiffuse,
							bool  & defSpecular,
							float & defShadowFraction,
							float & defPreShadowIntensity,
							void* & defBlindData);

	MStatus     setDefault( float defDirectionX,
							float defDirectionY,
							float defDirectionZ,
							float defIntensityR,
							float defIntensityG,
							float defIntensityB,
							bool  defAmbient,
							bool  defDiffuse,
							bool  defSpecular,
							float defShadowFraction,
							float defPreShadowIntensity,
							void* defBlindData);

	MObject     child( unsigned int index, MStatus* returnStatus );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnLightDataAttribute, MFnAttribute );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnLightDataAttribute */



