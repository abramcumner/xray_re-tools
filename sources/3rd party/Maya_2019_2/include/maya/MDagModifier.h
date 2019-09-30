#ifndef _MDagModifier
#define _MDagModifier
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
// CLASS:    MDagModifier
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MObject.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDagModifier)

//! \ingroup OpenMaya
//! \brief Dependency graph modifier. 
/*!
  An MDagModifier is used to change the structure of the DAG.  This
  includes adding nodes, making new connections, and removing existing
  connections.  To perform operations using an MDagModifier, register
  all of the changes that are to be made and then call the doIt method
  to make the changes.  Undo is provided through the undoIt method.
*/
class OPENMAYA_EXPORT MDagModifier : public MDGModifier
{
public:
	MDagModifier();
	~MDagModifier();

    MObject     createNode(     const MTypeId &typeId,
                                const MObject & parent =  MObject::kNullObj,
                                MStatus* ReturnStatus = NULL );
    MObject     createNode(     const MString &type,
                                const MObject & parent =  MObject::kNullObj,
                                MStatus* ReturnStatus = NULL );
    MStatus     reparentNode(   const MObject & node,
                                const MObject & newParent = MObject::kNullObj );

	static const  char*	className();

protected:
	MDagModifier(void*);
	MDagModifier( const MDagModifier & other );
	MDagModifier&	operator =( const MDagModifier & rhs );

private:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDagModifier */
