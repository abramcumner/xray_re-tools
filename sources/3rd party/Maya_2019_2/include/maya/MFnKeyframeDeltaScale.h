#ifndef _MFnKeyframeDeltaScale
#define _MFnKeyframeDeltaScale
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
// CLASS:    MFnKeyframeDeltaScale
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MFnKeyframeDeltaScale)
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnKeyframeDeltaScale)

//! \ingroup OpenMayaAnim MFn
//! \brief \obsolete 
/*!
\deprecated
Not used.
Maya instead generates MFnKeyframeDeltaMove and MFnKeyframeDeltaTangent events
when a group of keys is scaled.

*/
class OPENMAYAANIM_EXPORT MFnKeyframeDeltaScale : public MFnKeyframeDelta
{
	declareMFn(MFnKeyframeDeltaScale, MFnKeyframeDelta);

public:

	MTime		startTime( MStatus * ReturnStatus = NULL ) const;
	MTime		endTime( MStatus * ReturnStatus = NULL ) const;
	MTime		currentStartTime( MStatus * ReturnStatus = NULL ) const;
	MTime		currentEndTime( MStatus * ReturnStatus = NULL ) const;
	MTime		pivotTime( MStatus * ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnKeyframeDeltaScale, MFnKeyframeDelta );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected.

private:
	// No private.
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnKeyframeDeltaScale */
