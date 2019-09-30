#ifndef _MFnNObjectData
#define _MFnNObjectData
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
// CLASS:    MFnNObjectData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnNObjectData)

//! \ingroup OpenMayaFX MFn
//! \brief function set for nCloth object data 
/*!
  Class for transferring N object data between connections
*/
class OPENMAYAFX_EXPORT MFnNObjectData : public MFnData
{
    declareMFn( MFnNObjectData, MFnData );

public:


    MObject         create() const ;
BEGIN_NO_SCRIPT_SUPPORT:
	//!	NO SCRIPT SUPPORT
	MStatus			getObjectPtr( MnCloth *& ptr ) const;
	//!	NO SCRIPT SUPPORT
	MStatus			getObjectPtr( MnRigid *& ptr ) const;
	//!	NO SCRIPT SUPPORT
	MStatus			getObjectPtr( MnParticle *& ptr ) const;
END_NO_SCRIPT_SUPPORT:
	MnCloth *		getClothObjectPtr( MStatus *status=NULL ) const;
	MnRigid *		getRigidObjectPtr( MStatus *status=NULL ) const;
	MnParticle *	getParticleObjectPtr( MStatus *status=NULL ) const;
	MStatus			setObjectPtr( MnCloth * ptr );
	MStatus			setObjectPtr( MnRigid * ptr );
	MStatus			setObjectPtr( MnParticle * ptr );
    MStatus			getCollide( bool & ) const;
BEGIN_NO_SCRIPT_SUPPORT:
	//!	NO SCRIPT SUPPORT
    MStatus         getCached( bool & cached ) const;
END_NO_SCRIPT_SUPPORT:
	bool			isCached( MStatus *status=NULL ) const;
    MStatus         setCached( bool cached );


BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnNObjectData, MFnData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No Private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnNObjectData */
