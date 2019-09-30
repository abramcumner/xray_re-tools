#ifndef _MCurveAttribute
#define _MCurveAttribute
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
// CLASS:    MCurveAttribute
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MCurveAttribute)
//
// This class is used to manipulate curve attributes.  You can add,
// delete, and set entries in curve attributes.
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MFloatArray.h>
#include <maya/MIntArray.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MCurveAttribute)

//! \ingroup OpenMaya
//! \brief Create and manipulate curve attributes.
/*!
	A curve attribute consists of a set of entries describing a 3rd degree NURBS curve which
	is used to map an input value onto an output value. Each entry consists
	of an input \b position ranging from 0 to 1, an output \b value that
	position maps onto. The (position, value) pairs form a control vertex for the NURBS curve.

	The preferred way of creating a curve attribute is to use the \b createCurve or \b createCurveAttr methods. 
	Once created, curve widgets can be created for those attributes using the \b falloffCurve or \b falloffCurveAttr 
	MEL commands.

	A curve must always have at least four control vertices since it is a third degree NURBS curve. 

*/
class OPENMAYA_EXPORT MCurveAttribute
{
public:

	MCurveAttribute();
	MCurveAttribute( const MCurveAttribute& other );
	MCurveAttribute( const MPlug& obj, MStatus* ReturnStatus = NULL );
	MCurveAttribute( const MObject& node, const MObject& attr, MStatus* ReturnStatus = NULL );
	MCurveAttribute& operator =( const MCurveAttribute & other );
	~MCurveAttribute();

	unsigned int getNumEntries( MStatus * returnStatus = NULL );
	void getEntries(	MIntArray& indexes,
						MFloatArray& positions,
						MFloatArray& values,
						MStatus * returnStatus = NULL );
	void addEntries(	MFloatArray& positions,
						MFloatArray& values,
						MStatus * returnStatus = NULL );
	void deleteEntries(	MIntArray& indexes,
						MStatus * returnStatus = NULL );
	void setValueAtIndex( float value, unsigned int index, MStatus * returnStatus = NULL );
	void setPositionAtIndex( float position, unsigned int index, MStatus * returnStatus = NULL );
	void getValuesAtPositions(	const MFloatArray& positions, 
								MFloatArray& values,
								MStatus* returnStatus = NULL );
	void getValueAtPosition(	float position, 
								float& value, 
								MStatus * returnStatus = NULL );
	void sampleValueCurve(	unsigned int numberOfSamples, 
							MFloatArray& values, 
							MStatus * returnStatus = NULL );

	bool hasIndex( unsigned int index, MStatus* ReturnStatus = NULL ) const;
	MStatus sort( bool ascending = true );
	MStatus pack();

	MStatus setCurve( const MFloatArray& values, const MFloatArray& positions );

	static MObject createCurveAttr(	const MString& attrLongName, 
									const MString& attrShortName, 
									MStatus* ReturnStatus=NULL );
	static MObject createCurve(	const MString& attrLongName, 
								const MString& attrShortName, 
								const MObject& node, 
								const MFloatArray& values,
								const MFloatArray& positions,
								MStatus* ReturnStatus=NULL );

	static const char* className();

protected:
// No protected members

private:
	const void * fData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* MCurveAttribute */

