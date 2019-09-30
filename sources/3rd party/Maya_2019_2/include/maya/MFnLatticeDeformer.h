#ifndef _MFnLatticeDeformer
#define _MFnLatticeDeformer
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
// CLASS:    MFnLatticeDeformer
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnLatticeDeformer)

//! \ingroup OpenMayaAnim MFn
//! \brief FFD lattice deformer function set
/*!
MFnLatticeDeformer is the function set for lattice deformers.  Lattice
deformers use FFDs to deform geometry.

The lattice deformer is actually a small network of dependency nodes in
the dependency graph.  This function set is provided to make
manipulation of the network easier.  The main deformer node should be
given to this function set as its object.

There are two lattices associated with a lattice deformer.  There is a
base lattice that defines the start position for the lattice.  The
second lattice is the version that is modified to deform the geometry.
The difference between the two lattices defines the deformation that is
applied to the geometry.

The base lattice is a very simple shape that only defines a box in
space.  The base lattice should be modified by using the standard DAG
transformation support.  The second deformable lattice has geometry
that can be modified using the MFnLattice function set.

For a piece of geometry to be modified by this deformer, the geometry
must be attached to the deformer (use addGeometry method) and the
geometry must be contained within the base lattice.  The resetLattice
method can be used to make the lattice resize to the bounding box of
the attached geometry.
*/ 
class OPENMAYAANIM_EXPORT MFnLatticeDeformer : public MFnDependencyNode
{
	declareMFn(MFnLatticeDeformer, MFnDependencyNode );

public:

	MObject create( unsigned int xDiv, unsigned int yDiv, unsigned int zDiv,
					MStatus * ReturnStatus = NULL );

	MStatus addGeometry( const MObject & object );
	MStatus removeGeometry( const MObject & object );

	MStatus getAffectedGeometry( MObjectArray & objects );

	MStatus getDivisions( unsigned int & x, unsigned int & y, unsigned int & z );
	MStatus setDivisions( unsigned int   x, unsigned int   y, unsigned int   z );

	MStatus resetLattice( bool centerLattice = false );

	MObject deformLattice( MStatus * ReturnStatus = NULL );
	MObject baseLattice( MStatus * ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnLatticeDeformer, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnLatticeDeformer */
