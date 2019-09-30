#ifndef _MFnField
#define _MFnField
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
// CLASS:    MFnField
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnField)

//! \ingroup OpenMayaFX
//! \brief Function set for Dynamic Fields
/*!
  Function set for creation, edit, and query of Dynamic Fields.

  There are several types of dynamic fields: Air, Drag, Gravity,
  Newton Radial, Turbulence, Uniform, and Vortex.
*/
class OPENMAYAFX_EXPORT MFnField : public MFnDagNode
{
    declareDagMFn(MFnField, MFnDagNode);

public:
    MStatus getForceAtPoint(const MPointArray&   point,
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
							double deltaTime = 1.0 / 24.0 );
    MStatus getForceAtPoint(const MVectorArray&  point,
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
							double deltaTime = 1.0 / 24.0 );
    double       magnitude          ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setMagnitude       ( double mag );
    double       attenuation        ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setAttenuation     ( double atten );
    double       maxDistance        ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setMaxDistance     ( double maxDist );
    bool         perVertex          ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setPerVertex       ( bool enable );
    bool         useMaxDistance     ( MStatus *ReturnStatus = NULL ) const;
    MStatus      setUseMaxDistance  ( bool enable );
    double       falloffCurve       ( const double param, MStatus *ReturnStatus = NULL );
    bool         isFalloffCurveConstantOne       (MStatus *ReturnStatus = NULL);

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnField, MFnDagNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnField */
