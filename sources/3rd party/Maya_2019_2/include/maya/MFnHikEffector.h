#ifndef _MFnHikEffector
#define _MFnHikEffector
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
// CLASS:    MFnHikEffector
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnTransform.h>
#include <maya/MObject.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnHikEffector)

//! \ingroup OpenMayaAnim MFn
//! \brief Full Body IK end effector function set
/*!

  MFnHikEffector is the function set for full body ik effectors.  An
  Full Body IK (FBIK/HIK) effector is a special transform that allows
  users to manipulate a Full Body IK system.

  The methods of the parent class MFnTransform used to position the
  end effector.
*/
class OPENMAYAANIM_EXPORT MFnHikEffector : public MFnTransform
{
	declareDagMFn( MFnHikEffector, MFn::kHikEffector );

public:
	MObject create( MObject parent = MObject::kNullObj,
					MStatus * ReturnStatus = NULL );
	MVector	getPivotOffset( MStatus* ReturnStatus = NULL ) const;
	MStatus setPivotOffset( const MVector& vector );
	MColor	getEffColor ( MStatus* ReturnStatus = NULL ) const;
	MStatus setEffColor( const MColor& color );
	MStatus	getAuxiliaryEffectors( MObjectArray& effs ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnHikEffector, MFn::kHikEffector );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnHikEffector */
