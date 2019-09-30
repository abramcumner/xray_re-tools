#ifndef _MSetAttrEdit
#define _MSetAttrEdit
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
// CLASS:    MSetAttrEdit
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MEdit.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN



// ****************************************************************************
// CLASS DECLARATION (MSetAttrEdit)

//! \ingroup OpenMaya
//! \brief Class for describing setAttr edits.
/*!
  
This class is used to return information about setAttr edits. Such edits
occur when a file is referenced and changes are made to attributes within
the file reference. When a reference is unloaded, only the plug name may 
be queried successfully. When the referenced file is loaded, the plug itself
may also be queried.

The MItEdits class may be used to iterate over all the edits on a given
reference or assembly.

*/

class OPENMAYA_EXPORT MSetAttrEdit : public MEdit
{
	
public:
    class Imp;
	friend class MItEdits;
	
    virtual ~MSetAttrEdit();

	MPlug	plug(MStatus* ReturnStatus = NULL) const;
	MString plugName(MStatus* ReturnStatus = NULL) const;

	virtual EditType editType(MStatus* ReturnStatus = NULL) const;
	
	static const char* className();

private:
	MSetAttrEdit(const void* edit, int editType );

    void createImp(const void* edit, int editType);

    const Imp&  getImp() const;

    void*       fImp[2];
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MSetAttrEdit */



