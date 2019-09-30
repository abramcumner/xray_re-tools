#ifndef _MHairSystem
#define _MHairSystem
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
// CLASS:    MHairSystem
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>




// ****************************************************************************
// CLASS DECLARATION (MHairSystem)

// Function types for user-registerable hair/geometry collision handler.
//

//! \brief Pointer to a hair/geometry collision pre-frame function.
/*!
 \param[in] hairSystem Hair system shape node.
 \param[in] curTime Current time in seconds.
 \param[in,out] privateData Pointer to a pointer which the user can set with data to be passed into the solver callback function.
*/
typedef bool	(*MHairSystemCollisionSolverPreFrameFnPtr)(
						const MObject			hairSystem,
						const double			curTime,
						void					**privateData );

//! \brief Pointer to a hair/geometry collision/intersection function.
/*!
 \param[in] hairSystem Hair system shape node.
 \param[in] follicleIndex Index of current follicle. Can be passed to getFollicle() to obtain the follicle node.
 \param[in,out] hairPositions Points along the follicle showing where the hairSystem wants it to be.
 \param[in,out] hairPositionLast Positions along the follicle at the previous time.
 \param[in] hairWidths Width of the follicle at each point along its length.
 \param[in] startIndex First element of the above arrays which the callback is allowed to modify.
 \param[in] endIndex Last element of the above arrays which the callback is allowed to modify.
 \param[in] curTime Current time in seconds.
 \param[in] privateData Pointer to private data provided by the MHairSystem::registerCollistionSolverPreFrame callback.
*/
typedef bool	(*MHairSystemCollisionSolverCollideFnPtr)(
						const MObject			hairSystem,
						const int				follicleIndex,
						MVectorArray			&hairPositions,
						MVectorArray			&hairPositionsLast,
						const MDoubleArray		&hairWidths,
						const int				startIndex,
						const int				endIndex,
						const double			curTime,
						const double			friction,
						const void 				*privateData );

OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMayaFX
//! \brief Interface with Maya's Dynamic Hair System
/*!
  MHairSystem provides an interface with Maya's Dynamic Hair System.

  At present, this interface only supports the ability to override the
  geometry collision component of hair. There is currently no support
  for overriding the other hair collision components (i.e. implicit
  object collision and hair self collision).

  The rationale for providing a hair collision API is to allow the
  user to override Maya's internal algorithm which is a
  high-performance simulation. There is an example plug-in provided in
  the Developer's Toolkit called hairCollisionSolver.cpp.
*/
class OPENMAYAFX_EXPORT MHairSystem
{
public:
	// Methods for registering a user-definable hair/geometry collision
	// solver.
	//

	// Register the collide method.
	static MStatus	registerCollisionSolverCollide(
						MHairSystemCollisionSolverCollideFnPtr );

	// Register the pre-frame method.
	static MStatus	registerCollisionSolverPreFrame(
						MHairSystemCollisionSolverPreFrameFnPtr );

	// Unregister the collide method.
	static MStatus	unregisterCollisionSolverCollide();

	// Unregister the pre-frame method.
	static MStatus	unregisterCollisionSolverPreFrame();

	// Helper methods for user-definable hair/geometry collision solver.
	//

	// Get the ith collison geometry associated with the hair system.
	static MObject	getCollisionObject( const MObject hairSystem,
						const int index, MStatus *status );

	// Get all collison geometries associated with the hair system.
	static MStatus	getCollisionObject( const MObject hairSystem,
						MObjectArray &objects, MIntArray &logicalIndices );

	// Get the ith follicle associated with the hair system.
	static MObject	getFollicle( const MObject hairSystem,
						const int index, MStatus *status );
	// Get all follicles associated with the hair system.
	static MStatus	getFollicle( const MObject hairSystem,
						MObjectArray &follicles,
						MIntArray &logicalIndices );

	static void setRegisteringCallableScript();
	static bool registeringCallableScript();

	static const char	*className();

protected:
// No protected members

private:
	static bool			fRegisteringCallableScript;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MHairSystem */
