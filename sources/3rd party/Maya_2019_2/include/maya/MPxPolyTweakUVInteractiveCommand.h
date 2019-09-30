#ifndef _MPxPolyTweakUVInteractiveCommand
#define _MPxPolyTweakUVInteractiveCommand
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
// CLASS:    MPxPolyTweakUVInteractiveCommand
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MPxCommand.h>
#include <maya/MPxToolCommand.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxPolyTweakUVInteractiveCommand)

//! \ingroup OpenMayaUI MPx
//! \brief Base class used for moving polygon UV's.
/*!
This is the base class for UV editing interactive commands on polygonal objects.

The purpose of this tool command class is to simplify the process of moving
UVs on a polygonal object. The use is only required to provide the new
positions of the UVs that being modified, and finalize at the end of editing.
*/
class OPENMAYAUI_EXPORT MPxPolyTweakUVInteractiveCommand : public MPxToolCommand
{
public:

	MPxPolyTweakUVInteractiveCommand();
	virtual ~MPxPolyTweakUVInteractiveCommand(); 

	void					setUVs( const MObject & mesh, 
									MIntArray & uvList,
									MFloatArray & uPos,
									MFloatArray & vPos,
								    const MString *uvSet = NULL ); 
	
	virtual bool			isUndoable() const;
	virtual MStatus   		doIt( const MArgList& args ) ;	

	virtual MStatus 		cancel();
	virtual MStatus 		finalize();

	static	const char*	className();

private:

	void * fCommand;
}; 
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxPolyTweakUVInteractiveCommand */
