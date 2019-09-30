#ifndef _MFnNonAmbientLight
#define _MFnNonAmbientLight
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
// CLASS:    MFnNonAmbientLight
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
// CLASS DECLARATION (MFnNonAmbientLight)

//! \ingroup OpenMaya MFn
//! \brief Manage Non-ambient Light dependency Nodes.
/*!
 MFnNonAmbientLight facilitates modification of dependency graph nodes
 representing non-ambient lights.  This includes area lights, linear lights,
 and non-extended lights.
*/
class OPENMAYA_EXPORT MFnNonAmbientLight : public MFnLight
{
	declareDagMFn(MFnNonAmbientLight,MFnLight);
public:
	short		decayRate( MStatus * ReturnStatus = NULL ) const;
	MStatus		setDecayRate( const short& decay_rate );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnNonAmbientLight,MFnLight );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnNonAmbientLight */
