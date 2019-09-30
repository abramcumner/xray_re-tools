#ifndef _MFnUnitAttribute
#define _MFnUnitAttribute
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
// CLASS:    MFnUnitAttribute
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnUnitAttribute)

//! \ingroup OpenMaya MFn
//! \brief Unit attribute Function set. 
/*!
  MFnUnitAttribute is the function set for dependency nodes attributes
  that store one of the fundamental types of Maya data.  The currently
  supported types are MAngle, MDistance, and MTime.

  It is possible to set the defaults using a double.  This is valid
  for angles and distances.  In the case of angles, the value is
  assumed to be in radians, and in the case of distances, it is
  assumed to be in centimeters.  Time values should not be set using a
  double.

  It is possible to use unit attributes in place of double attributes
  when creating numeric compounds (see MFnNumericAttrbute).  To create
  a numeric compound of three distance values, create the three child
  attributes using MFnUnitAttribute and then pass the children into
  the create method of MFnNumericAttribute.
*/
class OPENMAYA_EXPORT MFnUnitAttribute : public MFnAttribute
{
	declareMFn(MFnUnitAttribute, MFnAttribute);

public:

	//! Type of unit represented by attribute values 
	enum Type {
		kInvalid,	//!< Invalid unit type
		kAngle,     //!< Angular attribute
		kDistance,  //!< Distance (linear) attribute
		kTime,		//!< Time attribute
		kLast		//!< Last value, for counting
	};

	MObject 	create( const MString& fullName,
						const MString& briefName,
						MFnUnitAttribute::Type unitType,
						double defaultValue = 0.0,
						MStatus* ReturnStatus = NULL );
	MObject 	create( const MString& fullName,
						const MString& briefName,
						const MTime& defaultValue,
						MStatus* ReturnStatus = NULL );
	MObject 	create( const MString& fullName,
						const MString& briefName,
						const MAngle& defaultValue,
						MStatus* ReturnStatus = NULL );
	MObject 	create( const MString& fullName,
						const MString& briefName,
						const MDistance& defaultValue,
						MStatus* ReturnStatus = NULL );
	MFnUnitAttribute::Type	unitType( MStatus* ReturnStatus = NULL ) const;
	bool		hasMin( MStatus* ReturnStatus = NULL) const;
	bool		hasMax( MStatus* ReturnStatus = NULL) const;
	bool		hasSoftMin( MStatus* ReturnStatus = NULL) const;
	bool		hasSoftMax( MStatus* ReturnStatus = NULL) const;
	MStatus		getMin( double& minValue ) const;
	MStatus		getMin( MTime& minValue ) const;
	MStatus		getMin( MAngle& minValue ) const;
	MStatus		getMin( MDistance& minValue ) const;
	MStatus		getMax( double& maxValue ) const;
	MStatus		getMax( MTime& maxValue ) const;
	MStatus		getMax( MAngle& maxValue ) const;
	MStatus		getMax( MDistance& maxValue ) const;
	MStatus		getSoftMin( double& minValue ) const;
	MStatus		getSoftMin( MTime& minValue ) const;
	MStatus		getSoftMin( MAngle& minValue ) const;
	MStatus		getSoftMin( MDistance& minValue ) const;
	MStatus		getSoftMax( double& maxValue ) const;
	MStatus		getSoftMax( MTime& maxValue ) const;
	MStatus		getSoftMax( MAngle& maxValue ) const;
	MStatus		getSoftMax( MDistance& maxValue ) const;
	MStatus		setMin( double minValue );
	MStatus		setMin( const MTime &minValue );
	MStatus		setMin( const MAngle &minValue );
	MStatus		setMin( const MDistance &minValue );
	MStatus		setMax( double maxValue );
	MStatus		setMax( const MTime &maxValue );
	MStatus		setMax( const MAngle &maxValue );
	MStatus		setMax( const MDistance &maxValue );
	MStatus		setSoftMin( double minValue );
	MStatus		setSoftMin( const MTime &minValue );
	MStatus		setSoftMin( const MAngle &minValue );
	MStatus		setSoftMin( const MDistance &minValue );
	MStatus		setSoftMax( double maxValue );
	MStatus		setSoftMax( const MTime &maxValue );
	MStatus		setSoftMax( const MAngle &maxValue );
	MStatus		setSoftMax( const MDistance &maxValue );
	MStatus		getDefault( double & defaultValue );
	MStatus		getDefault( MTime & defaultValue );
	MStatus     getDefault( MAngle & defaultValue );
	MStatus     getDefault( MDistance & defaultValue );
	MStatus		setDefault( double defaultValue );
	MStatus		setDefault( const MTime& defaultValue );
	MStatus     setDefault( const MAngle& defaultValue );
	MStatus     setDefault( const MDistance& defaultValue );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnUnitAttribute, MFnAttribute );

END_NO_SCRIPT_SUPPORT:

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnUnitAttribute */
