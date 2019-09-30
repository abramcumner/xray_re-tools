#ifndef _MFnKeyframeDeltaBlockAddRemove
#define _MFnKeyframeDeltaBlockAddRemove
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
// CLASS:    MFnKeyframeDeltaBlockAddRemove
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnKeyframeDeltaBlockAddRemove)

//! \ingroup OpenMayaAnim MFn
//! \brief Function set for block add or removal of keys.
/*!
   Certain keyframe editing operations work on group of keys rather than
   an single key. These operations require this function set to
   retrieve changes.

   Function set for reading a block of keyframe changes. They are used
   internally to set a large group of keys quickly. Because changes
   occur in groups using base class keyframe methods on this derived
   class do not make sense.  Therefore, methods describing where the
   block of keys were added or removed are available though methods on
   this class.
*/
class OPENMAYAANIM_EXPORT MFnKeyframeDeltaBlockAddRemove : public MFnKeyframeDelta
{
	declareMFn(MFnKeyframeDeltaBlockAddRemove, MFnKeyframeDelta);

public:
	//! Type of change.
	enum DeltaType {
		kAdded,		//!< Keys were added.
		kRemoved	//!< Keys were removed.
	};

	DeltaType	deltaType( MStatus *ReturnStatus = NULL ) const;
	MTime		startTime( MStatus *ReturnStatus = NULL ) const;
	MTime		endTime( MStatus *ReturnStatus = NULL ) const;
	unsigned int	numKeys( MStatus *ReturnStatus = NULL ) const;

	void		getValues( MDoubleArray &values, MStatus *ReturnStatus = NULL ) const;
	void		getTimes( MTimeArray &times, MStatus *ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnKeyframeDeltaBlockAddRemove, MFnKeyframeDelta );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected.

private:
	// No private.
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnKeyframeDeltaBlockAddRemove */
