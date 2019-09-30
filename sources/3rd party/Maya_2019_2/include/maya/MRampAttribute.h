#ifndef _MRampAttribute
#define _MRampAttribute

// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//
// CLASS:    MRampAttribute
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MFloatArray.h>
#include <maya/MColorArray.h>
#include <maya/MIntArray.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRampAttribute)

//! \ingroup OpenMaya
//! \brief Create and manipulate ramp attributes.
/*!
	A ramp attribute consists of a set of entries describing a curve which
	is used to map an input value onto an output value. Each entry consists
	of an input \b position ranging from 0 to 1, an output \b value that
	position maps onto, and an enumerator indicating the type of interpolation
	to use between that position and the next.

	Currently two types of output values are a supported: a float, in which
	case the attribute is displayed as a curve in the Attribute Editor, or
	a color, in which case the attribute is displayed as a color gradient,
	similar to how a ramp texture node is displayed.

	The preferred way of creating a ramp is to use the \b createRamp set of methods. These
	methods will initialize the newly-created ramp with input values.

	A ramp must always have at least one entry. Ramp attributes created with the \b createCurveRamp
	and \b creatColorRamp methods will automatically be given a default entry at position 0.0 with a value of
	0.0 or black, depending upon the ramp type.

	Any attempt to delete the last of a ramp's entries will fail. This means that if you want to initialize a
	newly-created ramp attribute with a specific set of entries you must add at least one of your new entries
	first before deleting the default entry. Alternatively, you can	modify the default entry to match one of
	your new entries and then add your remaining entries.

	The \b setRamp set of methods can be used for setting existing ramps with new entries, current entries are
	automatically removed and replaced with the new ones. \b setRamp is perfect for initializing ramps created
	with the \b createCurveRamp and \b creatColorRamp.
*/
class OPENMAYA_EXPORT MRampAttribute
{
public:

	MRampAttribute ();
	MRampAttribute (const MRampAttribute& other);

	MRampAttribute (const MPlug& obj, MStatus* ReturnStatus = nullptr);

	MRampAttribute (const MObject& node, const MObject& attr, MStatus* ReturnStatus = nullptr);

	MRampAttribute&    operator =( const MRampAttribute & other );

	~MRampAttribute ();

	//! Interpolation methods.
	enum MInterpolation	{
		kNone = 0,	//!< \nop
		kLinear = 1,	//!< \nop
		kSmooth = 2,	//!< \nop
		kSpline = 3	//!< \nop
	};

	unsigned int getNumEntries (MStatus * ReturnStatus = nullptr);
	void getEntries (MIntArray& indexes,
					MFloatArray& positions,
					MFloatArray& values,
					MIntArray& interps,
					MStatus * ReturnStatus = nullptr);
	void getEntries (MIntArray& indexes,
					MFloatArray& positions,
					MColorArray& colors,
					MIntArray& interps,
					MStatus * ReturnStatus = nullptr);
	void addEntries(MFloatArray& positions,
					MColorArray& values,
					MIntArray& interps,
					MStatus * ReturnStatus = nullptr);
	void addEntries(MFloatArray& positions,
					MFloatArray& values,
					MIntArray& interps,
					MStatus * ReturnStatus = nullptr);
	void deleteEntries(MIntArray& indexes,
					   MStatus * ReturnStatus = nullptr);
	void setColorAtIndex		(MColor& color, unsigned int index, MStatus * ReturnStatus = nullptr);
	void setValueAtIndex		(float value, unsigned int index, MStatus * ReturnStatus = nullptr);
	void setPositionAtIndex		(float position, unsigned int index, MStatus * ReturnStatus = nullptr);
	void setInterpolationAtIndex(MRampAttribute::MInterpolation interp, unsigned int index, MStatus * ReturnStatus = nullptr);
	bool isColorRamp			(MStatus * ReturnStatus = nullptr) const;
	bool isCurveRamp			(MStatus * ReturnStatus = nullptr) const;
	void getColorAtPosition		(float position, MColor& color, MStatus * ReturnStatus = nullptr);
	void getValueAtPosition		(float position, float& value, MStatus * ReturnStatus = nullptr);
	void sampleColorRamp		(unsigned int numberOfSamples, MColorArray& colors, MStatus * ReturnStatus = nullptr);
	void sampleValueRamp		(unsigned int numberOfSamples, MFloatArray& values, MStatus * ReturnStatus = nullptr);

	bool hasIndex	(unsigned int index, MStatus* ReturnStatus = nullptr) const;
	MStatus sort	(bool ascending = true);
	MStatus pack	();

	MStatus setRamp(float value, float position = 1.0f, MInterpolation interp = kLinear);
	MStatus setRamp(const MColor& color, float position = 1.0f, MInterpolation interp = kLinear);
	MStatus setRamp(const MFloatArray& values, const MFloatArray& positions, const MIntArray& interps);
	MStatus setRamp(const MColorArray& values, const MFloatArray& positions, const MIntArray& interps);

	static MObject createCurveRamp(	const MString& attrLongName, const MString& attrShortName, MStatus* ReturnStatus=nullptr );

	static MObject createColorRamp(	const MString& attrLongName, const MString& attrShortName, MStatus*	ReturnStatus = nullptr);

	static MObject createRamp( const MString& attrLongName, const MString& attrShortName, const MObject& node,
							   float val, float pos=1.f, MInterpolation interp = kLinear, MStatus* ReturnStatus=nullptr );
	static MObject createRamp( const MString& attrLongName, const MString& attrShortName, const MObject& node,
							   const MColor& color, float pos=1.0f, MInterpolation interp = kLinear,  MStatus* ReturnStatus=nullptr );
	static MObject createRamp( const MString& attrLongName, const MString& attrShortName, const MObject& node,
							   const MFloatArray& values,  const MFloatArray& positions, const MIntArray& interps, MStatus* ReturnStatus=nullptr );
	static MObject createRamp( const MString& attrLongName, const MString& attrShortName, const MObject& node,
							   const MColorArray& colors, const MFloatArray& positions, const MIntArray& interps, MStatus* ReturnStatus=nullptr );

	static const char* className();

protected:
// No protected members

private:
	const void * fData;

//======================================================================
// Obsolete variations

public:

	OPENMAYA_DEPRECATED(2018, "Call getColorAtPosition(float, MColor&, MStatus*) instead. Use MDGContextGuard to switch the context if needed.")
	void getColorAtPosition (float position, MColor& color, MStatus * ReturnStatus, MDGContext&);

	OPENMAYA_DEPRECATED(2018, "Call getValueAtPosition(float, float&, MStatus*) instead. Use MDGContextGuard to switch the context if needed.")
	void getValueAtPosition (float position, float& value,  MStatus * ReturnStatus, MDGContext&);

	OPENMAYA_DEPRECATED(2018, "Call sampleColorRamp(unsigned int, MColorArray&, MStatus*) instead. Use MDGContextGuard to switch the context if needed.")
	void sampleColorRamp	(unsigned int numberOfSamples, MColorArray& colors, MStatus * ReturnStatus, MDGContext&);

	OPENMAYA_DEPRECATED(2018, "Call sampleValueRamp(unsigned int, MFloatArray&, MStatus*) instead. Use MDGContextGuard to switch the context if needed.")
	void sampleValueRamp	(unsigned int numberOfSamples, MFloatArray& values, MStatus * ReturnStatus, MDGContext&);
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MFnRampDataAttribute */

