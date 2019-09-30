#ifndef _MItInstancer
#define _MItInstancer
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
// CLASS:    MItInstancer
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MItInstancer)
//
// Use the particle instancer node iterator to traverse all instancer nodes in
// Maya's Dependency Graph, and optionally
//
// With filtering enabled, the iterator checks to see if the node is
// compatible with the type specified by the filter.  See MFn::Type for a
// list of all valid types.
//
// Since MObjects may be compatible with more than one type (nodes are
// organised in a hierarchy) the MObject::hasFn() method can be used to
// further check for compatible types.
//
// Any compatible Function Set can be attached to the retrieved object to
// query or or edit it.  Often you will want to use the dependency node
// function set (MFnDependencyNode), which is compatible with all
// dependency nodes, to perform queries on each node as the iterator
// Traverses the Dependency Graph.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MDagPath.h>
#include <maya/MMatrix.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItInstancer)

//! \ingroup OpenMaya
//! \brief Particle iterator. 
/*!
Maya's Particle Replacement aka Particle Instancing feature allows geometry
to be instanced and those instances transformed using particles.  More
precisely, particles are replaced by arbitrary DAG nodes which maya be
transforms with many shapes under them, and multiple DAG paths to those
shapes.

The Instancer Iterator Class (MItInstancer) provides methods for iterating
over all the dag paths to the shapes created in the scene by the replacement
of particles by dag nodes.

When particles are being replaced by a shape node there will be one dag path
corresponding with each particle.  If, say, particles are instead being
replaced by a transform with five shape nodes somewhere underneath it, then
this iterator will visit five paths for every particle.  Dag-instancing is
also respected: if the given dag node has two child transforms each of
which is a path to one shape underneath them, then this iterator will visit
two paths.  In other words, this iterator does not distinguish between paths
resulting from a multiplicity of actual shapes and paths resulting from
DAG-instances of individual shapes: all appear as different paths for the
current particle.

A simple usage might look like:

	MItInstancer it;
	while( ! it.isDone() )
	{
		MDagPath path = it.path();
		MString label = path.fullPathName();
		MGlobal::displayInfo( label );
		it.next();
	}
*/
class OPENMAYA_EXPORT MItInstancer
{
public:
				MItInstancer();
	virtual		~MItInstancer();

	MStatus		reset();

	MStatus		next();

   	MStatus   	nextParticle();

	MStatus		nextInstancer();

   	bool		isDone() const;

   	MObject		instancer() const;

	MDagPath 	instancerPath() const;

   	MDagPath	path();

	MMatrix		matrix();

	MUintPtrSz	instancerId() const;

	unsigned int particleId() const;

	unsigned int pathId() const;

	static const char* className();

protected:
// No protected members

private:
	void *data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItInstancer */
