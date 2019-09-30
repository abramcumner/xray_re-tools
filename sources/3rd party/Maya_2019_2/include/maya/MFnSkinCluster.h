#ifndef _MFnSkinCluster
#define _MFnSkinCluster
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
// CLASS:    MFnSkinCluster
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnGeometryFilter.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnSkinCluster)

//! \ingroup OpenMayaAnim MFn
//! \brief skinCluster function set 
/*!
MFnSkinCluster is the function set for skinClusters. SkinCluster nodes
are created during a smooth bindSkin.  The purpose of the skinCluster
is to store a weight per influence object for each component of each
geometry that is deformed. Influence objects can be joints or any
transform.

Note that unlike most deformers, a skinCluster node can deform only a
single geometry. Therefore, if additional geometries are added to the
skinCluster set, they will be ignored.

This function set provides methods for getting and setting weights on
skin cluster nodes.  It parent class, MFnGeometryFilter provides
methods for accessing the skin cluster's input and output geometry.
*/
class OPENMAYAANIM_EXPORT MFnSkinCluster : public MFnGeometryFilter
{
	declareMFn(MFnSkinCluster, MFnGeometryFilter);

public:
	MStatus		getWeights(const MDagPath& path,
						   const MObject& components,
						   unsigned int influenceIndex,
						   MDoubleArray& weights) const;
	MStatus		getWeights(const MDagPath& path,
						   const MObject& components,
						   MDoubleArray& weights,
						   unsigned int& influenceCount) const;
	MStatus		getWeights(const MDagPath& path,
						   const MObject& components,
						   MIntArray& influenceIndices,
						   MDoubleArray& weights) const;
	MStatus		setWeights(const MDagPath& path,
						   const MObject& components,
						   unsigned int jointIndex,
						   double value,
						   bool normalize = true,
						   MDoubleArray *oldValues = NULL);
	MStatus		setWeights(const MDagPath& path,
						   const MObject& components,
						   MIntArray& influenceIndices,
						   MDoubleArray& values,
						   bool normalize = true,
						   MDoubleArray *oldValues = NULL);
	MStatus		getPointsAffectedByInfluence(const MDagPath& path,
											 MSelectionList& result,
											 MDoubleArray& weights);
	unsigned int 	indexForInfluenceObject(const MDagPath& path,
										MStatus *ReturnStatus = NULL) const;
	unsigned int 	influenceObjects(MDagPathArray& paths,
								 MStatus *ReturnStatus = NULL) const;
	MStatus		getWeights(const MDagPath& path,
						   const MObject& components,
						   unsigned int influenceIndex,
						   MFloatArray& weights) const;
	MStatus		getWeights(const MDagPath& path,
						   const MObject& components,
						   MFloatArray& weights,
						   unsigned int& influenceCount) const;
	MStatus		setWeights(const MDagPath& path,
						   const MObject& components,
						   unsigned int jointIndex,
						   float value,
						   bool normalize = true,
						   MFloatArray *oldValues = NULL);
	MStatus		setWeights(const MDagPath& path,
						   const MObject& components,
						   MIntArray& influenceIndices,
						   MFloatArray& values,
						   bool normalize = true,
						   MFloatArray *oldValues = NULL);
	MStatus		getPointsAffectedByInfluence(const MDagPath& path,
											 MSelectionList& result,
											 MFloatArray& weights);

	MStatus		getBlendWeights(const MDagPath& path,
						   const MObject& components,
						   MDoubleArray& weights) const;

	MStatus		setBlendWeights(const MDagPath& path,
						   const MObject& components,
						   const MDoubleArray& weights);


BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnSkinCluster, MFnGeometryFilters );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
	bool		isValidComponentType(const MObject& comp) const;
	MStatus		getWeightsHelper(bool useInfluenceIndex,
								 unsigned int influenceIndex,
								 const MDagPath& path,
								 const MObject& components,
								 unsigned int& influenceCount,
								 MDoubleArray& weights) const;
	MStatus		getWeightsHelper(const MDagPath& path,
					 const MObject& components,
					 const MIntArray& infArray,
					 MDoubleArray& weights) const;

	MStatus		setWeightsHelper(const MDagPath& path,
								 const MObject& components,
								 bool useInfluenceIndex,
								 unsigned int influenceIndex,
								 const MIntArray& infArray,
								 const MDoubleArray& wtArray,
								 bool normalize /* = true */,
								 MDoubleArray *oldValues /* = NULL */);

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnSkinCluster */
