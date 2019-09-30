#ifndef _MPxMidiInputDevice
#define _MPxMidiInputDevice
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
// CLASS:    MPxMidiInputDevice
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MTypes.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxMidiInputDevice)

//! \ingroup OpenMayaUI MPx
//! \brief Midi input device 
/*!
This is the base class for user defined MIDI input devices.

Child classes of MPxMidiInputDevice should define:

\li <b>a constructor</b>		which defines number of buttons and axes
\li <b>nameAttributes()</b>	which names the buttons and axes (optional)
\li <b>deviceState()</b>		which turns a midi event into an MDeviceState
								or returns NULL if it is not a midi event
								from this device
*/
class OPENMAYAUI_EXPORT MPxMidiInputDevice
{
public:
	MPxMidiInputDevice();
	virtual ~MPxMidiInputDevice();

	virtual	MStatus			openDevice();
	virtual	void			closeDevice();

	virtual	void			nameAxes();
	virtual void			nameButtons();
	virtual	MDeviceState* 	deviceState();

	virtual MStatus 		sendMessage(	const char* const messageType,
											const char* const messageParams );
	virtual char* 			getMessage(	const char* const messageType,
										char* messageResponse );

	virtual void 			doButtonEvents( bool = true );
	virtual void 			doMovementEvents( bool = true );

	MPxMidiInputDevice( void * init );

	static	const char*		className();

protected:
	MStatus		setNamedButton( const MString &, unsigned short );
	MStatus		addChannel( MDeviceChannel & );
	MStatus 	setDegreesOfFreedom( int freedom );
	MStatus 	setNumberOfButtons( int buttons );

private:
	void   setData( void* );
	void * 	data;
	int 	degreesOfFreedom;
	int 	numberOfButtons;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxMidiInputDevice */
