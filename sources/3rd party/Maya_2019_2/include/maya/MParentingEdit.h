#ifndef _MParentingEdit
#define _MParentingEdit
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
// CLASS:    MParentingEdit
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MEdit.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN



// ****************************************************************************
// CLASS DECLARATION (MParentingEdit)

//! \ingroup OpenMaya
//! \brief Class for describing edits to parenting.
/*!
  
This class is used to represent information about edits to parenting.
Such edits occur when a file is referenced and changes are made to the
dag hierarchy of nodes within that reference. When a reference is
unloaded, only the name of the nodes involved may be queried successfully.
When the referenced file is loaded, the nodes themselves may also be queried.

The MItEdits class may be used to iterate over all the edits on a given
reference or assembly.

*/

class OPENMAYA_EXPORT MParentingEdit : public MEdit
{
	
public:
    class Imp;
	friend class MItEdits;

    virtual ~MParentingEdit();

	MObject	parentedObject(MStatus* ReturnStatus = NULL) const;
	MObject	parent(MStatus* ReturnStatus = NULL) const;	

	MString	parentedObjectName(MStatus* ReturnStatus = NULL) const;
	MString	parentName(MStatus* ReturnStatus = NULL) const;	

	virtual EditType editType(MStatus* ReturnStatus = NULL) const;
	
	static const char* className();	

private:
	MParentingEdit(const void* edit, int editType);

    void createImp(const void* edit, int editType);

    const Imp& getImp() const;

    void* fImp[2];
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MParentingEdit */



