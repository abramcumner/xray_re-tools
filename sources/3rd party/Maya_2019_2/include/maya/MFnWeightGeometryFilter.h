#ifndef _MFnWeightGeometryFilter
#define _MFnWeightGeometryFilter
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
// CLASS:    MFnWeightGeometryFilter
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnGeometryFilter.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnWeightGeometryFilter)

//! \ingroup OpenMayaAnim MFn
//! \brief Weight geometry filter function set 
/*!
MFnWeightGeometryFilter is the function set for weight geometry
filters.  Weight geometry filter nodes include clusters, cluster
flexors, and user-defined deformers derived from MPxDeformerNode.  The
purpose of the weight geometry filter is to store the weights for each
component of each geometry that is deformed.  The weight geometry
filter is independent of any algorithm that calculates a deformation
based on the component weight values.  Clusters, cluster flexors, and
user-defined deformers each have their own algorithm to determine the
deformation based on the component weights.

This function set provides methods for getting and setting weights on
weight geometry filter nodes.  In addition to getting and settings
weights on components, this class provides methods for accessing the
weight geometry filter's input and output geometry through its parent
class, MFnGeometryFilter.

When getting and setting weights on components, it is more
efficient to use the methods that take a plug index.  The plug index
is a sparse array index, and is the same index used in MEL scripts to
index into an array of plugs.
*/
class OPENMAYAANIM_EXPORT MFnWeightGeometryFilter : public MFnGeometryFilter
{
	declareMFn(MFnWeightGeometryFilter, MFnGeometryFilter);

public:
	MStatus		getWeights(unsigned int index,
						   const MObject &components,
						   MFloatArray &weights) const;
	MStatus		getWeights(const MDagPath &path,
						   const MObject &components,
						   MFloatArray &weights) const;
	MStatus		setWeight(const MDagPath &path,
						  unsigned int index,
						  const MObject &components,
						  float weight,
						  MFloatArray *oldValues = NULL);
	MStatus		setWeight(const MDagPath &path,
						  const MObject &components,
						  float weight,
						  MFloatArray *oldValues = NULL);
	MStatus		setWeight(const MDagPath &path,
						  unsigned int index,
						  const MObject &components,
						  MFloatArray &values);
	MStatus		setWeight(const MDagPath &path,
						  const MObject &components,
						  MFloatArray &values);
	MString		weightPlugStrings(const MSelectionList &list,
									 MStatus * ReturnStatus = NULL ) const;
	MStatus		getWeightPlugStrings(const MSelectionList &list,
									 MStringArray &plugStringArray) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnWeightGeometryFilter, MFnGeometryFilter );

	//!     NO SCRIPT SUPPORT
	MStatus		getWeightPlugStrings(const MSelectionList &list,
									 MString &plugStrings) const;

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnWeightGeometryFilter */
