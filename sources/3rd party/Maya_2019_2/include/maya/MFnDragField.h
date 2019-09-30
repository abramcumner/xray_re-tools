#ifndef _MFnDragField
#define _MFnDragField
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
// CLASS:    MFnDragField
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
// CLASS DECLARATION (MFnDragField)

//! \ingroup OpenMayaFX MFn
//! \brief Function set for Drag Fields 
/*!
Function set for creation, edit, and query of Drag Fields.

A drag field exerts a friction, or braking force, proportional
to the speed of a moving object.
*/
class OPENMAYAFX_EXPORT MFnDragField : public MFnField
{
    declareDagMFn(MFnDragField, MFnField);

public:
    MVector      direction          ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setDirection       ( const MVector & dragDirection );
    bool         useDirection       ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setUseDirection    ( bool enable );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnDragField, MFnField );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnDragField */
