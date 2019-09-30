#ifndef _MnObject
#define _MnObject
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
// CLASS:    MnObject
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>


class TnObject;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MnObject)

//! \ingroup OpenMayaFX
//! \brief Class for wrapping N cloth objects
/*!
  This class wraps the internal Maya representation of N cloth objects
  suitable for use with the Nucleus solver.
*/
class OPENMAYAFX_EXPORT MnObject
{

public:
	MnObject();
	virtual			~MnObject();

protected:
	TnObject * fNObject;
	bool   fOwn;

private:
	MnObject( TnObject * );
	void  setNObject(TnObject * nObj,bool own);
	TnObject * getNObject() { return fNObject; }

	friend class MFnNObjectData;
	friend class MnSolver;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MnCloth */
