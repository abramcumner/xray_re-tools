#ifndef _MFnAnisotropyShader
#define _MFnAnisotropyShader
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
// CLASS:    MFnAnisotropyShader
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
// CLASS DECLARATION (MFnAnisotropyShader)

//! \ingroup OpenMaya MFn
//! \brief Manage anisotropic shaders.
/*!
 MFnAnisotropyShader facilitates creation and manipulation of dependency graph
 nodes representing anisotropy shaders.
*/
class OPENMAYA_EXPORT MFnAnisotropyShader : public MFnDependencyNode
{
	declareMFn( MFnAnisotropyShader, MFnDependencyNode );

public:
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	MColor      tangentUCamera( MStatus * ReturnStatus = NULL ) const;
	MStatus     setTangentUCamera( const MColor & tangu_camera );
	MColor      tangentVCamera( MStatus * ReturnStatus = NULL ) const;
	MStatus     setTangentVCamera( const MColor & tangv_camera );
	float      correlationX( MStatus * ReturnStatus = NULL ) const;
	MStatus     setCorrelationX( const float &correl_x );
	float      correlationY( MStatus * ReturnStatus = NULL ) const;
	MStatus     setCorrelationY( const float &correl_y );
	float       roughness( MStatus * ReturnStatus = NULL ) const;
	MStatus     setRoughness( const float &roughness );
	float       rotateAngle( MStatus * ReturnStatus = NULL ) const;
	MStatus     setRotateAngle( const float& rotate_angle );
	float       refractiveIndex( MStatus * ReturnStatus = NULL ) const;
	MStatus     setRefractiveIndex( const float& refractive_index );
	bool       anisotropicReflectivity( MStatus * ReturnStatus = NULL ) const;
	MStatus     setAnisotropicReflectivity( const bool& anisotropic_reflectivity );

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	declareMFnConstConstructor( MFnAnisotropyShader, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnAnisotropyShader */
