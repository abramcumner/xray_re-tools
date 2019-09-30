#ifndef _MFnDirectionalLight
#define _MFnDirectionalLight
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
// CLASS:    MFnDirectionalLight
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnNonExtendedLight.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnDirectionalLight)

//! \ingroup OpenMaya MFn
//! \brief Manage Directional Light dependency nodes. 
/*!
 MFnDirectionalLight facilitates creation and manipulation of dependency nodes
 representing direction lights.
*/
class OPENMAYA_EXPORT MFnDirectionalLight : public MFnNonExtendedLight
{
	declareDagMFn(MFnDirectionalLight,MFnNonExtendedLight);

public:
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	MObject     create( const MObject& parent, bool UIvisible = true,
				  bool wantTransform = false, MStatus * ReturnStatus = NULL );
	float       shadowAngle( MStatus * ReturnStatus = NULL ) const;
	MStatus     setShadowAngle( const float& shadow_angle );

	bool          useLightPosition( MStatus * ReturnStatus = NULL ) const;
	MStatus       setUseLightPosition( const bool use_light_position );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnDirectionalLight,MFnNonExtendedLight );

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
#endif /* _MFnDirectionalLight */
