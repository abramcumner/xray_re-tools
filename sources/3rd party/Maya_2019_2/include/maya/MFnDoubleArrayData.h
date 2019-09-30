#ifndef _MFnDoubleArrayData
#define _MFnDoubleArrayData
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
// CLASS:    MFnDoubleArrayData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS


// ****************************************************************************
// CLASS DECLARATION (MFnDoubleArrayData)

//! \ingroup OpenMaya MFn
//! \brief double array function set for dependency node data.
/*!
  MFnDoubleArrayData allows the creation and manipulation of MDoubleArray data
  objects for use in the dependency graph.

  If a user written dependency node either accepts or produces MDoubleArrays,
  then this class is used to extract or create the data that comes from or
  goes to other dependency graph nodes.  The MDataHandle::type() method will
  return kDoubleArray when data of this type is present.  To access it, the
  MDataHandle::data() method is used to get an MObject for the data and this
  should then be used to initialize an instance of MFnDoubleArrayData.
*/
class OPENMAYA_EXPORT MFnDoubleArrayData : public MFnData
{
	declareMFn(MFnDoubleArrayData, MFnData);

public:
	unsigned int		length( MStatus* ReturnStatus = NULL ) const;
	double          operator[]( unsigned int index ) const;
	MStatus			set( double element, unsigned int index );
	MStatus			copyTo( MDoubleArray& ) const;
	MStatus			set( const MDoubleArray& newArray );
	MDoubleArray	array( MStatus*ReturnStatus=NULL );
	MObject			create( MStatus*ReturnStatus=NULL );
	MObject			create( const MDoubleArray& in, MStatus*ReturnStatus=NULL );

BEGIN_NO_SCRIPT_SUPPORT:

	declareMFnConstConstructor(MFnDoubleArrayData, MFnData);

	//!	NO SCRIPT SUPPORT
	double&	        operator[]( unsigned int index );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnDoubleArrayData */
