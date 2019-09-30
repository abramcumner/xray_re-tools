#ifndef _MFnAmbientLight
#define _MFnAmbientLight
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
// CLASS:    MFnAmbientLight
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnLight.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnAmbientLight)

//! \ingroup OpenMaya MFn
//! \brief Manage Ambient Light dependency Nodes.
/*!
MFnAmbientLight facilitates creation and manipulation of ambient light
dependency graph nodes.
*/
class OPENMAYA_EXPORT MFnAmbientLight : public MFnLight
{
	declareDagMFn(MFnAmbientLight,MFnLight);
public:
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	MObject     create( const MObject& parent, bool UIvisible = true,
				  bool wantTransform = false, MStatus * ReturnStatus = NULL );
	float       ambientShade( MStatus * ReturnStatus = NULL ) const;
	MStatus     setAmbientShade( const float& ambient_shade );
	bool        castSoftShadows( MStatus * ReturnStatus = NULL ) const;
	MStatus     setCastSoftShadows( const bool& cast_soft_shadows );
	float       shadowRadius( MStatus * ReturnStatus = NULL ) const;
	MStatus     setShadowRadius( const float& shadow_radius );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnAmbientLight,MFnLight );

	//!	NO SCRIPT SUPPORT
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
#endif /* _MFnAmbientLight */
