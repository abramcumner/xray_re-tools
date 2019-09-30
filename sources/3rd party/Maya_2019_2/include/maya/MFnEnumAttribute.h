#ifndef _MFnEnumAttribute
#define _MFnEnumAttribute
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
// CLASS:    MFnEnumAttribute
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnEnumAttribute)

//! \ingroup OpenMaya MFn
//! \brief Enumerated attribute function set. 
/*!
  Function set for enumerated attributes.  These attributes accept
  fields as input. Each field consists of a short and its associated
  string, which is a descriptive name for the field.
*/
class OPENMAYA_EXPORT MFnEnumAttribute : public MFnAttribute
{
	declareMFn(MFnEnumAttribute, MFnAttribute);

public:

	MObject     create( const MString& fullName,
					    const MString& briefName,
						short defaultValue = 0,
					    MStatus* ReturnStatus = NULL );
	MStatus		addField( const MString & fieldString, short index);
	MString     fieldName( short index, MStatus *ReturnStatus = NULL ) const;
	short       fieldIndex( const MString & fieldString,
							MStatus *ReturnStatus = NULL ) const;
	MStatus		getMin ( short& minValue ) const;
	MStatus		getMax ( short& maxValue ) const;

	MStatus 	setDefault( short index );
	MStatus 	setDefault( const MString &fieldString );

	MStatus		getDefault( short &index ) const;
	MString		defaultValue( MStatus * ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnEnumAttribute, MFnAttribute );
	//!     NO SCRIPT SUPPORT
	MStatus		getDefault( MString &fieldString ) const;

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:

	MStatus		commonGetDefault( short &index ) const;
	MStatus		commonSetDefault( short index ) const;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnEnumAttribute */
