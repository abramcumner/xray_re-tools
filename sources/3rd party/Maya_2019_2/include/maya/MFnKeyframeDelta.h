#ifndef _MFnKeyframeDelta
#define _MFnKeyframeDelta
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
// CLASS:    MFnKeyframeDelta
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnKeyframeDelta)

//! \ingroup OpenMayaAnim MFn
//! \brief Base function set for keyframe deltas 
/*!

   Keyframe delta objects are returned via the
   MAnimMessage::animKeyframeEditedCallback( ... ). They describe
   atomic changes to keyframes as a result of a curve edit
   operation. Refer to the documentation in MAnimMessage class for
   further information.

   Base function set for all keyframe delta objects.  A keyframe delta
   object is generated from the
   MAnimMessage::addAnimKeyframeEditedCallback.
*/
class OPENMAYAANIM_EXPORT MFnKeyframeDelta : public MFnBase
{
	declareMFn(MFnKeyframeDelta, MFnBase);

public:

	MObject  paramCurve( MStatus * ReturnStatus = NULL ) const;
	virtual unsigned int keyIndex( MStatus * ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnKeyframeDelta, MFnBase );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected.

private:
	// No private.
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnKeyframeDelta */
