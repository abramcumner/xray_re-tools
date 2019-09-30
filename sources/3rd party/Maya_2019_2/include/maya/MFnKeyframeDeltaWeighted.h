#ifndef _MFnKeyframeDeltaWeighted
#define _MFnKeyframeDeltaWeighted
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
// CLASS:    MFnKeyframeDeltaWeighted
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnKeyframeDeltaWeighted)

//! \ingroup OpenMayaAnim MFn
//! \brief Function set for changes in a key's weighted state. 
/*!
  Delta keyframe object for changes in weighted state of a key.
*/
class OPENMAYAANIM_EXPORT MFnKeyframeDeltaWeighted : public MFnKeyframeDelta
{
	declareMFn(MFnKeyframeDeltaWeighted, MFnKeyframeDelta);

public:

	bool		wasWeighted( MStatus *ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnKeyframeDeltaWeighted, MFnKeyframeDelta );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected.

private:
	// No private.
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnKeyframeDeltaWeighted */
