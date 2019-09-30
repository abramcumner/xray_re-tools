#ifndef _MFnKeyframeDeltaMove
#define _MFnKeyframeDeltaMove
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
// CLASS:    MFnKeyframeDeltaMove
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnKeyframeDeltaMove)

//! \ingroup OpenMayaAnim MFn
//! \brief Function set for change in keyframe value or time. 
/*!
This function set is used for keyframe deltas of type
MFn::kKeyframeDeltaMove. These keyframe deltas are generated from
'move' events. For example, the user drags or scales a key from one
position to another.
*/
class OPENMAYAANIM_EXPORT MFnKeyframeDeltaMove : public MFnKeyframeDelta
{
	declareMFn(MFnKeyframeDeltaMove, MFnKeyframeDelta);

public:

	MTime		previousTime( MStatus * ReturnStatus = NULL ) const;
	MTime		currentTime( MStatus * ReturnStatus = NULL ) const;
	double		previousValue( MStatus * ReturnStatus = NULL ) const;
	double		currentValue( MStatus * ReturnStatus = NULL ) const;

	unsigned int	previousIndex( MStatus * ReturnStatus = NULL ) const;
	virtual unsigned int keyIndex( MStatus * ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnKeyframeDeltaMove, MFnKeyframeDelta );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected.

private:
	// No private.
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnKeyframeDeltaMove */
