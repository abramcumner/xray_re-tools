#ifndef _MFnScaleManip
#define _MFnScaleManip
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
// CLASS:    MFnScaleManip
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MFnManip3D.h>
#include <maya/MObject.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnScaleManip)

//! \ingroup OpenMayaUI MFn
//! \brief ScaleManip function set 
/*!
This class provides access to the built-in Maya scale manipulator.  The
manipulator consists of three constrained-axis scale handles for
non-proportional scaling, and a central handle for proportional scaling.

The manipulator provides data to the plugin through the scale manipVal.
The scale value is a vector consisting of X, Y, and Z scale values.  Each
scale value represents a factor controlling how much an object should be
extended along that dimension.  The scale values are absolute and the initial
scale value has a default of <1.0,1.0,1.0>.

The manipulator can be configured either to display with an object (which
must be a DAG node) or to display at an arbitrary point using the
scaleCenter manipVal.
*/
class OPENMAYAUI_EXPORT MFnScaleManip : public MFnManip3D
{
	declareDagMFn(MFnScaleManip, MFnManip3D);

public:
	
		enum ScaleOrientationMode {
			kDefaultOrientation
			,kArbitraryOrientation
		};
		
	MObject		create(MStatus *ReturnStatus = NULL);
	MObject		create(const MString &manipName,
					   const MString &scaleName,
					   MStatus *ReturnStatus = NULL);
	MStatus		connectToScalePlug(const MPlug &scalePlug);
	MStatus		connectToScaleCenterPlug(const MPlug &scaleCenterPlug);
	MStatus		setInitialScale(const MVector& scale);
	MStatus		displayWithNode(const MObject& node);
	MStatus		setSnapMode(bool snapEnabled);
	bool		isSnapModeOn() const;
	MStatus		setSnapIncrement(double snapInc);
	double		snapIncrement() const;
	MStatus              setOrientation(const MEulerRotation& direction);
	MEulerRotation getOrientation(MStatus *ReturnStatus = NULL) const;
	MStatus              setOrientationMode(ScaleOrientationMode mode);
	ScaleOrientationMode getOrientationMode(MStatus *ReturnStatus = NULL) const;
	unsigned int	scaleIndex(MStatus *ReturnStatus = NULL) const;
	unsigned int	scaleCenterIndex(MStatus *ReturnStatus = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnScaleManip, MFnManip3D );

END_NO_SCRIPT_SUPPORT:

protected:
	// No protected members
private:
	// No private members
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnScaleManip */
