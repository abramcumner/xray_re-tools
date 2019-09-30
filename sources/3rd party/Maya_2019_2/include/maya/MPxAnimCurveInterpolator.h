#ifndef _MPxAnimCurveInterpolator
#define _MPxAnimCurveInterpolator
//
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
// CLASS:    MPxAnimCurveInterpolator
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxAnimCurveInterpolator)
//
// This class allows plugin developers to subclass their own animation curve
// interpolators.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MFnAnimCurve.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// *****************************************************************************

// CLASS DECLARATION (MPxAnimCurveInterpolator)

//! \ingroup  OpenMaya MPx
//! \brief Base Class for User-defined Animation Curve Interpolation Types
/*!
	MPxAnimCurveInterpolator is the base class for user defined Anim Curve Interpolation Types.
	It allows for the creation and evaluation of customized animation curves, in addition to 
	determination of the type ID and name of the curve. The evaluation of an animCurve between 
	two of its keyframes is determined by interpolators (also known as "tangent types") at those 
	keyframes. This class allows for the creation of custom tangent types. Note that the valid 
	type ranges are:

	Types 1 to 18		Maya's built-in tangent types. See MFnAnimCurve::TangentType.
	Types 19 to 26		Custom tangent types which are available to all users, 
						and should only be used internally.
	Types 27 to 63		Maya's built-in tangent types. See MFnAnimCurve::TangentType.
	Types 64 to 32767	Custom tangent types which can be reserved by customers. You can request 
						between 1 to 8 id's per request. Visit <a href="http://mayaid.autodesk.io/">
						Autodesk Maya Developer Tangent Types ID Block Registration</a> to submit
						your request. See also <a href="http://www.autodesk.com/developmaya">Autodesk Developer Network</a>
						for more information. 
*/

class OPENMAYAANIM_EXPORT MPxAnimCurveInterpolator  
{
public:

	//! Defines the flags used when registering a new animation curve iterpolator.	
	enum InterpolatorFlags {
		/*! Animation curves do not typically evaluate at the keyframe times.  Instead, the keyframe value
			is used.  For custom interpolators that may want to define their curves such that they do not
			pass through the keyframe values, <i>kEvaluateAtKey</i> can be set which will cause the interpolator
			to be evaluated at the keyframe times.
		*/
		kEvaluateAtKey	 = 0x001,

		/*! Many curve operations to move keys or change tangent types may cause a ripple of tangent type changes
			for neighboring keyframes to a tangent type known to be compatible with the new curve shape.  Some
			custom interpolators may be able to accomodate such changes to neighboring keyframes without being
			exchanged for a different type.  Setting <i>kLockType</i> will prevent the custom tangent type from
			being automatically exchanged.
		*/
		kLockType  		 = 0x002
	};

	virtual ~MPxAnimCurveInterpolator();

	// Initialize the interpolator to evaluate keyframe values within the time
	// span of the given interval.  The interval starts at the keyframe denoted
	// by the value of the interval and continues to the next keyframe.
	//
	virtual	void	initialize(const MObject &animCurve, unsigned int keyIndex);

	// Compute an interpolated keyframe value at the given time,
	// which is an absolute time between the start and end times.
	virtual	double	evaluate(const MTime &val) = 0;

	// Returns the registered typeId for this class.
	//
	MFnAnimCurve::TangentType typeId() const;

	// Returns the registered type name for this class.
	//
	MString         typeName() const;

protected:
	void*			instance;

private:
// No private members
};

// *****************************************************************************

OPENMAYA_NAMESPACE_CLOSE
#endif /* __cplusplus */
#endif /* _MPxAnimCurveInterpolator */
