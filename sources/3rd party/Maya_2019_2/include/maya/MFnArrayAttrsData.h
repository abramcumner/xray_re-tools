#ifndef _MFnArrayAttrsData
#define _MFnArrayAttrsData
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
// CLASS:    MFnArrayAttrsData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnArrayAttrsData)

//! \ingroup OpenMaya MFn
//! \brief Function set for multiple arrays of attributes for dependency node data. 
/*!
  MFnArrayAttrsData allows the creation and manipulation of multiple
  arrays of attributes as a data object over a single connection for use
  as dependency graph data.

  If a user written dependency node either accepts or produces
  MFnArrayAttrsData, then this class is used to extract or create
  the data that comes from or goes to other dependency graph nodes.
  The MDataHandle::type method will return kDynArrayAttrsData when data
  of this type is present. To access it, the MDataHandle::data() method
  is used to get an MObject for the data and this should then be used
  to initialize an instance of MFnArrayAttrsData.

  NOTE: these data attributes are not storable.
*/
class OPENMAYA_EXPORT MFnArrayAttrsData : public MFnData
{
	declareMFn( MFnArrayAttrsData, MFnData );

public:

	enum Type {
	kInvalid,
	//! use vectorArray() method to extract the attribute array.
	kVectorArray,
	//! use doubleArray() method to extract the attribute array.
	kDoubleArray,
	//! use intArray() method to extract the attribute array.
	kIntArray,
	//! use stringArray() method to extract the attribute array.
	kStringArray,
	kLast
	};

	MStatus			clear();

	unsigned int		count() const;

	MStringArray	list( MStatus *ReturnStatus = NULL) const;

	bool			checkArrayExist( const MString attrName,
									 MFnArrayAttrsData::Type &arrayType,
									 MStatus *ReturnStatus = NULL);

	MVectorArray	vectorArray( const MString attrName,
									MStatus *ReturnStatus = NULL );
	MDoubleArray	doubleArray( const MString attrName,
									MStatus *ReturnStatus = NULL );
	MIntArray		intArray( const MString attrName,
									MStatus *ReturnStatus = NULL );
	MStringArray	stringArray( const MString attrName,
									MStatus *ReturnStatus = NULL );

	MObject			create( MStatus *ReturnStatus = NULL );

	MVectorArray     getVectorData( const MString attrName, MStatus *ReturnStatus = NULL);

	MDoubleArray     getDoubleData( const MString attrName, MStatus *ReturnStatus = NULL);

	MIntArray        getIntData( const MString attrName, MStatus *ReturnStatus = NULL);

	MStringArray     getStringData( const MString attrName, MStatus *ReturnStatus = NULL);

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	declareMFnConstConstructor( MFnArrayAttrsData, MFnData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnArrayAttrsData */
