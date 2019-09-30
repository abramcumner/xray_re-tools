#ifndef _MnCloth
#define _MnCloth
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
// CLASS:    MnCloth
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MnObject.h>


class TnCloth;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MnCloth)

//! \ingroup OpenMayaFX
//! \brief Class for wrapping N cloth objects
/*!
  This class wraps the internal Maya representation of N cloth objects
  suitable for use with the Nucleus solver.
*/
class OPENMAYAFX_EXPORT MnCloth : public MnObject
{

public:
					MnCloth();
    virtual			~MnCloth();

    MStatus         createNCloth();
    MStatus         setTopology(const int numFaces, int * numVertsPerFace, int * faces, const int numEdges, int * edges );
    MStatus         setPositions(const MFloatPointArray & positions,bool startFrame = true);
    MStatus         setVelocities(const MFloatPointArray & velocities);

    MStatus         setBendRestAngleFromPositions(const MFloatPointArray & positions);

    MStatus         setLinksRestLengthFromPositions(const MFloatPointArray & positions);

    MStatus         setInputMeshAttractPositions(const MFloatPointArray & positions);

    MStatus         setInputMeshAttractDamping(float damping);

    MStatus         setInputMeshAttractAndRigidStrength(float * inputAttractArray,float *  rigidArray,float *  deformArray);

    MStatus         setComputeRestLength ( const bool b );

    MStatus         setComputeRestAngles ( const bool b );

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
    MStatus         setSelfCollisionFlags(const bool vertToVert, const bool vertToEdge = false,
                                      const bool vertToFace = false, const bool edgeToEdge = false,
                                      const bool edgeToFace = false);

    MStatus         setCollisionFlags(const bool vertToVert,
                                      const bool edgeToEdge = false,
                                      const bool faceToFace= false);
    MStatus         setDisableGravity ( const bool b );

    MStatus         setMaxIterations(const int it);

    MStatus         setMaxSelfCollisionIterations(const int it);

    MStatus         setSelfCollisionSoftness(float softness);

    MStatus         setSelfTrappedCheck(bool on);

    MStatus         setSelfCrossoverPush(float val);

    MStatus         setDragAndLift(float drag,float lift);

    MStatus         setTangentialDrag(float tangentialDrag);

    MStatus         setStartPressure(float startPressure);

    MStatus         setSealHoles(bool seal);

    MStatus         setPressure(float pressure);

    MStatus         setTrackVolume(bool track);

    MStatus         setIncompressibility(float v);

    MStatus         setPressureDamping(float damp);

    MStatus         setPumpRate(float pump);

    MStatus         setAirTightness(float airTightness);

    MStatus         setAddCrossLinks(bool addCrossLinks);

    MStatus         setBendAngleDropoff(float dropoff);

    MStatus         setShearResistance(float resistance);

    MStatus         setStretchAndCompressionResistance(float stretchResist,float compressionResist);

    MStatus         setStretchAndCompressionResistance(float * stretchResist,float * compressionResist);

    MStatus         setBendAngleScale(float scale);

    MStatus         setRestitutionAngle(float angle);

    MStatus         setRestitutionTension(float tension);

    MStatus         setSelfCollideWidth(float width);

    MStatus         setBendResistance(float strength);

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
	MnCloth( TnCloth * );
    void  setNCloth(TnCloth * nObj,bool own);
    TnCloth * getNCloth() { return (TnCloth *) fNObject; };
    friend class MFnNObjectData;
    //	TnCloth * fNObject;
    //	bool   fOwn;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MnCloth */
