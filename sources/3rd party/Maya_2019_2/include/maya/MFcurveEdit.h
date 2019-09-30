#ifndef _MFcurveEdit
#define _MFcurveEdit
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
// CLASS:    MFcurveEdit
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MEdit.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN


// ****************************************************************************
// CLASS DECLARATION (MFcurveEdit)

//! \ingroup OpenMaya
//! \brief Class for describing fcurve edits.
/*!
  
This class is used to return information about fcurve edits. Such edits
occur when a file is referenced and changes are made to fcurves within
the file reference. When a reference is unloaded, not all the queries on this
method will work. In particular, the fcurve node itself
cannot be returned since it no longer exists in Maya.

The MItEdits class may be used to iterate over all the edits on a given
reference or assembly.

*/

class OPENMAYA_EXPORT MFcurveEdit : public MEdit
{
	friend class MItEdits;

public:
	MObject fcurve(MStatus* ReturnStatus = NULL) const;	
	MString fcurveName(MStatus* ReturnStatus = NULL) const;

	virtual EditType editType(MStatus* ReturnStatus = NULL) const;
	
	static const char* className();	

private:
	MFcurveEdit(const void* edit, bool loaded );

    const void* fEdit;
    bool        fLoaded;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFcurveEdit */
