#ifndef __MGraphEditorInfo_h
#define __MGraphEditorInfo_h
//
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
// CLASS:    MGraphEditorInfo
//
// ****************************************************************************
//
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MDoubleArray.h>
#include <maya/MObjectArray.h>
#include <maya/MMessage.h>
#include <maya/MString.h>
#include <maya/MStatus.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MGraphEditorInfo)

//! \ingroup OpenMayaUI
//! \brief Graph Editor state information with manipulation capabilities
/*!
This class provides methods to obtain UI related information from a specific
Graph Editor. Support is included for obtaining/setting viewport bounds
and obtaining animation curve nodes.

Note: the following are built-in GraphEditor-specific event callback names:
	
\li <b>graphEditorChanged</b>
	- Triggered whenever there is a Graph Editor refresh/resize event

\li <b>graphEditorParamCurveSelected</b>
	- Triggered whenever an animation curve is selected or de-selected
	
\li <b>graphEditorOutlinerHighlightChanged</b>
	- Triggered whenever there is a change to the Outliner list selection

\li <b>graphEditorOutlinerListChanged</b>
	- Triggered whenever there is a change to the Outliner list contents

<b>Python 1.0 Example:</b>
(getting the names of the curves selected in the default Graph Editor)

\code
import maya.OpenMaya as om
import maya.OpenMayaAnim as oma
import maya.OpenMayaUI as omu

def printParamCurves(*queryCriteria):
    
    info = omu.MGraphEditorInfo()
    dependNodeArray=om.MObjectArray()
    info.getAnimCurveNodes(dependNodeArray, queryCriteria[0])
    
    for i in range(0, dependNodeArray.length()):
		mObj=dependNodeArray[i]
		if (mObj is None or not mObj.hasFn(om.MFn.kAnimCurve)):
			continue
		animCurveNode = oma.MFnAnimCurve(mObj)
		animCurveNodeName = om.MFnDependencyNode(mObj).name()
		print animCurveNodeName
	
eventId0 = om.MEventMessage.addEventCallback('graphEditorParamCurveSelected',
								printParamCurves,
								omu.MGraphEditorInfo.kAnimCurveSelected)

eventId1 = om.MEventMessage.addEventCallback(
								'graphEditorOutlinerHighlightChanged',
								printParamCurves,
								omu.MGraphEditorInfo.kAnimCurveHighlighted)

eventId2 = om.MEventMessage.addEventCallback('graphEditorOutlinerListChanged',
								printParamCurves,
								omu.MGraphEditorInfo.kAnimCurveOutlinerOnly)

# Note: execute these lines later when the above callbacks aren't required
#
om.MMessage.removeCallback(eventId0)
om.MMessage.removeCallback(eventId1)
om.MMessage.removeCallback(eventId2)
\endcode

<b>Python 1.0 Example:</b>
(getting the default Graph Editor's viewport bounds when it encounters a
refresh event)

\code
import maya.OpenMaya as om
import maya.OpenMayaUI as omu

def printViewportBounds(*obj):
	info = omu.MGraphEditorInfo()
	leftSu=om.MScriptUtil(0.0)
	leftPtr=leftSu.asDoublePtr()
	rightSu=om.MScriptUtil(0.0)
	rightPtr=rightSu.asDoublePtr()
	bottomSu=om.MScriptUtil(0.0)
	bottomPtr=bottomSu.asDoublePtr()
	topSu=om.MScriptUtil(0.0)
	topPtr=topSu.asDoublePtr()

	info.getViewportBounds(leftPtr, rightPtr, bottomPtr, topPtr)

	print '%d %d %d %d' % (om.MScriptUtil(leftPtr).asDouble(), 
	om.MScriptUtil(rightPtr).asDouble(), om.MScriptUtil(bottomPtr).asDouble(),
	om.MScriptUtil(topPtr).asDouble())

eventId = om.MEventMessage.addEventCallback('graphEditorChanged', 
											printViewportBounds)

# Note: execute this line later when the above callback isn't required
#
om.MMessage.removeCallback(eventId)
\endcode

<b>Python 1.0 Example:</b>
(setting the default Graph Editor's viewport bounds)

\code
import maya.OpenMayaUI as omu

info = omu.MGraphEditorInfo()
info.setViewportBounds(-100,100,-100,100)
\endcode
*/
class OPENMAYAUI_EXPORT MGraphEditorInfo
{
public:

	/*!
	Defines the query criteria for animation curves specifically with respect
	to the attached Graph Editor's own viewport
	*/	
	enum AnimCurveQuery {

		//! Curve(s) present in Outliner (but not visible in the viewport).
		//! <b>Note: this is not currently supported</b>
		kAnimCurveOutlinerOnly = 0,
		
		//! Curve(s) highlighted in Outliner (and visible in the viewport)
		kAnimCurveHighlighted,
		
		//! Curve(s) selected (via keys/tangent handles) in the viewport
		kAnimCurveSelected,
		
		//! Curve(s) known to exist (independent of any Outliner filtering)
		kAnimCurveAllKnown
		
	};

	MGraphEditorInfo( MStatus* ReturnStatus=NULL );
	MGraphEditorInfo( const MString &graphEditorName,
		MStatus* ReturnStatus=NULL);
	virtual ~MGraphEditorInfo();

	MStatus getViewportBounds(double& left, double& right, double& bottom,
		double& top) const;

	MStatus getViewportBounds(MDoubleArray& boundsArray) const;
	
	MStatus setViewportBounds(double left, double right, double bottom,
		double top) const;

	MStatus setViewportBounds(const MDoubleArray& boundsArray) const;

	MStatus getAnimCurveNodes(MObjectArray& animCurveNodeArray, 
		AnimCurveQuery animCurveQuery) const;

	bool isStackedViewportMode() const;
	bool isNormalizedViewportMode() const;

	void reset();

	MString name() const;

	static const char* className();

private:

	void attachToGraphEditor(const MString &graphEditorName,
		MStatus* ReturnStatus);

	bool isUnsupportedViewportMode() const;

	void* fInternalPtr;
	MCallbackId fGraphEditorCallbackId;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MGraphEditorInfo */
