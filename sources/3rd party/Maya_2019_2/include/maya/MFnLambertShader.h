#ifndef _MFnLambertShader
#define _MFnLambertShader
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
// CLASS:    MFnLambertShader
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnDependencyNode.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS


// ****************************************************************************
// CLASS DECLARATION (MFnLambertShader)

//! \ingroup OpenMaya MFn
//! \brief Manage Lambert shaders.
/*!
 MFnLambertShader facilitates creation and manipulation of dependency graph
 nodes representing lambertian shaders.
*/ 
class OPENMAYA_EXPORT MFnLambertShader : public MFnDependencyNode
{
	declareMFn( MFnLambertShader, MFnDependencyNode );

public:
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	short       refractedRayDepthLimit( MStatus * ReturnStatus = NULL ) const;
	MStatus     setRefractedRayDepthLimit( const short& new_limit );
	float       refractiveIndex( MStatus * ReturnStatus = NULL ) const;
	MStatus     setRefractiveIndex( const float& refractive_index );
	bool        rtRefractedColor( MStatus * ReturnStatus = NULL ) const;
	MStatus     setRtRefractedColor( const bool& rt_refracted_color );
	float       diffuseCoeff( MStatus * ReturnStatus = NULL ) const;
	MStatus     setDiffuseCoeff( const float& diffuse_coeff );
	MColor      color( MStatus * ReturnStatus = NULL ) const;
	MStatus     setColor( const MColor & col );
	MColor      transparency( MStatus * ReturnStatus = NULL ) const;
	MStatus     setTransparency( const MColor & transp );
	MColor      ambientColor( MStatus * ReturnStatus = NULL ) const;
	MStatus     setAmbientColor( const MColor & ambient_color );
	MColor      incandescence( MStatus * ReturnStatus = NULL ) const;
	MStatus     setIncandescence( const MColor & incand );
	float       translucenceCoeff( MStatus * ReturnStatus = NULL ) const;
	MStatus     setTranslucenceCoeff( const float& translucence_coeff );
	float       glowIntensity( MStatus * ReturnStatus = NULL ) const;
	MStatus     setGlowIntensity( const float& glow_intensity );
	bool        hideSource( MStatus * ReturnStatus = NULL ) const;
	MStatus     setHideSource( const bool& hide_source );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnLambertShader, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnLambertShader */
