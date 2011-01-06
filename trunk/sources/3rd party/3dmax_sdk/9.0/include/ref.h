//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// FILE:        ref.h
// DESCRIPTION: Defines reference system classes
// AUTHOR:      Rolf Berteig & Dan Silva
// HISTORY:     created 9 September 1994
//**************************************************************************/

#ifndef _REF_H_
#define _REF_H_

#include "maxheap.h"
#include "strbasic.h"

/* This module implements a first run at the concept of references.
Some other stuff is in here too like time and intervals, but
these are implemented only to get the reference classes working. */

typedef void *ReferenceData;
typedef unsigned int TargetParam;

//==============================================================================
//	Part ID's
//==============================================================================

//! \brief This is NOT just a bitmask, the parameter is also used to stuff pointers, 
//! hence the ULONG_PTR. SR NOTE64: 
typedef ULONG_PTR PartID;

//! \name Defines PartID Bits
/* OBJECT STATE: SUB-PARTS These are the 4 sub-channels in the object state.
	Don't change these defines (See CHANNELS.H for define of TOPO_CHANNEL, etc.)
*/
//@{
#define PART_TOPO			TOPO_CHANNEL   
#define PART_GEOM			GEOM_CHANNEL   
#define PART_TEXMAP			TEXMAP_CHANNEL 
#define PART_MTL			MTL_CHANNEL    
#define PART_SELECT			SELECT_CHANNEL 
#define PART_SUBSEL_TYPE 	SUBSEL_TYPE_CHANNEL
#define PART_DISPLAY    	DISP_ATTRIB_CHANNEL
#define PART_VERTCOLOR		VERTCOLOR_CHANNEL
#define PART_GFX_DATA		GFX_DATA_CHANNEL
#define PART_DISP_APPROX	DISP_APPROX_CHANNEL
#define PART_EXTENSION		EXTENSION_CHANNEL
//@}

//! \name Defines PartID object states
//! These represent the transform matrix and material parts of the object state.
//@{
#define PART_TM_CHAN   		TM_CHANNEL
#define PART_MTL_CHAN		GLOBMTL_CHANNEL

#define PART_OBJECT_TYPE   	(1<<11)      
//! node transform matrix
#define PART_TM  			(1<<12)		
#define PART_OBJ  			(PART_TOPO|PART_GEOM)

#define PART_ALL			(ALL_CHANNELS|PART_TM)
//@}

//! \name Defines PartID Misc
//@{
//! Special part ID that prevents the radiosity engine from processing the change
#define PART_EXCLUDE_RADIOSITY (1<<29)

//! Special part ID sent by visibility controllers when they change the hidden in
//! viewport state. Sent with REFMSG_CHANGE message
#define PART_HIDESTATE		(1<<30)

//! Special part ID sent by MAXScript when it changes an object's parameter. Sent
//! with REFMSG_CHANGE message
#define PART_MXS_PROPCHANGE			(1<<16)
//@}

//! \name Defines PartID to notify dependents
/*! One of these is set as the PartID when notify dependents is called with 
REFMSG_FLAGDEPENDENTS */
//@{
#define PART_PUT_IN_FG				(1<<0)
#define PART_SHOW_DEPENDENCIES		(1<<1)
#define PART_SHOWDEP_ON				(1<<2)
//@}

// [DL|June.7.2001]
//! \name Defines PartID for changing GI properties
//! Part IDs sent with the REFMSG_NODE_GI_PROP_CHANGED message.
//@{
#define PART_GI_EXCLUDED				(1<<0)
#define PART_GI_OCCLUDER				(1<<1)
#define PART_GI_RECEIVER				(1<<2)
#define PART_GI_DIFFUSE				(1<<3)
#define PART_GI_SPECULAR				(1<<4)
#define PART_GI_NBREFINESTEPS			(1<<5)
#define PART_GI_MESHSIZE                (1<<6)
#define PART_GI_MESHINGENABLED          (1<<7)
#define PART_GI_USEGLOBALMESHING        (1<<8)
#define PART_GI_EXCLUDEFROMREGATHERING	(1<<9)
#define PART_GI_STOREILLUMMESH			(1<<10)
#define PART_GI_RAYMULT					(1<<11)
//! New in 3DS Max version 8.0
#define PART_GI_USE_ADAPTIVE_SUBDIVISION (1<<12)
//! New in 3DS Max version 8.0
#define PART_GI_MIN_MESH_SIZE			(1<<13)
//! New in 3DS Max version 8.0
#define PART_GI_INITIAL_MESH_SIZE		(1<<14)
//! New in 3DS Max version 8.0
#define PART_GI_CONTRAST_THRESHOLD		(1<<15)
//@}

// ASzabo|june.04.03
//! \name Defines PartID for changing render properties
//! PartID's sent with the REFMSG_NODE_RENDERING_PROP_CHANGED message.
//@{
#define PART_REND_PROP_RENDERABLE					(1<<0)
#define PART_REND_PROP_CAST_SHADOW					(1<<1)
#define PART_REND_PROP_RCV_SHADOW					(1<<2)
#define PART_REND_PROP_RENDER_OCCLUDED				(1<<3)
#define PART_REND_PROP_VISIBILITY					(1<<4)
#define PART_REND_PROP_INHERIT_VIS					(1<<5)
#define PART_REND_PROP_PRIMARY_INVISIBILITY			(1<<6)
#define PART_REND_PROP_SECONDARY_INVISIBILITY		(1<<7)
//! Part IDs sent with the REFMSG_NODE_DISPLAY_PROP_CHANGED message.
#define PART_DISP_PROP_IS_HIDDEN					(1<<0)
//! Part IDs sent with the REFMSG_NODE_DISPLAY_PROP_CHANGED message.
#define PART_DISP_PROP_IS_FROZEN					(1<<1)
//@}

//==============================================================================
//	Reference Messages
//==============================================================================
/* This section describes some of the common messages used by references and the
meaning of the PartID parameter associated with these messages. Some messages are
sent by the system while others are sent by the plugin. Each method may need to 
pass along additional information so the reference maker may process the message.
This information is passed in the PartID parameter. The meaning of the 
information stored in the PartID is specific to the message sent along with it.
\note that not all messages use the PartID parameter (in fact most don't). In 
these cases the PartID will be set to 0. If the plugin is sending the message, it
should set the PartID to 0 if not used. In the cases where the PartID is used, it
is documented below.

Developers who define their own reference messages should do so using a value greater than:

#define REFMSG_USER 0x00010000

The system uses numbers less than this value.
*/


//! \brief The message passed to notify and evaluate.
typedef unsigned int RefMessage;

//! \brief This tests for a cyclic reference. It will return REF_FAIL if there is a loop.
#define REFMSG_LOOPTEST 				0x00000010

//! \brief This message is sent when a reference target is deleted. 
/*! This allows the reference maker to handle this condition if it depends on 
the deleted item. For example this is sent when the item you are referencing is 
actually deleted and you need to set your pointer to the item to NULL.
*/
#define REFMSG_TARGET_DELETED 			0x00000020
//! \brief Used Internally
#define REFMSG_MODAPP_DELETING			0x00000021
//! \brief Used Internally
#define REFMSG_EVAL  					0x00000030
//! \brief Used Internally
#define REFMSG_RESET_ORIGIN 			0x00000040

//! \brief This is the most common message sent.
/*! Any time a reference target changes in a way that may affect items which 
reference it, this message should be sent. Note the following for the partIDs 
that are sent during this message:
-# <b>PART_HIDESTATE</b> This is a special partID sent by visibility controllers
	when they change the hidden in viewport state.
-# <b>PART_TM</b> This is passed in partID when the reference is to a node in the
	scene and its transformation matrix has changed.
-# <b>PART_OBJECT_TYPE</b> This is sent if the object type changes.

Objects and Modifier set the PartID to the channel which changed. See the section
on the Geometric Pipeline for more information on channels. 
There are several specific PartID referring to channels. These are:

\li <b>PART_TOPO</b> = TOPO_CHANNEL
\li <b>PART_GEOM</b> = GEOM_CHANNEL
\li <b>PART_TEXMAP</b> = TEXMAP_CHANNEL
\li <b>PART_MTL</b> = MTL_CHANNEL
\li <b>PART_SELECT</b> = SELECT_CHANNEL
\li <b>PART_SUBSEL_TYPE</b> = SUBSEL_TYPE_CHANNEL
\li <b>PART_DISPLAY</b> = DISP_ATTRIB_CHANNEL
\li <b>PART_VERTCOLOR</b> = VERTCOLOR_CHANNEL */
#define REFMSG_CHANGE 					0x00000050
//! \brief Used Internally
#define REFMSG_FLAGDEPENDENTS			0x00000070
//! \brief Used Internally
#define REFMSG_TARGET_SELECTIONCHANGE	0x00000080
//! \brief This is used by modifiers to indicate when they are beginning an edit. 
/*! For example, in SimpleMod::BeginEditParams() this message is sent. */
#define REFMSG_BEGIN_EDIT				0x00000090
//! \brief This is used by modifiers to indicate when they are ending an edit. 
/*! For example in SimpleMod::EndEditParams() this message is sent. Typically 
what a modifier will do while it is being edited it will have its LocalValidity()
return NEVER so that a cache is built before it. This will ensure it is more 
interactive while it is being edited. When this message is sent to indicate 
the edit is finished the system can discard the cache. */
#define REFMSG_END_EDIT					0x000000A0
//! \brief Used Internally
#define REFMSG_DISABLE					0x000000B0
//! \brief Used Internally
#define REFMSG_ENABLE					0x000000C0
//! \brief Used Internally
#define REFMSG_TURNON					0x000000D0
//! \brief Used Internally
#define REFMSG_TURNOFF					0x000000E0
//! \brief Used Internally
#define REFMSG_LOOKAT_TARGET_DELETED  	0x000000F0 
//! \brief Used Internally
#define REFMSG_INVALIDATE_IF_BG			0x000000F1

//! \brief This is used by modifiers to indicate that their apparatus (gizmo) is displayed.
/*! For example in SimpleMod::BeginEditParams() this message is sent. */
#define REFMSG_MOD_DISPLAY_ON			0x000000F2
//! \brief This is used by modifiers to indicate that their apparatus (gizmo) is no 
//! longer displayed.
#define REFMSG_MOD_DISPLAY_OFF			0x000000F3

//! Modifier uses this to tell Modapps to call their Eval() procedure.
/*! This is sent by a modifier to cause its ModApp to call Eval() on the modifier. 
If a modifier wants its ModifyObject() method to be called it can send this message.
The PartID should contain the bits that specify which channels are to be evaluated, 
for example PART_GEOM|PART_TOPO or ALL_CHANNELS. The interval passed should be set 
to Interval(t, t), where t is the time the to evaluate. Note that before 
NotifyDependents() returns, ModifyObject() will be called. */
#define REFMSG_MOD_EVAL					0x000000F4

//! \brief Ask if it is ok to change topology.  
/*! If any dependents have made topology-dependent modifications, they should 
	return REF_FAIL:
	\li A return of REF_SUCCEED means that the answer is YES, it is okay to change 
	the topology. 
	\li A return of REF_FAIL means that the answer is NO, it is not okay to change 
	the topology. */
#define REFMSG_IS_OK_TO_CHANGE_TOPOLOGY	0x000000F5


// This main purpose of these notifications is to cause the tree
// view to update when one of these events takes place.

//! \brief Sent by a node when it has a child linked to it or unlinked from it.
#define REFMSG_NODE_LINK				0x000000F6

//! \brief Sent by a node when it's name has been changed. 
/*! For example, the path controller displays the name of the node in the scene 
which it follows. It responds to this message by changing the name displayed in 
the UI. */
#define REFMSG_NODE_NAMECHANGE			0x000000F7

//! <b>DEPRECATED</b> Sent by a node (or derived object) when the object it references changes.
/*! \note This message is no longer used. It is basically synonymous with 
	REFMSG_SUBANIM_STRUCTURE_CHANGED. */
#define REFMSG_OBREF_CHANGE				0x000000F8

//! \brief Sent by a derived object when a modifier is a added or deleted.
#define REFMSG_MODIFIER_ADDED			0x000000F9

//! \brief Sent when an animatable switches controllers for one of it's parameters.
#define REFMSG_CONTROLREF_CHANGE		0x000000FA

//! \brief A parameter block sends the message to it's client when it needs the 
//! anim name of the ith parameter. 
//! PartID is set to a pointer to a GetParamName structure defined in iparamb.h
#define REFMSG_GET_PARAM_NAME			0x000000FB

//! \brief A parameter block sends this message to it's client when it needs to know
//! the dimension type of the ith parameter.
/*! PartID is set to a pointer to a GetParamDim structure  defined in iparamb.h */
#define REFMSG_GET_PARAM_DIM			0x000000FC

//! \brief A controller can send this to it's client to get it's param dimension.
//! It should set partID to a ParamDimension.
#define REFMSG_GET_CONTROL_DIM			0x000000FD

//! \brief Sent when a Nodes transform matrix (TM) has changed in a different time. 
/*! This message is sent by a node when it's TM has changed because it was evaluated
	at a different time. Normally this isn't necessary - anyone depending on the
	node's TM would have a validity interval that reflected the validity of the
	TM. The axis system doesn't store a validity interval (it probably should) so this
	message is needed for it. */
#define REFMSG_TM_CHANGE 				0x000000FE

//! A node sends this message when it's animation range changes.
#define REFMSG_RANGE_CHANGE				0x000000FF

//! Sent to the tree view when an animatable's line height changes.
#define REFMSG_LINEHEIGHT_CHANGE		0x00000100

//! \brief A controller should send this message to the track view when it becomes animated.
/*! If the user has the animated only filter on then the track view will display this item. */
#define REFMSG_BECOMING_ANIMATED		0x00000101

//! \brief Used in the TrackView to regenerate an objects sub components.
/*! This is intended mainly for the TrackView to tell it to regenerate it's view
	below the message sender's level. If a plugin has a variable number of 
	parameters this message may be used. */
#define REFMSG_SUBANIM_STRUCTURE_CHANGED 0x00000102

//! \brief A target has had a reference deleted.
/* Materials use this to tell the material whether or not the number of node
references has gone to zero. */
#define REFMSG_REF_DELETED			0x00000103

//! \brief A target has had a reference added. 
/* Materials use this to tell the material whether or not the number of node
references are non zero.*/
#define REFMSG_REF_ADDED			0x00000104

//! \brief Sent by an object that provides branching in the history to notify that
//! the structure of the branches changed.
#define REFMSG_BRANCHED_HISTORY_CHANGED	0x00000105

//! \brief The selection set sends this notification when it receives REFMSG_CHANGE
//! from an item in the selection set. 
/*! The selection set doesn't propagate the REFMSG_CHANGE message. */
#define REFMSG_NODEINSELSET_CHANGED	0x00000106

//! \brief This method is used to see if this reference target depends on something.
/*! New in 3DS Max version 2.0. If the partID is nonzero, the dependency test will 
include child nodes. Otherwise, child nodes will not be considered dependents. See
ReferenceTarget::BeginDependencyTest(). 
 */
#define REFMSG_TEST_DEPENDENCY	0x00000107

//! \brief Queries whether to display "Parameters" in the track view.
/*!	A Parameter block sends this to its client to ask if it should display a 
	distinct "Parameters" level in the track view hierarchy. A pointer to a 
	boolean is passed in for PartID: set this to the desired answer. The default
	is NO -- in this case the message doesn't need to be responded to. */
#define REFMSG_WANT_SHOWPARAMLEVEL 	0x00000108

//! \brief Sent before a paste has been done.
/*! Sent as partID is a pointer to a data structure containing three 
	RefTargetHandle's: the reference maker, the old target,  and the new target. The 
	message is sent to the reference maker initially. */
#define REFMSG_BEFORE_PASTE 	0x00000109
//! \brief Sent after a paste has been done.
/*! Sent as partID is a pointer to a data structure containing three 
RefTargetHandle's: the reference maker, the old target,  and the new target. The 
message is sent to the reference maker initially. */
#define REFMSG_NOTIFY_PASTE 	0x0000010A

//! \brief Sent when a UV Generator changes symmetry, so interactive texture display 
//! updates.
#define REFMSG_UV_SYM_CHANGE    0x0000010B

//! \brief Gets the node's name
/*! The first node that gets this message will fill in the MSTR, which partID 
points to, with its name and stop the message from propagating. */
#define REFMSG_GET_NODE_NAME			0x0000010C

//! Sent by the selection set whenever it has just deleted nodes
#define REFMSG_SEL_NODES_DELETED		0x0000010D

//! Sent before a reference target is pasted. Sent by the target about to be replaced.
#define REFMSG_PRENOTIFY_PASTE 	0x0000010E

//! \brief Sent when a shape starts changing.
/*! Sent when a shape enters a state where it'll be changing a lot and it
	would be a good idea for anybody using it for mesh generation to suppress
	updates. */
#define REFMSG_SHAPE_START_CHANGE	0x0000010F

//! \brief Sent when a shape stops changing.
#define REFMSG_SHAPE_END_CHANGE		0x00000110

//! \brief A texture map has been removed. 
/*! This tells the material editor to remove it from the viewport if it is active. */
#define REFMSG_TEXMAP_REMOVED	0x00000111

//! \brief Sent by an unselected node to see if any selected nodes depend on it.
/*! The PartID param points to a boolean. If a selected node receives this 
message it should set the boolean to true and return REF_STOP. */
#define REFMSG_FLAG_NODES_WITH_SEL_DEPENDENTS	0x00000112

//! \brief Sent by objects which contain shapes when the shape position changes.
#define REFMSG_CONTAINED_SHAPE_POS_CHANGE 0x00000120
//! \brief Sent by objects which contain shapes when the shape position changes.
#define REFMSG_CONTAINED_SHAPE_SEL_CHANGE 0x00000121
//! \brief Sent by objects which contain shapes when general changes occur.
#define REFMSG_CONTAINED_SHAPE_GENERAL_CHANGE 0x00000122

//! \brief Select sub-object branch.
/*! When an object receives this message it should do what ever it needs
	to do (usually select the appropriate sub-object) to make the dependent
	object be the object returned from GetPipeBranch().
	The partID will point to an INode pointer that will be filled in by
	the first node to receive this message. New in 3DS Max version 2.0*/
#define REFMSG_SELECT_BRANCH	0x00000130

//! brief Sent when a user begins a mouse operation in the viewport
/*! These messages are sent to dependents of the transform matrix controllers
for selected objects when the user begins and ends a mouse transformation in the
viewports (move/rotate/scale). New in 3DS Max version 2.0. */
#define REFMSG_MOUSE_CYCLE_STARTED		0x00000140
//! brief Sent when a user ends a mouse operation in the viewport
#define REFMSG_MOUSE_CYCLE_COMPLETED	0x00000150

//! \brief Sent when linking nodes
/*! Sent by a node to other nodes (which depend on that node) when the
user attempts to link another node to a node. The partID parameter contains a 
pointer to the new parent node. New in 3DS Max version 2.0*/
#define REFMSG_CHECK_FOR_INVALID_BIND	0x00000161

//! \brief Sent when a cache is dumped in the pipeline. 
/*! Sent when a cache is dumped in the pipeline. A REFMSG_CHANGE message used to
be sent, however that was misleading since the object itself didn't change even 
though any old object pointer has become invalid. For example, if a path 
controller depends on a spline object and that object dumps some caches in the 
pipeline, the path controller hasn't actually changed. 
Also modifiers that reference INode to only get its transformation data should
stop the propagation of this message. For example: 
\code
case REFMSG_OBJECT_CACHE_DUMPED:
	return REF_STOP;
	break;
\endcode
 New in 3DS Max version 2.0 */
#define REFMSG_OBJECT_CACHE_DUMPED	0x00000162

//! \brief Sent by Atmospheric or Effect when it makes or deletes a reference to a node.
/*! When Atmospherics or Effects add or delete a gizmo they should send this message 
via NotifyDependents(). New in 3DS Max version 3.0 */
#define REFMSG_SFX_CHANGE 		0x00000170

//! \brief Sent when updating object xrefs. 
/*! PartID contains new material. When a node receives this message
it will set its material to the new one. For internal use only. */
#define REFMSG_OBJXREF_UPDATEMAT	0x00000180
//! \brief Sent when updating object xrefs. 
/*! PartID contains new controller. When a node receives this message
it will set its controller to the new one. For internal use only. */
#define REFMSG_OBJXREF_UPDATECTRL	0x00000181

//! \name Defines XRef Messages
//! \brief Sent to build a list of nodes which use a particular XRef object. New in 3DS Max 8.0
//@{
//! \brief For Internal use only.
#define REFMSG_OBJXREF_GETNODES			0x00000190
/*! \brief PartID points to a table of base node pointers. For internal use only. New in 3DS Max 8.0*/ 
#define REFMSG_CTRLXREF_GETNODES		0x00000191
/*! \brief PartID points to a table of base node pointers. For internal use only. New in 3DS Max 8.0*/
#define REFMSG_MTLXREF_GETNODES			0x00000192
//@}

//! \brief Sent when objects are replaced from another scene (File->Replace). 
/*! Other objects referencing the object that is replaced may want to perform 
some validity checking; this message is more specific than 
REFMSG_SUMANIM_STRUCTURE_CHANGED. New in 3DS Max 3.0*/ 
#define REFMSG_OBJECT_REPLACED		0x00000200

//! \brief Sent when nodes wireframe color is changed
/*! New in 3DS Max 4.0 */ 
#define REFMSG_NODE_WIRECOLOR_CHANGED 0x00000210

//! \brief Indicates that the subobject types have changed and that the StackView should be updated.
/*! New in 3DS Max 4.0 */
#define REFMSG_NUM_SUBOBJECTTYPES_CHANGED 0x00000211

//! \brief Returns a unique (per session) node handle integer
/*! The partID contains a pointer to a ULONG. The first node that gets
this message will assign its node handle to this ULONG, and will return
REF_STOP to terminate further propogation of the message. New in 3DS Max 4.0.
*/
#define REFMSG_GET_NODE_HANDLE			0x00000220

//! \brief This will cause EndEditParams to be called on the object displayed in the modify panel
/*! New 3DS Max 4.0 */
#define REFMSG_END_MODIFY_PARAMS	0x00000230
//! \brief This will cause BeginEditParams to be called on the object displayed in the modify panel
/*! New 3DS Max 4.0 */
#define REFMSG_BEGIN_MODIFY_PARAMS 0x00000231

//! \brief Sent when a Tab<> is deleted.
/*! Sent by a ParamBlock2 to its owner whenever a reftarg element in a Tab<> 
	parameter is forcably deleted and the reference set to NULL (typically for 
	INODE_TABs when a scene node is deleted in the viewport. New in 3DS Max 4.0 */
#define REFMSG_TAB_ELEMENT_NULLED 0x00000232

//! \brief Sent to merged objects so that they can convert node handles.
/*! After merging nodes into the scene, all merged objects will receive this reference 
notification. The PartID will be a pointer to a merge manager interface that you 
can use to see if a specific handle was converted and convert between the old and  
the new handle; IMergeManager* pMergeManager = (IMergeManager*)partID; 
Node handles can change when a scene is merged and if you keep track of nodes
using their handles you need to intercept this message. The PartID will be a
pointer to an IMergeManager object that you can use to map between the old 
and new handle. */
#define REFMSG_NODE_HANDLE_CHANGED 0x00000233

//! \brief The pipeline was reevaluated and the wscache was updated.
/*! Sent from the node (without propagation) whenever the world state cache gets updated
 (e.g. when the pipeline gets reevaluated). */
#define REFMSG_NODE_WSCACHE_UPDATED 0x00000234

//! This notification is sent after a new material was assigned to a node
#define REFMSG_NODE_MATERIAL_CHANGED    0x00000235

//! \brief This notification is sent to dependents when a subanim's changes order.
/*!  It is used by things like scripted plugin's and custom attributes to tell 
	expression and wire controllers when the user redefines the ordering of 
	parameters so these controllers can keep pointing at the correct parameter.  
	The PartID is a Tab<DWORD>* in which each DWORD contains an old-to-new mapping
	with the LOWORD() = old subanim number and the HIWORD() = new subanim number.
	A new subanim ID of -1 implies the subanim was removed.   
	See maxsdk/samples/control/exprctrl.cpp for example use. NOTE: If you send 
	this message, the 'propagate' argument of NotifyDependents must be false. 
	Otherwise, dependents of dependents think that their ref's subAnim numbering
	is changing. JBW 11.7.00 New in 3DS Max version 4.0*/
#define REFMSG_SUBANIM_NUMBER_CHANGED    0x00000236   

//! \brief 
#define REFMSG_NODE_FLAGOMB_RENDER 0x00000237 

//! Notification sent AFTER the Global Illumination (radiosity) properties of a node changed.
/*! The PartId will contain information about the property that has changed. [dl 7june2001]. 
For Internal use only.*/
#define REFMSG_NODE_GI_PROP_CHANGED	0x00000238

//! \brief Sent when key selection changes.
/*! New in 3DS Max version 5.1 */
#define REFMSG_KEY_SELECTION_CHANGED 0x00000239

//! \brief Notification sent AFTER the Node Rendering Properties have changed
/*! The part id will contain information about the property that has changed. ASzabo|june.04.03*/
#define REFMSG_NODE_RENDERING_PROP_CHANGED	0x00000240
//! \brief Notification sent AFTER the Node Display Properties have changed
/*! The part id will contain information about the property that has changed. */
#define REFMSG_NODE_DISPLAY_PROP_CHANGED	0x00000241

//! \brief Message numbers above this value can be defined for use by sub-classes,
//! below are reserved.
#define REFMSG_USER		0x00010000


//! \name Messages sent from ReferenceMaker to ReferenceTarget
/*! \brief Codes used by a ReferenceMaker to send 'reverse' notification messages to a RefTarget.
Developers who define their own reference target messages should do so using a value greater than:
#define TARGETMSG_USER 0x00010000. New in 3DS Max 4.0. JBW 9.9.00 */
//@{
//! \brief Target notify message IDs above this value can be used by other plugins
/*! Best if large random IDs */
#define TARGETMSG_USER				0x00010000
//! \brief Send to a Node's ObjectRef when the node is attaching the object to itself	
#define TARGETMSG_ATTACHING_NODE	0x00000010		
//! \brief Send to a Node's ObjectRef when the node is about to be explicitly deleted
#define TARGETMSG_DELETING_NODE		0x00000020		
//! \brief Send to a Node's ObjectRef when the node is detaching the object from itself
/*! New in 3DS Max version 4.2. If your plugin utilizes this new mechanism, be 
sure that your clients are aware that they must run your plugin with 3ds max
version 4.2 or higher. */
#define TARGETMSG_DETACHING_NODE	0x00000030
//@}




//! \brief Return codes for references.
enum RefResult {
	//! The operation failed.
	REF_FAIL,		
	//! The operation succeeded.
	REF_SUCCEED,
	//! The results of the operation don't matter.
	REF_DONTCARE,
	//! Stop the operation, and the propogation of reference messages.
	REF_STOP,
	//! Attempts to delete an invalid reference will return this value.
	REF_INVALID,
	//! Return this from NotifRefChanged() when a REFMGS_TAGET_DELETED message is sent
	//! if a ReferenceMaker should be removed from the scene when all its references are removed.
	REF_AUTO_DELETE
};


// Use this to disable ref counting on objects.
#define MakeFakeHandleClass( className, handleClassName ) 	\
	class className; typedef className* handleClassName		

MakeFakeHandleClass( ReferenceTarget, RefTargetHandle );
MakeFakeHandleClass( ReferenceMaker, RefMakerHandle );

// This replaces the .Valid() method of handles.
#define VALID(x) (x)

//! If this super class is passed to NotifyDependents() all dependents will be notified
#define NOTIFY_ALL		0xfffffff0

class PostPatchProc;

//! \brief For remapping references during a Clone.
/*! This class is used for remapping references during a Clone. It is used when 
	cloning items that are instanced so that the plugin can maintain the same 
	instance relationship within the clone. All methods of this class are 
	implemented by the system. */
class RemapDir: public MaxHeapOperators {
	public:
//! \name Clone Methods
//@{
		//! \brief If Cloned, return the copy.
		/*! If an item has been cloned, this method will return the cloned copy 
			of it. If it has not been cloned, NULL will be returned.
			\param[in] from - The item to check.
			\return Returns the cloned copy.
		*/
		virtual	RefTargetHandle FindMapping(RefTargetHandle from) = 0;

		//! \brief Should be used to implement a clone on references.
		/*! In the ReferenceTarget::Clone() procedure when an item is cloning 
			itself it should clone all its references. Instead of calling Clone()
			on all of its references it should instead call this method passing 
			it the item to copy. This method will return a copy of the item or a
			pointer to a copy of the item if it was already copied.
			\param[in] oldTarg - This is the item that is to be copied.
			\return A copy of the item or a pointer to a copy of the item if it 
			was already copied.
		*/
		virtual RefTargetHandle CloneRef(RefTargetHandle oldTarg);

		//! \brief This method is used to patch the pointer for cloned items.
		/*!  This method is used by system plugins for example. The Ring Array 
		system has an array of nodes it maintains. When the system is cloned this
		method is used to clone the table of nodes. The new ring array master 
		controller will also have a table of nodes, but it does not want the 
		pointer to point to the old nodes, it should point to the new cloned nodes.
		The nodes may not be cloned yet at the time the master controller was 
		cloned however. This method allows the pointer to be changed to point at
		the new nodes.
		The following sample code demonstrates the use of this method to patch
		the pointer to the table of nodes maintained by the ring array system.
		\code
		RefTargetHandle RingMaster::Clone(RemapDir& remap) {
			int i;
			RingMaster* newm = new RingMaster();
			newm->ReplaceReference(0,remap->Clone(pblock));
			newm->numNodes = numNodes;
			newm->nodeTab.SetCount(numNodes);
			for (i=0; i<numNodes; i++) newm->nodeTab[i] = NULL;
			for (i=0; i<numNodes; i++) {
				remap.PatchPointer((RefTargetHandle*)&newm->nodeTab[i],	(RefTargetHandle)nodeTab[i]);
			}
			return(newm);
		}
		\endcode
		\param[out] patchThis - The pointer should point at this item.
		\param[in] oldTarg - The original target.
		*/
		virtual void PatchPointer(RefTargetHandle* patchThis, RefTargetHandle oldTarg) = 0;
		
		//! \brief Adds a Procedure that is called after cloning and back patching.
		/*! This method is used for adding a PostPatchProc whose procedure method
		    is called after the reference hierarchy has been cloned and any 
		    back patching has occurred. These will be called in the order that
		    they are registered.The PostPatchProc procs are called from 
		    the RemapDirImp::Backpatch(). More information can be found at the
		    RefTargMonitorRefMaker::Proc method. 
		    See NodeMonitor::Clone for an example.
		    \param[in] proc - Points to the callback object.
		    \param[in] toDelete - If true, the callback object is deleted when the RemapDir is deleted.
		*/
		virtual void AddPostPatchProc(PostPatchProc* proc, bool toDelete) = 0;
//@}

//! \name Internal methods
//@{
		//! \brief Used Internally
		virtual void AddEntry(RefTargetHandle hfrom, RefTargetHandle hto)=0;
		//! \brief Used internally
		virtual void Backpatch()=0;
		//! \brief Used internally
		virtual bool BackpatchPending()=0;
		//! \brief Used internally
		virtual void Clear()=0;
		//! \brief Used internally
		virtual void ClearBackpatch()=0;
		//! \brief Used internally
		virtual	void DeleteThis()=0;
		//! \brief Used internally
		virtual	~RemapDir() {}
//@}
};

class PostPatchProc: public MaxHeapOperators {
public:
	virtual ~PostPatchProc(){}
    virtual int Proc(RemapDir& remap) = 0;
};

//! \brief Reference list link-node
/*! A linked list of these objects constitute the list of ReferenceMakers that a
	ReferenceTarget keeps internally. These RefListItems are not accessed directly
	by the reference target, but are used in the RefList class. */
class CoreExport RefListItem: public MaxHeapOperators {
	
	public:
		RefMakerHandle  maker;
		RefListItem		*next;
		
		//! Constructor
		RefListItem( RefMakerHandle hmaker, RefListItem *list ) { 
			next = list;
			maker = hmaker;
		}
};

//! \brief Stores the list of active references for Reference Targets.
/*! Each Reference target uses one object of this class to store it's active list
of reference makers. Each item in the list is pointed to by a RefListItem which
are associated in a linked list. The method ReferenceTarget::GetRefList() returns
a list of references to a reference target.
*/
class CoreExport RefList: public MaxHeapOperators {
	public:
		//! \brief Public member that points to the first item in the reference
		//! list
		RefListItem* first;	

		//! \brief Constructor
		//! \return Returns a NULL pointer to the first Item in the list.
		RefList() { first = NULL;}

		//! \brief Gets the first item in the reference list
		RefListItem*  FirstItem() { return first; }

		//! \brief Deletes the specified item from the list.
		/*! \param[in] hmaker - The item to delete.
		\param[in] eval - If nonzero then when inside of NotifyDependents(),
			just set maker to NULL.   
		\return If the item was deleted REF_SUCCEED is returned; otherwise 
		REF_INVALID is returned.
		*/
		RefResult DeleteItem(RefMakerHandle hmaker, int eval);

		//! \brief Adds an item to the list by creating a RefListItem pointer.
		/*! The new	pointer is placed at the start of the list.
		\param[in] hmaker - Handle to the ReferenceMaker
			\return Always returns REF_SUCCEED.	*/
		RefResult AddItem(RefMakerHandle hmaker );

		//! \brief Removes Null Entries from the list.
		void Cleanup();
	};

class DeleteRefRestore;
class MakeRefRestore;
class ParamBlock;
class ISave;
class ILoad;
class ILoadImp;
class DependentIterator;

//! \name Defines enumeration return values
//! \brief Possible return values for DependentEnumProc::proc()
//@{
//! Continues enumeration
#define DEP_ENUM_CONTINUE	0
//! Halts enumeration
#define DEP_ENUM_HALT		1
//! Skip enumeration
#define DEP_ENUM_SKIP		2	
//@}

//! \brief A callback class for enumerating dependents.
/*! This class is a callback object for the ReferenceMaker::DoEnumDependentsImpl() 
and ReferenceMaker::DoEnumDependents() methods. The proc() method is called by the system. */
class DependentEnumProc: public MaxHeapOperators {

	friend class ReferenceTarget;

	class DependentEnumProcImplData;
	const MaxSDK::Util::AutoPtr<DependentEnumProcImplData> mDependentEnumProcImplData;

protected:
	CoreExport DependentEnumProc();
	CoreExport ~DependentEnumProc();

	// Hide it - no copy constructor or assignment to the public.
	DependentEnumProc(const DependentEnumProc&); // not implemented
	DependentEnumProc& operator=(const DependentEnumProc& rhs); // not implemented

public:
		//! \brief This is the method called by system from ReferenceTarget::DoEnumDependentsImpl().
		/*! 
			\param[in] rmaker - A pointer to the reference maker
			\return One of the following values:
			\li <b>DEP_ENUM_CONTINUE</b>: This continues the enumeration
			\li <b>DEP_ENUM_HALT</b>: This stops the enumeration.
			\li <b>DEP_ENUM_SKIP</b>: New in 3DS Max version 3.0. Reference Targets can
			have multiple Reference Makers (dependents). In certain instances 
			when DoEnumDependents() is used to enumerate them you may not want to 
			travel up all of the "branches". By returning DEP_ENUM_SKIP from this
			method you tell the enumerator to not enumerate the current Reference
			Maker's dependents but not to halt the eumeration completely.
			\see ReferenceTarget::DoEnumDependentsImpl(DependentEnumProc* dep)
			\see ReferenceTarget::DoEnumDependents(DependentEnumProc* dep)
		*/
		virtual	int proc(ReferenceMaker *rmaker)=0;

		//! \brief This method sets and checks whether a ReferenceMaker was visited
		/*! This method is used to check whether a ReferenceMaker was previously visited
			by this callback object, and registers it as having been visited if not.
			This is used to only call the proc on each ReferenceMaker once. If you 
			override ReferenceTarget::EnumDependentsImp, you should use this method
			to process your instance only if it was not previously visited. 
			\param[in] rmaker - A pointer to the reference maker
			\return Returns true of the rmaker was previously visited, false if not.
			\see ReferenceTarget::DoEnumDependentsImpl(DependentEnumProc* dep)
		*/
		CoreExport bool CheckIfAndSetAsVisited(ReferenceMaker *rmaker);

	};

//! \brief A callback class for saving dependents
class SaveEnumProc: public MaxHeapOperators {
	public:
		virtual	void proc(ReferenceMaker *rmaker)=0;
		virtual int terminate(ReferenceMaker *rmaker)=0; 
	};


//! \brief Implements a default version of a RemapDir.
/*! Instances of this class are used as the default argument to the 
	ReferenceTarget::Clone method. 
	When the boolean use_A_WORK2_flag is set to TRUE, 
	the remap directory will set this flag on all entries in the remap directory, 
	and will assume that any object that DOESN'T have this flag set is not in 
	the remap directory. This avoids the search through the directory and speeds
	up things greatly. When using this feature, you must first clear A_WORK2 on 
	all of the objects being cloned. DS: 3/17/00.
	When the instance is deleted, the Backpatch() method is automatically called 
	if backpatch operations were added but never performed.
	\pre First clear the A_WORK2 flag on all objects being cloned.
	\post Flag is set on all entries in remap directory.
	\param[in] use_A_WORK2_flag - used to set this flag on all entries in the 
	remap directory.
*/
class DefaultRemapDir: public RemapDir
{
private:
	const MaxSDK::Util::AutoPtr<RemapDir> mRemapDir;
	// Copy construction and assignment of objects of this class is disallowed
	DefaultRemapDir(const DefaultRemapDir& aDefaultRemapDir); // not implemented
	DefaultRemapDir& operator=(const DefaultRemapDir& rhs); // not implemented
public:
	CoreExport DefaultRemapDir(BOOL use_A_WORK2_flag = FALSE);
	CoreExport ~DefaultRemapDir();
	RefTargetHandle FindMapping(RefTargetHandle from);
	void AddEntry(RefTargetHandle hfrom, RefTargetHandle hto);
	void PatchPointer(RefTargetHandle* patchThis, RefTargetHandle oldTarg);
	void Backpatch();
	bool BackpatchPending();
	void Clear();
	void ClearBackpatch();
	void DeleteThis();
	void AddPostPatchProc(PostPatchProc* proc, bool toDelete);
};

//! \brief Gets a pointer to a default version of a RemapDir.
/*! You must delete it when done. When the boolean use_A_WORK2_flag is set to TRUE, 
	the remap directory will set this flag on all entries in the remap directory, 
	and will assume that any object that DOESN'T have this flag set is not in 
	the remap directory. This avoids the search through the directory and speeds
	up things greatly. When using this feature, you must first clear A_WORK2 on 
	all of the objects being cloned. DS: 3/17/00.
	When the instance is deleted, the Backpatch() method is automatically called 
	if backpatch operations were added but never performed.
	\pre First clear the A_WORK2 flag on all objects being cloned.
	\post Flag is set on all entries in remap directory.
	\param[in] use_A_WORK2_flag - used to set this flag on all entries in the 
	remap directory.
*/
CoreExport RemapDir* NewRemapDir(BOOL use_A_WORK2_flag=FALSE); 

//! \brief A ULONG Interface Id to determine if an Animatable is a Reference Maker or Target
/*! New in 3DS Max version 4.2. */
#define REFERENCE_MAKER_INTERFACE 0x2f96f73
//! \brief A ULONG Interface Id to determine if an Animatable is a Reference Maker or Target
/*! New in 3DS Max version 4.2. */
#define REFERENCE_TARGET_INTERFACE 0x66b073ea

class RefEnumProc;

// We might want to keep this class separate from Animatable just for clarity.

//! \brief An entity that listens and responds to external scene events and messages.
/*! Any entity that makes references must be derived from this class. A reference
	creates a record of the dependency between a ReferenceMaker and a 
	ReferenceTarget. 3ds Max uses a messaging system to notify dependent entities
	about changes. This class has a method that receives the notifications its 
	dependents send when they change. It has methods that return the number of 
	references if has, and methods to get and set these references. Also, there 
	are methods for creating, replacing and deleting the references. File 
	input and output is handled via methods of this class (Load() and Save()).

	See the section on \link <../../../prog/main/sdk_data_refs_root.html>
	References \endlink for an overview of the 3ds Max reference architecture.
*/
class ReferenceMaker : public Animatable {
	friend class DeleteRefRestore;
	friend class MakeRefRestore;
	friend class ReferenceTarget;
	friend class ParamBlock;
	friend class RootNode;
	friend class BaseNode;
	friend class ILoadImp;
	friend class RefMakerPLC;
	
	// temporary friend declarations until figure out what the code there is doing
	friend class LayerManagerRestore; 
	friend class RepMtlRest;  
	friend class MAXMaterialLibrary;  

	//! \name Internal Methods
	//! \brief Implemented by the system. These method are used internally.
	//@{	
	private:
		//! \brief Creates a reference
		/*!	This function has been deprecated for public use. Use ReplaceReference instead. 
		Implemented by the System. This method creates a reference between the 
		object which calls the method, and the ReferenceTarget specified by the 
		htarget parameter. Note that this method calls SetReference() after the 
		reference is made to initialize it.
		\deprecated Use ReplaceReference instead. 
		\param[in] refInterval - Currently, all plug-ins must use FOREVER for this 
		interval. Indicates the interval of time over which this reference is 
		active. Outside this interval, the reference is not considered to be a 
		dependency. This allows the plug-in to have dependent relationship over 
		only portions of an entire animation time range. If a plug-in has a 
		dependency over the entire animation it may use the pre-defined interval
		FOREVER for this parameter.
		\param[in] which - Indicates which virtual array index this newly created 
		reference is assigned to. The system uses a virtual array mechanism to 
		access the references an item has. The developer simply assigns an 
		integer index to each reference.
		\param[in] htarget - This parameter is the handle of the item we are making 
		a reference to.
		\return The return value from this method is of type RefResult. This is 
		usually REF_SUCCEED indicating the reference was created and is 
		registered by the reference target; otherwise REF_FAIL.
		*/
		CoreExport MAX_DEPRECATED RefResult MakeRefByID(
			Interval refInterval, 
			int which, 
			RefTargetHandle htarget);

		//! \brief Enumerator to search back in the dependency network.
		/*! This function has been deprecated. If you were calling this method, call
		DoEnumDependents instead. If you are implementing this method, implement 
		DoEnumDependentsImpl instead.
		\see ReferenceMaker::DoEnumDependentsImpl(DependentEnumProc* dep)
		\see ReferenceMaker::DoEnumDependents(DependentEnumProc* dep)
		*/
		MAX_DEPRECATED virtual int EnumDependents(DependentEnumProc* dep) { return 0; }

		void CheckForNonNullRefsOnInitialization(int ignoreRef = -1);		
		bool ValidateExistingReference(int which);		
	//@}

		//! \brief Prevent accidental copies (declared, not defined)
		/*! Performing a straight copy would not correctly establish reference hierarchy 
		relationships between this instance and the references held by the source object. 
		Therefore, copies using the C++ copy constructor are not permitted. 
		\see ReferenceMaker::operator=(const ReferenceMaker&)
		\see ReferenceTarget::Clone(RemapDir &remap)
		*/
		ReferenceMaker(const ReferenceMaker&);

		//! \brief Prevent accidental assignment (declared, not defined)
		/*! Performing an assignment would not correctly establish reference hierarchy 
		relationships between this instance and the references held by the source object. 
		Therefore, assigments are not permitted. 
		\see ReferenceMaker::ReferenceMaker(const ReferenceMaker&)
		\see ReferenceTarget::Clone(RemapDir &remap)
		*/
		ReferenceMaker& operator=(const ReferenceMaker&);

	public:
		//! \brief Constructor:
		ReferenceMaker() { ClearAFlag(A_EVALUATING); } 

		//! \brief Destructor:
		CoreExport ~ReferenceMaker();

		virtual void GetClassName(MSTR& s) { s = _M("ReferenceMaker"); }
		CoreExport virtual SClass_ID SuperClassID();

//! \name Making / Replacing / Deleting References
//@{
		//! \brief Used when cloning reference makers.
		/*! This routine is used when cloning reference makers, to delete old 
		reference and make a new one.
		In max r9, additional checks have have been added to ReplaceReference. If
		which is < 0, REF_FAIL will be immediately returned. If which is >= NumRefs,
		an attempt will be made to set the reference (via MakeRefByID), and then another
		check will be make to see if which is >= NumRefs, and if so REF_FAIL is returned.
		\param[in] which   - The virtual array index of the reference to replace.
		\param[in] newtarg - The new reference target
		\param[in] delOld  - If TRUE, the old reference is deleted.
		\return This is usually REF_SUCCEED indicating the reference was 
		replaced, otherwise REF_FAIL.
		*/ 
		CoreExport RefResult ReplaceReference(
			int which, 
			RefTargetHandle newtarg, 
			BOOL delOld = TRUE);

		//! \brief Deletes all references from this ReferenceMaker.
		/*! Implemented by the System. 
			\return This is always REF_SUCCEED indicating the references were deleted.
		*/
		CoreExport RefResult DeleteAllRefsFromMe();
			
		//! \brief Deletes all refs to this RefMaker/RefTarget.
		/*! Implemented by the System. This method deletes all the references to this 
		reference maker/reference target. This also sends the REFMSG_TARGET_DELETED 
		message to all dependents.
		\return This is REF_SUCCEED if the references were deleted; otherwise it
		is REF_FAIL.
		*/
		virtual RefResult DeleteAllRefsToMe() { return REF_SUCCEED; }
			
		//! \brief Deletes all references both to and from this item.
		/*! Implemented by the System. Deletes all references both to and from this 
		item.
		\return This is REF_SUCCEED if the references were deleted; otherwise it is 
		REF_FAIL.
		*/
		CoreExport RefResult DeleteAllRefs();

		//! \brief Deletes this item.
		/*! This deletes all reference to and from, sends REFMSG_TARGET_DELETED 
		messages, handles UNDO, and deletes the object.
		*/
		CoreExport void DeleteMe();

		//! \brief Deletes the specified reference
		/*! Implemented by the System. This method deletes the reference whose 
		virtual array index is passed. The other reference indices are not 
		affected, i.e. the number of references is not reduced nor are they 
		reordered in any way. Note the system calls SetReference(i, NULL) to set
		that reference to NULL. Also, if this is the last reference to the item,
		the item itself is deleted by calling its DeleteThis() method.
		\param[in] i - The virtual array index of the reference to delete.
		\return This is REF_SUCCEED if the reference was deleted; otherwise it 
		is REF_FAIL.
		*/
		CoreExport RefResult DeleteReference( int i);

		//! \brief Tells whether this reference can be transfered.
		/*! A ReferenceMaker can choose not to let TransferReferences() affect it. 
		Note that plugins probably should not use this. It is used by certain 
		system objects that have references.
		\param[in] i - Currently not used.
		\return Default to return TRUE
		*/
		//! \todo Find where this TransferReferences Method is and put a link to it.
		virtual BOOL CanTransferReference(int i) { return TRUE; }
//@} end group

//! \name Reference Access	
//! 3ds Max manages the access to an items references by using a virtual 
//! array. ALL classes that make references MUST implement these three 
//! methods to handle access to their references
//@{
		//! \brief Returns the number of references held.
		/*! Virtual method. The plugin implements this method to return the number 
			of references it makes. NumRefs() indicates valid values for 'i' when 
			doing a GetReference(i) or SetReference(i). An item may return different
			values at different times if the number of references is changing.
		\return The number of references made by the plugin. The default 
			implementation is return 0.
		*/
		CoreExport virtual	int NumRefs();

		//! \brief Returns the 'i-th' reference
		/*! Virtual method. The plugin implements this method to return its 
		'i th' reference. The plug-in simply keeps track of its references 
		using an integer index for each one. When the system calls this 
		method, the plug-in returns its 'i th' reference.
		\param[in] i - The virtual array index of the reference to get.
		\return The reference handle of the 'i-th' reference. The default
			implementation
		*/
		CoreExport virtual RefTargetHandle GetReference(int i);
	
		//! \brief Stores a reference to a ReferenceTarget
		/*! Virtual method. The plugin implements this method to store the reference
		handle passed into its 'i-th' reference. The plug-in simply keeps track 
		of its references using an integer index for each one. When the system 
		calls this method, the plug-in stores its 'i-th' reference.
		\param[in] i - The virtual array index of the reference to store.
		\param[in] rtarg - The reference handle to store.
		*/
		CoreExport virtual void SetReference(int i, RefTargetHandle rtarg);
//@} End Group

//! \name Loading and Saving Methods
//@{
		//! \brief Called for saving data.
		/*! Virtual method. Called by the system to allow the plugin to save 
		its data.
		\param[in] isave - This pointer may be used to call methods to write
		data to disk. See the section on
		\link <../../../prog/main/sdk_adv_loading_and_saving.html>Loading and 
		Saving \endlink for an overview of the load/save process.
		\return The default implementation is return IO_OK.
		\li IO_OK means the result was acceptable, with no errors.
		\li IO_ERROR This should be returned if an error occurred.
		*/
		CoreExport virtual IOResult Save(ISave *isave);
		
		//! \brief Called for loading data.
		/*! Virtual method. Called by the system to allow the plug-in to load 
		its data. See the section on
		\link <../../../prog/main/sdk_adv_loading_and_saving.html>Loading and
		Saving \endlink for an overview of the load - save process.
		\param[in] iload - This interface pointer may be used to call methods
		to read data from disk.
		\return The default implementation is return IO_OK.
		\li IO_OK means the result was acceptable, with no errors.
		\li IO_ERROR This should be returned if an error occurred.
		*/
		CoreExport virtual IOResult Load(ILoad *iload);

		//! \brief Used to load old files with references
		/*! Implement this if you have added or deleted references and are 
		loading an old file that needs to have its references remapped.		
		This method is used when you have modified a ReferenceMaker to add or
		delete references, and are loading old files. It gets called during 
		the reference mapping process, after the Load() method is called. You
		determine what version is loading in the Load(), and store the 
		version in a variable which you can look at in RemapRefOnLoad() to 
		determine how to remap references. The default implementation of this
		method just returns the same value it is passed, so you don't need to
		implement it unless you have added or deleted references from your 
		class. This method makes it a lot easier to load old files when the 
		reference topology has changed. 
		\param[in] iref - The input index of the reference.
		\return The output index of the reference.
		*/
		virtual int RemapRefOnLoad(int iref) { return iref; }

		//! \brief Rescale size of all world units in reference hierarchy.
		/*! This method is available in release 2.0 and later only. Must call 
		ClearAFlagInHierarchy(rm, A_WORK1) or ClearAFlagInAllAnimatables(A_WORK1) before 
		doing this on a reference hierarchy.
		This may be implemented to rescale the size of all world units in 
		a reference hierarchy. Developers must call 
		\code 
		if (TestAFlag(A_WORK1))
			return;
		SetAFlag(A_WORK1);
		\endcode
		before doing this on a reference hierarchy. See the section on 
		\link <../../../prog/what_was_new/idx_moving_from_max_1_to_2.html>
		'Scaling Parameter Values' in Updating MAX 1.0 Plugins to work with 
		MAX 2.0\endlink for additional details.
		\param[in] f - The scale factor.
		*/
		CoreExport virtual void RescaleWorldUnits(float f);
//@} end Group

//! \name Dependent Notification Methods
//@{ 
		//! \brief Put something here
		/*! Put something here
			\param[in] changeInt - Put something here
			\param[in] partID - Put something here
			\param[in] message - Put something here
			\param[in] sclass - Put something here
			\param[in] propagate - Put something here
			\param[in] hTarg - Put something here
			\return Put something here
		*/
		CoreExport virtual RefResult NotifyDependents(
			Interval changeInt, 
			PartID partID, 
			RefMessage message, 
			SClass_ID sclass = NOTIFY_ALL,
			BOOL propagate = TRUE, 
			RefTargetHandle hTarg = NULL);
//@} end group

//! \name Enumeration Methods
//@{ 
		//! \brief Enumerate auxiliary files (e.g. bitmaps)
		/*!  Enumerate auxiliary files (e.g. bitmaps). The default implementation
		just calls itself on all references and calls Animatable::EnumAuxFiles to
		pick up Custom Attributes Entities which actually need to load aux files
		must implement this, possibly calling ReferenceMaker::EnumAuxFiles also 
		to recurse. If you don't call ReferenceMaker::EnumAuxFiles call 
		Animatable::EnumAuxFiles. 
		\param[in] nameEnum - The Callback object that gets called on all
		Auxillary files.
		\param[in] flags - Flags.
		*/ 
		CoreExport virtual void EnumAuxFiles(NameEnumCallback& nameEnum, DWORD flags);

		//! \brief The default save enumeration.
		/*! This method is used internally. */
		CoreExport virtual void SaveEnum(SaveEnumProc& sep, BOOL isNodeCall = 0);

		//! \brief Begins an enumeration that searches back in the dependency network.
		/*! Implemented by the System. See the documentation for ReferenceTarget
		for more details. When called on instances that derive from ReferenceTarget,
		the call is passed along to the ReferenceTarget::DoEnumDependents method. Otherwise
		this method returns 0.
		\see ReferenceTarget::DoEnumDependents(DependentEnumProc* dep)
		*/
		//! \param [in] dep - The callback object called for each dependent.
		//! \return Return 1 to stop the enumeration and 0 to continue. The
		//! default implementation for ReferenceMaker is return 0;
		CoreExport int DoEnumDependents(DependentEnumProc* dep);

		//! \brief Method to perform an enumeration on a ReferenceTarget.
		/*! Implemented by the System. See the documentation for ReferenceTarget
		for more details.
		\param[in] dep - The callback object called for each dependent.
		\return Return 1 to stop the enumeration and 0 to continue.
		\see ReferenceTarget::DoEnumDependentsImpl(DependentEnumProc* dep)
		*/
		CoreExport virtual int DoEnumDependentsImpl(DependentEnumProc* dep);

		//! \brief This method provides a general purpose reference enumerator.
		/*! This method provides a general purpose reference enumerator that calls 
		RefEnumProc::proc() on each element in a reference hierarchy. This function 
		walks down the reference hierarchy, recursively calling RefEnumProc::proc() 
		on the references held by a reference, This function ensures that 
		RefEnumProc::proc() is called only once on each reference. Processing each 
		reference only once improves efficiency, and prevents potential infinite 
		recursive loops when processing indirect references. To enumerate the up 
		the reference hierarchy, see ReferenceTarget::DoEnumDependents.
		\param[in] proc - The callback object whose proc() method is called for each element.
		\param[in] includeCustAttribs - Added in 3ds Max 6 SDK. Defaults to true. 
		With includeCustAttribs set to true, RefEnumProc::proc will be called for the 
		custom attributes applied to the reference maker.
		\param[in] includeIndirectRefs - Added in 3ds Max 9 SDK. Defaults to true. 
		With includeIndirectRefs set to true, RefEnumProc::proc will be called for the 
		indirect references held by the reference maker.
		\param[in] includeNonPersistentRefs - Added in 3ds Max 9 SDK. Defaults to true. 
		With includeNonPersistentRefs set to false, RefEnumProc::proc will not be called 
		on direct references from the reference	maker unless IsRealDependency or 
		ShouldPersistWeakRef returns true for that reference; and the proc method will 
		not be called on indirect references from the reference maker unless 
		ShouldPersistIndirectRef returns true for that indirect reference.
		\param[in] preventDuplicatesViaFlag - Added in 3ds Max 9 SDK. Defaults to true. 
		With preventDuplicatesViaFlag set to true, processing of a duplicate reference 
		is detected by requesting and clearing a flag bit on all animatables, and then 
		testing/setting the flag bit prior to calling the proc on each reference maker. 
		When set to false, a list of reference makers visited is maintained. If you 
		expect to enumerate only a small number	of references, this argument should be 
		set to false. Note however that you do not know what references are held by the 
		references you may enumerate into, so in most cases this argument should be true.
		\return Returns false if the enumeration was terminated because the RefEnumProc's 
		proc returned REF_ENUM_HALT, true otherwise
		\see Class RefEnumProc
		\see ReferenceMaker::IsRealDependency(ReferenceTarget *rtarg)
		\see ReferenceMaker::ShouldPersistWeakRef(RefTargetHandle rtarg)
		\see IIndirectReferenceMaker::ShouldPersistIndirectRef(RefTargetHandle rtarg)
		\see ReferenceTarget::DoEnumDependents(DependentEnumProc* dep)
		*/
		CoreExport bool EnumRefHierarchy(
			RefEnumProc &proc, 
			bool includeCustAttribs = true, 
			bool includeIndirectRefs = true, 
			bool includeNonPersistentRefs = true, 
			bool preventDuplicatesViaFlag = true);
//@}

//! \name Finding and Checking Reference Targets
//@{

		//! \brief Get the index of the ReferenceTarget
		/*! Implemented by the System. This method returns the virtual array 
		index of the reference target passed.
		\param[in] rtarg - The reference target to find the index of.
		\return The virtual array index of the reference target to find. If the 
		reference target is not found, -1 is returned.
		*/
		CoreExport int FindRef(RefTargetHandle rtarg);

		//! \brief Tells whether it is a ReferenceTarget
		/*! This function differentiates things sub classed from ReferenceMaker 
		from subclasses of ReferenceTarget. The implementation of this method 
		(in ReferenceMaker) returns FALSE and its implementation in ReferenceTarget 
		returns TRUE. This can be useful when tracing back up the reference hierarchy, 
		to know when you run into something that was sub classed directly off of 
		ReferenceMaker, and hence to stop the traversal at that point.
		\return Default of FALSE.
		*/
		virtual BOOL IsRefTarget() { return FALSE; }

		//! \brief Tells whether it is a ReferenceMaker
		/*! This function differentiates things sub classed from Animatable 
		from subclasses of ReferenceMaker. The implementation of this method 
		(in Animatable) returns FALSE and its implementation in ReferenceMaker 
		returns TRUE.
		\return Returns TRUE.
		*/
		virtual BOOL IsRefMaker() { return TRUE; }

		//! \brief Returns whether this is a "real" (strong) dependency or not.
		/*! Used Internally. When a reference target's last "real" (strong) 
		reference is deleted the target is deleted. Any leftover "non-real" 
		(weak) reference makers will receive a REFMSG_TARGET_DELETED 
		notification. This method returns TRUE if the reference dependency 
		is "real" (strong). Otherwise it returns FALSE. Certain references 
		are not considered "real" (strong) dependencies. For instance, 
		internally there are certain reference makers such as the object that
		handles editing key information in the motion branch. This object 
		implements this method to return FALSE because it is not a "real" 
		strong) reference dependency. It's just needed while the editing is 
		taking place. Plugin developers dont' need to concern themselves 
		with this method because it is used internally.
		\param[in] rtarg - A pointer to the reference target.
		\return TRUE if the reference dependency is "real". Otherwise it returns
		FALSE. Default implementation is TRUE.
		*/
		virtual BOOL IsRealDependency(ReferenceTarget *rtarg) {return TRUE;}

		//! \brief Specifies whether a weak reference is to be persisted on a 
		//! partial load or save. 
		/*! This method specifies the partial load/save behavior of a weak 
		reference. This method will only be called if IsRealDependency returns 
		FALSE. If this method returns true, and this ref maker is loaded/saved, 
		the weak reference will be forced to be loaded/saved. If false, the 
		reference will not be forced to be loaded/saved, but will be hooked back
		up if it is loaded.

		The default implementation is to return FALSE. Otherwise, it is possible
		that on a partial load that this reference maker would hold the only 
		reference to the target. If something else temporarily referenced the 
		target, then the target will be deleted when that reference is dropped. 
		From the user's perspective, this is a randomly occurring event.

		Typical cases where an implementation of this method would return TRUE is
		when post load callbacks are used to check and process the references, 
		checking for things like owner-less parameter blocks.

		\param rtarg - The weak reference. 
		\returns Whether to force the load/save of the weak reference if this 
		reference maker is saved.
		*/
		virtual BOOL ShouldPersistWeakRef(RefTargetHandle rtarg) { return FALSE; }
//@} end group

//! \name Methods introduced in 3ds Max 4.2
//! Methods inherited from Animatable. If your plugin utilizes this new 
//! mechanism, be sure that your clients are aware that they must run 
//! your plugin with 3ds max version 4.2 or higher.

//@{
		//! \brief Returns a pointer to the interface
		/*! 
			\param[in] id - The ID of the interface
		*/
		virtual void* GetInterface(ULONG id) { 
			return (id == REFERENCE_MAKER_INTERFACE ? this : Animatable::GetInterface(id)); 
		}

		//! \brief Returns a pointer to the Base Interface.
		/*! Returns a pointer to the Base Interface for the interface ID passed.
			\param[in] id - The unique ID of the interface to get
		*/
		virtual BaseInterface* GetInterface(Interface_ID id) { return Animatable::GetInterface(id); }
//@}

	protected:
//! \name Internal Methods
//! \brief Implemented by the system. These method are used internally.
//@{	
		void BlockEval()	{   SetAFlag(A_EVALUATING); }
		void UnblockEval()	{ ClearAFlag(A_EVALUATING); }		
		int Evaluating()	{ return TestAFlag(A_EVALUATING); }		
		
		CoreExport RefResult StdNotifyRefChanged(
			Interval changeInt, 
			RefTargetHandle hTarget, 
			PartID partID, 
			RefMessage message, 
			BOOL propagate = TRUE);
//@}

	private:

//! \name Dependent Notification
//! Methods used to respond to notifications 
//@{
		//! \brief Receives and responds to messages
		/*! A plugin which makes references must implement a method to receive 
			and respond to messages broadcast by its dependents. This is done by
			implementing NotifyRefChanged().
			The plugin developer usually implements this method as a switch 
			statement where each case is one of the messages the plugin needs to
			respond to.
			The Method StdNotifyRefChanged calls this, which can change the 
			partID to new value. If it doesn't depend on the particular message&
			partID, it should return REF_DONTCARE.
		\li For developer that need to update a dialog box with data about 
			an object you reference note the following related to this method: 
			This method may be called many times. For instance, say you have a 
			dialog box that displays data about an object you reference. This 
			method will get called many time during the drag operations on that 
			object. If you updated the display every time you'd wind up with a 
			lot of 'flicker' in the dialog box. Rather than updating the dialog 
			box each time, you should just invalidate the window in response to 
			the NotifyRefChanged() call. Then, as the user drags the mouse your 
			window will still receive paint messages. If the scene is complex 
			the user may have to pause (but not let up on the mouse) to allow 
			the paint message to go through since they have a low priority. This
			is the way many windows in 3ds Max work.

		\param[in] changeInt - This is the interval of time over which the 
			message is active. Currently, all plug-ins will receive FOREVER for 
			this interval.
		\param[in] hTarget - This is the handle of the reference target the 
			message was sent by. The reference maker uses this handle to know 
			specifically which reference target sent the message.
		\param[in] partID - This contains information specific to the message 
			passed in. Some messages don't use the partID at all. See the 
			section List of Reference Messages for more information about the 
			meaning of the partID for some common messages.
		\param[in] message - The message parameters passed into this method is 
			the specific message which needs to be handled.
		\return The return value from this method is of type RefResult. This is 
			usually REF_SUCCEED indicating the message was processed. Sometimes,
			the return value may be REF_STOP. This return value is used to stop 
			the message from being propagated to the dependents of the item.
		*/
	    virtual RefResult NotifyRefChanged(
				Interval changeInt, 
				RefTargetHandle hTarget, 
				PartID& partID,  
				RefMessage message) = 0;
//@} end group

//! \name Internal Methods
//! \brief Implemented by the system. These method are used internally.
//@{	
		bool EnumRefHierarchyImpl(
			RefEnumProc &proc, 
			bool includeCustAttribs, 
			bool includeIndirectRefs, 
			bool includeNonPersistentRefs);
//@}
};
	
//! \brief The class that implements "look at targets" of references.
/*!	This class is used by anything that may need to have references made to it. 
	Most plugins are derived from this class. This class includes methods to 
	send notification of changes by the object, seeing which other objects in the
	system depend on it (reference it), and a method used to create a copy of the
	object ('clone' it). See the section on \link<> References \endlink for an 
	overview of the 3ds Max reference architecture.
*/
class ReferenceTarget : public ReferenceMaker {
	friend class DependentIterator;
	friend class DeleteRefRestore;
	friend class MakeRefRestore;
	friend class ReferenceMaker;

	public:
//! \name Constructor / Destructor
//@{
		//! \brief Constructor
		ReferenceTarget() { ClearAFlag(A_LOCK_TARGET); } 

		//! \brief Destructor:
		/* Added post r7. Existing 3rd party plugin objects that derive directly
		from ReferenceTarget will not call this implementation unless recompiled
		(vtable points at synthesized dtor). Current implementation does and must
		not do any "real work". Current implementation does DbgAssert if RefList
		is not empty. */
		CoreExport ~ReferenceTarget(); 
//@}

		CoreExport virtual void GetClassName(MSTR& s);  
		CoreExport virtual SClass_ID SuperClassID();

//! \name Testing for Class / Cyclic References/ Dependencies
//@{
		//! \brief Checks if this is a ReferenceTarget
		/*! This function differentiates things subclassed from ReferenceMaker 
		from subclasses of ReferenceTarget.
		\return Returns TRUE.
		*/
		virtual BOOL IsRefTarget() { return TRUE; }

		//! \brief Tests for a cyclical reference.
		/*! Implemented by the System.
		This method may be called to test for cyclical references. Normally 
		developers don't need to call this method since when a developer calls 
		MakeRefByID() to make a reference it performs its own test internally 
		and will not succeed if there is a cyclical reference being created.
		\param[in] refInterval - This interval is reserved for future use. 
		Currently any plugin should specify FOREVER for this interval.
		\param[in] hmaker - The reference maker performing the loop test.
		\return REF_SUCCEED if a cyclic reference would be created; otherwise REF_FAIL.
		*/
		CoreExport RefResult TestForLoop( Interval refInterval, RefMakerHandle hmaker);

		//! \brief Checks if a ReferenceTarget has references.
		/*! Implemented by the System. 
		\return Returns 1 if the reference target has items that reference it, 
		otherwise 0.
		*/
		CoreExport BOOL HasDependents();

		//! \brief Checks if this has Real (Strong) Dependents
		/*! This function goes through it's list of dependents or ReferenceMakers
		and queries the results of their IsRealDependency Functions.
		\return TRUE if any one of them are, FALSE otherwise.
		*/
		CoreExport BOOL HasRealDependents();  // not counting tree view

		//! \brief Gets the list of References.
		/*! Implemented by the System.
		\return Returns a list of references to this reference target.
		*/
		RefList& GetRefList() { return refs; }

		//! \brief Starts Dependency Test
		/*! To see if this reference target depends on something:
		\li first call BeginDependencyTest()
		\li then call NotifyDependents() on the thing with the REFMSG_TEST_DEPENDENCY
		If EndDependencyTest() returns TRUE this target is dependent on the thing.
		\return TRUE if this target is dependent on the thing.
		*/
		void BeginDependencyTest() { ClearAFlag(A_DEPENDENCY_TEST); }

		//! \brief Ends Dependency Test
		/*! To see if this reference target depends on something:
		\li first call BeginDependencyTest()
		\li then call NotifyDependents() on the thing with the 	REFMSG_TEST_DEPENDENCY
		If EndDependencyTest() returns TRUE this target is dependent on the thing.
		\return TRUE if this target is dependent on the thing.
		*/
		BOOL EndDependencyTest() { return TestAFlag(A_DEPENDENCY_TEST); }
//@}

//! \name Adding / Deleting / Transferring References
//@{
		//! \brief Called after a reference is made to a target.
		/*! This is called after a reference is made to this. If the target (this)
		needs to know that a reference to made to it, the target (this) can override this function.
		\param[in] rm - The ReferenceMaker creating the reference.
		*/
		virtual void RefAdded(RefMakerHandle rm) {}

		//! \brief Called after a reference is made to a target because of undo 
		//! or redo.
		/*! This method is available in release 2.0 and later only.
		Called when reference is added because of and undo or a redo. 
		Otherwise	it is similar to RefAdded.
		\param[in] rm - The ReferenceMaker creating the reference.
		*/
		virtual void RefAddedUndoRedo(RefMakerHandle rm) {}

		// This is called after deleting a reference to a ref target,
		// in the case that the target was not deleted. If target needs
		// to know, it should override this method.
		//! \brief Called after a references to this is deleted.
		/*! This is called after deleting a reference to a reference target, 
		in the case that the target was not deleted. When the last strong 
		reference to a ReferenceTarget is removed, the ReferenceTarget is deleted,
		the destructor is called, and the memory cleared.
		*/
		virtual void RefDeleted() {}

		//! \brief Called after a references to this is deleted because of undo 
		//! or redo.
		/*! This is called after deleting a reference to a reference target, 
		in the case that the target was not deleted. When the last strong 
		reference to a ReferenceTarget is removed, the ReferenceTarget is deleted,
		the destructor is called, and the memory cleared.
		*/
		virtual void RefDeletedUndoRedo() {}
		
		//! \brief Deletes all references to this ReferenceTarget.
		/*! implemented by the System. Deletes all references to this ReferenceTarget.
		\return always returns REF_SUCCEED.
		*/
		CoreExport RefResult DeleteAllRefsToMe();

		//! \brief This function is called when a targets last reference is deleted.
		/*! This function is called when a targets last reference is deleted. 
		Most subclasses will not need to override this. If you don't want this
		ReferenceTarget to be deleted when the last reference is deleted, then 
		override this function with a null operation.
		\return Always returns REF_SUCCEED.
		*/
		CoreExport virtual RefResult AutoDelete();

		//! \brief Transfers all the references from oldTarget to this 
		/*! Implemented by the System. This method is used to transfer all the 
		references from oldTarget to this reference target.
			\param[in] oldTarget - The previous reference target.
			\param[in] delOld - If this is TRUE the previous reference target is deleted.
			\return Always returns REF_SUCCEED.
		*/
		CoreExport RefResult TransferReferences(
			RefTargetHandle oldTarget, 
			BOOL delOld = FALSE);
//@}	

//! \name Dependent Enumeration
//@{		
		// 
		//! \brief Begins an enumeration that searches back in the dependency network.
		/*! Implemented by the System. This method is called to initiate an enumeration of all the 
		references to the ReferenceTarget. This method prepares the DependentEnumProc for enumeration, 
		and calls DoEnumDependentsImpl on the ReferenceTarget.
		To enumerate the down the reference hierarchy, see EnumRefHierarchy.
			\param[in] dep - The callback object called for each dependent.
			\return Return 1 to stop the enumeration and 0 to continue.
			\see EnumRefHierarchy
		*/
		CoreExport int DoEnumDependents(DependentEnumProc* dep);	

	protected:
		// 
		//! \brief Method to perform an enumeration on a ReferenceTarget.
		/*! Implemented by the System. This method is initially called by DoEnumDependents. 
		This method allows a ReferenceTarget to call the given callback object's proc 
		on itself, and then optionally enumerate all references to it. How to continue 
		enumeration is based on the return value from the callback object's proc. 
		All ReferenceTargets have a list of back pointers to entities that directly 
		reference it. The default implementation of this method first checks to see 
		if this ReferenceTarget was previously processed as part of the enumeration, 
		and if so immediately returns 0. If the ReferenceTarget was not previously 
		processed, it calls the callback object's proc on itself and then, dependent 
		on the return value from the callback object's proc, enumerates those back 
		pointers calling DoEnumDependentsImpl once per dependent.
		Normally you will not need to override this method. If you do override this 
		method, you should use DependentEnumProc::CheckIfAndSetAsVisited to process 
		your instance and its dependents only if it was not previously processed. 
		\param[in] dep - The callback object called for each dependent.
		\return Return 1 to stop the enumeration and 0 to continue.
		\see DependentEnumProc::CheckIfAndSetAsVisited(ReferenceMaker *rmaker)
		\code
			int RefTargMonitorRefMaker::DoEnumDependentsImpl(DependentEnumProc* dep) {
				// if already processed this reftarg once, no need to process it or its dependents again
				if (dep->CheckIfAndSetAsVisited(this)) 
					return 0;
				int res = dep->proc(this);
				if (res == DEP_ENUM_SKIP)
					return 0;
				else if (res)
					return 1;
				return mOwner.ProcessEnumDependents(dep);
			}
		\endcode
		*/
		CoreExport virtual int DoEnumDependentsImpl(DependentEnumProc* dep);

	public:

//@}

//@{ \name Cloning

		//! \brief Call this before calling ReferenceTarget::BaseClone, when 
		//! cloning an object.
		/*! Virtual method. This method is called to have the plugin clone 
		itself. This method should copy both the data structure and all the data
		residing in the data structure of this reference target. The plugin 
		should clone all its references as well.
		\note See the remarks in method BaseClone() below.
		\param[out] remap - This class is used for remapping references during a
		Clone.
		\return A pointer to the cloned item.
		*/
		CoreExport virtual RefTargetHandle Clone(RemapDir &remap = DefaultRemapDir());

		//! \brief Call this after calling ReferenceTarget::Clone.
		/*! This method is available in release 4.0 and later only. Virtual method.
		\note All plugins that implement a Clone() method have to call this 
		BaseClone() method with the old and the new object as parameters. The 
		ordering in regards to when this method is called is unimportant, however
		this method must, of course, be called after the cloned object is created.
		This method allows base classes to copy their data into a new object 
		created by the clone operation. All overrides of BaseClone() must call 
		the base class implementation. The base class implementation copies the 
		CustAttrib objects into the newly created object.
		\param[in] from - Points to the old object to clone.
		\param[in] to - Points to the new object created.
		\param[in] remap - This class is used for remapping references during a 
		Clone.
		*/
		CoreExport virtual void BaseClone(
			ReferenceTarget *from, 
			ReferenceTarget *to,
			RemapDir &remap);
		
		//! \brief Deletes this if no references exist.
		/*! This method is used internally. Checks if there are no references, 
		and if this object is deletable, deletes it.
		\return REF_SUCCEED if successful, or REF_FAIL if this failed.
		*/
		CoreExport RefResult MaybeAutoDelete();
//@}

//! \name Dependent Notification
//@{
		//! \brief Notify all dependent RefMakers concerned with the message 
		/*! Implemented by the System.
		This method broadcasts the message specified by the message parameter
		to all the items which reference this item.
		Note the following on how reference messages propagate (that is, travel 
		to the dependents):
		- When a plugin sends a message via NotifyDependents(), the message
			-# Propagates to ALL the items that reference it. 
			-# And also to all the items which reference those items. 
			-# And so on. 
		- The only exceptions to this are as follows:
		-# The propagate parameter passed is FALSE. In that case the message 
			only goes to the immediate dependents.
		-# If the SClass_ID sclass = NOTIFY_ALL parameter limits the dependents
			to a certain specified Super Class.
		-# If one of the items that references the plugin, processes the message
			inside its NotifyRefChanged() and returns REF_STOP instead of 
			REF_SUCCEED. In this case, the message is not further propagated.

		Also, whenever a message propogates, the hTarget parameter received in 
		NotifyRefChanged() is reset to the this pointer of the immediate dependent
		(not the originator) who propogates the message.

		\param[in] changeInt - Currently all plug-ins must pass FOREVER for this
			interval. This indicates the interval of time over which the change 
			reported by the message is in effect.
		\param[in] partID - This parameter is used to pass message specific 
			information to the items which will receive the message. See the 
			ReferenceMaker::NotifiyRefChanged() method for more details.
		\param[in] message - The message to broadcast to all dependents. See
			the ReferenceMaker::NotifiyRefChanged() method for more details. 
			See \link<> List of Reference Messages.\endlink
		\param[in] sclass - This parameter defaults to NOTIFY_ALL. If this 
			value is passed to NotifyDependents() all dependents will be notified.
			Other super class values may be passed to only send the message to 
			certain items whose SuperClassID matches the one passed.
		\param[in] propagate - This parameter defaults to TRUE. This indicates 
			that the message should be sent to all 'nested' dependencies. If 
			passed as FALSE, this parameter indicates the message should only be
			sent to first level dependents. Normally this should be left to 
			default to TRUE.
		\param[in] hTarg - This parameter must always default to NULL.
		\return This method always returns REF_SUCCEED.
		*/
		CoreExport virtual RefResult NotifyDependents(
			Interval changeInt, 
			PartID partID, 
			RefMessage message, 
			SClass_ID sclass = NOTIFY_ALL,
			BOOL propagate = TRUE, 
			RefTargetHandle hTarg = NULL);
 
		//! \brief This sends the REFMSG_FLAGDEPENDENTS message up the pipeline.
		/*! This sends the REFMSG_FLAGDEPENDENTS message up the pipeline. There 
		are two reasons to flag dependents:
		-# To put the node in the FG plane. (PART_PUT_IN_FG)
		-# To set the node's mesh color to green to indicate it is a dependent. 
		(PART_SHOW_DEPENDENCIES). 
		If the PART_SHOWDEP_ON bit is set, the dependency display is turned on, 
		otherwise it is turned off.
		\param[in] t - Currently all plug-ins must pass FOREVER for this interval.
		\param[in] which - The part id that is sent with REFMSG_FLAGDEPENDENTS 
		*/
		void FlagDependents( TimeValue t, PartID which=PART_PUT_IN_FG ) { 
			NotifyDependents( Interval(t,t), which,	REFMSG_FLAGDEPENDENTS );
		}

		//! \brief This method is called to flag dependents into the FG.
		/*! This method is called to flag dependents into the FG. 
		(Note that the above method is obsolete)
		The default implementation just sends out the notification 
		REFMSG_FLAGDEPENDENTS with PART_PUT_IN_FG as the partID. In particular, 
		a slave controller could override this method and call its master's 
		version of this method
		*/
		virtual void NotifyForeground(TimeValue t) {
			NotifyDependents(Interval(t,t),PART_PUT_IN_FG,REFMSG_FLAGDEPENDENTS);
		}

		//! \brief Sends messages to ReferenceTargets
		/*! This method is available in release 4.0 and later only.
		Used by a ReferenceMaker to send 'reverse' notification messages to its 
		ReferenceTargets, or to this ReferenceTarget.
		\param[in] message - The message sent to the reference target. See the 
			\link<>NotifyTarget message\endlink codes above.
		\param[in] hMaker - The ReferenceMaker sending the message.
		*/  
		virtual void NotifyTarget(int message, ReferenceMaker* hMaker) { }
//@}

//! \name Methods introduced in 3DS Max version 4.2
//@{	
		//! \brief Inherited from Animatable
		/*! Returns a pointer to the interface.
			\param[in] id - The id of the interface.
			\return A Pointer to the Interface
		*/
		virtual void* GetInterface(ULONG id) { 
			return (id == REFERENCE_TARGET_INTERFACE ? this : ReferenceMaker::GetInterface(id)); 
		}

		//! \brief Inherited from Animatable
		/*! Returns a pointer to the Base Interface for the interface ID passed.
		\param[in] id - The unique ID of the interface to get
		\return A Pointer to the Interface
		*/
		virtual BaseInterface* GetInterface(Interface_ID id) { 
			return ReferenceMaker::GetInterface(id); 
		}
//@}

	private:		
		//! \brief Used to create references.
		/*! 
		Method used internally for creating References. MakeRefByID calls this method. 
		\param[in] refInterval - Currently must always pass FOREVER for this interval.
		\param[in] hmaker - The handle to the ReferenceMaker.
		\param[in] whichRef - which reference to set. If -1, SetReference is not called on hmaker
		\return Returns REF_SUCCEED if successful.
		*/ 
		CoreExport RefResult MakeReference(
			Interval refInterval,
			RefMakerHandle hmaker,
			int whichRef = -1);

		//! \brief This is the list of active references that refer to us.
		RefList	refs;
};



//! \brief This class is used to iterate through all the dependents of a 
//! ReferenceTarget.
/*! It maintains an internal pointer so a developer can keep calling the Next() 
	method to get the next dependent. For instance, you can count the number of 
	reference makers that depend on a target by doing the following:
	\code
	int CountRefs(ReferenceTarget *rt) 
	{
		DependentIterator di(rt);
		int nrefs = 0;
		RefMakerHandle rm;
		while (NULL!=(rm=di.Next()))
			nrefs++;
		return nrefs;   
	}
	\endcode
	All methods of this class are implemented by the system.
*/
class DependentIterator: public MaxHeapOperators {
	ReferenceTarget *rt;
	RefListItem *next;
	public:

	//! \brief Constructor
	/*! \param[in] rtarg - Points to the RefereceTarget whose dependents are
		iterated through. */
	DependentIterator(ReferenceTarget *rtarg) { rt = rtarg; next = rt->refs.first; }
	//! \brief Returns a pointer to the next ReferenceMaker or NULL when there are no more.
	CoreExport ReferenceMaker *Next();
	//! \brief Resets the iterator object so it starts at the beginning again 
	//! with the original ReferenceTarget passed.
	void Reset() { next = rt->refs.first; }
	};

class DeletedRestore: public RestoreObj {
	RefMakerHandle anim, svanim;
	public:
		CoreExport DeletedRestore();
		CoreExport DeletedRestore(RefMakerHandle an);
		CoreExport ~DeletedRestore();
		CoreExport void Restore(int isUndo);
		CoreExport void Redo();
		CoreExport MSTR Description();
	};

//! \name Defines enumeration return values
//! \brief Possible return values for RefEnumProc::proc()
//@{
//! Continues enumeration
#define REF_ENUM_CONTINUE	0
//! Halts enumeration
#define REF_ENUM_HALT		1
//! Skip enumeration
#define REF_ENUM_SKIP		2	
//@}

//! \brief A callback class for ReferenceMaker::EnumRefHierarchy.
/*! This is the callback object for ReferenceMaker::EnumRefHierarchy. This class's 
	proc() method is called for each element in the reference hierarchy.
	When enumerating indirect references, it is possible to enter an infinite recursive 
	loop unless references are not processed more than once. The system prevents 
	processing duplicates by either maintaining a list of all references processed, 
	or by clearing a flag bit on all references and then testing/setting the flag 
	bit as references are processed. In the ReferenceMaker::EnumRefHierarchy method, 
	this object's BeginEnumeration method is called	before starting an enumeration, 
	and EndEnumeration at the end. These methods increment and decrement an internal
	counter. When the counter is decremented to 0, the list of visited references is
	cleared. This ensures that the list of visited references is maintained during 
	recursive calls to EnumRefHierarchy, but cleared when the outter most call is 
	exited. In some cases however, it is desirable to maintain the list of visited 
	references across	calls to EnumRefHierarchy. In these cases, call this object's 
	BeginEnumeration method prior to the calls to EnumRefHierarchy, and 
	EndEnumeration when done. For example:
\code
	FreeMapsRefEnum freeEnum(this);
	for ( rmaker = <enum over some refmaker set> )
		-- list of visited references cleared after each call
		EnumRefHierarchy(rmaker, freeEnum); 

	versus

	FreeMapsRefEnum freeEnum(this);
	freeEnum.BeginEnumeration()
	for ( rmaker = <enum over some refmaker set> )
		-- list of visited references maintained across each call
		EnumRefHierarchy(rmaker, freeEnum); 
	freeEnum.EndEnumeration()
\endcode
	See the section on \link<>References\endlink
*/
class RefEnumProc: public MaxHeapOperators {

	friend class ReferenceTarget;
	class RefEnumProcImplData;
	const MaxSDK::Util::AutoPtr<RefEnumProcImplData> mRefEnumProcImplData;

protected:
	CoreExport RefEnumProc();
	CoreExport ~RefEnumProc();

	// Hide it - no copy constructor or assignment to the public.
	RefEnumProc(const RefEnumProc&); // not implemented
	RefEnumProc& operator=(const RefEnumProc& rhs); // not implemented

	public:
	//! \brief This method is called once for each element in the reference hierarchy.
	/*! This method is called once for each element in the reference hierarchy. 
	The return value specifies whether continue processing the references, indirect 
	references, and custom attributes of the refmaker; whether to skip processing of 
	those and continue to the next element, or whether to terminate enumeration. 
	Typically the return value will be REF_ENUM_CONTINUE.
	\param[in] rm - A pointer to the reference maker to this item. 
	\return One of the following values:
	\li <b>REF_ENUM_CONTINUE</b>: This continues the enumeration
	\li <b>REF_ENUM_HALT</b>: This stops the enumeration.
	\li <b>REF_ENUM_SKIP</b>: Reference Makers can hold multiple References. 
	In certain instances you may not want to travel down all of the "branches". 
	By returning REF_ENUM_SKIP from this method you tell the enumerator to not 
	enumerate the current Reference Maker's references but not to halt the eumeration 
	completely.
	*/
	virtual int proc(ReferenceMaker *rm)=0;

	//! \brief This method sets and checks whether a ReferenceMaker was visited
	/*! Implemented by system. This method is used to check whether a ReferenceMaker 
	was previously visited by this callback object, and registers it as having been 
	visited if not. This method is used to ensure we call the proc only once on each 
	ReferenceMaker. 
	\param[in] rmaker - A pointer to the reference maker
	\return Returns true of the rmaker was previously visited, false if not.
	*/
	CoreExport virtual bool CheckIfAndSetAsVisited(ReferenceMaker *rmaker);

	//! \brief This method prepares the RefEnumProc instance for enumeration
	/*! Implemented by system. This method is used to initialize the RefEnumProc 
	instance for tracking the reference makers that have been visited. Normally, 
	you do not need to call this method as ReferenceMaker::EnumRefHierarchy calls 
	this method prior to enumerating the specified reference maker. In some cases 
	though, you may want to have RefEnumProc instance track the reference makers 
	visited over multiple calls to EnumRefHierarchy. For example, you may want to 
	call RefEnumProc::proc() on each node in a selection set, tracking the reference 
	makers that have been visited across the entire selection set. Note that each 
	call this method must be paired with a call to EndEnumeration.
	\param[in] preventDuplicatesViaFlag - Defaults to true. With preventDuplicatesViaFlag 
	set to true, processing of a duplicate reference is detected by requesting and 
	clearing a flag bit on all animatables, and then testing/setting the flag bit 
	prior to calling the proc on each reference maker. When set to false, a list of 
	reference makers visited is maintained. If you expect to enumerate only a small 
	number of references, this argument should be set to false. Note however that 
	you do not know what references are held by the references you may enumerate into, 
	so in most cases this argument should be true. Note that the setting specified 
	here overrides the preventDuplicatesViaFlag argument setting in the EnumRefHierarchy call.
	\see ReferenceMaker::EnumRefHierarchy
	*/
	CoreExport void BeginEnumeration(bool preventDuplicatesViaFlag = true);

	//! \brief This method finalizes the RefEnumProc instance after enumeration
	/*! Implemented by system. This method is used to finalize the RefEnumProc 
	instance after enumeration, freeing any memory or max system resources used to 
	track the reference makers that have been visited. This method should only, 
	and must be, called if you called the BeginEnumeration method.
	*/
	CoreExport void EndEnumeration();
};

//! \brief This function has been deprecated. Use ReferenceMaker::EnumRefHierarchy instead. 
MAX_DEPRECATED void EnumRefHierarchy(
	ReferenceMaker *rm, 
	RefEnumProc &proc, 
	bool includeCustAttribs = true);


CoreExport ReferenceTarget *CloneRefHierarchy(ReferenceTarget *rm);


//! \brief This class lets you tap in to ALL reference messages in the entire
//! system. 
/*! Once registered, the NotifyRefChanged() method will be called once for every
	time NotifyRefChanged() is called on a regular ReferenceTarget effectively 
	allowing you to wire tap the entire reference network.
	New in 3DS Max version 2.0
	<b>WARNING</b> This should be used with extreme care. NotifyRefChange() will
	be called MANY MANY times so it is important to do very little processing 
	within this method. This most that should probably be done is to set a dirty bit.
*/
class GlobalReferenceMaker : public MaxHeapOperators {
	public:

		class GlobalReferenceDisableGuard;

		//! \brief Callback function to receive messages
		/*! This method is implemented to receive and respond to messages 
			broadcast by all the Dependants in the entire system.
			\param[in] iv - This is the interval of time over which the message 
			is active. Currently, all plugins will receive FOREVER for this 
			interval.
			\param[in] hTarg - This is the handle of the reference target the 
			message was sent by. The reference maker uses this handle to know 
			specifically which reference target sent the message.
			\param[in] partID - This contains information specific to the message
			passed in. Some messages don't use the partID at all. See the section
			\link<>List of Reference Messages\endlink for more information about
			the meaning of the partID for some common messages.
			\param[in] msg - The msg parameters passed into this method is the 
			specific message which needs to be handled. See \link<>List of 
			Reference Messages.\endlink
			\return The return value from this method is of type RefResult. This
			is usually REF_SUCCEED indicating the message was processed. Sometimes,
			the return value may be REF_STOP. This return value is used to stop 
			the message from being propagated to the dependents of the item.
		*/
		virtual RefResult NotifyRefChanged(
			Interval iv, 
			RefTargetHandle hTarg,
			PartID& partID, 
			RefMessage msg) = 0;		
	};

//! \brief This class is used to temporarily disable global reference makers.
/*! Implements the guard pattern to disable global reference makers. While disabled,
	none of the reference messages are sent to the global reference makers
	that were registered through RegisterGlobalReference().
	<br>
	The class correctly handles nested disables by using a counter which is
	incremented in the constructor and decremented in the destructor.
	<br>
	An example of a case where it is useful to disable global reference makers
	is an operation which creates a lot of temporary references, such as
	temporary materials. The global reference makers may not need to be
	aware of the existence of these temporary references, so they may be disabled
	for performance reasons. */
class GlobalReferenceMaker::GlobalReferenceDisableGuard : public MaxHeapOperators {
public:

	//! Determines whether global reference makers are currently disabled.
	//! \return true if global reference makers are disabled; false otherwise.
	CoreExport static bool IsDisabled();

	//! Constructor; disables global reference makers.
	/*! A boolean parameter may be used to conditionally have this class
		do nothing.
		\param[in] doDisable - When true, this class will disable the global
		reference makers. When false, this class does nothing. */
	CoreExport GlobalReferenceDisableGuard(bool doDisable = true);

	//! Destructor; re-enables global reference makers.
	/*! Global reference makers are only re-enabled if they were disabled
		in the constructor. */
	CoreExport ~GlobalReferenceDisableGuard();

private:

	//! Counter used to keep track of the number of disables that have occurred.
	static int m_disableCount;

	//! Set to true if and only if global refernce makers were disabled in the
	//! constructor.
	bool m_doDisable;
};

//! \brief Registers an instance of this class whose NotifyRefChanged() method will 
//! receive the messages. 
/*! New in 3DS Max version 2.0 
	\param[in] maker - Points to an instance of this class whose NotifyRefChanged() 
	method will receive the messages
*/
CoreExport void RegisterGlobalReference(GlobalReferenceMaker *maker);

//! \brief Unregisters the class so it won't be called anymore
/*! New in 3DS Max version 2.0 
	\param[in] maker - Points to an instance of this class whose NotifyRefChanged() 
	method will no longer receive the messages
*/
CoreExport void UnRegisterGlobalReference(GlobalReferenceMaker *maker);

//! \brief Clears the specified Animatable Flag bits in a reference hierarchy
/*! This method calls ClearAFlag on the ReferenceMaker and all references from it, recursively,
with the specified mask
\param[in] rm -  The handle to the root ReferenceMaker to enumerate from
\param[in] mask The bits to turn off in the Animatable flags
*/
CoreExport void ClearAFlagInHierarchy(RefMakerHandle rm, DWORD mask);

//! \brief Clears the specified Animatable Flag bits on all Animatables
/*! This method calls ClearAFlag on all Animatables with the specified mask
\param[in] mask The bits to turn off in the Animatable flags
*/
CoreExport void ClearAFlagInAllAnimatables(DWORD mask);

//! \brief Checks if there is a path of references from a ReferenceMaker to a 
//! ReferenceTarget.
/*!	\param[in] mkr - The handle to the ReferenceMaker
	\param[in] targ - The handle to the ReferenceTarget
	\return TRUE if there is a path of references from the to targ. This returns
	TRUE if mkr==targ)
*/
CoreExport BOOL DependsOn(RefMakerHandle mkr, RefMakerHandle targ);

//! \brief Function to find out if we are saving an old version of a .MAX file.  
/*! \return 
	\li If this returns 0, then either we are not in a save or we are saving the
	current version. 
	\li If it returns non-zero, it is the max release number being saved, 
	multiplied by 1000. 
	
	Thus, when saving MAX R2 files, it will return 2000, when saving MAX R8 files,
	it will return 8000.  This function can be used in NumRefs() and GetRef() to
	make an objects references appear as they did in the old Max version.
*/
CoreExport DWORD GetSavingVersion(); 

//! \brief Function used internally to maintain the SavingVersion number, which 
//! should not be called by plugins.
CoreExport DWORD SetSavingVersion(DWORD version); 

//! \brief Disable reference messages globally.
/*! <b>Beware</b> Use with extreme caution.  Used for speeding up File/Reset, but not
	recommended anywhere else. (DS 3/16/00) */
CoreExport void DisableRefMsgs();
//! \brief Enable reference messages globally.
/*! <b>Beware:</b> Use with extreme caution.  Used for speeding up File/Reset, but not
recommended anywhere else. (DS 3/16/00) */
CoreExport void EnableRefMsgs();

//! \brief Reference Maker to single entity
/*! New in 3DS Max version 6.0 */ 
class SingleRefMaker: public ReferenceMaker {
public:
	//! Handle to the single reference
	RefTargetHandle rtarget;

	//! \brief Constructor
	CoreExport SingleRefMaker();
	//! \brief Destructor
	/*! Suspended from Undo system */
	CoreExport ~SingleRefMaker();

	//! \brief Set a Reference
	/*! Suspended from Undo system */
	CoreExport	void SetRef(RefTargetHandle rt);
	CoreExport RefTargetHandle GetRef();

	// ReferenceMaker 
	/*! \brief By default handles REFMSG_TARGET_DELETED message only */
	CoreExport RefResult NotifyRefChanged(
		Interval changeInt,
		RefTargetHandle hTarget, 
		PartID& partID, 
		RefMessage message );
	CoreExport void DeleteThis();
	CoreExport	SClass_ID  SuperClassID();

	// Overrides From ReferenceMaker
	CoreExport	int NumRefs();
	CoreExport	RefTargetHandle GetReference(int i);
	CoreExport	void SetReference(int i, RefTargetHandle rtarg);
	CoreExport	BOOL CanTransferReference(int i);
};

//! \brief A class to help control the Animate system state in an exception safe manner
/*! Create an instance of this class, and when it is destructed any Animate suspends are resumed.
Makes it safe in case a throw occurs after the suspend, but before the resume.
*/
class CoreExport AnimateSuspend: public MaxHeapOperators {
private:
	BOOL animateSuspended; 
	BOOL setKeyModeSuspended;
	BOOL macroRecSuspended;
	BOOL refMsgsSuspended;
public:
	//! Constructor
	/*!	Create an instance of this class, setting the specified MAX states.
	\param[in] macroRecToo - disable/enable macro recorder system
	\param[in] setKeyModeToo - disable/enable set key system
	\param[in] refMsgsToo - disable/enable passing of reference messages
	*/
	AnimateSuspend(
		BOOL setKeyModeToo = FALSE, 
		BOOL macroRecToo = FALSE, 
		BOOL refMsgsToo = FALSE );
	//! \brief Restore Animate system state to initial state.
	void Resume();
	//! \brief Destructor - restores Animate system state to initial state.
	~AnimateSuspend() {Resume ();}
};					  

//! \brief A class to help control the various MAX states in an exception safe manner
/*!	Create an instance of this class, setting the specified MAX states. When the instance is destructed, 
any specified states are restored. Makes it safe in case a throw occurs from within the scope of this object.
*/
class CoreExport SuspendAll: public MaxHeapOperators {
private:
	BOOL holdSuspended;
	BOOL macroRecSuspended;
	BOOL animateSuspended;
	BOOL setKeyModeSuspended;
	BOOL autoBackupSuspended;
	BOOL saveRequiredState;
	BOOL refMsgsSuspended;
	BOOL m_previousSaveRequiredState;
	BOOL m_previousAutoBackupState;
public:
	//! Constructor
	/*!	Create an instance of this class, setting the specified MAX states.
	\param[in] holdState - disable/enable hold system
	\param[in] macroRec - disable/enable macro recorder system
	\param[in] animateMode - disable/enable animate mode
	\param[in] setKeyMode - disable/enable set key system
	\param[in] autoBackup - disable/enable auto backup
	\param[in] saveRequired - save/restore save required flag
	\param[in] refMsgs - disable/enable passing of reference messages
	*/
	SuspendAll(
		BOOL holdState = FALSE, 
		BOOL macroRecorder = FALSE, 
		BOOL animateMode = FALSE, 
		BOOL setKeyMode = FALSE, 
		BOOL autoBackup = FALSE, 
		BOOL saveRequired = FALSE, 
		BOOL refMsgs = FALSE );
	//! \brief Restore MAX state to initial state.
	void Resume();
	//! \brief Destructor - restores MAX state to initial state.
	~SuspendAll() {Resume ();}
};               
#endif
