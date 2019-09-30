#ifndef _MFnInstancer
#define _MFnInstancer
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
// CLASS:    MFnInstancer
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MFnDagNode.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnInstancer)

//! \ingroup OpenMayaFX MFn
//! \brief Particle Instancer object access class 
/*!
	Class for obtaining information about a particle instancer node.
*/
class OPENMAYAFX_EXPORT MFnInstancer : public MFnDagNode
{
    declareDagMFn(MFnInstancer, MFnDagNode);

public:

	int		particleCount	() const;

	int			instancesForParticle	(int p, MDagPathArray& paths,
										 MMatrix& instancerMatrix,
										 MStatus* ReturnStatus=NULL) const;

	MStatus		allInstances	( MDagPathArray& paths, MMatrixArray& matrices,
								  MIntArray& particlePathStartIndices,
								  MIntArray& pathIndices ) const;

BEGIN_NO_SCRIPT_SUPPORT:

        declareDagMFnConstConstructor( MFnInstancer, MFnDagNode );

END_NO_SCRIPT_SUPPORT:

protected:

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnInstancer */
