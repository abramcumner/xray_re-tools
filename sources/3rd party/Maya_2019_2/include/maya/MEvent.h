#ifndef _MEvent
#define _MEvent
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
// CLASS:    MEvent
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MEvent)

//! \ingroup OpenMayaUI
//! \brief System event information
/*!
The MEvent class is used for querying system events such as mouse presses.

Events are handled by an MPxContext derived class in which MEvents are passed
and can be accessed.

Since Maya has default actions for several events, only a subset are avalaible
through the API.
The events that can be accessed are:

\li <b>buttonPress</b>       - mouse button press event (left & middle only)
\li <b>buttonRelease</b>     - mouse buttons release event (left & middle only)
\li <b>mouseDrag</b>         - mouse button drag event (left & middle only)
\li <b>hold</b>              - mouse button hold event (left & middle only)
\li <b>deleteEvent</b>       - delete/backspace key event
\li <b>commandCompletion</b> - complete key event

Several modifiers for events are also accessible through the API.  Modifiers
are actions that occur during an event.  For example, holding down the shift
key while pressing a mouse button causes a button press event to occur with
a shift modifier.

A modifier can be used to determine if two mouse events occur simulaneously.
The second mouse event is registered as a modifier in the hold event of the
first mouse button.  So if you wanted to determine if both the left and middle
buttons are pressed then you would query the modifier in the hold event
of the first mouse button using the isModifierMiddleMouseButton() and
isModifierLeftMouseButton() methods.
*/
class OPENMAYAUI_EXPORT MEvent
{
public:
    //! Modifier key types.
    enum ModifierType {
        shiftKey    = 1<<0,	//!< \nop
        controlKey  = 1<<2	//!< \nop
    };

    	//! Mouse button types.
	enum MouseButtonType {
		kLeftMouse		= 1<<6,	//!< \nop
		kMiddleMouse	= kLeftMouse<<1	//!< \nop
	};

public:
    MEvent();
    virtual ~MEvent();

    MStatus         getPosition( short& x_pos, short& y_pos ) const;
    MStatus         setPosition( short x_pos, short y_pos );
    MStatus         getWindowPosition( short& x_pos, short& y_pos ) const;
    MouseButtonType mouseButton( MStatus * ReturnStatus = NULL ) const;
    bool            isModifierKeyRelease( MStatus * ReturnStatus = NULL ) const;
    ModifierType    modifiers( MStatus * ReturnStatus = NULL ) const;
    MStatus         setModifiers( const ModifierType& modType );
    bool            isModifierNone( MStatus * ReturnStatus = NULL ) const;
    bool            isModifierShift( MStatus * ReturnStatus = NULL ) const;
    bool            isModifierControl( MStatus * ReturnStatus = NULL ) const;
    bool            isModifierLeftMouseButton( MStatus * ReturnStatus = NULL )
					const;
    bool            isModifierMiddleMouseButton( MStatus * ReturnStatus = NULL )
					const;

    static const char* className();

protected:
// No protected members

private:

    MEvent( const void * );
    const void * fEventPtr;
    void * fModifier;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MEvent */
