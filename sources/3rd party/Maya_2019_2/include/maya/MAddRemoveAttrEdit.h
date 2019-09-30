#ifndef _MAddRemoveAttrEdit
#define _MAddRemoveAttrEdit
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
// CLASS:    MAddRemoveAttrEdit
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MEdit.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN



// ****************************************************************************
// CLASS DECLARATION (MAddRemoveAttrEdit)

//! \ingroup OpenMaya
//! \brief Class for describing edits involving attributes which are added or removed.
/*!
  
This class is used to represent information about edits involving
attributes which are added or removed. Such edits occur when
attributes are added or removed from nodes in a referenced file. When
a reference is unloaded, only the node and attribute name may be queried
successfully. When the referenced file is loaded, the node itself may
also be queried.

The MItEdits class may be used to iterate over all the edits on a given
reference or assembly.

*/

class OPENMAYA_EXPORT MAddRemoveAttrEdit : public MEdit
{
	
public:
    class Imp;
	friend class MItEdits;

    virtual ~MAddRemoveAttrEdit();
	
	MObject node(MStatus* ReturnStatus = NULL) const;
	MString attributeName(MStatus* ReturnStatus = NULL) const;
	MString nodeName(MStatus* ReturnStatus = NULL) const;

	bool	isAttributeAdded() const;

	virtual EditType editType(MStatus* ReturnStatus = NULL) const;
	
	static const char* className();	

private:
	MAddRemoveAttrEdit(const void* edit, int editType, bool isAdd );

    void createImp(const void* edit, int editType, bool isAdd);

    const Imp&  getImp() const;

	bool  fAdd;
    void* fImp[2];
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAddRemoveAttrEdit */



