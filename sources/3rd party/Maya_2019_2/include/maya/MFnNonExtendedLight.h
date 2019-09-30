#ifndef _MFnNonExtendedLight
#define _MFnNonExtendedLight
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
// CLASS:    MFnNonExtendedLight
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnNonAmbientLight.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnNonExtendedLight)

//! \ingroup OpenMaya MFn
//! \brief Manage Non-extended Light dependency Nodes.
/*!
 MFnNonExtendedLight facilitates manipulation of dependency graph nodes
 representing non-extended lights.  This includes point lights, directional
 lights, and spotlights.
*/
class OPENMAYA_EXPORT MFnNonExtendedLight : public MFnNonAmbientLight
{
	declareDagMFn(MFnNonExtendedLight,MFnNonAmbientLight);
public:
	float         shadowRadius( MStatus * ReturnStatus = NULL ) const;
	MStatus       setShadowRadius( const float& shadow_radius );
	bool          castSoftShadows( MStatus * ReturnStatus = NULL ) const;
	MStatus       setCastSoftShadows( const bool& cast_soft_shadows );
	bool          useDepthMapShadows( MStatus * ReturnStatus = NULL ) const;
	MStatus       setUseDepthMapShadows( const bool& use_depth_map );
	short         depthMapFilterSize( MStatus * ReturnStatus ) const;
	MStatus       setDepthMapFilterSize( const short& depth_map_filter_size );
	short         depthMapResolution( MStatus * ReturnStatus ) const;
	MStatus       setDepthMapResolution( const short& depth_map_resolution );
	float         depthMapBias( MStatus * ReturnStatus ) const;
	MStatus       setDepthMapBias( const float& depth_map_bias );
	bool          useDepthMapAutoFocus( MStatus * ReturnStatus = NULL ) const;
	MStatus       setUseDepthMapAutoFocus( const bool use_depth_map_auto_focus );
	float         depthMapWidthFocus( MStatus * ReturnStatus ) const;
	MStatus       setDepthMapWidthFocus( const float depth_map_width_focus );
	float         depthMapFocus( MStatus * ReturnStatus ) const;
	MStatus       setDepthMapFocus( const float depth_map_focus );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnNonExtendedLight,MFnNonAmbientLight );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnNonExtendedLight */



