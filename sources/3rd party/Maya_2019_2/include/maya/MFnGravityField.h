#ifndef _MFnGravityField
#define _MFnGravityField
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
// CLASS:    MFnGravityField
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnField.h>
#include <maya/MVector.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnGravityField)

//! \ingroup OpenMayaFX MFn
//! \brief Function set for Gravity Fields 
/*!
Function set for creation, edit, and query of Gravity Fields.

A gravity field simulates the Earth's gravitational force. It pulls
objects in a fixed direction entirely independent of their position or mass.
*/
class OPENMAYAFX_EXPORT MFnGravityField : public MFnField
{
    declareDagMFn(MFnGravityField, MFnField);

public:
    MVector      direction          ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setDirection       ( const MVector & gravityDirection );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnGravityField, MFnField );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnGravityField */
