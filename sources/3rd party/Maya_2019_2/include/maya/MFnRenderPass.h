#ifndef _MFnRenderPass
#define _MFnRenderPass
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
// CLASS:    MFnRenderPass
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MTypes.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPxRenderPassImpl.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnRenderPass)

//! \ingroup OpenMayaRender MFn
//! \brief Function set for render passes.
/*!
Provide functionalities for working with render passes such as
retrieving renderer-specific implementations.
*/

class OPENMAYARENDER_EXPORT MFnRenderPass : public MFnDependencyNode
{
	declareMFn(MFnRenderPass, MFnDependencyNode);

public:

	MPxRenderPassImpl* setImplementation( 
		const MString& renderer, 
		MStatus *ReturnStatus = NULL 
		);

	MPxRenderPassImpl* getImplementation() const;

	MPxRenderPassImpl::PassTypeBit frameBufferType( MStatus *ReturnStatus = NULL ) const;

	unsigned int				frameBufferChannels( MStatus *ReturnStatus = NULL ) const;

	bool                        usesFiltering( MStatus *ReturnStatus = NULL ) const;

	MString						passID( MStatus *ReturnStatus = NULL ) const;

	MString						customTokenString( MStatus *ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnRenderPass, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected members

private:
	void *renderPass;

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnRenderPass */
