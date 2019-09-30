#ifndef _MFnGeometryFilter
#define _MFnGeometryFilter
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
// CLASS:    MFnGeometryFilter
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnGeometryFilter)

//! \ingroup OpenMayaAnim MFn
//! \brief  geometry filter function set

/*!
  MFnGeometryFilter is the function set for geometry filters, the node
  that is the base class for deformers.  Geometry filter nodes include
  clusters, ffds, nonlinears, user-defined deformers, sculpts, wires
  and blendShapes.  The purpose of the geometry filter is to connect
  to the geometry that is deformed.  The geometry filter is
  independent of any algorithm that calculates the deformation.

  This function set provides methods for finding out which geometries
  are connected to geometry filter nodes.
*/
class OPENMAYAANIM_EXPORT MFnGeometryFilter : public MFnDependencyNode
{
	declareMFn(MFnGeometryFilter, MFnDependencyNode);

public:
	MStatus		getInputGeometry(MObjectArray &objects) const;
	MStatus		getOutputGeometry(MObjectArray &objects) const;
	MObject		inputShapeAtIndex(unsigned int index,
								  MStatus *ReturnStatus = NULL) const;
	MObject		outputShapeAtIndex(unsigned int index,
								   MStatus *ReturnStatus = NULL) const;
	unsigned int	indexForOutputShape(const MObject &shape,
									MStatus *ReturnStatus = NULL) const;
	MStatus		getPathAtIndex(unsigned int index,
							   MDagPath &path) const;
	unsigned int	indexForGroupId(unsigned int groupId,
								MStatus *ReturnStatus = NULL) const;
	unsigned int	groupIdAtIndex(unsigned int index,
							   MStatus *ReturnStatus = NULL) const;
	unsigned int	numOutputConnections(MStatus *ReturnStatus = NULL) const;
	unsigned int	indexForOutputConnection(unsigned int connectionIndex,
										 MStatus *ReturnStatus = NULL) const;
	MObject		deformerSet(MStatus *ReturnStatus = NULL) const;
	float		envelope(MStatus *ReturnStatus = NULL) const;
	MStatus		setEnvelope(float envelope);

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnGeometryFilter, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnGeometryFilter */
