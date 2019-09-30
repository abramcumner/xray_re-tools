#ifndef _MItEdits
#define _MItEdits
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
// CLASS:    MItEdits
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MEdit.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItEdits)

//! \ingroup OpenMaya
//! \brief Edits iterator. 
/*!

Use the edits iterator to traverse all the edits on a reference
or assembly. An edit is any operation that modifies the dependency graph
such as modifications to values, connections, parenting and so on. 

In the case of references, edits are the changes that occur to referenced nodes. 
In the case of assemblies, edits are the changes that occur to assembly members.

Edits are returned as either strings representing an equivalent MEL call, or as
a class derived from MEdit. 

The edits iterator works regardless of whether the related reference
or assembly is loaded or unloaded. However, please note that loading
or unloading the related reference or assembly will cause any
in-progress iteration to be reset.

If new edits were created while iterating, the iterator will not necessarily 
traverse them.

To control which edits you will traverse over, use the combination of the 
'editsOwner' and 'targetNode'/'targetNodeName' parameters.

The editsOwner is the reference or assembly node that "owns" edits. This is the 
top-level assembly/reference at the time the edit was made. If you were to save 
a file, this is the node that the edits would be stored on.

The targetNode is the innermost reference or assembly node containing the edited 
nodes. Edits will be physically stored on the edits owner, so use targetNode to
get just edits that affect nodes in a specific assembly/reference. If the nested 
assembly or reference is not loaded, you can still query edits that affect that 
node by specifying a string targetNodeName, instead of a targetNode MObject.

Hierarchical Example:

The following example explains the above terminology in the hierarchical case.
Suppose you have a reference or assembly hierarchy that looks like this.

\verbatim
A
|_ B
   |_ C
\endverbatim

Edits can be made at three different levels:
<ol>
<li> Edits made from C to nodes in C. These edits were made from the C file/assembly.  These edits are stored on C

<li> Edits made from B to nodes in B or C. These edits were made from the B file/assembly.  These edits are stored on B

<li> Edits made from A to nodes in A, B or C. These edits were made from the main scene.  These edits are stored on A
</ol>

To query all the edits stored on A, use:
editsOwner = A
targetNode = MObject::kNullObj

To query edits stored on A that affect nodes in A, use: 
editsOwner = A
targetNode = A

To query edits stored on A that affect nodes in B, use:
editsOwner = A
targetNode = B

To query all the edits stored on B, use:
editsOwner = B
targetNode = MObject::kNullObj

To only query edits stored on B that affect nodes in C, use:
editsOwner = B
targetNode = C
*/

class OPENMAYA_EXPORT MItEdits
{
public:
    class Imp;
    friend class Imp;

	//! The status of edits this iterator will visit
	enum EditStatus
	{
		//! Visit successful edits only
		SUCCESSFUL_EDITS, 
		//! Visit all the edits
		ALL_EDITS
	};

	//! The direction in which the iterator will visit its edits
	enum Direction
	{
		//! Visit the edits in the order in which they were added
		kForward, 
		//! Visit the edits in the reverse order from how they were added
		kReverse
	};

	MItEdits( MObject& editsOwner,
			  MObject& targetNode = MObject::kNullObj,
			  EditStatus editStatus = ALL_EDITS,
			  Direction direction = kForward,
			  MStatus * ReturnStatus = NULL );

	MItEdits( MObject& editsOwner,
		      const MString& targetNodeName,
			  EditStatus editStatus = ALL_EDITS,
			  Direction direction = kForward,
		      MStatus * ReturnStatus = NULL );

	virtual		~MItEdits();

	MStatus		reset();
	MStatus		next();
	bool		isDone( MStatus * ReturnStatus = NULL );
	bool		isReverse( MStatus * ReturnStatus = NULL ) const;

	MString		currentEditString( MStatus* ReturnStatus = NULL ) const;
	MEdit::EditType	currentEditType( MStatus* ReturnStatus = NULL ) const;
	bool		removeCurrentEdit(MStatus* ReturnStatus = NULL);

	MEdit				edit( MStatus * ReturnStatus = NULL ) const;
	MAddRemoveAttrEdit 	addRemoveAttrEdit( MStatus * ReturnStatus = NULL ) const;
	MSetAttrEdit 		setAttrEdit( MStatus * ReturnStatus = NULL ) const;
	MParentingEdit		parentingEdit( MStatus * ReturnStatus = NULL ) const;
	MFcurveEdit 		fcurveEdit( MStatus * ReturnStatus = NULL ) const;	
	MConnectDisconnectAttrEdit connectDisconnectEdit( MStatus * ReturnStatus = NULL ) const;

	static const char* className();

protected:
// No protected members

private:
	void		validateInitialization();

    static MEdit                      makeEdit(const void* edit, int editType);
    static MConnectDisconnectAttrEdit makeConnectDisconnectAttrEdit(
       const void* edit, int editType, bool isConnect);
    static MAddRemoveAttrEdit         makeAddRemoveAttrEdit(
       const void* edit, int editType, bool isAdd);
    static MSetAttrEdit               makeSetEdit(
       const void* edit, int editType);
    static MParentingEdit             makeParentingEdit(
       const void* edit, int editType);
    static MFcurveEdit                makeFcurveEdit(
       const void* edit, bool loaded);

    Imp* fImp;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItEdits */
