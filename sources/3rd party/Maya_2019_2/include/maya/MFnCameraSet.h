
#ifndef _MFnCameraSet
#define _MFnCameraSet
//
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
// CLASS:    MFnCameraSet
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MString.h>
#include <maya/MObject.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

// *****************************************************************************
// CLASS DECLARATION (MFnCameraSet)

//! \ingroup OpenMaya MFn
//! \brief Camera set node function set.
/*!
	MFnCameraSet is the function set that is used for manipulating cameraSet
	nodes.

	A cameraSet node defines a list of camera layeres to be processed in order,
	defining a multi-layer viewing of the scene.  A camera layer consists of an
	object set to be used by the layer, and a camera through which the set is
	viewed. Individual layers may be marked active or inactive and ordered
	arbitrarily.
*/
class OPENMAYA_EXPORT MFnCameraSet : public MFnDependencyNode 
{

	declareMFn(MFnCameraSet, MFnDependencyNode);

public:
	
	MObject     	create( MStatus * ReturnStatus = NULL );
	MObject     	create( const MDagPathArray &camList, MStatus *returnStatus = NULL );
	MObject     	create( const MString &name, MStatus * ReturnStatus = NULL );
	MObject     	create( const MDagPathArray &camList, const MObjectArray &setList, MStatus *returnStatus = NULL );
	MObject     	create( const MString &name, const MDagPathArray &camList, const MObjectArray &setList, MStatus *returnStatus = NULL );
	MStatus     	appendLayer( const MDagPath &camera, const MObject &set );
	MStatus     	appendLayer( const MDagPath &camera );
	MStatus     	insertLayer( unsigned int index, const MDagPath &camera, const MObject &set );
	MStatus     	insertLayer( unsigned int index, const MDagPath &camera );
	MStatus     	deleteLayer( unsigned int index );
	MStatus     	clear();
	unsigned int    getNumLayers( MStatus *returnStatus = NULL );
	MStatus     	setLayerCamera( unsigned int index, const MDagPath &camera );
	MStatus 		getLayerCamera( unsigned int index, MDagPath & camera );
	MStatus     	setLayerSceneData( unsigned int index, const MObject &set );
	MStatus 		getLayerSceneData( unsigned int index, MObject &set );
	MStatus     	setLayerActive( unsigned int index, bool isActive );
	bool			isLayerActive( unsigned int index, MStatus *returnStatus = NULL );
	MStatus     	setLayerOrder( unsigned int index, int order );
	MStatus			getLayerOrder( unsigned int index, int &order );
	MStatus			getSortedIndices( MUintArray &sortedIndices );
	MStatus			setLayerClearDepthValue( unsigned int index, bool depth );
	MStatus			getLayerClearDepthValue( unsigned int index, bool &depth ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnCameraSet, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

private:
	void			updateAttributeEditor();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

OPENMAYA_NAMESPACE_CLOSE
#endif /* __cplusplus */
#endif /* _MFnCameraSet */
