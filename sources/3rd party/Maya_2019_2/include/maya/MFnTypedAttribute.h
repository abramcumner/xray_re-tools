#ifndef _MFnTypedAttribute
#define _MFnTypedAttribute
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
// CLASS:    MFnTypedAttribute
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>
#include <maya/MFnData.h>
#include <maya/MObject.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnTypedAttribute)

//! \ingroup OpenMaya MFn
//! \brief Typed attribute function set. 
/*!
  MFnTypedAttribute is the function set for typed dependency node
  attributes.  A typed attribute accepts exactly one type of data.
  The type that it accepts is set when the attribute is created.

  Once a typed attribute had been created, the data for it can be
  accessed using an MDataHandle while in your node's compute method.
*/
class OPENMAYA_EXPORT MFnTypedAttribute : public MFnAttribute
{
	declareMFn(MFnTypedAttribute, MFnAttribute);

public:
	MObject 	create( const MString& fullName,
						const MString& briefName,
						const MTypeId& id,
						MObject defaultData = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );
	MObject 	create( const MString& fullName,
						const MString& briefName,
						MFnData::Type type,
						MObject defaultData = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );
	MFnData::Type	attrType ( MStatus* ReturnStatus = NULL ) const;
	MStatus		getDefault( MObject & defaultCustomData );
	MStatus		setDefault( const MObject & defaultCustomData );

BEGIN_NO_SCRIPT_SUPPORT:

	// obsolete
    OPENMAYA_DEPRECATED(2019, "Use the other MFnTypedAttribute::create method instead.")
	MObject 	create( const MString& fullName,
						const MString& briefName,
						MFnData::Type type,
						MStatus* ReturnStatus );

 	declareMFnConstConstructor( MFnTypedAttribute, MFnAttribute );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnTypedAttribute */
