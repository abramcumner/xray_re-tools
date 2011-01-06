/**********************************************************************
 *<
	FILE: notify.h

	DESCRIPTION: Include file for event notification support

	CREATED BY: Tom Hudson

	HISTORY: Created 8 April 1995

 *>	Copyright (c) 1995, All Rights Reserved.
 **********************************************************************/

#ifndef _NOTIFY_H_

#define _NOTIFY_H_
#include "maxheap.h"

class View;
class FrameRendParams;
class INode;
class ViewParams;

// Pre-defined Max system notification codes

#define NOTIFY_UNITS_CHANGE				0x00000001
#define NOTIFY_TIMEUNITS_CHANGE	 		0x00000002
#define NOTIFY_VIEWPORT_CHANGE			0x00000003
#define NOTIFY_SPACEMODE_CHANGE	 		0x00000004
#define NOTIFY_SYSTEM_PRE_RESET	 		0x00000005	// Sent before system is reset
#define NOTIFY_SYSTEM_POST_RESET 		0x00000006	// Sent after system is reset
#define NOTIFY_SYSTEM_PRE_NEW			0x00000007	// Sent before system is NEW'd-out
#define NOTIFY_SYSTEM_POST_NEW			0x00000008	// Sent after system is NEW'd-out
#define NOTIFY_FILE_PRE_OPEN			0x00000009	// Sent before a new file is opened
#define NOTIFY_FILE_POST_OPEN			0x0000000A	// Sent after a new file is opened successfully
#define NOTIFY_FILE_PRE_MERGE			0x0000000B	// Sent before a file is merged
#define NOTIFY_FILE_POST_MERGE			0x0000000C	// Sent after a file is merged successfully
#define NOTIFY_FILE_PRE_SAVE			0x0000000D	// Sent before a file is saved ( callParam is TCHAR * to file name )
#define NOTIFY_FILE_POST_SAVE			0x0000000E	// Sent after a file is saved ( callParam is TCHAR * to file name )
#define NOTIFY_FILE_OPEN_FAILED	        0x0000000F	// Sent after a file is failed to opened
#define NOTIFY_FILE_PRE_SAVE_OLD 		0x00000010	// Sent before an old version file is saved
#define NOTIFY_FILE_POST_SAVE_OLD		0x00000011	// Sent after an old version file is saved
#define NOTIFY_SELECTIONSET_CHANGED		0x00000012	// Sent after the selection set has changed
#define NOTIFY_BITMAP_CHANGED			0x00000013	// Sent after the bitmap is reloaded ( callParam is TCHAR * to bitmap file name )
#define NOTIFY_PRE_RENDER				0x00000014	// Sent before rendering is started
#define NOTIFY_POST_RENDER				0x00000015	// Sent after rendering has finished
// Note: The NOTIFY_PRE_RENDERFRAME and NOTIFY_POST_RENDERFRAME callbacks are for internal use.
// The scene is to be considered read-only at those callbacks.
#define NOTIFY_PRE_RENDERFRAME			0x00000016	// Sent before rendering each frame ( callParam is RenderGlobalContext* )
#define NOTIFY_POST_RENDERFRAME			0x00000017	// Sent after rendering each frame  ( callParam is RenderGlobalContext* )
#define NOTIFY_PRE_IMPORT				0x00000018	// Sent before a file is imported
#define NOTIFY_POST_IMPORT				0x00000019	// Sent after a file is imported
#define NOTIFY_IMPORT_FAILED			0x0000001A	// Sent if import fails
#define NOTIFY_PRE_EXPORT				0x0000001B	// Sent before a file is exported
#define NOTIFY_POST_EXPORT				0x0000001C	// Sent after a file is exported
#define NOTIFY_EXPORT_FAILED			0x0000001D	// Sent if export fails
#define NOTIFY_NODE_RENAMED				0x0000001E	// Sent if node renamed. (call Param is pointer to struct{ TCHAR* oldname; TCHAR* newname; } }
#define NOTIFY_PRE_PROGRESS				0x0000001F	// Sent before the progress bar is displayed (so you can hide your window)
#define NOTIFY_POST_PROGRESS			0x00000020	// Sent after the progress bar is finished (so you can show your window again)
#define NOTIFY_MODPANEL_SEL_CHANGED		0x00000021	// Sent when the modify panel focuses on a new object (via open Mod panel or changing selection)
#define NOTIFY_HEIGHT_CHANGED			0x00000022	// VIZ Sent when the user operates the height menu
#define NOTIFY_RENDPARAM_CHANGED		0x00000023	// VIZ Sent when the common renderer parameters have changed
#define NOTIFY_MATLIB_PRE_OPEN			0x00000024	// Sent before loading a material library
#define NOTIFY_MATLIB_POST_OPEN			0x00000025	// Sent after loading a material library (call Param is ptr to MtlBaseLib if success, else NULL)
#define NOTIFY_MATLIB_PRE_SAVE			0x00000026	// Sent before saving a material library
#define NOTIFY_MATLIB_POST_SAVE			0x00000027	// Sent after saving a material library
#define NOTIFY_MATLIB_PRE_MERGE			0x00000028	// Sent before merging a material library
#define NOTIFY_MATLIB_POST_MERGE		0x00000029	// Sent after merging a material library

// The following File Link codes are sent while theHold is holding.
#define NOTIFY_FILELINK_BIND_FAILED		0x0000002A	// Sent if a File Link Bind fails for some reason
#define NOTIFY_FILELINK_DETACH_FAILED	0x0000002B	// Sent if a File Link Detach fails for some reason
#define NOTIFY_FILELINK_RELOAD_FAILED	0x0000002C	// Sent if a File Link Reload fails for some reason
#define NOTIFY_FILELINK_ATTACH_FAILED	0x0000002D	// Sent if a File Link Attach fails for some reason
#define NOTIFY_FILELINK_PRE_BIND		0x00000030	// Sent before a File Link Bind
#define NOTIFY_FILELINK_POST_BIND		0x00000031	// Sent after a successful File Link Bind
#define NOTIFY_FILELINK_PRE_DETACH		0x00000032	// Sent before a File Link Detach
#define NOTIFY_FILELINK_POST_DETACH		0x00000033	// Sent after a successful File Link Detach
#define NOTIFY_FILELINK_PRE_RELOAD		0x00000034	// Sent before a File Link Reload (partial, full, or dynamic)
#define NOTIFY_FILELINK_POST_RELOAD		0x00000035	// Sent after a successful File Link Reload (partial, full, or dynamic)
#define NOTIFY_FILELINK_PRE_ATTACH		0x00000036	// Sent before a File Link Attach
#define NOTIFY_FILELINK_POST_ATTACH		0x00000037	// Sent after a successful File Link Attach
// See also: NOTIFY_FILELINK_POST_RELOAD_PRE_PRUNE

#define NOTIFY_AB_NAVIGATE_URL			0x00000038  // VIZ request to load a URL into the Asset Browser
#define NOTIFY_RENDER_PREEVAL			0x00000039  // Sent before the render start evaluating objects

#define NOTIFY_NODE_CREATED				0x0000003A  // Sent when a node is created (callParam is pointer to node)
#define NOTIFY_NODE_LINKED				0x0000003B  // Sent when a node is linked (callParam is pointer to node)
#define NOTIFY_NODE_UNLINKED			0x0000003C  // Sent when a node is unlinked (callParam is pointer to node)
#define NOTIFY_NODE_HIDE				0x0000003D  // Sent when a node is hidden (callParam is pointer to node)
#define NOTIFY_NODE_UNHIDE				0x0000003E  // Sent when a node is unhidden (callParam is pointer to node)
#define NOTIFY_NODE_FREEZE				0x0000003F  // Sent when a node is frozen (callParam is pointer to node)
#define NOTIFY_NODE_UNFREEZE			0x00000040  // Sent when a node is unfrozen (callParam is pointer to node)
#define NOTIFY_NODE_PRE_MTL				0x00000041	// Node is about to get a new material (callParam is pointer to node)
#define NOTIFY_NODE_POST_MTL			0x00000042	// Node just got a new material (callParam is pointer to node)
#define NOTIFY_SCENE_ADDED_NODE			0x00000043	// Node just added to scene (callParam is pointer to node)
#define NOTIFY_SCENE_PRE_DELETED_NODE	0x00000044	// Node just removed from scene (callParam is pointer to node)
#define NOTIFY_SCENE_POST_DELETED_NODE	0x00000045	// Node just removed from scene
#define NOTIFY_SEL_NODES_PRE_DELETE		0x00000046	// selected nodes will be deleted. (callParam is pointer to Tab<INode*>)
#define NOTIFY_SEL_NODES_POST_DELETE	0x00000047	// selected nodes just deleted.

#define NOTIFY_WM_ENABLE				0x00000048  // sent when main window gets an WM_ENABLE (BOOL enabled)

#define NOTIFY_SYSTEM_SHUTDOWN			0x00000049	// Max is about to exit,  (system shutdown starting)
#define NOTIFY_SYSTEM_STARTUP			0x00000050	// Max just went live

#define NOTIFY_PLUGIN_LOADED			0x00000051	// A plug-in was just loaded. (callParam is pointer to DllDesc)

#define NOTIFY_SYSTEM_SHUTDOWN2			0x00000052	// Last broadcast before exit. (system shutdown completed)

#define NOTIFY_ANIMATE_ON				0x00000053	// sent when Animate UI mode activated
#define NOTIFY_ANIMATE_OFF				0x00000054	// sent when Animate UI mode de-activated

#define NOTIFY_COLOR_CHANGE				0x00000055	// sent when the system is updating it's custom colors
#define NOTIFY_PRE_EDIT_OBJ_CHANGE  	0x00000056	// sent just before the current edit object is about to chage
#define NOTIFY_POST_EDIT_OBJ_CHANGE  	0x00000057	// sent just afterthe current edit object chages

// NOTE: The radiosity_process notifications are designed to be broadcasted by radiosity plugins (derived from
//       class RadiosityEffect). The broadcast must be implemented in the plugin for the notification to work.
#define NOTIFY_RADIOSITYPROCESS_STARTED	0x00000058	// sent when radiosity processing is started
#define NOTIFY_RADIOSITYPROCESS_STOPPED	0x00000059	// sent when radiosity processing is stopped, but not done
#define NOTIFY_RADIOSITYPROCESS_RESET	0x0000005A	// sent when radiosity processing is reset
#define NOTIFY_RADIOSITYPROCESS_DONE	0x0000005B	// sent when radiosity processing is done

#define NOTIFY_LIGHTING_UNIT_DISPLAY_SYSTEM_CHANGE		0x0000005C	//sent when lighting unit display system is changed

// #define NOTIFY_INSTANCE_CREATED			0x00000053	// sent when a new plugin instance is created via Interface::CreateInstance(), 
												        // callParam is ptr to CreateInstanceCallParam (see below)

// > 10/29/01 - 1:57pm --MQM-- 
// this is helpful for any plugins needing to
// know when we're starting a reflect/refract map,
// or when we're starting the actual frame.
#define NOTIFY_BEGIN_RENDERING_REFLECT_REFRACT_MAP	0x0000005D
#define NOTIFY_BEGIN_RENDERING_ACTUAL_FRAME			0x0000005E
#define NOTIFY_BEGIN_RENDERING_TONEMAPPING_IMAGE	0x0000005F

// [dl | 6mar2002] Broadcast when the radiosity plugin is changed (a new one is assigned)
#define NOTIFY_RADIOSITY_PLUGIN_CHANGED     0x00000060

// [LAM - 3/13/02] Broadcast on scene undo/redo
#define NOTIFY_SCENE_UNDO                   0x00000061 // callParam is TCHAR* with the undo entry name
#define NOTIFY_SCENE_REDO                   0x00000062 // callParam is TCHAR* with the redo entry name
#define NOTIFY_MANIPULATE_MODE_OFF			0x00000063
#define NOTIFY_MANIPULATE_MODE_ON			0x00000064

// Broadcast when loading XRef (typically surrounding Merge notices)
// 020607  --prs.
#define NOTIFY_SCENE_XREF_PRE_MERGE			0x00000065
#define NOTIFY_SCENE_XREF_POST_MERGE		0x00000066
#define NOTIFY_OBJECT_XREF_PRE_MERGE		0x00000067
#define NOTIFY_OBJECT_XREF_POST_MERGE		0x00000068

// [J.Zhao - 6/10/02] The mirror tool applies to nodes in the selection list
// one at time. Following two notification messages indicates the beginning
// and end of the mirror operations. callParam in the NotifyInfo is a pointer
// to Tab<INode*> consisting of the nodes currently in the selection list
// that the mirror tool is to be applied to.
#define NOTIFY_PRE_MIRROR_NODES				0x00000069
#define NOTIFY_POST_MIRROR_NODES			0x0000006A

