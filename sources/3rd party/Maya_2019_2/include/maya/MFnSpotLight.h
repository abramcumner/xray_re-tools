#ifndef _MFnSpotLight
#define _MFnSpotLight
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
// CLASS:    MFnSpotLight
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnNonExtendedLight.h>


// ****************************************************************************
// DECLARATIONS

class MFltMatrix;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnSpotLight)

//! \ingroup OpenMaya MFn
//! \brief Manage Spot Light dependency Nodes. 
/*!
 MFnSpotLight facilitates creation and manipulation of dependency graph nodes
 representing spotlights.
*/
class OPENMAYA_EXPORT MFnSpotLight : public MFnNonExtendedLight
{
	declareDagMFn(MFnSpotLight,MFnNonExtendedLight);
public:
	//! Available doors for barnDoor-related methods.
	enum MBarnDoor { kLeft,		//!< \nop
					 kRight,	//!< \nop
					 kTop,		//!< \nop
					 kBottom	//!< \nop
	};
	//! Decay regions.
	enum MDecayRegion { kFirst,		//!< \nop
						kSecond,	//!< \nop
						kThird		//!< \nop
	};
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	MObject     create( const MObject& parent, bool UIvisible = true,
				  bool wantTransform = false, MStatus * ReturnStatus = NULL );
	double      coneAngle( MStatus * ReturnStatus = NULL ) const;
    MStatus     setConeAngle( const double& coneAngle );
	double      penumbraAngle( MStatus * ReturnStatus = NULL ) const;
	MStatus     setPenumbraAngle( const double& penumbraAngle );
	double      dropOff( MStatus * ReturnStatus = NULL ) const;
	MStatus     setDropOff( const double& dropOff );
	bool        barnDoors( MStatus * ReturnStatus = NULL ) const;
	MStatus     setBarnDoors( const bool& barnDoors );
	double      barnDoorAngle( MBarnDoor which_door,
						MStatus * ReturnStatus = NULL ) const;
	MStatus     setBarnDoorAngle( MBarnDoor which_door,
						const double& barn_door_angle );
	bool        useDecayRegions( MStatus * ReturnStatus = NULL ) const;
	MStatus     setUseDecayRegions( const bool& use_decay_regions );
	float       startDistance( MDecayRegion which_region,
						MStatus * ReturnStatus = NULL ) const;
	MStatus     setStartDistance( MDecayRegion which_region,
						const float& start_distance );
	float       endDistance( MDecayRegion which_region,
						MStatus * ReturnStatus = NULL ) const;
	MStatus     setEndDistance( MDecayRegion which_region,
						const float& end_distance );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnSpotLight,MFnNonExtendedLight );

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
#endif /* _MFnSpotLight */

