#ifndef _MAnimControl
#define _MAnimControl
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
// ****************************************************************************
//
// CLASS:    MAnimControl
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTime.h>
#include <maya/MTypes.h>
#include <maya/MFnAnimCurve.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAnimControl)

//! \ingroup OpenMayaAnim
//! \brief Control over animation playback and values 
/*!
  This class provide access to the values that control how an
  animation is played.  This includes the minimum and maximum frames
  included in the playback, whether the playback loops, and whether
  the animation runs in all views, or only the active view, etc.

  Methods also exist that mirror the functionality of the controls
  found on the time slider in the UI to start and stop the playback.
*/
class OPENMAYAANIM_EXPORT MAnimControl
{
public:

	//! Animation playback modes.
	enum PlaybackMode {
		kPlaybackOnce,			//!< Play once then stop.
		kPlaybackLoop,			//!< Play continuously.
		kPlaybackOscillate		//!< Play forwards, then backwards continuously.
	};

	//! Animation playback viewing modes.
	enum PlaybackViewMode {
		kPlaybackViewAll,		//!< Playback in all views
		kPlaybackViewActive		//!< Playback in only the active view
	};


											MAnimControl();
	virtual									~MAnimControl();
	static MAnimControl::PlaybackMode		playbackMode();
	static MStatus							setPlaybackMode( PlaybackMode
															 newMode );
	static MAnimControl::PlaybackViewMode	viewMode();
	static MStatus							setViewMode( PlaybackViewMode
														 newMode );
	static double							playbackBy();
	static MStatus							setPlaybackBy( const double& newTime );
	static MTime							minTime();
	static MTime							maxTime();
	static MStatus							setMinTime( MTime newMinTime );
	static MStatus							setMaxTime( MTime newMaxTime );
	static MStatus							setMinMaxTime( MTime min,
														   MTime max );
	static MTime							animationStartTime();
	static MTime							animationEndTime();
	static MStatus							setAnimationStartTime( MTime newStartTime );
	static MStatus							setAnimationEndTime( MTime newEndTime );
	static MStatus							setAnimationStartEndTime( MTime newStartTime,
														   MTime newEndTime );
	static MTime							currentTime();
	static MStatus							setCurrentTime( const MTime&
															newTime );
	static double							playbackSpeed();
	static MStatus							setPlaybackSpeed( double newSpeed );
	static MStatus							playForward();
	static MStatus							playBackward();
	static bool								isPlaying();
	static bool								isScrubbing();
	static MStatus							stop();
	static bool			autoKeyMode ();
	static MStatus		setAutoKeyMode ( bool mode );
	static MFnAnimCurve::TangentType globalInTangentType (
												MStatus * ReturnStatus = NULL );
	static MStatus		setGlobalInTangentType (const MFnAnimCurve::TangentType
												&tangentType );
	static MFnAnimCurve::TangentType globalOutTangentType (
												MStatus * ReturnStatus = NULL );
	static MStatus		setGlobalOutTangentType (const MFnAnimCurve::TangentType
												 &tangentType );
	static bool			weightedTangents( MStatus * ReturnStatus = NULL );
	static MStatus		setWeightedTangents( bool weightState );

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAnimControl */
