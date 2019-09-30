#ifndef _MPxBlendShape
#define _MPxBlendShape
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//
// CLASS:    MPxBlendShape
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
// CLASS DECLARATION (MPxBlendShape)



OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMayaAnim MPx
//! \brief Base class for user-defined blendshape deformers 
/*!
 MPxBlendShape allows the creation of user-defined blendshape deformers. 
 It derives from MPxGeometryFilter and so offers all the functionality of that class.
 Additionally, it has the input target and all other attributes
 of the Maya built-in blendShape node.

 Custom nodes derived from MPxBlendShape are treated by Maya just like the built-in blendShape,
 so all the weight painting/editing etc. tools that artists are used to also work on the custom nodes.
*/
class OPENMAYAANIM_EXPORT MPxBlendShape : public MPxGeometryFilter
{
public:

	MPxBlendShape();

	virtual ~MPxBlendShape();

	virtual MPxNode::Type type() const;

	// Methods to overload

	// deformData is called by computePlug when an output geometry
	// value is evaluated
	//

    virtual MStatus        deformData(MDataBlock& block,
								  MDataHandle geomData,
								  unsigned int groupId,
								  const MMatrix& mat,
								  unsigned int multiIndex);

	// Inherited attributes
	//! weight attribute, multi
	static MObject weight;
	//! inputTarget attribute, multi
	static MObject inputTarget;
	//! inputTargetGroup attribute, multi
	static MObject inputTargetGroup;
	//! inputTargetItem attribute, multi
	static MObject inputTargetItem;
	//! inputGeomTarget attribute
	static MObject inputGeomTarget;
	//! inputPointsTarget attribute
	static MObject inputPointsTarget;
	//! inputComponentsTarget attribute
	static MObject inputComponentsTarget;
	//! targetWeights attribute, multi
	static MObject targetWeights;

	static const char*	    className();

protected:
// No protected members

private:
	static void				initialSetup();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxBlendShape */
