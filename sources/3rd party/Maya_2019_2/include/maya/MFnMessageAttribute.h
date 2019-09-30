#ifndef _MFnMessageAttribute
#define _MFnMessageAttribute
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
// CLASS:    MFnMessageAttribute
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnMessageAttribute)

//! \ingroup OpenMaya MFn
//! \brief Message attribute function set. 
/*!
 Function set for message attributes.  A message attribute is a dependency node
 attribute that does not transmit data.  Message attributes only exist to
 formally declare relationships between nodes.  By connecting two nodes
 via message attributes, a relationship between those nodes is expressed.
 Message attribute connections can be traversed in the same manner as
 any other connection in the dependency graph. Message attributes cannot be 
 stored as they do not contain data. 
*/
class OPENMAYA_EXPORT MFnMessageAttribute : public MFnAttribute
{
	declareMFn(MFnMessageAttribute, MFnAttribute);

public:
	MObject    create( const MString& fullName,
					   const MString& briefName,
					   MStatus* ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnMessageAttribute, MFnAttribute );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnMessageAttribute */
