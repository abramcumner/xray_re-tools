#ifndef _MPxMultiPolyTweakUVCommand
#define _MPxMultiPolyTweakUVCommand
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
// CLASS:    MPxMultiPolyTweakUVCommand
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MPxCommand.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxMultiPolyTweakUVCommand)

//! \ingroup OpenMaya MPx
//! \brief Base class used for moving UV's on multiple objects.
/*!
This is the base class for UV editing commands on multiple polygonal objects.

The purpose of this command class is to simplify the process of moving
UVs on multiple polygonal objects. The use is only required to provide the new
positions of the UVs that have been modified.
*/
class OPENMAYA_EXPORT MPxMultiPolyTweakUVCommand : public MPxCommand {
public:
	MPxMultiPolyTweakUVCommand();
	virtual ~MPxMultiPolyTweakUVCommand();
	virtual MStatus parseSyntax(MArgDatabase &argData);
	static  MSyntax newSyntax();
	virtual MStatus preProcessUVs(const MObject &mesh,
								  const MIntArray &uvList);
	virtual MStatus getTweakedUVs(const MObject &mesh,
								  MIntArray &uvList,
								  MFloatArray &uPos,
								  MFloatArray &vPos);
private:
	virtual MStatus doIt(const class MArgList &);
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool    isUndoable() const;
	void *          fData;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxPolyTweakUVCommand */
