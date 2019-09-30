#ifndef _MFnStringArrayData
#define _MFnStringArrayData
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
// CLASS:    MFnStringArrayData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnStringArrayData)

//! \ingroup OpenMaya MFn
//! \brief String array function set for dependency node data. 
/*!
  MFnStringArrayData allows the creation and manipulation of MStringArray data
  objects for use in the dependency graph.

  If a user written dependency node either accepts or produces MStringArrays,
  then this class is used to extract or create the data that comes from or
  goes to other dependency graph nodes.  The MDataHandle::type method will
  return kStringArray when data of this type is present.  To access it, the
  MDataHandle::data method is used to get an MObject for the data and this
  should then be used to initialize an instance of MFnStringArrayData.
*/
class OPENMAYA_EXPORT MFnStringArrayData : public MFnData
{
	declareMFn(MFnStringArrayData, MFnData);

public:
	unsigned int		length( MStatus* ReturnStatus = NULL ) const;
	MStatus		    set( const MString& element, unsigned int index );
	MStatus			copyTo( MStringArray& ) const;
	MStatus			set( const MStringArray& newArray );
	MStringArray	array( MStatus*ReturnStatus=NULL );
	MObject			create( MStatus*ReturnStatus=NULL );
	MObject			create( const MStringArray& in, MStatus*ReturnStatus=NULL );
	const MString&	operator[]( unsigned int index ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnStringArrayData, MFnData );
	//!	NO SCRIPT SUPPORT
	MString&	    operator[]( unsigned int index );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnStringArrayData */
