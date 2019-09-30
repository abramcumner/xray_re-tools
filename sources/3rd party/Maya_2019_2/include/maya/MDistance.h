#ifndef _MDistance
#define _MDistance
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
// CLASS:    MDistance
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MDeprecate.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDistance)

//! \ingroup OpenMaya
//! \brief Manipulate Linear Data. 
/*!
The MDistance class provides a fundamental type for the Maya API to hold
and manipulate linear data.  All API methods that require or return
distance information do so through variables of this type.
*/
class OPENMAYA_EXPORT MDistance
{
public:
	//! Available Unit Systems
	enum Unit
	{
		kInvalid,
		//! Inches
		kInches,
		//! Feet
		kFeet,
		//! Yards
		kYards,
		//! Miles
		kMiles,
		//! Millimeters
		kMillimeters,
		//! Centimeters
		kCentimeters,
		//! Kilometers
		kKilometers,
		//! Meters
		kMeters,
		kLast
	};
					MDistance();
					MDistance( double value, Unit unitSystem = kCentimeters );
					MDistance( const MDistance& src );
					~MDistance();
	MDistance&		operator=( const MDistance& src );
	Unit			unit() const;
	double			value() const;
	MStatus			setUnit( Unit newUnit );
	MStatus			setValue( double newValue );

BEGIN_NO_SCRIPT_SUPPORT:
	double			as( Unit newUnit, MStatus *ReturnStatus = NULL ) const;
END_NO_SCRIPT_SUPPORT:

	double          asUnits( Unit newUnit, MStatus *ReturnStatus = NULL ) const;
	double			asInches() const;
	double			asFeet() const;
	double			asYards() const;
	double			asMiles() const;
	double			asMillimeters() const;
	double			asCentimeters() const;
	double			asKilometers() const;
	double			asMeters()  const;
	static Unit		uiUnit();
	static MStatus	setUIUnit( Unit newUnit );
	static Unit		internalUnit();
	static double	internalToUI( double internalValue );
	static double	uiToInternal( double uiValue );

	static const char* className();

	// Obsolete
	// Changing the internal units is generally not a good idea and
	// using this method may often produce unexpected results.
    OPENMAYA_DEPRECATED(2019, "Changing the internal units is generally not a good idea and using the method may often produce unexpected results. It is recommended to just read the values and convert them to the units desired.")
	static MStatus  setInternalUnit( Unit internalUnit );

protected:
	double			val;
	Unit			valUnit;

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDistance */
