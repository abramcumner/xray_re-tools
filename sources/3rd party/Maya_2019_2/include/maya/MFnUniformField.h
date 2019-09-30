#ifndef _MFnUniformField
#define _MFnUniformField
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
// CLASS:    MFnUniformField
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
// CLASS DECLARATION (MFnUniformField)

//! \ingroup OpenMayaFX MFn
//! \brief Function set for Uniform Fields
/*!
  Function set for creation, edit, and query of Uniform Fields.

  A uniform field pushes objects uniformly in a single direction.
*/
class OPENMAYAFX_EXPORT MFnUniformField : public MFnField
{
    declareDagMFn(MFnUniformField, MFnField);

public:
    MVector      direction          ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setDirection       ( const MVector & uniformDirection );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnUniformField, MFnField );
	
END_NO_SCRIPT_SUPPORT:

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnUniformField */
