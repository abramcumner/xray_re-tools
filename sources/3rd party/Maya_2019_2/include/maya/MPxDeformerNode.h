#ifndef _MPxDeformerNode
#define _MPxDeformerNode
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//
// CLASS:    MPxDeformerNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxGeometryFilter.h>
#include <maya/MSelectionList.h>


// ****************************************************************************
// CLASS DECLARATION (MPxDeformerNode)



OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMayaAnim MPx
//! \brief Base class for user defined deformers with per-vertex weights
/*!
 MPxDeformerNode builds on MPxGeometryFilter to allow the creation of deformers
 with per-vertex weights. Built-in Maya nodes which work in this way include
 the jiggle and cluster deformers.

 The weight values can be modified by the user using the set editing window or the percent command.
*/
class OPENMAYAANIM_EXPORT MPxDeformerNode : public MPxGeometryFilter
{
public:

	MPxDeformerNode();

	virtual ~MPxDeformerNode();

	virtual MPxNode::Type type() const;

	// return the weight value for the given index pair
	//
	float						weightValue( MDataBlock& mblock,
											 unsigned int multiIndex,
											 unsigned int wtIndex);

	void					setUseExistingConnectionWhenSetEditing(bool state);

	MStatus					setDeformationDetails(unsigned int flags);
	unsigned int			getDeformationDetails(MStatus * ReturnStatus = NULL);

	// Inherited attributes
	//! weight list attribute, multi
	static MObject weightList;
	//! weight attribute, multi
	static MObject weights;

	static const char*	    className();

protected:
// No protected members

private:
	static void				initialSetup();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxNode */
