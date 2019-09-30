#ifndef _MEdit
#define _MEdit
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
// CLASS:    MEdit
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MDeprecate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MEdit)

//! \ingroup OpenMaya
//! \brief Base class for representing information about edits 
/*!
  
MEdit serves as the base class for representing information about edits that
are made to certain types of entities, specifically references and assemblies.
An edit is any operation that modifies a member of the entity.  Examples of
such modifications include changing attribute values, connections, parenting
and so on.

In the case of references, edits are the changes that occur to referenced nodes. 

In the case of assemblies, edits are the changes that occur to assembly members.

Derived classes implement specific types of edits, allowing them to be queried depending on the type of operation.

*/

class OPENMAYA_EXPORT MEdit
{
public:
    class Imp;
	friend class MItEdits;

	//! Type of edit
	enum EditType
	{
		//! Null edit
		kNullEdit,
		//! Edit indicating a change in plug value
		kSetAttrEdit,
		//! Edit indicating a connection or disconnection
		kConnectDisconnectEdit,
		//! Edit indicating an attribute added or removed
		kAddRemoveAttrEdit,
		//! Edit indicating a parenting operation
		kParentEdit,
		//! Edit indicating an fcurve modification
		kFcurveEdit
	};
		
	virtual 		~MEdit();

	virtual	EditType	editType(MStatus* ReturnStatus = NULL) const;	
	MString			getString(MStatus* ReturnStatus = NULL) const;

    bool 			isApplied(MStatus* ReturnStatus = NULL) const;
    bool 			isFailed(MStatus* ReturnStatus = NULL) const;
    bool            isTopLevel(MStatus* ReturnStatus = NULL) const;

    OPENMAYA_DEPRECATED(2019, "Does nothing but return success. Present for backward-compatibility purposes only. ")
    MStatus			setApplied(bool isApplied);
    OPENMAYA_DEPRECATED(2019, "Does nothing but return success. Present for backward-compatibility purposes only. ")
    MStatus			setFailed(bool failed);

	// Query user-defined editData
	bool			hasEditData() const;
	bool			matches(const MPxEditData *editData) const;

	static const char* className();	

protected:
	MEdit(const void* edit, int editType);

private:
    void createImp(const void* edit, int editType);

    const Imp&  getImp() const;

    void*       fImp[2];
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MEdit */



