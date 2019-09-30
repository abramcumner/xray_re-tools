#ifndef _MFnStringData
#define _MFnStringData
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
// CLASS:    MFnStringData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnStringData)

//! \ingroup OpenMaya MFn
//! \brief String function set for dependency node data. 
/*!
MFnStringData allows the creation and manipulation of MString data objects
for use in the dependency graph.

If a user written dependency node either accepts or produces MString, then
this class is used to extract or create the data that comes from or goes to
other dependency graph nodes. The MDataHandle::type method will return
kStringData when data of this type is present. To access it, the
MDataHandle::data method is used to get an MObject for the data and this
should then be used to initialize an instance of MFnStringData.
*/
class OPENMAYA_EXPORT MFnStringData : public MFnData
{
	declareMFn(MFnStringData, MFnData);

public:
	MString			string( MStatus* ReturnStatus = NULL ) const;
	MStatus			set( const MString& newString );
	MObject			create( const MString& str, MStatus* ReturnStatus = NULL );
	MObject			create( MStatus* ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnStringData, MFnData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnStringData */
