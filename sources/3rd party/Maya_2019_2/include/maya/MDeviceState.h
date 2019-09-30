#ifndef _MDeviceState
#define _MDeviceState
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
// CLASS:    MDeviceState
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDeviceState)

//! \ingroup OpenMayaUI
//! \brief Input device state 
/*!
MDeviceState is a generic event class for input devices.
Input device classes (such as MPxMidiInputDevice) are responsible for
converting specific event types to an MDeviceState which Maya understands.
*/
class OPENMAYAUI_EXPORT MDeviceState
{
public:
	virtual ~MDeviceState();

	int		devicePosition( const unsigned short int axis) const;
	int		devicePosition( const MString & axisName ) const;
	void	setDevicePosition( const int position,
							   const unsigned short int axis);
	void	setDevicePosition( const int position,
							   const MString & axisName );

	bool	buttonState( const unsigned short int button ) const;
	bool	buttonState( const MString & buttonName ) const;
	void	setButtonState( const bool state,
							const unsigned short int button );
	void	setButtonState( const bool state,
							const MString & buttonName );

	int		maxAxis() const;

	bool	isNull();

protected:
// No protected members

private:
	MDeviceState( void * );
	void *   data();
	void * api_stateData;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDeviceState */
