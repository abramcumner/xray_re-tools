#ifndef _MTimer
#define _MTimer
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
// CLASS:    MTimer
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MTimer)
//
// The MTimer class works similar to timerX MEL command.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MTimer)

//! \ingroup OpenMaya
//! \brief This class works similar to timerX MEL command.
/*!
  MTimer class provides the functionality for calculating the time
  elapsed/consumed in executing a particular portion of the code. The
  portion of the code to be timed is embedded inbetween the calls
  to'beginTimer()' and 'endTimer()'. A call to 'elapsedTime()' gives
  the time spent in executing that portion of the code.  The
  calculated time in seconds will be with 2 decimal accuracy.
*/
class OPENMAYA_EXPORT MTimer
{

public:
					MTimer();
					MTimer( const MTimer & );
					~MTimer();

					void beginTimer();
					void endTimer();
					double elapsedTime();
	MTimer&			operator =  ( const MTimer& rhs );
	bool			operator == ( const MTimer& rhs ) const;
	bool			operator != ( const MTimer& rhs ) const;
	void			clear();

private:
	double				getCurrentTime();

	double				startTime;
	double				endTime;

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MTimer */
