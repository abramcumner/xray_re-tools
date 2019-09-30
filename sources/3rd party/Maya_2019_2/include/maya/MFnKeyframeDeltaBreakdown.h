#ifndef _MFnKeyframeDeltaBreakdown
#define _MFnKeyframeDeltaBreakdown
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
// CLASS:    MFnKeyframeDeltaBreakdown
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnKeyframeDeltaBreakdown)

//! \ingroup OpenMayaAnim MFn
//! \brief Function set for changes in a key's breakdown state. 
/*!
Delta keyframe object for changes in breakdown state of a key.
*/
class OPENMAYAANIM_EXPORT MFnKeyframeDeltaBreakdown : public MFnKeyframeDelta
{
	declareMFn(MFnKeyframeDeltaBreakdown, MFnKeyframeDelta);

public:

	bool		wasBreakdown( MStatus *ReturnStatus = NULL ) const;
	bool		isBreakdown( MStatus *ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnKeyframeDeltaBreakdown, MFnKeyframeDelta );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected.

private:
	// No private.
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnKeyframeDeltaBreakdown */
