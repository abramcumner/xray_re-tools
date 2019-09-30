#ifndef _MFnMatrixArrayData
#define _MFnMatrixArrayData
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
// CLASS:    MFnMatrixArrayData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnMatrixArrayData)

//! \ingroup OpenMaya MFn
//! \brief Matrix array function set for dependency node data. 
/*!
  MFnMatrixArrayData allows the creation and manipulation of MMatrixArray data
  objects for use in the dependency graph.

  If a user written dependency node either accepts or produces MMatrixArrays,
  then this class is used to extract or create the data that comes from or
  goes to other dependency graph nodes.  The MDataHandle::type() method will
  return kMatrixArray when data of this type is present.  To access it, the
  MDataHandle::data() method is used to get an MObject for the data and this
  should then be used to initialize an instance of MFnMatrixArrayData.
*/
class OPENMAYA_EXPORT MFnMatrixArrayData : public MFnData
{
	declareMFn(MFnMatrixArrayData, MFnData);

public:
	unsigned int    length( MStatus* ReturnStatus = NULL ) const;
	MStatus         set( MMatrix& element, unsigned int index );
	MStatus         copyTo( MMatrixArray& ) const;
	MStatus         set( const MMatrixArray& newArray );
	MMatrixArray    array( MStatus*ReturnStatus=NULL );
	MObject         create( MStatus*ReturnStatus=NULL );
	MObject         create( const MMatrixArray& in, MStatus*ReturnStatus=NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnMatrixArrayData, MFnData );

	//!	NO SCRIPT SUPPORT
	MMatrix&        operator[]( unsigned int index );
	//!	NO SCRIPT SUPPORT
	const MMatrix&  operator[]( unsigned int index ) const;

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnMatrixArrayData */
