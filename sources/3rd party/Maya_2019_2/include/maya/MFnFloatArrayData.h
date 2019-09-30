#ifndef _MFnFloatArrayData
#define _MFnFloatArrayData
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
// CLASS:    MFnFloatArrayData
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
// CLASS DECLARATION (MFnFloatArrayData)

//! \ingroup OpenMaya MFn
//! \brief float array function set for dependency node data.
/*!
  MFnFloatArrayData allows the creation and manipulation of MFloatArray data
  objects for use in the dependency graph.

  If a user written dependency node either accepts or produces MFloatArrays,
  then this class is used to extract or create the data that comes from or
  goes to other dependency graph nodes.  The MDataHandle::type() method will
  return kFloatArray when data of this type is present.  To access it, the
  MDataHandle::data() method is used to get an MObject for the data and this
  should then be used to initialize an instance of MFnFloatArrayData.
*/
class OPENMAYA_EXPORT MFnFloatArrayData : public MFnData
{
	declareMFn(MFnFloatArrayData, MFnData);

public:
	unsigned int		length( MStatus* ReturnStatus = NULL ) const;
	float           operator[]( unsigned int index ) const;
	MStatus			set( float element, unsigned int index );
	MStatus			copyTo( MFloatArray& ) const;
	MStatus			set( const MFloatArray& newArray );
	MFloatArray	array( MStatus*ReturnStatus=NULL );
	MObject			create( MStatus*ReturnStatus=NULL );
	MObject			create( const MFloatArray& in, MStatus*ReturnStatus=NULL );

BEGIN_NO_SCRIPT_SUPPORT:

	declareMFnConstConstructor(MFnFloatArrayData, MFnData);

	//!	NO SCRIPT SUPPORT
	float&	        operator[]( unsigned int index );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnFloatArrayData */
