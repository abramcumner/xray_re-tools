#ifndef _MFnVolumeLight
#define _MFnVolumeLight
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
// CLASS:    MFnVolumeLight
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnPointLight.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnVolumeLight)

//! \ingroup OpenMaya MFn
//! \brief Manage volume Light dependency Nodes. 
/*!
 MFnVolumeLight facilitates creation and manipulation of dependency graph nodes
 representing voume lights.
*/
class OPENMAYA_EXPORT MFnVolumeLight : public MFnPointLight
{

	declareDagMFn(MFnVolumeLight, MFnPointLight);
public:
	//! Available shapes for a volume light.
	enum MLightShape
	{
		kBoxVolume,		//!< \nop
		kSphereVolume,		//!< \nop
		kCylinderVolume,	//!< \nop
		kConeVolume		//!< \nop
	};

	//! Direction that light travels.
	enum MLightDirection
	{
		/*!
		Light travels outwared from the center of a box- or
		sphere-shaped volume, and outward from the axis of a
		cylinder- or cone-shaped volume.
		*/
		kOutward,

		/*!
		Light travels inward toward the center of a box- or
		sphere-shaped volume, and inward toward the axis of a
		cylinder- or cone-shaped volume.
		*/
		kInward,

		/*!
		Light travels down the axis, like a directional
		light pointing in the negative Y direction.
		*/
		kDownAxis
	};

	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	MObject     create( const MObject& parent, bool UIvisible = true,
				  bool wantTransform = false, MStatus * ReturnStatus = NULL );
	MFnVolumeLight::MLightShape  lightShape ( MStatus * ReturnStatus = NULL ) const;
	MStatus     setLightShape ( const MFnVolumeLight::MLightShape& light_shape);
	MFnVolumeLight::MLightDirection volumeLightDirection (MStatus * ReturnStatus = NULL) const;
	MStatus		setVolumeLightDirection (const MFnVolumeLight::MLightDirection& volume_light_direction);
	float		arc ( MStatus * ReturnStatus = NULL ) const;
	MStatus		setArc (const float& arc);
	float		coneEndRadius ( MStatus * ReturnStatus = NULL ) const;
	MStatus		setConeEndRadius (const float& cone_end_radius);
	bool		emitAmbient ( MStatus * ReturnStatus = NULL ) const;
	MStatus		setEmitAmbient (const bool& emit_ambient);
	MRampAttribute colorRamp (MStatus * ReturnStatus = NULL) const ;
	MRampAttribute penumbraRamp (MStatus * ReturnStatus = NULL) const;
        float       shadowAngle( MStatus * ReturnStatus = NULL ) const;
        MStatus     setShadowAngle( const float& shadow_angle );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnVolumeLight, MFnPointLight );

	//! NO SCRIPT SUPPORT
	MObject     create( const MObject& parent, bool UIvisible = true,
						MStatus * ReturnStatus = NULL );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnVolumeLight */



