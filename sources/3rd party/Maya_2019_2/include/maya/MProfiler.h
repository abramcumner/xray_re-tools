#ifndef _MProfiler
#define _MProfiler
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
// CLASS:    MMProfiler
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MStringArray.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MDeprecate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MProfiler)

//! \ingroup OpenMaya
//! \brief Class for working with Maya's profiler. 
/*!
  MProfiler is a static class which provides access to Maya's profiler.
  Maya's profiler is used to profile execution time of instrumented code.
  User can review the profiling result in the Profiler Window. Or use the 
  MEL command "profiler" to save the profiling result to a file.

  The profiling results are represented by two types of events: Normal
  Event and Signal Event.
  Normal Event has a duration representing the execution time of the
  instrumented code.
  A Signal Event only remembers the start moment and has no knowledge about
  duration. It can be used in cases when the user does not care about the  
  duration but only cares if this event does happen. 

  This class provides access to profiler settings and the profiling result. 
  To instrument a specific code snippet you must create an instance of MProfilerScope. 
  See that class for more details.

  This class does not provide the ability to start/stop the recording.
  Recording can be started/stopped using either the Profiler Window or 
  the "profiler -sampling true/false" command. 
  When recording is active only those categories which have been set to 
  be recorded (i.e. categoryRecording(categoryId) returns true) will have 
  their profiling data included in the recording.

  There are two ways to instrument code, one is to use MProfiler::eventBegin
  and MProfier::eventEnd, which give user more flexible control over when to
  start and stop the profiling for certain part of code, but it is also the 
  user's responsibility to call eventEnd for eventBegin, if user fails to do
  so, the event will be taken as a signal event. 
  The other way is to use MProfilingScope, which will stop the event automatically 
  when the MProfilingScope object is out of its life scope.
*/

class OPENMAYA_EXPORT MProfiler
{
public:

    //! Colors for different profiling categories in Profiler Window.
    enum ProfilingColor
    {
        kColorA_L1 = 0,
        kColorA_L2,
        kColorA_L3,
        kColorB_L1,
        kColorB_L2,
        kColorB_L3,
        kColorC_L1,
        kColorC_L2,
        kColorC_L3,
        kColorD_L1,
        kColorD_L2,
        kColorD_L3,
        kColorE_L1,
        kColorE_L2,
        kColorE_L3,
        kColorG_L1,
        kColorG_L2,
        kColorG_L3,
        kColorCount
    };

    // Methods for category
    static int	addCategory				(const char* categoryName, const char* categoryInfo);
    static int	removeCategory			(const char* categoryName);
    static void	getAllCategories		(MStringArray& categoryNames, MStringArray& categoryInfo);
    static int	getCategoryIndex		(const char* categoryName);
    static const char* getCategoryName	(int categoryId);
    static const char* getCategoryInfo	(int categoryId);
    static bool	categoryRecording		(const char* categoryName);
    static bool	categoryRecording		(int categoryId);
    static void	setCategoryRecording	(const char* categoryName, bool active);
    static void	setCategoryRecording	(int categoryId, bool active);
    
    // Method for signal event
    static void signalEvent(int categoryId, ProfilingColor colorIndex, const char* eventName, const char* description = NULL);

    // Methods for normal event
    static int eventBegin(int categoryId, ProfilingColor colorIndex, const char* eventName, const char* description = NULL);
    static void eventEnd(int eventId);

    // Methods to access data in profiling buffer
    static int		getEventCount		();
    static MUint64	getEventTime		(int eventIndex);
	static int		getEventDuration	(int eventIndex);
    static const char* getEventName		(int eventIndex);
    static const char* getDescription	(int eventIndex);
    static int		getEventCategory	(int eventIndex);
    static int		getThreadId			(int eventIndex);
    static int		getCPUId			(int eventIndex);
    static bool		isSignalEvent		(int eventIndex);
    static MProfiler::ProfilingColor getColor(int eventIndex);
	static int		getThreadDuration	(int eventIndex);

    // Methods for profiling buffer size
    static int		getBufferSize	();
    static MStatus	setBufferSize	(int sizeInMegaytes);

	// Methods for handling recordings
	static bool recordingActive();
	static bool eventDataAvailable();
	static bool resetRecording();
	static void setRecordingActive(bool val);
	static bool saveRecording(const MString &fileName);
	static bool loadRecording(const MString &fileName);
	static bool isDataFromFile();

	static int getNumberOfCPUs();

	static const char* className	();

private:
    MProfiler();
    ~MProfiler();

	//======================================================================
	//
obsolete_2019:
	OPENMAYA_DEPRECATED(2019, "Call addCategory(const char*, const char*) instead.")
    static int	addCategory		(const char* categoryName);
	OPENMAYA_DEPRECATED(2019, "Call getAllCategories(MStringArray&, MStringArray&) instead.")
    static void	getAllCategories(MStringArray& categoryNames);
};


// ****************************************************************************
// CLASS DECLARATION (MProfilingScope)

//! \ingroup OpenMaya
//! \brief MProfilingScope is used to profile code execution time.
/*!
  Profiling begins with the creation of an MProfilingScope instance 
  and ends when the instance is destroyed (e.g. when it goes out of  
  scope at the end of the block in which it was declared).

  For example, if you want to profile any code snippet, you can instrument
  it like this:
  \code
  {
      MProfilingScope profilingScope(testCategory, MProfiler::kColorD_L1, "eventName", "eventDescription", associatedNode);
     
      //////////////////////////////////////////////////////////////////////////////
      // The code snippet you want to profile.
      //////////////////////////////////////////////////////////////////////////////
  }
  \endcode
  When profilingScope is out of its life scope, the profiling stops.

  You can also assign an associated node during the instrumentation, then all the profiling events generated from this
  instrumentation can benefit from the ability of the profiler view to select an associated dependency node for certain events.
*/
class OPENMAYA_EXPORT MProfilingScope
{
public:

    MProfilingScope(int categoryId, 
		            MProfiler::ProfilingColor colorIndex, 
					const char* eventName, 
					const char* description = NULL, 
					const MObject& associatedNode = MObject::kNullObj,
					MStatus * ReturnStatus = NULL);
    ~MProfilingScope();
	
	static const char* className();

private:
    int		mEventId;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MProfiler */

