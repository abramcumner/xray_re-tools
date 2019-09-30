#ifndef _MAnimCurveChange
#define _MAnimCurveChange
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
// CLASS:    MAnimCurveChange
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAnimCurveChange)

//! \ingroup OpenMayaAnim
//! \brief  Anim Curve Change Cache
/*!
  Adding, removing and changing keyframes on an anim curve cannot be done
  simply by setting attribute values on the corresponding node. This makes it
  impossible to capture such changes in an MDGModifier for undo/redo purposes.

  The MAnimCurveChange class provides persistent storage for
  information concerning changes to anim curve nodes, along with methods to
  undo and redo those changes. MFnAnimCurve methods which add, remove or
  change keyframes take an optional MAnimCurveChange instance in which to
  store information about the changes being made.

  If the same MAnimCurveChange instance is used for multiple anim curve edit
  operations, then the cache maintains an undo/redo queue which allows all
  changes in the series to be undone or redone.  If selective undo/redo is
  required, then a different MAnimCurveCache instance must be used for each
  edit.
*/
class OPENMAYAANIM_EXPORT MAnimCurveChange
{

public:
	MAnimCurveChange( MStatus * ReturnStatus = NULL );
	~MAnimCurveChange();
	MStatus undoIt();
	MStatus redoIt();

	bool isInteractive() const;
	void setInteractive(bool value);

	static const char* className();

protected:
// No protected members

private:
	void*		 data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAnimCurveChange */
