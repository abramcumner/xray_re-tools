#ifndef _MPxTexContext
#define _MPxTexContext
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
// CLASS:    MPxTexContext
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MEvent.h>
#include <maya/MString.h>
#include <maya/MSyntax.h>
#include <maya/MObject.h>
#include <maya/MPxContext.h>
#include <maya/MSelectionMask.h>
#include <maya/MSelectionList.h>
#include <maya/MPxToolCommand.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxTexContext)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for user defined contexts working on uv editor 
/*!
This is the base class for user defined contexts working on uv editor.
*/

class OPENMAYAUI_EXPORT MPxTexContext : public MPxContext
{
public:

	MPxTexContext ();
	virtual	~MPxTexContext ();
										
BEGIN_NO_SCRIPT_SUPPORT:
	//! \noscript
	virtual MStatus		doPress ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
	//! \noscript
	virtual MStatus		doRelease( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
	//! \noscript
	virtual MStatus		doDrag ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
	//! \noscript
	virtual MStatus		doPtrMoved ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
END_NO_SCRIPT_SUPPORT:
	
	void viewToPort( double xView, double yView, short &xPort, short &yPort ) const; 
	void portToView( short xPort, short yPort, double &xView, double &yView) const;	
	void viewRect(double &left, double &right, double &bottom, double &top) const;
	void portSize(double &width, double &height) const ;
	
	static bool getMarqueeSelection( double xMin, double yMin, double xMax, double yMax, 
		const MSelectionMask &mask, bool bPickSingle, bool bIgnoreSelectionMode, MSelectionList &selectionList ); 

	static const char*	className();

	virtual MPxToolCommand *newToolCommand(); // override virtual interface from MPxContext;

private:
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxTexContext */
