#ifndef _MPxSelectionContext
#define _MPxSelectionContext
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
// CLASS:    MPxSelectionContext
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MPxContext.h>
#include <maya/MPoint.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxSelectionContext)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for interative selection tools 
/*!
This class is used in creating user defined tools that use the internal
selection mechanism in maya.
*/
class OPENMAYAUI_EXPORT MPxSelectionContext : public MPxContext
{
public:
	MPxSelectionContext ();
	virtual	~MPxSelectionContext ();

BEGIN_NO_SCRIPT_SUPPORT:
		// Interfaces for viewport 2.0
		//! \noscript
		virtual MStatus		doPress ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
		//! \noscript
		virtual MStatus		doRelease( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
		//! \noscript
		virtual MStatus		doDrag ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
		//! \noscript
		virtual MStatus		doHold ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
END_NO_SCRIPT_SUPPORT:

	virtual MStatus		doPress ( MEvent & event );
	virtual MStatus		doRelease ( MEvent & event );
	virtual MStatus		doDrag ( MEvent & event );
	virtual MStatus		doHold ( MEvent & event );
	virtual MStatus 	helpStateHasChanged ( MEvent & );

	virtual MStatus     addManipulator( const MObject & manipulator );
	virtual MStatus     deleteManipulators();

	MStatus				setAllowPreSelectHilight();
	MStatus				setAllowSoftSelect();
	MStatus				setAllowSymmetry();
	MStatus				setAllowDoubleClickAction();

	MStatus				setImage( const MString & image, ImageIndex index );
	MString				image( ImageIndex index, MStatus * ReturnStatus = NULL ) const;
BEGIN_NO_SCRIPT_SUPPORT:
	//!     NO SCRIPT SUPPORT
	MStatus				getImage( MString & image, ImageIndex index ) const;
END_NO_SCRIPT_SUPPORT:

	// SCRIPT USE ONLY
	bool _isSelecting()
		{ return isSelecting(); }
	MPoint _startPoint()
		{ return startPoint(); }
	MPoint _lastDragPoint()
		{ return lastDragPoint(); }
	MPxToolCommand* _newToolCommand()
		{ return newToolCommand(); }
	//

protected:

	// These methods determine whether the tool will select
	// an object or operate on an object
	//!	USE _isSelecting() IN SCRIPT
	bool        isSelecting();

	// Get the world space location of the selection point
	//!	USE _startPoint() IN SCRIPT
	MPoint 		startPoint();		// point where dragging started
	//!	USE _lastDragPoint() IN SCRIPT
	MPoint 		lastDragPoint();	// preview drag point (both in WS)


	// Create an instance of a tool command for use in
	// this context.
	//
	//! CALL _newToolCommand() IN SCRIPT
	virtual MPxToolCommand *	newToolCommand();

public:
	// Temporarily putting these virtual functions at the end
	virtual void		abortAction();
	virtual bool		processNumericalInput( const MDoubleArray &values,
											   const MIntArray &flags,
											   bool isAbsolute );
	virtual bool		feedbackNumericalInput() const;
	virtual MSyntax::MArgType	argTypeNumericalInput( unsigned int index ) const;

	static const char*	className();

private:

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxSelectionContext */
