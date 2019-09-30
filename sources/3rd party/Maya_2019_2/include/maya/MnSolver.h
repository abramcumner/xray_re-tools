#ifndef _MnSolver
#define _MnSolver
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
// CLASS:    MnSolver
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MnObject.h>


class TnSolver;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MnSolver)

//! \ingroup OpenMayaFX
//! \brief Class for wrapping N solver objects
/*!
  This class wraps the internal Maya representation of a Nucleus solver
  suitable for use with the Nucleus objects such as nCloth or nParticles
*/
class OPENMAYAFX_EXPORT MnSolver
{

public:
	MnSolver();
	virtual			~MnSolver();

	MStatus createNSolver();
	MStatus addNObject(MnObject *obj);
	MStatus removeNObject(MnObject *obj);
	MStatus setGravity(float grav);
	MStatus setGravityDir(float gravX, float gravY, float gravZ);
	MStatus setAirDensity (float dens);
	MStatus setWindSpeed (float speed);
	MStatus setWindDir(float windX, float windY, float windZ);
	MStatus setWindNoiseIntensity (float noise);
	MStatus setDisabled(bool disabled);
	MStatus setSubsteps(int substeps);
	MStatus setMaxIterations(int maxIter);
	MStatus setStartTime(float startTime);
	MStatus makeAllCollide();
	MStatus removeAllCollisions();
	MStatus solve(float solveTime);
	

protected:
	TnSolver * fNSolver;
	bool   fOwn;

private:
	MnSolver( TnSolver * );
	void  setNSolver(TnSolver * nSolv,bool own);
	TnSolver * getNSolver() { return fNSolver; };
	friend class MFnNObjectData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MnCloth */
