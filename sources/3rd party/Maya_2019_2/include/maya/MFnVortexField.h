#ifndef _MFnVortexField
#define _MFnVortexField
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
// CLASS:    MFnVortexField
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MFnField.h>
#include <maya/MVector.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnVortexField)

//! \ingroup OpenMayaFX MFn
//! \brief Function set for Vortex Fields 
/*!
  Function set for creation, edit, and query of Vortex Fields.

  A vortex field pulls objects in a circular direction, like a
  whirlpool or tornado.
*/
class OPENMAYAFX_EXPORT MFnVortexField : public MFnField
{
    declareDagMFn(MFnVortexField, MFnField);

public:
    MVector      axis               ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setAxis            ( const MVector & axisVector );

BEGIN_NO_SCRIPT_SUPPORT:

	declareDagMFnConstConstructor( MFnVortexField, MFnField );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnVortexField */
