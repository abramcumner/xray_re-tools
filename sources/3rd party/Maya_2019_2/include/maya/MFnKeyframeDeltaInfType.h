#ifndef _MFnKeyframeDeltaInfType
#define _MFnKeyframeDeltaInfType
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
// ****************************************************************************
//
// CLASS:    MFnKeyframeDeltaInfType
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>
#include <maya/MFnAnimCurve.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnKeyframeDeltaInfType)

//! \ingroup OpenMayaAnim MFn
//! \brief Function set for changes in pre or post infinity type. 
/*!
This delta object represents changes in the pre-infinity or
post-infinity type of the animation curve. Pre-infinity and
post-infinity types are specific to animation curves and do not have a
corresponding key associated with them.  It is, therefore, invalid to
have a keyframe associated with a MFnKeyframeDeltaInfType. To
determine if this class represents a change to pre or post infinity
use the method isPreInfinity on this class.
*/
class OPENMAYAANIM_EXPORT MFnKeyframeDeltaInfType : public MFnKeyframeDelta
{
	declareMFn(MFnKeyframeDeltaInfType, MFnKeyframeDelta);

public:

	MFnAnimCurve::InfinityType	previousInfinityType( MStatus *ReturnStatus = NULL ) const;
	MFnAnimCurve::InfinityType  currentInfinityType( MStatus *ReturnStatus = NULL ) const;

	bool    isPreInfinity( MStatus *ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnKeyframeDeltaInfType, MFnKeyframeDelta );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected.

private:
	// No private.
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnKeyframeDeltaInfType */
