#ifndef _MMessage
#define _MMessage
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
// CLASS:    MMessage
//
// ****************************************************************************

/*! \file
    \brief Base class and typedefs for setting callbacks on Maya messages.
*/

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// DECLARATIONS

//! Type used to hold callback identifiers.
typedef MUintPtrSz MCallbackId;

class QObject;

OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \internal Declaration for the callback table
struct OPENMAYA_EXPORT MMessageNode {
	void* fClientPtr;
	void* fServerPtr;
	void* fSubClientPtr;
	MCallbackId fId;
	struct MMessageNode * fNextNode;	// Points to the next message node in a chain of nodes
	struct MMessageNode * fHeadNode;	// Points to the head message node in a chain of nodes
	bool isValid;							//!< Unused

	MMessageNode();
};

OPENMAYA_NAMESPACE_CLOSE

typedef OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMessageNode * MMessageNodePtr;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MMessage)

//! \ingroup OpenMaya
//! \brief Message base class.
/*!
	This is the base class for message callbacks. This base class allows the
	user to remove a message callback.  To register a callback, the user must
	use the addCallback methods in the message classes which inherit from this
	base class.

	When a callback is added a number or id is returned. This id is used to
	keep track of the callback and is necessary for removing it.
	A callback id with the value 'NULL' represents an invalid callback.
	Use	the removeCallback member function of this class for removing a
	callback.

	It is the user's responsibility to keep track of the callback id's
	and remove all callbacks for a plug-in when it is unloaded.

	<b>Callbacks During File Read</b>

	Care must be taken when a callback executes while Maya is reading a
	scene file as the scene may be temporarily in an inconsistent state,
	which could give incorrect results.

	For example, consider a scene which has two nodes, X and Y, with a
	connection from <i>X.out</i> to <i>Y.in</i>. A "node added" message
	(see MDGMessage::addNodeAddedCallback) will be sent out for Y as soon
	as it is added to the scene, but before the connection has been made
	to <i>Y.in</i>. If a callback were to read the value of <i>Y.in</i> at
	this point it would get the wrong result.

	The difficulties are compounded by the fact that Maya's normal
	Dependency Graph evaluation and dirty propagation mechanisms are
	disabled during a file read. In our example above, retrieving the
	value of <i>Y.in</i> will mark the plug as clean. When the connection
	is from <i>X.out</i> is subsequently made, Y.in won't be marked dirty
	because dirty propagation is disabled. As a result, when the file read
	has completed and Maya draws the new scene, <i>Y.in</i> will not be
	re-evaluated which may produce an error in the draw.

	For these reasons, callbacks should avoid querying or modifying the DG
	in any way during file read. They should not add or remove nodes, make
	or break connections, change parenting, set or retrieve plug values,
	etc. Even something as innocuous as a call to
	MFnDagNode::isIntermediateObject may cause problems since it queries
	the value of the node's <i>intermediateObject</i> plug. All such
	actions should be postponed until the file read has completed.
*/
class OPENMAYA_EXPORT MMessage
{

public:
	//! Callback result action codes.
	typedef enum {
        kDefaultAction,		//!< do the action or not, whatever is the default
		kDoNotDoAction,		//!< do not do the action
		kDoAction,			//!< do the action
    } Action;        

	//! \brief Pointer to a basic callback function.
	/*!
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MBasicFunction)( void* clientData );

	//! \brief Pointer to an elapsed time callback function.
	/*!
	\param[in] elapsedTime The amount of time since the callback was last called.
	\param[in] lastTime The execution time at the previous call to this callback.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MElapsedTimeFunction)( float elapsedTime, float lastTime, void* clientData );

	//! \brief Pointer to callback function which returns a true/false result.
	/*!
	\param[in] retCode Result of the function. The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MCheckFunction)( bool* retCode, void* clientData );

	//! \brief Pointer to a callback function which takes a file object and returns a result.
	/*!
	\param[in] retCode Result of the function. The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] file File object. The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MCheckFileFunction)( bool* retCode, MFileObject& file, void* clientData );

	//! \brief Pointer to a callback function which takes a plug and returns a result.
	/*!
	\param[in] retCode Result of the function. The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] plug The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MCheckPlugFunction)( bool* retCode, MPlug& plug, void* clientData );

	//! \brief Pointer to a callback function which takes an array of component ids.
	/*!
	\param[in] componentIds Array of component ids.
	\param[in] count Number of component ids in the array.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MComponentFunction)(MUintArray componentIds[], unsigned int count,void *clientData);

	//! \brief Pointer to a callback function which takes a dependency node.
	/*!
	\param[in,out] node The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MNodeFunction)(MObject& node,void *clientData);

	//! \brief Pointer to callback function which takes a string.
	/*!
	\param[in] str The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MStringFunction)( const MString& str,void* clientData );

	//! \brief Pointer to a callback function which takes two strings.
	/*!
	\param[in] str1 Meaning depends upon the message for which the callback was registered.
	\param[in] str2 Meaning depends upon the message for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MTwoStringFunction)( const MString& str1, const MString& str2, void* clientData );


	//! \brief Pointer to a callback function which takes three strings.
	/*!
	\param[in] str1 Meaning depends upon the message for which the callback was registered.
	\param[in] str2 Meaning depends upon the message for which the callback was registered.
	\param[in] str3 Meaning depends upon the message for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MThreeStringFunction)( const MString& str1,const MString& str2,const MString& str3,void* clientData );

	//! \brief Pointer to callback function which takes a string, an index, a flag and a type
	/*!
	\param[in] str The meaning depends upon the specific message type for which the callback was registered.
	\param[in] index The meaning depends upon the specific message type for which the callback was registered.
	\param[in] flag The meaning depends upon the specific message type for which the callback was registered.
	\param[in] type The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MStringIntBoolIntFunction)( const MString& str, unsigned int index, bool flag, unsigned int type, void* clientData );

	//! \brief Pointer to callback function which takes a string and index.
	/*!
	\param[in] str The meaning depends upon the specific message type for which the callback was registered.
	\param[in] index The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MStringIndexFunction)( const MString &, unsigned int index, void* clientData );
	//! \brief Pointer to a callback function which takes a node, a string and a boolean.
	/*!
	\param[in,out] node The meaning depends upon the specific message type for which the callback was registered.
	\param[in] str The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MNodeStringBoolFunction)( MObject& node, const MString &,bool, void* clientData );

	//! \brief Pointer to a callback function which takes a boolean state.
	/*!
	\param[in] state The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MStateFunction)( bool state,void* clientData );

	//! \brief Pointer to callback function which takes a time.
	/*!
	\param[in,out] time The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MTimeFunction)( MTime& time,void* clientData );

	//! \brief Pointer to plug connection callback function.
	/*!
	\param[in,out] srcPlug Plug which is the source of the connection.
	\param[in,out] destPlug Plug which is the destination of the connection.
	\param[in] made True if the connection is being made, false if the connection is being broken.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MPlugFunction)( MPlug& srcPlug, MPlug& destPlug,bool made,void* clientData );

	//! \brief Pointer to a callback function which takes a dependency node and a plug.
	/*!
	\param[in,out] node The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] plug The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MNodePlugFunction)( MObject& node,MPlug& plug,void* clientData );

	//! \brief Pointer to a callback function which takes a dependency node and a string.
	/*!
	\param[in,out] node The meaning depends upon the specific message type for which the callback was registered.
	\param[in] str The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MNodeStringFunction)( MObject & node,const MString & str,void* clientData );

	//! \brief Pointer to to a callback function which takes two DAG nodes in a parent/child relationship.
	/*!
	\param[in,out] child Path to the child node.
	\param[in,out] parent Path to the parent node.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MParentChildFunction)( MDagPath &child, MDagPath &parent, void * clientData );

	//! \brief Pointer to a callback function which takes a DG modifier.
	/*!
	\param[in,out] modifier The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MModifierFunction)( MDGModifier& modifier, void* clientData );

	//! \brief Pointer to a callback function which takes a string array.
	/*!
	\param[in] strs The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MStringArrayFunction)( const MStringArray& strs, void* clientData );

	//! \brief Pointer to a callback function which takes a dependency node and a DG modifier.
	/*!
	\param[in,out] node The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] modifier The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MNodeModifierFunction)( MObject &node, MDGModifier& modifier, void* clientData );

	//! \brief Pointer to a callback function which takes an array of objects.
	/*!
	\param[in,out] objects The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MObjArray)(MObjectArray &objects,void *clientData);

	//! \brief Pointer to a callback function which takes a dependency node and an array of objects.
	/*!
	\param[in,out] node The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] objects The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MNodeObjArray)(MObject &node,MObjectArray &objects,void *clientData);

	//! \brief Pointer to a callback function which takes a string and a dependency node.
	/*!
	\param[in] str The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] node The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MStringNode)( const MString &str, MObject &node, void* clientData );

	//! \brief Pointer to a callback function which takes a dependency node, an unsigned integer, and a boolean value.
	/*!
	\param[in] node The node should be a cameraSet node.
	\param[in] unsigned int  The integer refers to a camera layer index in the given cameraSet node.
	\param[in] bool  The value denotes whether the given camera layer has been added or removed.  A value of true means the layer was added.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MCameraLayerFunction)(MObject& cameraSetNode, unsigned int multiIndex, bool added, void *clientData);

	//! \brief Pointer to a callback function which takes a dependency node, an unsigned integer, and two camera transform nodes.
	/*!
	\param[in] node The node should be a cameraSet node.
	\param[in] unsigned int  The integer refers to a camera layer index in the given cameraSet node.
	\param[in] node The node should be a camera transform previously assigned to the given camera layer.
	\param[in] node The node should be a camera transform newly assigned to the given camera layer.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MCameraLayerCameraFunction)(MObject& cameraSetNode, unsigned int multiIndex, MObject &oldCamera, MObject &newCamera, void *clientData);

	//! \brief Pointer to connection-failed callback function.
	/*!
	\param[in,out] srcPlug Plug which was to be the source of the connection, or a null plug if the plug did not exist.
	\param[in,out] destPlug Plug which was to be the destination of the connection, or a null plug if the plug did not exist.
	\param[in] srcPlugName The plug name which was used to look up the source plug.
	\param[in] dstPlugName The plug name which was used to look up the destination plug.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MConnFailFunction)( MPlug& srcPlug, MPlug& destPlug, const MString& srcPlugName, const MString& dstPlugName,void* clientData );


	//! \brief Pointer to a callback function which takes an array of MPlugs and an MDGModifier.
	/*!
	\param[in,out] plugs The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] modifier The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MPlugsDGModFunction)(MPlugArray &plugs, MDGModifier &modifier, void *clientData);

	//! \brief Pointer to a callback function which takes a dependency node and a UUID.
	/*!
	\param[in,out] node The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] uuid The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MNodeUuidFunction)( MObject &node, const MUuid &uuid, void* clientData );

	//! \brief Pointer to a callback function which takes a dependency node and a UUID, and returns a result.
	/*!
	\param[in] doAction The default action to be taken by the function, if kDefaultAction is returned. The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] node The meaning depends upon the specific message type for which the callback was registered.
	\param[in,out] uuid The meaning depends upon the specific message type for which the callback was registered.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	\return Action to be taken as a result of the function. The meaning depends upon the specific message type for which the callback was registered.
	*/
	typedef Action (*MCheckNodeUuidFunction)( bool doAction, MObject &node, MUuid &uuid, void* clientData );

	//! \brief Pointer to a callback function which takes an object and a file object.
	/*!
	\param[in] referenceNode The object (typically a DG node) to which the callback relates.
	\param[in] file The resolved file path of the referenced file.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MObjectFileFunction)( const MObject& object, const MFileObject& file, void* clientData );

	//! \brief Pointer to a callback function which takes an object and a file object and returns a result.
	/*!
	\param[in] retCode Result of the function. The meaning depends upon the specific message type for which the callback was registered.
	\param[in] referenceNode The object (typically a DG node) to which the callback relates.
	\param[in] file The resolved file path of the referenced file.
	\param[in] clientData Pointer to user-defined data supplied when the callback was registered.
	*/
	typedef void (*MCheckObjectFileFunction)( bool* retCode, const MObject& referenceNode, MFileObject& file, void* clientData );

	//! \brief Pointer to a callback function which receives origin, size and image data.
	/*!
	\param[out] originX X coordinate of the tile's origin point relative to the final image.
	\param[out] originY Y coordinate of the tile's origin point relative to the final image.
	\param[out] tileMaxX X coordinate of the tile's end point relative to the final image.
	\param[out] tileMaxY Y coordinate of the tile's end point relative to the final image.
  \param[out] tileData the pixel data for the tile.
	*/
  typedef void (*MRenderTileFunction)( int originX, int originY, int tileMaxX, int tileMaxY, const void* tileData );

	static MStatus  removeCallback( MCallbackId id );
	static MStatus  removeCallbacks( MCallbackIdArray &ids );
	static MCallbackId	currentCallbackId( MStatus* ReturnStatus = NULL );
	static MStatus	nodeCallbacks( MObject& node, MCallbackIdArray& ids );

	static void setRegisteringCallableScript();
	static bool registeringCallableScript();
	static void stopRegisteringCallableScript();

	static void setRegisteringCallableScriptNewAPI();
	static bool registeringCallableScriptNewAPI();

	static const char* 		className();

BEGIN_NO_SCRIPT_SUPPORT:

	// Obsolete, no script support
	OPENMAYA_DEPRECATED(2019, "Please use the other MMessage::removeCallbacks(MCallbackIdArray &idList) method.")
	static MStatus  removeCallbacks( MIntArray &ids );
	// Obsolete, no script support
    OPENMAYA_DEPRECATED(2019, "Please use the other MMessage::nodeCallbacks method.")
	static MStatus	nodeCallbacks( MObject& node, MIntArray& ids );

END_NO_SCRIPT_SUPPORT:

protected:
    friend class MRenderCallback;

    static void addNode( MMessageNodePtr node );
    static void removeNode( MMessageNodePtr node );
    static void addQtUICallback( QObject* client );

private:
	static bool fRegisteringCallableScript;
	static bool fRegisteringCallableScriptNewAPI;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MMessage */
