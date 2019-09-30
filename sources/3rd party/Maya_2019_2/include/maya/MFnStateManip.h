#ifndef _MFnStateManip
#define _MFnStateManip
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
// CLASS:    MFnStateManip
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MFnManip3D.h>
#include <maya/MObject.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnStateManip)

//! \ingroup OpenMayaUI MFn
//! \brief StateManip function set 
/*!
The StateManip allows the user to switch between multiple states. It
is drawn as a circle with a notch. Each click on the circle increments
the value of the state (modulo the maximum number of states). This
manipulator generates an integer value corresponding to the state of
the manip.
*/
class OPENMAYAUI_EXPORT MFnStateManip : public MFnManip3D
{
	declareDagMFn(MFnStateManip, MFnManip3D);

public:
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &stateName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToStatePlug(MPlug &statePlug);
	MStatus		setInitialState(unsigned int initialState);
	MStatus		setMaxStates(unsigned int numStates);
	unsigned int	maxStates(MStatus *ReturnStatus = NULL) const;
	unsigned int	state(MStatus *ReturnStatus = NULL) const;
	unsigned int	positionIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	stateIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnStateManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:

private:

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnStateManip */
