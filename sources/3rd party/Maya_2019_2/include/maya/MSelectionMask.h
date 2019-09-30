#ifndef _MSelectionMask
#define _MSelectionMask
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
// CLASS:    MSelectionMask
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MSelectionMask)

//! Manage what is selectable in Maya. (OpenMaya) (OpenMaya.py)
/*!
    Selection masks provide a way to control what is selectable in Maya.

    Selection masks can be created by OR'ing several types together, or
    by using the addMask method to add a selection type to an MSelectionMask.

    Example - to set the object selection mask to nurbsSurfaces, nurbsCurves,
              cameras, and lights

\code
    MSelectionMask mask( MSelectionMask::kSelectNurbsSurfaces );
    mask.addMask( MSelectionMask::kSelectNurbsCurves );
    mask = mask | MSelectionMask::kSelectCameras | MSelectionMask::kSelectLights;
    MGlobal::setObjectSelectionMask( mask );
\endcode
*/
class OPENMAYA_EXPORT MSelectionMask
{
public:
	//! Types of objects which can be selected.
	enum SelectionType {
		kSelectHandles,				//!< \nop
		kSelectLocalAxis,			//!< \nop

		kSelectIkHandles,			//!< \nop
		kSelectIkEndEffectors,			//!< \nop
		kSelectJoints,				//!< \nop

		kSelectLights,				//!< \nop
		kSelectCameras,				//!< \nop

		kSelectLattices,			//!< \nop
		kSelectClusters,			//!< \nop
		kSelectSculpts,				//!< \nop

		kSelectNurbsCurves,			//!< \nop
		kSelectNurbsSurfaces,			//!< \nop
		kSelectMeshes,				//!< \nop
		kSelectSubdiv,				//!< \nop
		kSelectSketchPlanes,			//!< \nop

		kSelectParticleShapes,			//!< \nop
		kSelectEmitters,			//!< \nop
		kSelectFields,				//!< \nop
		kSelectSprings,				//!< \nop
		kSelectRigidBodies,			//!< \nop
		kSelectRigidConstraints,		//!< \nop
		kSelectCollisionModels,			//!< \nop

		kSelectXYZLocators,			//!< \nop
		kSelectOrientationLocators,		//!< \nop
		kSelectUVLocators,			//!< \nop

		kSelectTextures,			//!< \nop

		kSelectCurves,				//!< \nop
		kSelectSurfaces,			//!< \nop
		kSelectLocators,			//!< \nop
		kSelectObjectsMask,			//!< \nop


		kSelectCVs,				//!< \nop
		kSelectHulls,				//!< \nop
		kSelectEditPoints,			//!< \nop

		kSelectMeshVerts,			//!< \nop
		kSelectMeshEdges,			//!< \nop
		kSelectMeshFreeEdges,			//!< \nop
		kSelectMeshFaces,			//!< \nop
		kSelectSubdivMeshPoints,		//!< \nop
		kSelectSubdivMeshEdges,			//!< \nop
		kSelectSubdivMeshFaces,			//!< \nop
		kSelectMeshUVs,				//!< \nop

		kSelectVertices,			//!< \nop
		kSelectEdges,				//!< \nop
		kSelectFacets,				//!< \nop
		kSelectMeshLines,			//!< \nop
		kSelectMeshComponents,			//!< \nop

		kSelectCurveParmPoints,			//!< \nop
		kSelectCurveKnots,			//!< \nop
		kSelectSurfaceParmPoints,		//!< \nop
		kSelectSurfaceKnots,			//!< \nop
		kSelectSurfaceRange,			//!< \nop
		kSelectSurfaceEdge,			//!< \nop
		kSelectIsoparms,			//!< \nop
		kSelectCurvesOnSurfaces,		//!< \nop
		kSelectPPStrokes,			//!< \nop

		kSelectLatticePoints,			//!< \nop

		kSelectParticles,			//!< \nop

		kSelectJointPivots,			//!< \nop
		kSelectScalePivots,			//!< \nop
		kSelectRotatePivots,			//!< \nop

		kSelectPivots,				//!< \nop

		kSelectSelectHandles,			//!< \nop

		kSelectComponentsMask,			//!< \nop

		kSelectAnimCurves,			//!< \nop
		kSelectAnimKeyframes,			//!< \nop
		kSelectAnimInTangents,			//!< \nop
		kSelectAnimOutTangents,			//!< \nop

		kSelectAnimMask,			//!< \nop
		kSelectAnimAny,				//!< \nop

		kSelectTemplates,			//!< \nop

		kSelectManipulators,			//!< \nop
		kSelectGuideLines,			//!< \nop
		kSelectPointsForGravity,		//!< \nop
		kSelectPointsOnCurvesForGravity,	//!< \nop
		kSelectPointsOnSurfacesForGravity,	//!< \nop
		kSelectObjectGroups,			//!< \nop
		kSelectSubdivMeshMaps,			//!< \nop
		kSelectFluids,				//!< \nop
		kSelectHairSystems,			//!< \nop
		kSelectFollicles,			//!< \nop
		kSelectNCloths,				//!< \nop
		kSelectNRigids,				//!< \nop
		kSelectDynamicConstraints,		//!< \nop
		kSelectNParticles			//!< \nop
	};

	MSelectionMask();
	MSelectionMask( SelectionType selType );
	MSelectionMask( const MString& selTypeName);
	MSelectionMask( const MSelectionMask& in );

	virtual ~MSelectionMask();

	MStatus		setMask( SelectionType selType );
	MStatus		setMask( MSelectionMask& mask );
	MStatus		addMask( SelectionType selType );

	bool		intersects( SelectionType selType,
							MStatus * ReturnStatus = NULL ) const;
	bool		intersects( MSelectionMask& mask,
							MStatus * ReturnStatus = NULL ) const;

	MSelectionMask	operator| ( SelectionType selType );
	MSelectionMask	operator| ( MSelectionMask& mask );
	MSelectionMask&	operator =( const MSelectionMask& other );
	static bool registerSelectionType(const MString& selTypeName, int priority = 0);

	static bool deregisterSelectionType(const MString& selTypeName);

	static int getSelectionTypePriority(const MString& selTypeName);

protected:
// No protected members

private:
	MSelectionMask( const void *, bool );
	const void * data;
	bool fOwn;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MSelectionMask */
