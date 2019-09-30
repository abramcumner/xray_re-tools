#ifndef _MAngle
#define _MAngle
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
// CLASS:    MAngle
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MDeprecate.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAngle)

//! \ingroup OpenMaya
//! \brief Manipulate Angular Data 
/*!
  Methods for setting and retreiving angular data in various unit systems

  The MAngle class provides a fundamental type for the Maya API to hold
  and manipulate angular data.  All API methods that require or return
  angular information do so through variables of this type.
*/
class OPENMAYA_EXPORT MAngle
{
public:
	//! Available Unit Systems
	enum Unit
	{
		kInvalid,							//!< Invalid value 
		kRadians,							//!< Radians
		kDegrees,							//!< Degrees
		kAngMinutes,						//!< Minutes of Arc
		kAngSeconds,						//!< Seconds of Arc
		kLast								//!< Last enum value
	};

	MAngle();
	MAngle( const MAngle & src );
	MAngle( double value, Unit u = kRadians );
 	~MAngle();

 	MAngle &     	operator=( const MAngle & other );

 	Unit    		unit() const;
 	double  		value() const;
 	MStatus         setUnit( Unit newUnit );
 	MStatus         setValue( double newValue );

BEGIN_NO_SCRIPT_SUPPORT:

    double          as( Unit otherUnit, MStatus *ReturnStatus = NULL ) const;

END_NO_SCRIPT_SUPPORT:

 	double         	asUnits( Unit otherUnit, MStatus *ReturnStatus = NULL ) const;
 	double         	asRadians() const;
 	double         	asDegrees() const;
 	double         	asAngMinutes() const;
 	double         	asAngSeconds() const;

 	static Unit		uiUnit();
 	static MStatus	setUIUnit( Unit newUnit );

	static Unit		internalUnit();
	static double	internalToUI( double internalValue );
	static double	uiToInternal( double uiValue );

	static const char* className();

	// Obsolete
    OPENMAYA_DEPRECATED(2019, "Changing the internal units is generally not a good idea and using the method may often produce unexpected results. It is recommended to just read the values and convert them to the units desired.")
	static MStatus  setInternalUnit( Unit internalUnit );

protected:
 	double 	val;
	Unit	valUnit;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAngle */
