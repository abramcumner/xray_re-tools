#ifndef _MDeviceChannel
#define _MDeviceChannel
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
// CLASS:    MDeviceChannel
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDeviceChannel)

//! \ingroup OpenMayaUI
//! \brief Input device channel 
/*!
  Input device channel class.
*/
class OPENMAYAUI_EXPORT MDeviceChannel
{
public:
	MDeviceChannel( const MString&, MDeviceChannel* = NULL, int = -1 );

	~MDeviceChannel();


	MString 			name() const;
	MString 			longName() const;

	int				axisIndex() const;

	bool     			hasChildren() const;
	MDeviceChannel	 	parent() const;
	MDeviceChannel 		childByIndex( int );
	int					numChildren() const;

protected:
	// no protected members

private:
	friend class MPxMidiInputDevice;
	MDeviceChannel( void * );
	void * data;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDeviceChannel */
