#ifndef _MGPUDeformerRegistry
#define _MGPUDeformerRegistry
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//
//
// CLASS:    MGPUDeformerRegistry
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MGPUDeformerRegistry)
//
//  MGPUDeformerRegistry allows the user to register their classes
//  derived from MPxGPUDeformer so they work with deformerEvaluator.
//

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MString.h>
#include <maya/MStatus.h>
#include <maya/MTypeId.h>
#include <maya/MObject.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MGPUDeformerRegistrationInfo)

//! \ingroup OpenMayaAnim
//! \brief Registration information for an MPxGPUDeformer
/*!
 MGPUDeformerRegistrationInfo provides a creator method to
 allocate MPxGPUDeformer objects, validation methods to determine
 if a given node is supported by the MPxGPUDeformer, as well as 
 utility methods to tell the deformer evaluator which attributes are the
 input and output mesh attributes of the deformer node.

 You must implement createGPUDeformer(), validateNodeInGraph() and
 validateNodeValues().  outputMeshAttribute() and inputMeshAttribute() have
 default implementations which will work correctly if the node being
 overridden by MPxGPUDeformer is an MPxDeformerNode.

 If the attribute returned by outputMeshAttribute() is a multi-attribute or
 a multi parent, then the attribute returned by inputMeshAttribute() must
 also be a multi-attribute or have a multi parent.  The total number of multi
 attributes in the hierarchy of the output attribute (including the output
 attribute itself) must be exactly one.  The total number of multi
 attributes in the hierarchy of the input attribute (including the input
 attribute itself) must be exactly one.

 For example, geometryFilter's input geometry attribute is inputGeometry.
 inputGeometry is not a multi attribute, but it has a single multi parent
 (input).  geometryFilter's output geometry attribute is outputGeometry.
 outputGeometry is a multi attribute.  In this case the input and output
 attribute hierarchies each have a single multi, so the GPU override
 works.

 Maya will match the multi index of each multi attribute in the input and
 output multi hierarchies for each connected input and output plug.  In
 the geometryFilter example we use the multi index of the outputGeometry
 attribute as the multi index of aInput when attempting to determine if
 inputGeometry is connected.
*/

class OPENMAYAANIM_EXPORT MGPUDeformerRegistrationInfo
{
public:
	MGPUDeformerRegistrationInfo() {}
	virtual ~MGPUDeformerRegistrationInfo() {}

	virtual MPxGPUDeformer* createGPUDeformer() = 0;
	virtual bool validateNodeInGraph(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) = 0;
	virtual bool validateNodeValues(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) = 0;
	virtual const MObject outputMeshAttribute();
	virtual const MObject inputMeshAttribute();
};

// ****************************************************************************
// CLASS DECLARATION (MGPUDeformerRegistry)

//! \ingroup OpenMayaAnim
//! \brief Registration class for MPxGPUDeformer
/*!
 MGPUDeformerRegistry allows each class derived from
 MPxGPUDeformer to register the type of deformer it supports.
 Only one override registered at a time for each type of deformer node.
 Overrides provided by default are registered with registrantId "Maya".
*/

class OPENMAYAANIM_EXPORT MGPUDeformerRegistry
{
public:
	static MStatus registerGPUDeformerCreator(
		const MString& nodeClassName,
		const MString& registrantId,
		MGPUDeformerRegistrationInfo* registrationInfo);

	static MStatus deregisterGPUDeformerCreator(
		const MString& nodeClassName,
		const MString& registrantId);
	
	static MStatus addConditionalAttribute(
		const MString& nodeClassName,
		const MString& registrantId,
		const MObject& conditionalAttribute);
private:
	MGPUDeformerRegistry();
	~MGPUDeformerRegistry();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* MGPUDeformerRegistry */


