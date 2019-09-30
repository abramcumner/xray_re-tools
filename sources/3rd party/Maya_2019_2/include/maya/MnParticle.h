#ifndef _MnParticle
#define _MnParticle
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
// CLASS:    MnParticle
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MnObject.h>


class TnParticle;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MnParticle)

//! \ingroup OpenMayaFX
//! \brief Class for wrapping N cloth objects
/*!
  This class wraps the internal Maya representation of N cloth objects
  suitable for use with the Nucleus solver.
*/
class OPENMAYAFX_EXPORT MnParticle : public MnObject
{

public:
					MnParticle();
    virtual			~MnParticle();

    MStatus         createNParticle();
    MStatus         setTopology(const int numPoints );
    MStatus         setPositions(const MFloatPointArray & positions,bool startFrame = true);
    MStatus         setVelocities(const MFloatPointArray & velocities);

    MStatus         setThickness(float radius);
    MStatus         setThickness(float * radius);
    MStatus         setInverseMass(float invMass);
    MStatus         setInverseMass(float * invMass);
    MStatus         setBounce(float bounce);
    MStatus         setBounce(float * bounce);
    MStatus         setFriction(float friction);
    MStatus         setFriction(float * friction);
    MStatus         setDamping(float damping);
    MStatus         setDamping(float * damping);

    MStatus         setDisableGravity( const bool b );

    MStatus         setLiquidSimulation( const bool b );
    MStatus         setIncompressibility(float incompressibility);
    MStatus         setRestDensity(float restDensity);
    MStatus         setLiquidRadiusScale(float liquidRadiusScale);
    MStatus         setViscosity(float viscosity);
    MStatus         setViscosity(float * viscosity);
    MStatus         setSurfaceTension(float surfaceTension );
    MStatus         setSurfaceTension(float * surfaceTension);

    MStatus         setMaxIterations(const int it);

    MStatus         setMaxSelfCollisionIterations(const int it);

    MStatus         setSelfCollisionSoftness(float softness);

    MStatus         setDragAndLift(float drag,float lift);

    MStatus         setCollide( const bool b );
    MStatus         setSelfCollide( const bool b );
    MStatus         setSelfCollideWidth(float width);

    MStatus         getNumVertices(unsigned int & numVerts) const;
    MStatus         getPositions(MFloatPointArray & positions) const;
    MStatus         getVelocities(MFloatPointArray & velocities) const;
    MStatus         getThickness(MFloatArray & radius) const;
    MStatus         getInverseMass(MFloatArray & inverseMass) const;
    MStatus         getBounce(MFloatArray & bounce) const;
    MStatus         getFriction(MFloatArray & friction) const;


protected:
// No protected members

private:
	MnParticle( TnParticle * );
    void  setNParticle(TnParticle * nObj,bool own);
    TnParticle * getNParticle() { return (TnParticle *) fNObject; };
    friend class MFnNObjectData;
    //	TnParticle * fNObject;
    //	bool   fOwn;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MnParticle */
