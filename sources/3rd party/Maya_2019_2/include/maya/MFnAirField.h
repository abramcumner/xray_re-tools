#ifndef _MFnAirField
#define _MFnAirField
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
// CLASS:    MFnAirField
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnField.h>
#include <maya/MVector.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnAirField)

//! \ingroup OpenMayaFX MFn
//! \brief Function set for Air Fields 
/*!
Function set for creation, edit, and query of Air Fields.

An air field simulates the effects of moving air.
*/
class OPENMAYAFX_EXPORT MFnAirField : public MFnField
{
    declareDagMFn(MFnAirField, MFnField);

public:
    MVector      direction          ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setDirection       ( const MVector & airDirection );
    double       speed              ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setSpeed           ( double value );
    double       inheritVelocity    ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setInheritVelocity ( double velocity );
    bool         inheritRotation    ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setInheritRotation ( bool enable );
    bool         componentOnly      ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setComponentOnly   ( bool enable );
    double       spread             ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setSpread          ( double value );
    bool         enableSpread       ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setEnableSpread    ( bool enable );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnAirField, MFnField );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnAirField */
