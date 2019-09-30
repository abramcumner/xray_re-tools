#ifndef _MFnPointLight
#define _MFnPointLight
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
// CLASS:    MFnPointLight
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
// CLASS DECLARATION (MFnPointLight)

//! \ingroup OpenMaya MFn
//! \brief Manage Point Light dependency Nodes. 
/*!
 MFnPointLight facilitates creation and manipulation of dependency graph nodes
 representing point lights.
*/
class OPENMAYA_EXPORT MFnPointLight : public MFnNonExtendedLight
{
	declareDagMFn(MFnPointLight,MFnNonExtendedLight);

public:
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	MObject     create( const MObject& parent, bool UIvisible = true,
				  bool wantTransform = false, MStatus * ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnPointLight,MFnNonExtendedLight );

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
#endif /* _MFnPointLight */



