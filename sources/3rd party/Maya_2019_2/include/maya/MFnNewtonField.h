#ifndef _MFnNewtonField
#define _MFnNewtonField
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
// CLASS:    MFnNewtonField
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MFnField.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnNewtonField)

//! \ingroup OpenMayaFX MFn
//! \brief Function set for Newton Fields
/*!
Function set for creation, edit, and query of Newton Fields.

A newton field pulls an object toward another with a force that is
dependent on the mass of the object exerting the field. It follows
Newton's Law of Universal Gravitation.
*/
class OPENMAYAFX_EXPORT MFnNewtonField : public MFnField
{
    declareDagMFn(MFnNewtonField, MFnField);

public:
    double       minDistance        ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setMinDistance     ( double distance );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnNewtonField, MFnField );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnNewtonField */
