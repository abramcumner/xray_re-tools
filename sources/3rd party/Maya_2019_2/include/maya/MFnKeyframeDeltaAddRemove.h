#ifndef _MFnKeyframeDeltaAddRemove
#define _MFnKeyframeDeltaAddRemove
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
// CLASS:    MFnKeyframeDeltaAddRemove
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnKeyframeDeltaAddRemove)

//! \ingroup OpenMayaAnim MFn
//! \brief Function set for the addition or removal keys on a curve. 
/*!
Captures the addition or removal of key on a curve.  keyIndex(...)
will return the index of the key that was added or removed.
*/
class OPENMAYAANIM_EXPORT MFnKeyframeDeltaAddRemove : public MFnKeyframeDelta
{
	declareMFn(MFnKeyframeDeltaAddRemove, MFnKeyframeDelta);

public:
	//! Type of change
	enum DeltaType {
		kAdded,		//!< Key added.
		kRemoved,	//!< Key removed.
		kReplaced	//!< Key replaced.
	};

	DeltaType	deltaType( MStatus *ReturnStatus = NULL ) const;
	double		value( MStatus *ReturnStatus = NULL ) const;
	MTime		time( MStatus *ReturnStatus = NULL ) const;
	virtual unsigned int keyIndex( MStatus * ReturnStatus = NULL ) const;
	double		replacedValue( MStatus *ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnKeyframeDeltaAddRemove, MFnKeyframeDelta );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected.

private:
	// No private.
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnKeyframeDeltaAddRemove */
