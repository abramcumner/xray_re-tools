#ifndef _MFnReflectShader
#define _MFnReflectShader
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
// CLASS:    MFnReflectShader
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnLambertShader.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS


// ****************************************************************************
// CLASS DECLARATION (MFnReflectShader)

//! \ingroup OpenMaya MFn
//! \brief Manage Reflective Surface Shaders. 
/*!
 MFnReflectShader allows manipulation of dependency graph nodes representing
 reflective surface shaders.  This includes Phong and Blinn shaders.
*/
class OPENMAYA_EXPORT MFnReflectShader : public MFnLambertShader
{
	declareMFn( MFnReflectShader, MFn::kReflect );

public:
	short       reflectedRayDepthLimit( MStatus * ReturnStatus = NULL ) const;
	MStatus     setReflectedRayDepthLimit( const short& new_limit );
	MColor      specularColor( MStatus * ReturnStatus = NULL ) const;
	MStatus     setSpecularColor( const MColor& specular_color );
	float       reflectivity( MStatus * ReturnStatus = NULL ) const;
	MStatus     setReflectivity( const float& reflectivity );
	MColor      reflectedColor( MStatus * ReturnStatus = NULL ) const;
	MStatus     setReflectedColor( const MColor& reflected_color );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnReflectShader, MFn::kReflect );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnReflectShader */
