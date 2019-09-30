#ifndef _MFnUInt64ArrayData
#define _MFnUInt64ArrayData
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
// CLASS:    MFnUInt64ArrayData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnUInt64ArrayData)

//! \ingroup OpenMaya MFn
//! \brief MUint64 array function set for dependency node data. 
/*!
  MFnUInt64ArrayData allows the creation and manipulation of MUint64Array data
  objects for use in the dependency graph.

  If a user written dependency node either accepts or produces MUint64Arrays,
  then this class is used to extract or create the data that comes from or
  goes to other dependency graph nodes.  The MDataHandle::type method will
  return kUInt64Array when data of this type is present.  To access it, the
  MDataHandle::data method is used to get an MObject for the data and this
  should then be used to initialize an instance of MFnUInt64ArrayData.
*/
class OPENMAYA_EXPORT MFnUInt64ArrayData : public MFnData
{
	declareMFn(MFnUInt64ArrayData, MFnData);

public:
	unsigned int		length( MStatus* ReturnStatus = NULL ) const;
	MStatus	        	set( MUint64 element, unsigned int index );
	MStatus				copyTo( MUint64Array& ) const;
	MStatus				set( const MUint64Array& newArray );
	MUint64Array		array( MStatus*ReturnStatus=NULL );
	MObject				create( MStatus*ReturnStatus=NULL );
	MObject				create( const MUint64Array& in, MStatus*ReturnStatus=NULL );

BEGIN_NO_SCRIPT_SUPPORT:

	declareMFnConstConstructor( MFnUInt64ArrayData, MFnData );

	//!	NO SCRIPT SUPPORT
	MUint64				operator[]( unsigned int index ) const;
	MUint64&			operator[]( unsigned int index );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnUInt64ArrayData */
