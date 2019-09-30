#ifndef _MFnIntArrayData
#define _MFnIntArrayData
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
// CLASS:    MFnIntArrayData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnIntArrayData)

//! \ingroup OpenMaya MFn
//! \brief int array function set for dependency node data.
/*!
  MFnIntArrayData allows the creation and manipulation of MIntArray data
  objects for use in the dependency graph.

  If a user written dependency node either accepts or produces MIntArrays,
  then this class is used to extract or create the data that comes from or
  goes to other dependency graph nodes.  The MDataHandle::type method will
  return kIntArray when data of this type is present.  To access it, the
  MDataHandle::data method is used to get an MObject for the data and this
  should then be used to initialize an instance of MFnIntArrayData.
*/
class OPENMAYA_EXPORT MFnIntArrayData : public MFnData
{
	declareMFn(MFnIntArrayData, MFnData);

public:
	unsigned int		length( MStatus* ReturnStatus = NULL ) const;
	int             operator[]( unsigned int index ) const;
	MStatus			set( int element, unsigned int index );
	MStatus			copyTo( MIntArray& ) const;
	MStatus			set( const MIntArray& newArray );
	MIntArray	    array( MStatus*ReturnStatus=NULL );
	MObject			create( MStatus*ReturnStatus=NULL );
	MObject			create( const MIntArray& in, MStatus*ReturnStatus=NULL );

BEGIN_NO_SCRIPT_SUPPORT:

	declareMFnConstConstructor( MFnIntArrayData, MFnData );

	//!	NO SCRIPT SUPPORT
	int&	        operator[]( unsigned int index );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnIntArrayData */
