#ifndef __MPxThreadedDeviceNode_h
#define __MPxThreadedDeviceNode_h
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
// CLASS:    MPxThreadedDeviceNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MPxNode.h>
#include <maya/MTypes.h>
#include <maya/MObjectArray.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxThreadedDeviceNode)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for threaded node creation 
/*!
  MPxThreadedDeviceNode is the base class used for creating threaded maya device
  nodes.

  Maya handles thread management.  The plug-in needs to implement the
  threadHandler() and threadShutdownHandler().  Maya will call these
  handlers as it manages the thread.
  
  The interface implements a memory queue system where data pointers are
  acquired, filled, then pushed into the node using mutex locking. This is done 
  from a separate, secondary thread. When the data has been acquired, the thread 
  will mark the output attribute as dirty so that the compute in the main thread
  will be called. The main thread will pop the oldest data buffer, process it,
  then release the thread data. The helper class MCharBuffer is used with the 
  memory methods.

  The "live" attribute allows finer control over the secondary thread.
  It can be used to stop the handling of incoming data without disconnecting
  the output attributes. Note that the thread continues to run even if the
  the live attribute is set to false.
  
  To implement an MPxThreadedDeviceNode plug-in, you must:
  1. Use the post constructor to call setRefreshOutputAttributes()
  	 and createMemoryPools()
  2. Implement the destructor to destroy the memory pools
  3. Implement the threadHandler() virtual.  Maya will call
	 this handler when it creates the thread. Typically the
	 thread handler will implement a loop for collecting
	 data.  This class contains a "done" state which is
	 accessible via the isDone() and setDone() methods.
	 Loop while isDone() is false
  4. Implement the threadShutdownHandler(). This method would
	 be called by Maya on File -> New and Exit for example.
	 Call setDone( true ) to stop the isDone() loop implemented
	 in the threadHandler if the plug-in uses the done state
  5. The secondary thread should call beginThreadLoop() and 
  	 endThreadLoop() with the data operations happening in-between.
	 The endThreadLoop() method will perform the throttling based on the 
	 frame rate attribute and additionally cause the output 
	 attribute to be marked dirty. This in turn will cause 
	 compute() to be called in the main thread once Maya processes
	 the dirty message
  6. The thread loop should acquireDataStorage() to get a buffer,
	 populate the buffer with information and then call pushThreadData()
  8. The compute method should popThreadData() and if it's valid,
	 process the data and release the data pointer by calling
	 releaseDataStorage().

  The live and frameRate attributes are included in this class. Changes
  to these attributes will cause the thread to be shutdown and restarted
  so that the new settings can be in effect.

  If this node is deleted then the thread will automatically be shutdown
  when the node is placed on the undo queue.  Undoing the delete will
  restart the thread if it was running at the time of the delete.

  NOTE: This class relies on the idle event queue to perform the node
  attribute updating.  As a result, it will not work in Maya batch mode.
*/

class OPENMAYAUI_EXPORT MPxThreadedDeviceNode  : public MPxNode
{
public:
	MPxThreadedDeviceNode();

	virtual ~MPxThreadedDeviceNode();

	virtual MPxNode::Type type() const;

	//
	// Thread handlers
	//

	virtual void threadHandler();

	virtual void threadShutdownHandler();

	//
	// Thread control
	//

	bool isLive();

	bool isDone();

	MStatus setDone( bool done );

	//
	// Thread throttling
	//

	MStatus beginThreadLoop();

	MStatus endThreadLoop();

	//
	// Memory management
	//

	MStatus createMemoryPools( int nCount, int nElements, long nSize );

	MStatus destroyMemoryPools();

	MStatus acquireDataStorage( MCharBuffer &buffer );

	MStatus pushThreadData( MCharBuffer& buffer );

	MStatus popThreadData( MCharBuffer &buffer );

	long threadDataCount();

	MStatus releaseDataStorage( MCharBuffer& buffer );

	//
	// Attributes
	//

	MStatus setRefreshOutputAttributes( const MObjectArray& attributeList );

	//! Output data, generic attribute
	static MObject output;
	//! Makes node live, boolean attribute
	static MObject live;
	//! Capture frame rate, double attribute
	static MObject frameRate;

	static const char *className();

private:
	static void initialSetup();
};

// ****************************************************************************
// CLASS DECLARATION (MCharBuffer)

//! \ingroup OpenMayaUI
//! \brief Memory buffer utility class
/*!
	Simple utility class so that we do not pass a reference
	to a char*
*/
class OPENMAYAUI_EXPORT MCharBuffer
{
public:
	MCharBuffer();

	char *ptr();
	long size();
private:
	// Unimplemented
	MCharBuffer( const MCharBuffer& other );
	MCharBuffer& operator=( const MCharBuffer & other );
	// Called from friend
	void setPtr( char *ptr );
	void setSize( long size );
	friend class MPxThreadedDeviceNode;
private:
	char *bufferPtr;
	long bufferSize;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* _cplusplus */
#endif /* MPxThreadedDeviceNode */
