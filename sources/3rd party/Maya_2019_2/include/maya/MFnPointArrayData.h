#ifndef _MFnPointArrayData
#define _MFnPointArrayData
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
// CLASS:    MFnPointArrayData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnPointArrayData)

//! \ingroup OpenMaya MFn
//! \brief Point array function set for dependency node data. 
/*!
  MFnPointArrayData allows the creation and manipulation of MPointArray data
  objects for use in the dependency graph.

  If a user written dependency node either accepts or produces MPointArrays,
  then this class is used to extract or create the data that comes from or
  goes to other dependency graph nodes.  The MDataHandle::type method will
  return kPointArray when data of this type is present.  To access it, the
  MDataHandle::data method is used to get an MObject for the data and this
  should then be used to initialize an instance of MFnPointArrayData.
*/
class OPENMAYA_EXPORT MFnPointArrayData : public MFnData
{
	declareMFn(MFnPointArrayData, MFnData);

public:
	unsigned int		length( MStatus* ReturnStatus = NULL ) const;
	MStatus			set( MPoint& element, unsigned int index );
	MStatus			copyTo( MPointArray& ) const;
	MStatus			set( const MPointArray& newArray );
	MPointArray		array( MStatus*ReturnStatus=NULL );
	MObject			create( MStatus*ReturnStatus=NULL );
	MObject			create( const MPointArray& in, MStatus*ReturnStatus=NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnPointArrayData, MFnData );

	//!	NO SCRIPT SUPPORT
	MPoint&			operator[]( unsigned int index );
	//!	NO SCRIPT SUPPORT
	const MPoint&	operator[]( unsigned int index ) const;

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnPointArrayData */
