#ifndef _MFnLight
#define _MFnLight
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
// CLASS:    MFnLight
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnLight)

//! \ingroup OpenMaya MFn
//! \brief Manage dependency graph nodes representing lights. 
/*!
 MFnLight allows the creation and manipulation of dependency graph nodes 
 representing lights.  This is the top level of a hierarchy of light 
 node function sets.  It permits manipulation of the attributes common to
 all types of lights.  
*/
class OPENMAYA_EXPORT MFnLight : public MFnDagNode
{
	declareDagMFn(MFnLight,MFnDagNode);

public:
	MColor       color( MStatus * ReturnStatus = NULL ) const;
	MStatus      setColor( const MColor &col );
	float        intensity( MStatus * ReturnStatus = NULL ) const;
	MStatus      setIntensity( const float& intens );
	bool         useRayTraceShadows( MStatus * ReturnStatus = NULL ) const;
	MStatus      setUseRayTraceShadows( const bool& useRayTraceShadows );
	MColor       shadowColor( MStatus * ReturnStatus = NULL ) const;
    MStatus      setShadowColor( const MColor& shadow_color );
	double       centerOfIllumination( MStatus * ReturnStatus = NULL ) const;
	MStatus      setCenterOfIllumination( const double& dist );
	short        numShadowSamples( MStatus * ReturnStatus = NULL ) const;
	MStatus      setNumShadowSamples( const short& num_shadow_samples );
	short        rayDepthLimit( MStatus * ReturnStatus = NULL ) const;
	MStatus      setRayDepthLimit( const short& rayDepthLimit );
    MColor       opticalFXvisibility( MStatus * ReturnStatus = NULL ) const;
	MStatus      setOpticalFXvisibility( const MColor& visibility );
	MColor       lightIntensity( MStatus * ReturnStatus = NULL ) const;
	MFloatVector lightDirection( int instance, MSpace::Space space = MSpace::kWorld, MStatus * ReturnStatus = NULL ) const;
	MFloatVector lightDirection( MStatus * ReturnStatus = NULL ) const;
	bool         lightAmbient( MStatus * ReturnStatus = NULL ) const;
	bool         lightDiffuse( MStatus * ReturnStatus = NULL ) const;
	bool         lightSpecular( MStatus * ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnLight,MFnDagNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnLight */
