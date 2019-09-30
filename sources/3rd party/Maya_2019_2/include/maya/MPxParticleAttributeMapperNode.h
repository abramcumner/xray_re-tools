#ifndef _MPxParticleAttributeMapperNode
#define _MPxParticleAttributeMapperNode
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
// CLASS:    MPxParticleAttributeMapperNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MPxNode.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxParticleAttributeMapperNode)

//! \ingroup OpenMayaFX MPx
//! \brief User defined per particle attribute mapping nodes.
/*!
  MPxParticleAttributeMapperNode is the parent class of all user
  defined per particle attribute mapping nodes. This class extends
  Maya's internal 'arrayMapper' node, inheriting its attributes and
  default behaviour.
*/
class OPENMAYAFX_EXPORT MPxParticleAttributeMapperNode : public MPxNode
{
public:

	MPxParticleAttributeMapperNode();
	virtual ~MPxParticleAttributeMapperNode();
	virtual MPxNode::Type type() const;
	virtual MStatus	compute(const MPlug& plug, MDataBlock& dataBlock);

	//
    // Inherited attributes
	//

	//! Input node used to compute output values
	static MObject computeNode;

	//! The U component of an array of sample positions.
	static MObject uCoordPP;

	//! The V component of an array of sample positions.
	static MObject vCoordPP;

	//! The minimum limit on the output attribute outValuePP
	static MObject outMinValue;

	//! The maximum limit on the output attribute outValuePP
	static MObject outMaxValue;

	//! This attribute allows the propagation of any changes to the
	// computeNode's outColor through the arrayMapper and onto the attribute
	// that the particleAttributeNode is driving.
	static MObject computeNodeColor;

	//! The R component of the computeNodeColor
	static MObject computeNodeColorR;

	//! The G component of the computeNodeColor
	static MObject computeNodeColorG;

	//! The B component of the computeNodeColor
	static MObject computeNodeColorB;

	//! This attribute holds the value for time (in seconds)
	static MObject time;

	//! The outputColor vector per-particle attribute
	static MObject outColorPP;

	//! The outputValue vector per-particle attribute
	static MObject outValuePP;

	static const char*	    className();

private:
	static void				initialSetup();

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxParticleAttributeMapperNode */
