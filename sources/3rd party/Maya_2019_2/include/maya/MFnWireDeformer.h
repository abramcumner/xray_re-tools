#ifndef _MFnWireDeformer
#define _MFnWireDeformer
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
// CLASS:    MFnWireDeformer
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MPoint.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnWireDeformer)

//! \ingroup OpenMayaAnim MFn
//! \brief wire deformer function set 
/*!
MFnWireDeformer is the function set for wire deformers.  Wire deformers modify
geometry based on its proximity to controlling wire curves.  As the wire
curves are modified, the parts of the geometry close to the curve will follow.

The wire deformer is actually a small network of dependency nodes in the
dependency graph.  This function set is provided to make manipulation of the
network easier.  The main deformer node should be given to this function set as
its object.
*/
class OPENMAYAANIM_EXPORT MFnWireDeformer : public MFnDependencyNode
{

	declareMFn(MFnWireDeformer, MFnDependencyNode );

public:

	MObject  create( MStatus * ReturnStatus = NULL );


	MStatus addGeometry( const MObject & object );
	MStatus removeGeometry( const MObject & object );
	MStatus getAffectedGeometry( MObjectArray & objects );

	unsigned int numWires( MStatus * ReturnStatus = NULL ) const;
	MStatus  addWire( const MObject & object );
	MObject  wire( unsigned int wireIndex, MStatus * ReturnStatus = NULL );
	float    wireDropOffDistance( unsigned int wireIndex,
								  MStatus * ReturnStatus = NULL ) const;
	MStatus  setWireDropOffDistance( unsigned int wireIndex, float dropOff );
	float    wireScale( unsigned int wireIndex,
								  MStatus * ReturnStatus = NULL ) const;
	MStatus  setWireScale( unsigned int wireIndex, float scale );
	MObject  holdingShape( unsigned int wireIndex,
						          MStatus * ReturnStatus = NULL ) const;
	MStatus  setHoldingShape( unsigned int wireIndex, MObject holdingCurve );

	float    envelope( MStatus * ReturnStatus = NULL ) const;
	MStatus  setEnvelope( float envelope );
	float    rotation( MStatus * ReturnStatus = NULL ) const;
	MStatus  setRotation( float rotation );
	float    localIntensity( MStatus * ReturnStatus = NULL ) const;
	MStatus  setLocalIntensity( float localIntensity );
	float    crossingEffect( MStatus * ReturnStatus = NULL ) const;
	MStatus  setCrossingEffect( float crossingEffect );

	unsigned int numDropoffLocators( unsigned int wireIndex,
								 MStatus * ReturnStatus = NULL ) const;
	MStatus  setDropoffLocator( unsigned int wireIndex, unsigned int locatorIndex,
						         float param, float percentage );
	MStatus  getDropoffLocator( unsigned int wireIndex, unsigned int locatorIndex,
						         float &param, float &percentage );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnWireDeformer, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnWireDeformer */
