#ifndef _MFnTurbulenceField
#define _MFnTurbulenceField
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
// CLASS:    MFnTurbulenceField
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MFnField.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnTurbulenceField)

//! \ingroup OpenMayaFX MFn
//! \brief Function set for Turbulence Fields
/*!
Function set for creation, edit, and query of Turbulence Fields.

A turbulence field causes irregularities in the motion of affected objects.
These irregularities are also called noise or jitter.
*/
class OPENMAYAFX_EXPORT MFnTurbulenceField : public MFnField
{
    declareDagMFn(MFnTurbulenceField, MFnField);

public:
    double       frequency          ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setFrequency       ( double value );
    double       phase              ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setPhase           ( double value );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor(MFnTurbulenceField, MFnField );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnTurbulenceField */
