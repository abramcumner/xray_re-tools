#ifndef _MFnVolumeAxisField
#define _MFnVolumeAxisField
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
// CLASS:    MFnVolumeAxisField
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnField.h>
#include <maya/MVector.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnVolumeAxisField)

//! \ingroup OpenMayaFX MFn
//! \brief Function set for VolumeAxis Fields
/*!
  Function set for creation, edit, and query of VolumeAxis Fields.

  A volume axis field provides in field form some of the speed
  controls of volume emitters.
*/
class OPENMAYAFX_EXPORT MFnVolumeAxisField : public MFnField
{
    declareDagMFn(MFnVolumeAxisField, MFnField);

public:
    bool         invertAttenuation  ( MStatus *ReturnStatus = NULL ) const;
    MVector      direction          ( MStatus *ReturnStatus = NULL ) const;
    double       speedAlongAxis     ( MStatus *ReturnStatus = NULL ) const;
    double       speedAroundAxis    ( MStatus *ReturnStatus = NULL ) const;
    double       speedAwayFromAxis  ( MStatus *ReturnStatus = NULL ) const;
    double       speedAwayFromCenter( MStatus *ReturnStatus = NULL ) const;
    double       directionalSpeed   ( MStatus *ReturnStatus = NULL ) const;
    double       turbulence         ( MStatus *ReturnStatus = NULL ) const;
    double       turbulenceSpeed    ( MStatus *ReturnStatus = NULL ) const;
    MVector      turbulenceFrequency ( MStatus *ReturnStatus = NULL ) const;
    MVector      turbulenceOffset   ( MStatus *ReturnStatus = NULL ) const;
    double       detailTurbulence   ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setInvertAttenuation  ( bool enable );
    MStatus      setDirection       ( const MVector & direction );
    MStatus      setSpeedAlongAxis  ( double value );
    MStatus      setSpeedAroundAxis  ( double value );
    MStatus      setSpeedAwayFromAxis  ( double value );
    MStatus      setSpeedAwayFromCenter  ( double value );
    MStatus      setDirectionalSpeed ( double value );
    MStatus      setTurbulence       ( double value );
    MStatus      setTurbulenceSpeed  ( double value );
    MStatus      setTurbulenceFrequency ( const MVector &value );
    MStatus      setTurbulenceOffset ( const MVector &value );
    MStatus      detailTurbulence    ( double value );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnVolumeAxisField, MFnField );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnVolumeAxisField */
