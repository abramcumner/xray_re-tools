#ifndef _MFnVectorArrayData
#define _MFnVectorArrayData
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
// CLASS:    MFnVectorArrayData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnVectorArrayData)

//! \ingroup OpenMaya MFn
//! \brief int array function set for dependency node data. 
/*!
  MFnVectorArrayData allows the creation and manipulation of MVectorArray data
  objects for use in the dependency graph.

  If a user written dependency node either accepts or produces MVectorArrays,
  then this class is used to extract or create the data that comes from or
  goes to other dependency graph nodes.  The MDataHandle::type method will
  return kVectorArray when data of this type is present.  To access it, the
  MDataHandle::data method is used to get an MObject for the data and this
  should then be used to initialize an instance of MFnVectorArrayData.
*/
class OPENMAYA_EXPORT MFnVectorArrayData : public MFnData
{
	declareMFn(MFnVectorArrayData, MFnData);

public:
	unsigned int		length( MStatus* ReturnStatus = NULL ) const;
	MStatus		    set( MVector& element, unsigned int index );
	MStatus			copyTo( MVectorArray& ) const;
	MStatus			set( const MVectorArray& newArray );
	MVectorArray	array( MStatus*ReturnStatus=NULL );
	MObject			create( MStatus*ReturnStatus=NULL );
	MObject			create( const MVectorArray& in, MStatus*ReturnStatus=NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnVectorArrayData, MFnData );

	//!	NO SCRIPT SUPPORT
	const MVector&  operator[]( unsigned int index ) const;
	//!	NO SCRIPT SUPPORT
	MVector&	    operator[]( unsigned int index );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnVectorArrayData */
