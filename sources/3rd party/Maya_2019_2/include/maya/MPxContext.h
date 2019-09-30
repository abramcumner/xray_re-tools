#ifndef _MPxContext
#define _MPxContext
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
// CLASS:    MPxContext
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

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxContext)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for user defined contexts 
/*!
This is the base class for user defined contexts.

Contexts provide a way to create interactive tools in Maya.
A context class defines what happens when interactive events,
such as mouse events, occur within an interactive panel in Maya.

Since there are default actions for all tools in Maya, such as the
right mouse button event which brings up an options menu, only as subset
of the events that occur in a view can be overridden.
The events that can be overridden are:

\li <b>doPress</b>        - mouse button press event (left & middle only)
\li <b>doRelease</b>      - mouse buttons release event (left & middle only)
\li <b>doDrag</b>         - mouse button drag event (left & middle only)
\li <b>doHold</b>         - mouse button hold event (left & middle only)
\li <b>deleteAction</b>   - delete/backspace key event
\li <b>completeAction</b> - complete key event
\li <b>abortAction</b> - abort/escape key event


A context is <b>activated</b> by pressing the toolButton for that context.
Once the context is activated, it will handle the events that occur within
3d panel.

A context is <b>deactivated</b> when some other tool button is pressed.

There can be more than one instance of a context in Maya, for example, dragging
a tool icon into the shelf creates another instance of that context. Since
there can be multiple instances of the same context there is a command class,
<b>MPxContextCommand</b>, which is responsible for the creation of contexts.
See <b>MPxContextCommand</b> for more information.
*/
class OPENMAYAUI_EXPORT MPxContext
{
public:
	//! Used to select between the three possible images associated
	//! with the context.
	enum ImageIndex {
		kImage1 = 0,	//!< \nop
		kImage2,	//!< \nop
		kImage3		//!< \nop
	};

	MPxContext ();
	virtual	~MPxContext ();

	virtual void        toolOnSetup( MEvent & event );
	virtual void        toolOffCleanup();

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
	//! \noscript
	virtual MStatus		drawFeedback ( MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context );

	virtual MStatus		doPtrMoved ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
	
END_NO_SCRIPT_SUPPORT:

	virtual MStatus		doPress ( MEvent & event );
	virtual MStatus		doRelease ( MEvent & event );
	virtual MStatus		doDrag ( MEvent & event );
	virtual MStatus		doHold ( MEvent & event );
	virtual MStatus		doPtrMoved ( MEvent & event);

	virtual MStatus 	doEnterRegion ( MEvent & event );
	virtual MStatus		helpStateHasChanged ( MEvent & event );
	virtual void		deleteAction();
	virtual void		completeAction();
	virtual MStatus     addManipulator( const MObject & manipulator );
	virtual MStatus     deleteManipulators();
	MStatus				setImage( const MString & image, ImageIndex index );
	MString				image( ImageIndex index, MStatus * ReturnStatus = NULL ) const;

	// SCRIPT USE ONLY
	MStatus _setHelpString( const MString& str )
		{ return setHelpString(str); }
	MStatus _setTitleString( const MString & str )
		{ return setTitleString(str); }
	MStatus _setCursor( const MCursor & newCursor )
		{ return setCursor(newCursor); }
	MStatus _beginMarquee( MEvent & event )
		{ return beginMarquee(event); }
	MStatus _releaseMarquee( MEvent & event,short& top, short& left, short& bottom, short& right )
		{ return releaseMarquee(event,top,left,bottom,right); }
	static bool _ignoreEntry( const MIntArray &flags, unsigned int entry )
		{ return ignoreEntry(flags,entry); }
	MPxToolCommand* _newToolCommand()
		{ return newToolCommand(); }
	//

protected:

    //!	USE _setHelpString() IN SCRIPT
	MStatus			    setHelpString( const MString & str );
    //!	USE _setTitleString() IN SCRIPT
	MStatus			    setTitleString( const MString & str );

    //!	USE _setCursor() IN SCRIPT
	MStatus				setCursor( const MCursor & newCursor );

    //!	USE _beginMarquee() IN SCRIPT
	MStatus		        beginMarquee( MEvent & event );
    //!	USE _dragMarquee() IN SCRIPT
	MStatus             dragMarquee( MEvent & event );
    //!	USE _releaseMarquee() IN SCRIPT
	MStatus             releaseMarquee( MEvent & event,
                                        short& top, short& left,
                                        short& bottom, short& right );

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
	virtual MString		stringClassName() const;

BEGIN_NO_SCRIPT_SUPPORT:
	//!     NO SCRIPT SUPPORT
	virtual	void		getClassName( MString & name ) const;
	//!     NO SCRIPT SUPPORT
	MStatus				getImage( MString & image, ImageIndex index ) const;
END_NO_SCRIPT_SUPPORT:

	static	const char*	className();


protected:
	//!	USE _ignoreEntry() IN SCRIPT
	static bool 		ignoreEntry( const MIntArray &flags,
									 unsigned int entry );

private:
	friend class MPxSelectionContext;
	friend class MPxTexContext;
	friend class MToolsInfo;
	void setData( void * ptr );
	void * data;
	MString title;
	MString help;
	MString fImage;
	unsigned int fImageIndex;
	MCursor * cursor;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxContext */
