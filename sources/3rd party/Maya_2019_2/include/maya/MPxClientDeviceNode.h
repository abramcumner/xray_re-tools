#ifndef __MPxClientDeviceNode_h
#define __MPxClientDeviceNode_h
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
// CLASS:    MPxClientDeviceNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MPxThreadedDeviceNode.h>
#include <maya/MTypes.h>
#include <maya/MObjectArray.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxClientDeviceNode)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for client device creation 
/*!
  MPxClientDeviceNode is an extension of MPxThreadedDevice node, and is
  intended for creating maya devices that act as clients.  An example 
  of this will be a Maya device that listens for animation data that is
  sent from another application.

  See MPxThreadedDevice node for details on how to use the memory queue system

  Plug-ins should create the MPxClientDeviceNode with a kCustomClient
  parameter.

  This class contains the serverName and deviceName attributes.  If these
  attributes are changed, the node will restart the thread so that 
  processing will continue with the new settings.

  NOTE: This class relies on the idle event queue to perform the node
  attribute updating.  As a result, it will not work in Maya batch mode.
*/

class OPENMAYAUI_EXPORT MPxClientDeviceNode : public MPxThreadedDeviceNode
{
public:
	//! Defines the type client server
	enum ClientType 
	{
		kUser,					//!< Custom client implemented in a plug-in
		kInternal,				//!< Autodesk specific
	};

	MPxClientDeviceNode( ClientType typ = kUser );

	virtual ~MPxClientDeviceNode();

	virtual MPxNode::Type type() const;

	ClientType clientType();

	// Thread handler
	virtual void threadHandler( const char* serverName, const char* deviceName );

	//
	// Attributes
	//

	//! Server
	static MObject serverName;
	//! Device
	static MObject deviceName;

	static const char *className();
private:
	MPxClientDeviceNode::ClientType clientTyp;

	static void initialSetup();
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* _cplusplus */
#endif /* MPxClientDeviceNode */