// [bayboro | 1may2002] Broadcast on node cloning
#define NOTIFY_NODE_CLONED					0x0000006B  // Sent after a node is cloned but before theHold.Accept(..) (callParam is pointer to node)

// [J.Zhao - 10/4/02] The following two notifications may be broadcast
// when NotifyDependents from outside the recursion, that is, not during
// the traversal of reference targets.
// Right now, for examples, the very sources of NotifyDependents() of
//  - REFMSG_MOUSE_CYCLE_STARTED
//  - REFMSG_MOUSE_CYCLE_COMPLETED
// are bracketed by the notifications.
// 
#define NOTIFY_PRE_NOTIFYDEPENDENTS			0x0000006C
#define NOTIFY_POST_NOTIFYDEPENDENTS		0x0000006D

//[hutchij 10/26/02] The following are sent by Mtl::RefAdded() and 
//Mtl::RefDeleted() respectively. CallParam is Mtl pointer.
#define	NOTIFY_MTL_REFADDED					0x0000006E
#define NOTIFY_MTL_REFDELETED				0x0000006F


//watje TIMERANGE CALLBACK
//watje time range call back for CS
#define NOTIFY_TIMERANGE_CHANGE				0x00000070 // Sent after the animate time range has been changed

//aszabo|dec.04.02|The NotifyInfo structure pointer callParam is passed a pointer 
// to a struct{ INode* node; Modifier* mod; ModContext* mc;}. 
#define NOTIFY_PRE_MODIFIER_ADDED			0x00000071
#define NOTIFY_POST_MODIFIER_ADDED			0x00000072
#define NOTIFY_PRE_MODIFIER_DELETED			0x00000073
#define NOTIFY_POST_MODIFIER_DELETED		0x00000074

//aszabo|dec.11.02|The callParam is a pointer to the list of nodes
// (INodeTab*) that is about to change \ has changed
#define NOTIFY_PRE_NODE_GENERAL_PROP_CHANGED	0x00000075
#define NOTIFY_POST_NODE_GENERAL_PROP_CHANGED	0x00000076
#define NOTIFY_PRE_NODE_GI_PROP_CHANGED			0x00000077
#define NOTIFY_POST_NODE_GI_PROP_CHANGED		0x00000078
#define NOTIFY_PRE_NODE_MENTALRAY_PROP_CHANGED	0x00000079
#define NOTIFY_POST_NODE_MENTALRAY_PROP_CHANGED	0x00000080
#define NOTIFY_PRE_NODE_BONE_PROP_CHANGED		0x00000081
#define NOTIFY_POST_NODE_BONE_PROP_CHANGED		0x00000082
#define NOTIFY_PRE_NODE_USER_PROP_CHANGED		0x00000083
#define NOTIFY_POST_NODE_USER_PROP_CHANGED		0x00000084

// CA - 1/23/03 - Added new notification for filelink. This one
// happens after all of the new objects for a reload have been
// created, but before any objects have been deleted.
#define NOTIFY_FILELINK_POST_RELOAD_PRE_PRUNE	0x00000085

// aszabo|jan.24.03|The two notification below are sent before
// and after each set of clones are created. For example, if
// there are N nodes cloned C times, the notification is sent 
// C times. 
// The CallParam for NOTIFY_PRE_NODES_CLONED is a pointer to the array of nodes
// that will be cloned (the original nodes): INodeTab* origNodes
// The CallParam for NOTIFY_POST_NODES_CLONED is a pointer to this struct:
// struct{ INodeTab* origNodes; INodeTab* clonedNodes; CloneType cloneType;}
#define NOTIFY_PRE_NODES_CLONED					0x00000086
#define NOTIFY_POST_NODES_CLONED				0x00000087
#define NOTIFY_POST_MERGE_PROCESS				0x00000088

// xavier robitaille | 03.02.07
// Broadcast a notification that a system path has changed. This 
// was added in order to notify the toolpalette if the Catalogs 
// dir gets modified from the Configure Path dialogue.
#define NOTIFY_SYSTEM_PRE_DIR_CHANGE			0x00000089
#define NOTIFY_SYSTEM_POST_DIR_CHANGE			0x0000008A

// a final notification for file open
//
// russom - 08-Mar-2005.
/*! \see NOTIFY_FILE_PRE_OPEN_PROCESS */
#define NOTIFY_FILE_POST_OPEN_PROCESS			0x0000008B

// Schematic View notifications
// callParam is ptr to index of schematic view (int*)
#define NOTIFY_SV_SELECTIONSET_CHANGED			0x0000008C
// callParam is IGraphNode*
#define NOTIFY_SV_DOUBLECLICK_GRAPHNODE			0x0000008D

// Notifications when changing the renderer
#define NOTIFY_PRE_RENDERER_CHANGE				0x0000008E
#define NOTIFY_POST_RENDERER_CHANGE				0x0000008F

// Schematic View notifications
// callParam is ptr to index of schematic view (int*)
#define NOTIFY_SV_PRE_LAYOUT_CHANGE				0x00000090
// callParam is ptr to index of schematic view (int*)
#define NOTIFY_SV_POST_LAYOUT_CHANGE			0x00000091

// Notification sent AFTER object categories were marked to be hidden\unhidden 
// Clients registered for this notification can retrieve the categories
// whose hidden state have changed by retrieving the category flags by calling
// DWORD Interface::GetHideByCategoryFlags()
#define NOTIFY_BY_CATEGORY_DISPLAY_FILTER_CHANGED			0x00000092
// Notification sent AFTER custom display filters have been activated\deactivated
// resulting in changes to some objects hidden state.
// Clients registered for this notification can retrieve the active 
// custom display filters by checking their On\Off state using
// BOOL Interface::GetDisplayFilter(int index)
#define NOTIFY_CUSTOM_DISPLAY_FILTER_CHANGED			0x00000093

// callParam is ptr to ILayer
#define NOTIFY_LAYER_CREATED				0x00000094 // called after layer is added to layer manager
#define NOTIFY_LAYER_DELETED				0x00000095 // called before layer is removed from layer manager
// callParam is ptr to struct{ INode* node; ILayer* oldLayer; ILayer* newLayer;}
// newLayer and oldLayer can be NULL when switching between layers, during create, and when loading files
// Layers may not be present in layer manager when sent during file load/merge
#define NOTIFY_NODE_LAYER_CHANGED			0x00000096

// following sent when a tabbed dialog is created or deleted. callparam is point to dialogID (Class_ID)
#define NOTIFY_TABBED_DIALOG_CREATED		0x00000097
#define NOTIFY_TABBED_DIALOG_DELETED		0x00000098

#define NOTIFY_NODE_NAME_SET				0x00000099	// Sent by BaseNode::SetName. (call Param is pointer to struct{ TCHAR* oldname; TCHAR* newname; INode* node} }

// Sent by the Material Editor when the "use texture in hardware shader" button is pressed.  This allow the standard
//material to force a rebuild of the hardware shader.  Param is a pointer to the material being effected.
#define NOTIFY_HW_TEXTURE_CHANGED			0x0000009A	

// Sent by MAXScript during its initialization immediately before it scans the
// registered plugin classes and wraps them in MAXClass values
// any runtime defined classes created in this callback will be detected by MXS
// any core interfaces installed in this callback will be detected by MXS
#define NOTIFY_MXS_STARTUP					0x0000009B

// Sent by MAXScript when it has completed its initialization 
#define NOTIFY_MXS_POST_STARTUP				0x0000009C

// Sent before and after an action item is executed
// NotifyInfo::callParam is ActionItem*
#define NOTIFY_ACTION_ITEM_PRE_EXEC				0x0000009D
#define NOTIFY_ACTION_ITEM_POST_EXEC			0x0000009E

// CCJ - 12.7.2004 - Scene State Notifications
// All the SceneState notifications have CallParam is a TCHAR* with the SceneState name
#define NOTIFY_SCENESTATE_PRE_SAVE			0x0000009F	// Sent before a Scene State is saved. CallParam is a TCHAR* with the SceneState name.
#define NOTIFY_SCENESTATE_POST_SAVE			0x000000A0	// Sent after a Scene State is saved. CallParam is a TCHAR* with the SceneState name.
#define NOTIFY_SCENESTATE_PRE_RESTORE		0x000000A1	// Sent before a Scene State is restored. CallParam is a TCHAR* with the SceneState name.
#define NOTIFY_SCENESTATE_POST_RESTORE		0x000000A2	// Sent after a Scene State is restored. CallParam is a TCHAR* with the SceneState name.
#define NOTIFY_SCENESTATE_DELETE			0x000000A3	// Sent after a Scene State is deleted. CallParam is a TCHAR* with the SceneState name.
#define NOTIFY_SCENESTATE_RENAME			0x000000A4	// Sent after a Scene State is renamed. CallParam is pointer to struct{ const TCHAR* oldname; const TCHAR* newname; }

// Added NH 20-Dec-04
/*! This provides a notification that is sent before an Undo/Redo starts.  This allows
a developer to get an accurate picture of what the undo/redo is doing, and not having to 
rely on calling RestoreOrRedoing().  This solves a problem of evaluating a node during an undo
when the hold system was active, but wasn't initiated from either script or the UI.  An example
would be right clicking to cancel object creation
*/
#define NOTIFY_SCENE_PRE_UNDO				0x000000A5

/*! This provides a notification that is sent before an Undo/Redo starts.  This allows
a developer to get an accurate picture of what the undo/redo is doing, and not having to 
rely on calling RestoreOrRedoing().  This solves a problem of evaluating a node during an undo
when the hold system was active, but wasn't initiated from either script or the UI.  An example
would be right clicking to cancel object creation
*/
#define NOTIFY_SCENE_PRE_REDO				0x000000A6

//! Added these defines to make the previous undo notification more explicit, and match the new PRE_UNDO.
#define NOTIFY_SCENE_POST_UNDO NOTIFY_SCENE_UNDO

//! Added these defines to make the previous undo notification more explicit, and match the new PRE_UNDO.
#define NOTIFY_SCENE_POST_REDO NOTIFY_SCENE_REDO

// Added LAM 19-Jan-05
/*! This provides a notification that MAXScript has been shut down. No access to any MXS value should be
made after this notification, including calls to event handlers
*/
#define NOTIFY_MXS_SHUTDOWN					0x000000A7

//Added NH 21-Jan-05
/*! This is sent by the D3D GFX (Dx9) when a device reset is happening.  This can allow Hardware shader
to release their resources allocated in the DEFAULT_POOL
*/
#define NOTIFY_D3D_PRE_DEVICE_RESET			0x000000A8

/*! This is sent by the D3D GFX (Dx9) when a device reset is happening.  This can allow Hardware shader
to release their resources allocated in the DEFAULT_POOL
*/
#define NOTIFY_D3D_POST_DEVICE_RESET		0x000000A9

// Added JH 2-18-05
//! \brief Toolpalette Listener Suspend / Resume notifications
/*! In VIZ, the tool palette system listens for additions and removals to the scene material library and tracks
    reference additions and removals to existing materials. You can suspend the system using the following.
Upon resumption, the scene material lib will be traversed and changes reflected in the palette. 
*/
#define NOTIFY_TOOLPALETTE_MTL_SUSPEND		0x000000AA
//! \brief Toolpalette Listener Suspend / Resume notifications
/*! In VIZ, the tool palette system listens for additions and removals to the scene material library and tracks
reference additions and removals to existing materials. You can suspend the system using the following.
Upon resumption, the scene material lib will be traversed and changes reflected in the palette. 
*/
#define NOTIFY_TOOLPALETTE_MTL_RESUME		0x000000AB

// Added LAM 04-Mar-05
/*! This provides a notification that a DataClassDesc is being replaced by a ClassDesc. This occurs 
when the dll containing a deferred loaded plugin is loaded. CallParam is pointer to 
struct{ const ClassDesc *oldClassDesc; const ClassDesc *newClassDesc; }. See struct ClassDescReplaced in plugapi.h
*/
#define NOTIFY_CLASSDESC_REPLACED			0x000000AC

// Added russom 08-Mar-05
/*! These notifications related to file open and file save all contain a CallParam that is pointer 
to a struct{ int iProcessType; const TCHAR* filename; }.  iProcessType is either FILE_PROCESS_SCENE (for
standard max scene file, FILE_PROCESS_HOLD_FETCH (when openning or saving a hold file), or
FILE_PROCESS_AUTOBAK (when the file save is trigger by an auto backup timer).  FILE_PROCESS_SCENE,
FILE_PROCESS_HOLD_FETCH, and FILE_PROCESS_AUTOBAK are defined below in this file.
\see FILE_PROCESS_SCENE, FILE_PROCESS_HOLD_FETCH, FILE_PROCESS_AUTOBAK
*/
#define NOTIFY_FILE_PRE_OPEN_PROCESS		0x000000AD
//#define NOTIFY_FILE_POST_OPEN_PROCESS		0x0000008B	// previously defined
/*! \see NOTIFY_FILE_PRE_OPEN_PROCESS */
#define NOTIFY_FILE_PRE_SAVE_PROCESS		0x000000AE
/*! \see NOTIFY_FILE_PRE_OPEN_PROCESS */
#define NOTIFY_FILE_POST_SAVE_PROCESS		0x000000AF

