#ifndef _MFnLattice
#define _MFnLattice
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
// CLASS:    MFnLattice
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MObject.h>
#include <maya/MTransformationMatrix.h>


// ****************************************************************************
// DECLARATIONS

class MPtrBase;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnLattice)

//! \ingroup OpenMayaAnim MFn
//! \brief Lattice function set 
/*!
MFnLattice is the function set for lattice shapes and lattice geometry.  It can
be used on lattices in the DAG or on lattice geometry from a dependency node
attribute.

Lattices are most commonly used for specifying FFDs (free-form deformations).
See MFnLatticeDeformer for more information on those.

MFnLatticeData can be used to create new lattice data objects for use with
dependency node attributes.
*/
class OPENMAYAANIM_EXPORT MFnLattice : public MFnDagNode
{
	declareDagMFn(MFnLattice, MFnDagNode);

public:

	MObject 	create( unsigned int xDiv, unsigned int yDiv, unsigned int zDiv,
						MObject parentOrOwner = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );
	MStatus     reset( double sSize = 1.0 , double tSize = 1.0,
						double uSize = 1.0 );
	MStatus     getDivisions( unsigned int & s, unsigned int & t, unsigned int & u );
	MStatus     setDivisions( unsigned int   s, unsigned int   t, unsigned int   u );

	MPoint &    point( unsigned int s, unsigned int t, unsigned int u,
					   MStatus* ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnLattice, MFnDagNode );

END_NO_SCRIPT_SUPPORT:

protected:
	virtual bool objectChanged( MFn::Type, MStatus * );
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnLattice */
