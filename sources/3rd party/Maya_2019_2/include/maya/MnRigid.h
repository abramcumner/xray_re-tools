#ifndef _MnRigid
#define _MnRigid
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
// CLASS:    MnRigid
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MnObject.h>


class TnRigid;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MnRigid)

//! \ingroup OpenMayaFX
//! \brief Class for wrapping N cloth objects
/*!
  This class wraps the internal Maya representation of N cloth objects
  suitable for use with the Nucleus solver.
*/
class OPENMAYAFX_EXPORT MnRigid : public MnObject
{

public:
					MnRigid();
    virtual			~MnRigid();

    MStatus         setTopology(const int numFaces, int * numVertsPerFace, int * faces, const int numEdges, int * edges );
    MStatus         createNRigid();
    MStatus         setPositions(const MFloatPointArray & positions,bool startFrame = true);
    MStatus         setVelocities(const MFloatPointArray & velocities);

    MStatus         setThickness(float radius);
    MStatus         setThickness(float * radius);
    MStatus         setBounce(float bounce);
    MStatus         setBounce(float * bounce);
    MStatus         setFriction(float friction);
    MStatus         setFriction(float * friction);

    MStatus         setCollisionFlags(const bool vertToVert,
                                      const bool edgeToEdge = false,
                                      const bool faceToFace= false);

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
	MnRigid ( TnRigid * );
    void  setNRigid(TnRigid * nObj,bool own);
    TnRigid * getNRigid() { return (TnRigid *)fNObject; };
    friend class MFnNObjectData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MnCloth */