// Added LAM 15-Mar-05
/*! This provides a notification that a ClassDesc was loaded from a plugin dll. callParam is pointer to the ClassDesc.
*/
#define NOTIFY_CLASSDESC_LOADED				0x000000B0

// Added MAB 7-June-05
//! \brief This provides a notification that a toolbar configuration is being loaded, prior to load.
#define NOTIFY_TOOLBARS_PRE_LOAD				0x000000B1
//! \brief This provides a notification that a toolbar configuration is being loaded, after the load.
#define NOTIFY_TOOLBARS_POST_LOAD				0x000000B2

/*! This notification indicates that the Asset Tracking System is about to traverse the entire
	scene (or ReferenceTarget hierarchy, in the case of a general retarget action)
	and repath any assets that should be repathed.  No parameters are sent with this notification.
*/
#define NOTIFY_ATS_PRE_REPATH_PHASE				0x000000B3
/*! This notification indicates that the Asset Tracking System has completed its traversal of the entire
	scene (or ReferenceTarget hierarchy, in the case of a general retarget action) and has 
	repathed any assets that should have been repathed.  No parameters are sent with this notification.
*/
#define NOTIFY_ATS_POST_REPATH_PHASE			0x000000B4

//@{
/*!	These notifications are used to temporarily disable the bitmap proxy system.
	Any specific operation during which proxies should be disable can be surrounded
	by these notifications. This method of disabling proxies is different from 
	actually disabling the bitmap proxies through the proxy manager; these notifications
	do not actually result in re-loading of any bitmaps. Rather, the Bitmap Texture
	will only load the high-res image when it is asked to, which makes the process
	efficient.
	\br
	Note that one should ideally NOT broadcast these notifications. Instead, use
	class BitmapProxyTemporaryDisableGuard; it is safer as it handles multiple 
	nested disables.
*/
#define NOTIFY_PROXY_TEMPORARY_DISABLE_START	0x000000B5
#define NOTIFY_PROXY_TEMPORARY_DISABLE_END		0x000000B6

// Added russom 11-Apr-06
/*! This notification allows a plugin to query the system for the status of the specified file.
The Callparam is a pointer to a struct { const TCHAR* szFilename; int iStatus; }.  Plugins that
register for this notification can add to the iStatus member as needed, but should not clear any bits
already in iStatus.  Valid status masks for iStatus are defined below in this file.  Look for
FILE_STATUS_xxxxx.
*/
#define NOTIFY_FILE_CHECK_STATUS				0x000000B7

//@}

// Added CPJ 22-Apr-06
//! Sent when a Named selection set is created either in the UI, or via maxscript.
#define NOTIFY_NAMED_SEL_SET_CREATED			0x000000B8
//! Sent when a Named selection set is deleted either in the UI, or via maxscript.
#define NOTIFY_NAMED_SEL_SET_DELETED			0x000000B9
//! Sent when a Named selection set name is changed, either in the old UI, or via maxscript.
#define NOTIFY_NAMED_SEL_SET_RENAMED            0x000000BC
//! Sent when the sub-object level changes in the modify panel
#define NOTIFY_MODPANEL_SUBOBJECTLEVEL_CHANGED  0x000000BA


// Added NH 13-May-2006
//! Notification sent when a bitmap failed to load in the DirectX Shader Material
/*! This will be sent when a bitmap fails to load.  This is usually when DirectX does not support the file format.
The developer can register for this notification so that they can convert the the format into a DirectX texture resource. The developer is responsible 
for maintaining this resource, and can register for NOTIFY_D3D_POST_DEVICE_RESET to release and rebuild the resource on Lost device situations
The callParam is a struct{ TCHAR * fileName, TCHAR* paramName, Mtl* dxMaterial, bool forceUpdate}  From the material the developer can access the IEffectParser
and use LoadTexture using the paramName.  The forceUpdate will be set if this was an auto update of the bitmap from disk, or the user hit the reload 
button from the UI.  The developer will need to release and rebuild the bitmap under these situations.
*/
#define NOTIFY_FAILED_DIRECTX_MATERIAL_TEXTURE_LOAD		0x000000BB

//! \brief Sent just after NOTIFY_RENDER_PREEVAL. 
/*! The call param is a NotifyPreEval_FrameInfo*, defined at the bottom of this 
	header file. Sent before the render start evaluating objects.
	\note This message is sent ONLY when using the mental ray renderer. It is 
	NOT sent when using the scanline renderer.
*/
#define NOTIFY_RENDER_PREEVAL_FRAMEINFO					0x000000BC

//! \brief Sent just after deleting all refs in existing scene. 
/*! This notification is sent after wiping the existing scene. Examples of when this occurs is 
immediately before loading in the new scene from a file, and when doing a file new
*/
#define NOTIFY_POST_SCENE_RESET							0x000000BD

// Note #1: If you add more built-in notification codes, consider
//    increasing NUM_BUILTIN_NOTIFIERS in core\notify.cpp - currently 0x00D0

// Note #2: If you add more built-in notification codes, consider
//    adding them to MAXScript. See maxscrpt\maxcallbacks.cpp

// Start of messages for internal use only.
#define NOTIFY_INTERNAL_USE_START				0x70000000

// Flag values in callParam for NOTIFY_SYSTEM_PRE_NEW:
#define PRE_NEW_NEW_ALL						0x1
#define PRE_NEW_KEEP_OBJECTS				0x2
#define PRE_NEW_KEEP_OBJECTS_AND_HIERARCHY	0x3

// Added russom 08-Mar-05
/*! iProcessType value for scene file save/load
	\see NOTIFY_FILE_PRE_OPEN_PROCESS 
*/
#define FILE_PROCESS_SCENE					0x1
/*! iProcessType value for hold/fetch
	\see NOTIFY_FILE_PRE_OPEN_PROCESS 
*/
#define FILE_PROCESS_HOLD_FETCH				0x2
/*! iProcessType value for auto backup
	\see NOTIFY_FILE_PRE_OPEN_PROCESS 
*/
#define FILE_PROCESS_AUTOBAK				0x3

//  Added russom 11-Apr-06
/*! iStatus value for NOTIFY_FILE_CHECK_STATUS
	\see NOTIFY_FILE_CHECK_STATUS 
*/
#define FILE_STATUS_READONLY				0x1

// Notification information structure -- Passed to NOTIFYPROC to inform it what
// it's being notified about...
struct NotifyInfo: public MaxHeapOperators {
	int intcode;
	void *callParam;  // this param can be passed in with BroadcastNotification;
};

// The notification callback function
typedef void (* NOTIFYPROC)(void *param, NotifyInfo *info);

// Integer versions -- For pre-defined MAX codes
CoreExport int RegisterNotification(NOTIFYPROC proc, void *param, int code);
CoreExport int UnRegisterNotification(NOTIFYPROC proc, void *param, int code);
CoreExport void BroadcastNotification(int code);
CoreExport void BroadcastNotification(int code, void *callParam);

// Unregister a callback from all codes
CoreExport int UnRegisterNotification(NOTIFYPROC proc, void *param);

class ClassDesc;

class CreateInstanceCallParam: public MaxHeapOperators {
public:
	void* instance;
	ClassDesc* cd;
	CreateInstanceCallParam (void* i, ClassDesc* cd) : instance(i), cd(cd) { }
}; 

//! The call parameter that accompanies the notification code NOTIFY_RENDER_PREEVAL_FRAMEINFO
//! is a pointer to this class, which is sed to broadcast information about the view and 
//! the frame to be rendered.
struct NotifyPreEval_FrameInfo {
	//! Information about the view being rendered - same as that passed to GeomObject::GetRenderMesh().
	View* view;
	//! Information about the frame to be rendered.
	FrameRendParams* frp;
	//! The view node, as passed to Renderer::Open(). May be NULL.
	INode* viewNode;
	//! The view parameters, as passed to Renderer::Render(). Only valid if 'viewNode' is NULL.
	ViewParams* viewParams;
};

#endif // _NOTIFY_H_
