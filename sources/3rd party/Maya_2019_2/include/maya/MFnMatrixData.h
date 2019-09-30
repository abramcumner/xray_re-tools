#ifndef _MFnMatrixData
#define _MFnMatrixData
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
// CLASS:    MFnMatrixData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnMatrixData)

//! \ingroup OpenMaya MFn
//! \brief Matrix function set for dependency node data. 
/*!
  MFnMatrixData allows the creation and manipulation of MMatrix data objects
  for use in the dependency graph.

  If a user written dependency node either accepts or produces MMatrix, then
  this class is used to extract or create the data that comes from or goes to
  other dependency graph nodes. The MDataHandle::type method will return
  kMatrix when data of this type is present. To access it, the
  MDataHandle::data method is used to get an MObject for the data and this
  should then be used to initialize an instance of MFnMatrixData.
*/
class OPENMAYA_EXPORT MFnMatrixData : public MFnData
{
	declareMFn(MFnMatrixData, MFnData);

public:
	bool					isTransformation(
										MStatus* ReturnStatus = NULL ) const;
	MTransformationMatrix	transformation(
										MStatus* ReturnStatus = NULL ) const;
	const MMatrix&			matrix( MStatus* ReturnStatus = NULL ) const;
	MStatus					set( const MTransformationMatrix& transformation );
	MStatus					set( const MMatrix& matrix );
	MObject					create( MStatus* ReturnStatus=NULL );
	MObject					create( const MMatrix&,
									MStatus* ReturnStatus=NULL );
	MObject					create( const MTransformationMatrix&,
									MStatus* ReturnStatus=NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnMatrixData, MFnData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnMatrixData */
