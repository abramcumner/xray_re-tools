#ifndef _MFnRenderLayer
#define _MFnRenderLayer
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
// CLASS:    MFnRenderLayer
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnRenderLayer)

//! \ingroup OpenMayaRender MFn
//! \brief Function set for render layer.
/*!
Provide functionalities for working with render layers such as
getting the objects present in the render layer or checking whether the
given object is in the current layer.
*/
class OPENMAYARENDER_EXPORT MFnRenderLayer : public MFnDependencyNode
{
	declareMFn(MFnRenderLayer, MFnDependencyNode);

public:

	bool				inLayer( const MObject &transform,
								 MStatus *ReturnStatus = NULL );

	MStatus				layerChildren(MObjectArray &children, bool recurse = false);

	bool				inCurrentRenderLayer( const MDagPath& objectPath,
											  MStatus *ReturnStatus = NULL );
	MStatus				listMembers( MObjectArray& objectsInLayer ) const;

	bool				isPlugAdjusted( const MPlug& scenePlug,
										MStatus *ReturnStatus = NULL) const;

    MPlug               adjustmentPlug( const MPlug& scenePlug,
                                        MStatus *ReturnStatus = NULL) const;

	MStatus				externalRenderPasses( MObjectArray& renderPassArray ) const;

    bool                passHasObject( const MObject& renderPass, const MDagPath& objectPath, MStatus *ReturnStatus = NULL ) const;

    bool                passHasLight( const MObject& renderPass, const MObject& light, MStatus *ReturnStatus = NULL ) const;

	static MObject		findLayerByName( const MString &renderLayer,
										 MStatus *ReturnStatus = NULL );

	static MObject		defaultRenderLayer( MStatus *ReturnStatus = NULL );

	static MStatus		listAllRenderLayers( MObjectArray &rl );

	static MObject		currentLayer( MStatus *ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnRenderLayer, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected members

private:
	void *renderLayer;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnRenderLayer */
