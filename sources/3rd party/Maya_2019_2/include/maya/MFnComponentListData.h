#ifndef _MFnComponentListData
#define _MFnComponentListData
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
// CLASS:    MFnComponentListData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnComponentListData)

//! \ingroup OpenMaya MFn
//! \brief Component list function set for dependency node data.
/*!
MFnComponentListData allows the creation and manipulation of component list
(represented as MObjects) data objects for use in the dependency graph.

If a user written dependency node either accepts or produces MObjects
containing component lists, then this class is used to extract or
create the data that comes from or goes to other dependency graph
nodes.  To access it, the MDataHandle::data method is used to get an
MObject for the data and this should then be used to initialize an
instance of MFnComponentListData.
*/
class OPENMAYA_EXPORT MFnComponentListData : public MFnData
{
	declareMFn(MFnComponentListData, MFnData);

public:
	unsigned int		length( MStatus* ReturnStatus = NULL ) const;
	bool			has( const MObject& obj,
						 MStatus* ReturnStatus = NULL ) const;
	MObject			operator[]( unsigned int index) const;
	MStatus			add( MObject& );
	MStatus			remove( const MObject& );
	MStatus			remove( unsigned int index );
	MStatus			clear();
	MObject			create( MStatus* ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	declareMFnConstConstructor( MFnComponentListData, MFnData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnComponentListData */
