#ifndef _MFnRadialField
#define _MFnRadialField
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
// CLASS:    MFnRadialField
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MFnField.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnRadialField)

//! \ingroup OpenMayaFX MFn
//! \brief Function set for Radial Fields
/*!
Function set for creation, edit, and query of Radial Fields.

A radial field pushes objects directly away or pulls them directly
toward itself, like a magnet.
*/
class OPENMAYAFX_EXPORT MFnRadialField : public MFnField
{

    declareDagMFn(MFnRadialField, MFnField);

public:
    double       radialType        ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setType           ( double value );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnRadialField, MFnField );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnRadialField */
