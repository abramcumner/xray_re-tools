/*********************************************************************
 *<
	FILE: maxapi.h

	DESCRIPTION: These are functions that are exported from the 
	             3DS MAX executable.

	CREATED BY:	Rolf Berteig

	HISTORY: Created 28 November 1994

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/

#ifndef __MAXAPI__
#define __MAXAPI__

#include "maxheap.h"
#include <ole2.h>
#include "cmdmode.h"
#include "sceneapi.h"
#include "rtclick.h"
#include "evuser.h"
#include "maxcom.h"
#include "plugin.h"
#include "log.h"
#include "ActionTable.h"
#include "iTreeVw.h"
#include "iRenderPresets.h"
#include "excllist.h"
#include "DefaultActions.h"
#include "RendType.h"

class ViewParams;
class ModContext;
class HitData;
class HitLog;
class CtrlHitLog;
class MtlBase;
class Mtl;
class Texmap;
class PickNodeCallback;
class Renderer;
class IScanRenderer;
class RendParams;
class IRenderElementMgr;
class RendProgressCallback;
class Bitmap;
class BitmapInfo;
class Texmap;
class SoundObj;
class GenCamera;
class GenLight;
class NameTab;
class ShadowType;
class MacroRecorder;
class CommandLineCallback;

#ifdef _OSNAP
class IOsnapManager;
class IOsnapManager7;
class MouseManager;
#endif
class MtlBaseLib;
class Atmospheric;
class IRollupWindow;
class ITrackViewNode;
class DllDir;
class Effect;
class SpaceArrayCallback;
class IMenuManager;
class IColorManager;
class Modifier;

// mjm - begin - 05.26.00
// used to define distinct sets of rendering parameters
enum RenderSettingID
{
	RS_Production = 0,
	RS_Draft,
	RS_IReshade,
	RS_MEdit		// [dl | 15may2003]
};
// mjm - end

// This type describes the clone type when cloning nodes
enum CloneType { NODE_COPY, NODE_INSTANCE, NODE_REFERENCE, NODE_NONE };

/*! \defgroup DupNodeNameActions Duplicate Node Name Actions 
The following represent flags describing possible actions to be taken when merging 
nodes with names identical to the name of nodes already present in the scene.
These values can be passed to the Interface::MergeFromFile method's dupAction parameter 
and other methods as documented.
\see Interface::MergeFromFile, IObjXRefManager8::SetDupObjNameAction 
*/
//@{
#define MERGE_DUPS_PROMPT 0    //!< Prompt user when duplicate node names are encountered
#define MERGE_DUPS_MERGE  1	   //!< Merge nodes and keep old ones in the scene
#define MERGE_DUPS_SKIP   2    //!< Do not merge nodes with duplicated names
#define MERGE_DUPS_DELOLD 3    //!< Merge nodes and delete old ones from the scene 
#define MERGE_LIST_NAMES  4    //!< Causes a list of objects to be placed into mrgList parameter of Interface::MergeFromFile. No actual merging will take place. The mergeAll parameter of Interface::MergeFromFile must be TRUE.
#define MERGE_DUPS_RENAME 1000 //!< Merge nodes and automatically rename them
//@}

/*! \defgroup DupMtlNameActions Duplicate Material Name Actions
The following represent flags describing possible action to be taken when merging materials
with names identical to the names of materials already present in the scene.
These values can be passed to the Interface::MergeFromFile method's dupMtlAction parameter
and other methods as documented.
\see Interface::MergeFromFile, IObjXRefManager8::SetDupMtlNameAction
*/
//@{
#define MERGE_DUP_MTL_PROMPT      MERGE_DUPS_PROMPT	//!< Prompt user when duplicate material names are encountered
#define MERGE_DUP_MTL_USE_MERGED  MERGE_DUPS_DELOLD	//!< Merge material and replace all scene materials with the same name with the merged material
#define MERGE_DUP_MTL_USE_SCENE   MERGE_DUPS_SKIP		//!< Don't merge material, use instead the scene material with the same name
#define MERGE_DUP_MTL_RENAME      MERGE_DUPS_RENAME	//!< Merge material and automatically rename them
//@}

/*! \defgroup NodeReparentActions Node Reparent Actions
The following represent flags describing the possible actions to be taken regarding reparenting 
of nodes during a merge operation. These values can be passed to the Interface::MergeFromFile method's 
reparentAction parameter and other methods as documented.
\see Interface::MergeFromFile
*/
//@{
#define MERGE_REPARENT_PROMPT     0    //!< Prompt user for reparenting action
#define MERGE_REPARENT_ALWAYS     1    //!< Maintain node hierarchies are maintained when merging nodes.
#define MERGE_REPARENT_NEVER      2    //!< Don't maintain node hierarchies are maintained when merging nodes. Merged nodes become children of the scene's root node.
//@}

// ConvertFlagedNodesToXRefs() xflags bits
/*! \defgroup XRefObjectOptions Object Xref Options
The following represent options gouverning how scene entities are xrefed.
Note that some of these options should not be combined. For example, modifiers
should either be xrefed, merged or dropped, but not xrefed and dropped in the same time. 
\see IObjXRefManager8::AddXRefItemsFromFile, Interface7::ConvertFlagedNodesToXRefs
*/
//@{
#define XREF_AS_PROXY								(1<<0) //!< The xref objects are created as proxies
#define XREF_XREF_MODIFIERS					0  //!< Modifiers will be part of the xrefed object, i.e. modifiers won't be accesible in the master scene's modifier panel, but will be updated on subsequent updates of the xrefed objects
#define XREF_DROP_MODIFIERS					(1<<1) //!< Modifiers will not be xrefed at all, i.e. the non-modified version of the source objects are xrefed
#define XREF_MERGE_MODIFIERS				(1<<2) //!< Modifiers will be merged, i.e modifiers will be accessible in the master scene's modifier panel, but won't get update on subsequent updates of the xrefed objects
#define XREF_MERGE_MANIPULATORS			(1<<3) //!< Manipulators will be merged rather than xrefed
#define XREF_SELECT_NODES						(1<<5) //!< The xrefed nodes are added to the current selection set
#define XREF_MERGE_CONTROLLERS			(1<<6) //!< Controllers will be merged rather than being xrefed
#define XREF_MERGE_MATERIALS				(1<<7) //!< Materials will be merged rather than xrefed
//@}

// CCJ: Property Set definitions
// For internal reasons these must be bitflags
#define PROPSET_SUMMARYINFO		0x01
#define PROPSET_DOCSUMMARYINFO	0x02
#define PROPSET_USERDEFINED		0x04

// Interface::Execute cmd values
#define  I_EXEC_ACTIVATE_TEXTURE    1  //	arg1:  Texmap *tx;	arg2:  Mtl *mtl; arg3:  int subnum;
#define  I_EXEC_DEACTIVATE_TEXTURE  2  //	arg1:  Texmap *tx;	arg2:  Mtl *mtl; arg3:  int subnum;
#define  I_EXEC_RENDER_MTL_SAMPLE 3    //   arg1:  MtlBase *mb; arg2: int size; arg3: UBYTE *image;  returns 0:fail, 1: success
#define  I_EXEC_GET_VPDISPLAY_DIB 4    // 	arg1: Texmap *tx; arg2: TimeValue t; arg3: TexHandleMaker *thm; arg4: BOOL mono; 
										//   arg5: int forceW; arg6: int forceH --	returns *BITMAPINFO

// added as complement to Interface::GetDir(), 020919  --prs.
#define  I_EXEC_SET_DIR 10             //   arg1:  int which; arg2: MCHAR *dir;

#ifdef ACAD_MAXSCRIPT_EXTENSIONS
#define  I_EXEC_OFFSET_SPLINE   80     //   arg1:  INode *spline; arg2: float amount;
#define  I_EXEC_OFFSET_MEASURE  81     //   arg1:  INode *spline; arg2: Point3 *point; arg3: float *result;
#define  I_EXEC_TRIM_EXTEND     82     //   arg1:  INodeTab *all; arg2: INodeTab *act;
//#define  I_EXEC_REG_VIEWWINDOW2 85     //   arg1:  ViewWindow2* (deprecated)
#endif

#ifdef DESIGN_VER
#define  I_EXEC_SET_DISABLED_COLOR 84  //   arg1:  COLORREF 
#endif

//SS 3/11/2002: Added to max to support Substitute modifier
#define  I_EXEC_NEW_OBJ_XREF_DLG 83    //   arg1:  INodeTab *nodes; arg2: BOOL forceSingle;
#define	 I_EXEC_MODIFYTASK_INVALIDATEPANEL 86 //No Args

// russom - 08/27/03 - ECO 1146
#define I_EXEC_INVALIDATE_VIEWEXP 87	// arg1: ViewExp *vp -- set invalidate flag on ViewExp*; arg2: BOOL Update all view related objects

#define I_EXEC_SET_NUDGE 88	// arg1: BOOL bEnableNude -- Enable/Disable viewport arrow key nudge

#define I_EXEC_COUNT_MTL_SCENEREFS  0x2001 // arg1 Mtl *mt: returns number of refs from scene 

// Interface::Execute return values
#ifdef ACAD_MAXSCRIPT_EXTENSIONS
#define I_EXEC_RET_NULL_NODE    1
#define I_EXEC_RET_NULL_OBJECT  2
#define I_EXEC_RET_NOT_SPLINE   3
#endif
#define I_EXEC_RET_OFFSET_FAIL  4

class NameMaker: public MaxHeapOperators {
	public:
		virtual void MakeUniqueName(MSTR &name)=0;
		virtual void AddName(MSTR &name) = 0;
        virtual BOOL NameExists(MSTR& name) = 0;
        virtual ~NameMaker() {}
	};

// Viewport degradation flags used by Interface7::RedrawViewportsNow
#define VP_DEFAULT_RENDER	0x0000	// same as REDRAW_INTERACTIVE
#define VP_DONT_RENDER		0x0001
#define VP_DONT_SIMPLIFY	0x0002	// same as REDRAW_NORMAL
#define VP_START_SEQUENCE	0x0004	// same as REDRAW_BEGIN
#define VP_END_SEQUENCE		0x0008	// same as REDRAW_END
#define VP_SECOND_PASS		0x0010	// for when both back- and fore-ground is rendered

// Viewport Rendering Levels
#define SHADE_LEVELS		10
enum ShadeType {
	SMOOTH_HIGHLIGHT,
	SMOOTH,
	FACET_HIGHLITE,
	FACET,
	CONSTANT,
	HIDDENLINE,
	LITE_WIREFRAME,
	Z_WIREFRAME,
	WIREFRAME,
	BOX,
};

//JH 05/06/98 
// VIEW_OTHER must be last, since "other" types are then numbered consecutively!!!
// And the order can't be changed, or old MAX files won't load properly DB 11/98
enum ViewType { VIEW_LEFT,VIEW_RIGHT,VIEW_TOP,VIEW_BOTTOM,VIEW_FRONT,VIEW_BACK, 
	VIEW_ISO_USER, VIEW_PERSP_USER, VIEW_CAMERA, VIEW_GRID, VIEW_NONE, VIEW_TRACK, 
	VIEW_SPOT, VIEW_SHAPE, VIEW_SCHEMATIC, VIEW_RENDER, VIEW_OTHER};


// class for registering a window that can appear in a MAX viewport DB 10/6/98
class ViewWindow : public InterfaceServer {
public:
	virtual MCHAR *GetName()=0;
	virtual HWND CreateViewWindow(HWND hParent, int x, int y, int w, int h)=0;
	virtual void DestroyViewWindow(HWND hWnd)=0;
	// CanCreate can be overridden to return FALSE if a ViewWindow can only have
	// a single instance, and that instance is already present.  If CanCreate
	// returns FALSE, then the menu item for this ViewWindow will be grayed out.
	virtual BOOL CanCreate() { return TRUE; }
	// mep 7/12/99
	// NumberCanCreate() returns the number of instances of a given window that
	// can be created. This allows, for example, the UI to know without physically
	// creating a window that only a limited number of windows of that type can
	// be created. A -1 implies that the count is not known.
	virtual int NumberCanCreate() { return -1; }
};

//docking states for opening a trackview window
static const int FLOAT_TRACKVIEW		= 0;	// float window.  can't dock on top (the default)
static const int DOCK_TRACKVIEW_TOP		= 1;	// dock on top
static const int DOCK_TRACKVIEW_BOTTOM	= 2;	// dock on bottom.  can't dock on top
static const int CAN_DOCK_TRACKVIEW_TOP	= 3;	// floating but able to dock on top

#define TRACKBAR_FILTER_ALL			1
#define TRACKBAR_FILTER_TMONLY		2
#define TRACKBAR_FILTER_CURRENTTM	3
#define TRACKBAR_FILTER_OBJECT		4
#define TRACKBAR_FILTER_MATERIAL	5

/// The TrackBar filter callback function
/// ****************************************************
/// The anim argument is the node you are filtering.
/// The parent argument is the owning animatable.  It returns the anim with its cooresponding subAnimIndex.
///		anim == parent->SubAnim(subAnimIndex)
/// The grandParent argument is provided as an additional layer that is needed in case the parent is a ParamBlock(2) or something similar.  
///		For instance you may need to know that the grandParent is a Bend modifier and the subAnimIndex is 
///		an index into the param block storage unit.
/// The node argument is the originating scene node.
/// Return true if the node should be included, otherswise return false.
typedef bool (* TRACKBAR_FILTER_PROC)(Animatable* anim, Animatable* parent, int subAnimIndex, Animatable* grandParent, INode* node);

/// The TrackBar filter addition callback function
/// ****************************************************
/// The anims argument is a Tab of the parent animatables to add
/// The subAnims argument is a Tab of the SubAnim numbers.  
///		The actual animatable added will be anim = anims[i]->SubAnim(subAnims[i])
/// The node argument is the originating scene node.
///
/// The anims, subAnims, and nodes Tab should all have the same count.
/// Return true if the addition should occur or false if is should not.
typedef bool (* TRACKBAR_ADDITION_PROC)(Tab<Animatable*> &anims, Tab<int> &subAnims, Tab<INode*> &nodes);

/// The ITrackBarFilterManager interface is implemented by the TrackBar.
/// You can get an instance of this interface from the TrackBar using the following:
///	ITrackBarFilterManager* filterManager = (ITrackBarFilterManager*)GetCOREInterface()->GetTrackBar()->GetInterface(TRACKBAR_FILTER_MANAGER_INTERFACE);
#define TRACKBAR_FILTER_MANAGER_INTERFACE Interface_ID(0xc85046, 0x2def7c7d)
class ITrackBarFilterManager : public BaseInterface
{
public:
	/// Register a new filter with the filter manager.  This is usually done at startup, but does not have to be.
	/// callbackFilter or callbackAddition can be NULL
	/// The name is a UI displayable name.
	/// The ID is a unique ID so filters are not duplicated.
	/// If active is true the filter will be used to filter TrackBar key display.
	/// If stopTraversal is false. This filter will not filter out subAnims of the object is supports.  
	virtual int	RegisterFilter(TRACKBAR_FILTER_PROC callbackFilter, TRACKBAR_ADDITION_PROC callbackAddition, MSTR name, Class_ID filterID, bool active = false, bool stopTraversal = true)=0;
	/// Remove a registered filter from the filter manager.
	virtual void UnRegisterFilter(int index)=0;
	/// Remove a registered filter from the filter manager.
	virtual void UnRegisterFilter(Class_ID filterID)
	{
		for (int i = FilterCount()-1; i >=0; i--)
		{
			if (GetFilterID(i) == filterID)
			{
				UnRegisterFilter(i);
				break;
			}
		}
	}
	/// Returns the number of filters registered with the filter manager
	virtual int FilterCount()=0;
	/// Get a registered filter's ID.
	virtual Class_ID GetFilterID(int index)=0;
	/// Get a registered filter's Name.
	virtual MSTR GetFilterName(int index)=0;
	/// If the filter is active its Filter() method will be called once for 
	/// each animatable visited by the TrackBar's traverser.
	virtual bool IsFilterActive(int index)=0;
	/// Activate or deactivate the filter.  If active it will be used to filter the key display.
	virtual void SetFilterActive(int index, bool state)=0;

private:
friend class ITrackBar;
	/// MAXScript support through function publishing
	virtual int	RegisterFilter(Value* callbackFilter, Value* callbackAddition, MSTR name, int id, bool active = false, bool stopTraversal = true)=0;
};


#define TRACKBAR_INTERFACE Interface_ID(0x2aff3557, 0x16aa714b)

// class for accessing the TrackBar (the Mini TrackView)
class ITrackBar : public FPMixinInterface {
public:
	virtual void		SetVisible(BOOL bVisible) = 0;
	virtual BOOL		IsVisible() = 0;
	virtual void		SetFilter(UINT nFilter) = 0;
	virtual UINT		GetFilter() = 0;
	virtual TimeValue	GetNextKey(TimeValue tStart, BOOL bForward) = 0;
	virtual void		RedrawTrackBar(bool bForce = false) = 0;
	virtual void		SetShowFrames(bool b) = 0;
	virtual bool		GetShowFrames() = 0;
	virtual void		SetShowAudio(bool b) = 0;
	virtual bool		GetShowAudio() = 0;
	virtual void		SetShowSelectionRange(bool b) = 0;
	virtual bool		GetShowSelectionRange() = 0;
	virtual void		SetSnapToFrames(bool b) = 0;
	virtual bool		GetSnapToFrames() = 0;
	virtual void		SetKeyTransparency(int xp) = 0;
	virtual int			GetKeyTransparency() = 0;
	virtual void		SetSelKeyTransparency(int xp) = 0;
	virtual int			GetSelKeyTransparency() = 0;
	virtual void		SetCursorTransparency(int xp) = 0;
	virtual int			GetCursorTransparency() = 0;
	
	//Function ID's for Function Publishing -- Added by AF (09/27/00)
	enum { fIdgetVisible, fIdsetVisible, fIdgetFilter, fIdsetFilter, fIdgetNextKeyTime, fIdgetPreviousKeyTime, fIdredrawTrackBar,
		fIdgetShowFrames, fIdsetShowFrames, fIdgetShowAudio, fIdsetShowAudio, fIdgetShowSelectionRange, fIdsetShowSelectionRange,
		fIdgetSnapToFrames, fIdsetSnapToFrames, fIdgetKeyTransparency, fIdsetKeyTransparency, 
		fIdgetSelKeyTransparency, fIdsetSelKeyTransparency, fIdgetCursorTransparency, fIdsetCursorTransparency,
		fIdregisterFilter, fIdUnRegisterFilter, fIdFilterCount, fIdFilterID, fIdFilterName, fIdIsFilterActive, fIdSetFilterActive,
	};
	//symbolic enum ID for Function Publishing
	enum {enumIDfilterType,
	};
	//Function Publishing methods added by AF (9/27/00)
	BaseInterface* GetInterface(Interface_ID id) { return (id == TRACKBAR_INTERFACE) ? this : FPMixinInterface::GetInterface(id); }
	FPInterfaceDesc* GetDesc() { return (FPInterfaceDesc*)GetCOREInterface(TRACKBAR_INTERFACE); }

	// dispatch map for FP-published functions
	BEGIN_FUNCTION_MAP
	VFN_1(fIdredrawTrackBar, RedrawTrackBar, TYPE_bool);
	FNT_0(fIdgetNextKeyTime, TYPE_TIMEVALUE, fpGetNextKey);
	FNT_0(fIdgetPreviousKeyTime, TYPE_TIMEVALUE, fpGetPreviousKey);
	FN_6(fIdregisterFilter, TYPE_INDEX, fpRegisterFilter, TYPE_VALUE, TYPE_VALUE, TYPE_TSTR, TYPE_INT, TYPE_bool, TYPE_bool);
	VFN_1(fIdUnRegisterFilter, fpUnRegisterFilter, TYPE_INDEX);
	FN_0(fIdFilterCount, TYPE_INT, fpFilterCount);
	FN_1(fIdFilterID, TYPE_INT_TAB_BV, fpGetFilterID, TYPE_INDEX);
	FN_1(fIdFilterName, TYPE_TSTR_BV, fpGetFilterName, TYPE_INDEX);
	FN_1(fIdIsFilterActive, TYPE_bool, fpIsFilterActive, TYPE_INDEX);
	VFN_2(fIdSetFilterActive, fpSetFilterActive, TYPE_INDEX, TYPE_bool);

	PROP_FNS(fIdgetVisible, IsVisible, fIdsetVisible, SetVisible, TYPE_BOOL);
	PROP_FNS(fIdgetFilter, GetFilter, fIdsetFilter, SetFilter, TYPE_ENUM);
	PROP_FNS(fIdgetShowFrames, GetShowFrames, fIdsetShowFrames, SetShowFrames, TYPE_bool);
	PROP_FNS(fIdgetShowAudio, GetShowAudio, fIdsetShowAudio, SetShowAudio, TYPE_bool);
	PROP_FNS(fIdgetShowSelectionRange, GetShowSelectionRange, fIdsetShowSelectionRange, SetShowSelectionRange, TYPE_bool);
	PROP_FNS(fIdgetSnapToFrames, GetSnapToFrames, fIdsetSnapToFrames, SetSnapToFrames, TYPE_bool);
	PROP_FNS(fIdgetKeyTransparency, GetKeyTransparency, fIdsetKeyTransparency, SetKeyTransparency, TYPE_INT);
	PROP_FNS(fIdgetSelKeyTransparency, GetSelKeyTransparency, fIdsetSelKeyTransparency, SetSelKeyTransparency, TYPE_INT);
	PROP_FNS(fIdgetCursorTransparency, GetCursorTransparency, fIdsetCursorTransparency, SetCursorTransparency, TYPE_INT);
	END_FUNCTION_MAP 

private:
	//Private methods for Maxscript exposure -- Added by AF (09/27/00)
	TimeValue fpGetNextKey(TimeValue t) {return GetNextKey(t, TRUE);}
	TimeValue fpGetPreviousKey(TimeValue t) { return GetNextKey(t, FALSE);}
	int	 fpRegisterFilter(Value* callbackFilter, Value* callbackAddition, MSTR name, int id, bool active = false, bool stopTraversal = true)
		{
		ITrackBarFilterManager* filterManager = (ITrackBarFilterManager*)this->GetInterface(TRACKBAR_FILTER_MANAGER_INTERFACE);
		if (filterManager != NULL)
			return filterManager->RegisterFilter(callbackFilter, callbackAddition, name, id, active, stopTraversal);
		return -1;
		}
	void fpUnRegisterFilter(int index)
		{
		ITrackBarFilterManager* filterManager = (ITrackBarFilterManager*)this->GetInterface(TRACKBAR_FILTER_MANAGER_INTERFACE);
		if (filterManager != NULL)
			filterManager->UnRegisterFilter(index);
		}
	int fpFilterCount()
		{
		ITrackBarFilterManager* filterManager = (ITrackBarFilterManager*)this->GetInterface(TRACKBAR_FILTER_MANAGER_INTERFACE);
		if (filterManager != NULL)
			{
			return filterManager->FilterCount();
			}
		return 0;
		}
	Tab<int> fpGetFilterID(int index)
		{
		Tab<int> ids;
		ITrackBarFilterManager* filterManager = (ITrackBarFilterManager*)this->GetInterface(TRACKBAR_FILTER_MANAGER_INTERFACE);
		if (filterManager != NULL)
			{
			Class_ID id = filterManager->GetFilterID(index);
			int IDa = id.PartA();
			int IDb = id.PartB();
			ids.Append(1, &IDa);
			ids.Append(1, &IDb);
			}
		return ids;
		}
	MSTR fpGetFilterName(int index)
		{
		ITrackBarFilterManager* filterManager = (ITrackBarFilterManager*)this->GetInterface(TRACKBAR_FILTER_MANAGER_INTERFACE);
		if (filterManager != NULL)
			{
			return filterManager->GetFilterName(index);
			}
		return _M("");
		}

	bool fpIsFilterActive(int index)
		{
		ITrackBarFilterManager* filterManager = (ITrackBarFilterManager*)this->GetInterface(TRACKBAR_FILTER_MANAGER_INTERFACE);
		if (filterManager != NULL)
			{
			return filterManager->IsFilterActive(index);
			}
		return false;
		}
	void fpSetFilterActive(int index, bool state)
		{
		ITrackBarFilterManager* filterManager = (ITrackBarFilterManager*)this->GetInterface(TRACKBAR_FILTER_MANAGER_INTERFACE);
		if (filterManager != NULL)
			filterManager->SetFilterActive(index, state);
		}
	};

// This class provides functions that expose the portions of View3D
// that are exported for use by plug-ins.
class ViewExp : public InterfaceServer {
	public:
		virtual Point3 GetPointOnCP(const IPoint2 &ps)=0;
		virtual Point3 SnapPoint(const IPoint2 &in, IPoint2 &out, Matrix3 *plane2d = NULL, DWORD flags = 0)=0;
#ifdef _OSNAP
		virtual void SnapPreview(const IPoint2 &in, IPoint2 &out, Matrix3 *plane2d = NULL, DWORD flags = 0)=0;
		virtual void GetGridDims(float *MinX, float *MaxX, float *MinY, float *MaxY) = 0;
#endif							  
		virtual float SnapLength(float in)=0;
		virtual float GetCPDisp(const Point3 base, const Point3& dir, 
                        const IPoint2& sp1, const IPoint2& sp2, BOOL snap = FALSE )=0;
		virtual GraphicsWindow *getGW()=0;
		virtual int IsWire()=0;
		virtual Rect GetDammageRect()=0;		

		virtual Point3 MapScreenToView( IPoint2& sp, float depth )=0;
		virtual void MapScreenToWorldRay(float sx, float sy, Ray& ray)=0;

		// set the affine tm for the view and ret TRUE if the view is ISO_USER or PERSP_USER
		// else do nothing and return FALSE
		virtual BOOL SetAffineTM(const Matrix3& m)=0;
		virtual void GetAffineTM( Matrix3& tm )=0;
		virtual int GetViewType() = 0;
		virtual BOOL IsPerspView()=0;
		virtual float GetFOV()=0;
		virtual float GetFocalDist()=0;
		virtual float GetScreenScaleFactor(const Point3 worldPoint)=0;
		//! \brief Get the FPS from the active viewport.
		//! \return Return the frame per second of the active viewport.
		virtual float GetFPS()=0;

		// return the viewPort screen width factor in world space at 
		// a point in world space
		virtual float GetVPWorldWidth(const Point3 wPoint)=0;
		virtual Point3 MapCPToWorld(const Point3 cpPoint)=0;
		virtual void GetConstructionTM( Matrix3 &tm )=0;
		virtual void SetGridSize( float size )=0;
		virtual float GetGridSize()=0;
		virtual BOOL IsGridVisible()=0;
		virtual int GetGridType()=0;

		// Get the camera if there is one.
		virtual INode *GetViewCamera()=0;

		// Set this viewport to a camera view
		virtual void SetViewCamera(INode *camNode)=0;

		// Set this viewport to a user view 
		virtual void SetViewUser(BOOL persp)=0;

		// Get the spot if there is one
		virtual INode *GetViewSpot()=0;

		// Set this viewport to a spotlight view
		virtual void SetViewSpot(INode *spotNode)=0;

		// node level hit-testing
		virtual void ClearHitList()=0;
		virtual INode *GetClosestHit()=0;
		virtual INode *GetHit(int i)=0;
		virtual int HitCount()=0;
		// subobject level hit-testing
		virtual	void LogHit(INode *nr, ModContext *mc, DWORD dist, ulong info, HitData *hitdat = NULL)=0;		
		virtual HitLog&	GetSubObjHitList()=0;
		virtual void ClearSubObjHitList()=0;
		virtual int NumSubObjHits()=0;

		// For controller apparatus hit testing
		virtual void CtrlLogHit(INode *nr,DWORD dist,ulong info,DWORD infoExtra)=0;
		virtual CtrlHitLog&	GetCtrlHitList()=0;
		virtual void ClearCtrlHitList()=0;
		
		virtual float NonScalingObjectSize()=0;  // 1.0 is "default"

		// Turn on and off image background display
		virtual BOOL setBkgImageDsp(BOOL onOff)=0;
		virtual int	getBkgImageDsp(void)=0;		

		// Turn on and off safe frame display
		virtual void setSFDisplay(int onOff)=0;
		virtual int getSFDisplay(void)=0;

		// This is the window handle of the viewport. This is the
		// same window handle past to GetViewport() to get a ViewExp*
		virtual HWND GetHWnd()=0;

		// Test if the viewport is active
		virtual	BOOL IsActive() = 0;
		// Test if the viewport is enabled
		virtual	BOOL IsEnabled() = 0;

		//methods for floating grids
		virtual void TrackImplicitGrid(IPoint2 m, Matrix3* mat = NULL, ULONG hitTestFlags = 0) = 0;
		virtual void CommitImplicitGrid(IPoint2 m, int mouseflags, Matrix3* mat = NULL) = 0;
		virtual void ReleaseImplicitGrid() = 0;

		// Generic expansion function
		virtual INT_PTR Execute(int cmd, ULONG_PTR arg1=0, ULONG_PTR arg2=0, ULONG_PTR arg3=0) { return 0; } 
	};


// return values for CreateMouseCallBack
#define CREATE_CONTINUE 1
#define CREATE_STOP	0	    // creation terminated normally
#define CREATE_ABORT -1		// delete the created object and node

//JH 5/15/01
//return values for MouseCallback
//Historically this method just returned 0 or 1 to indicate if processing should continue
//Now we'll add a return value to indicate that the Mouse proc requires window capture
#define MOUSE_STOP	0
#define MOUSE_CONTINUE	1
#define MOUSE_CAPTURE_CONTINUE 2

// This is a specific call-back proc for handling the creation process
// of a plug-in object.
// The vpt passed in will have had setTransform called with the 
// transform of the current construction plane.
class CreateMouseCallBack: public MaxHeapOperators {
	public:
		virtual int proc( 
			ViewExp *vpt,
			int msg, 
			int point, 
			int flags, 
			IPoint2 m,
			Matrix3& mat
			)=0;
		virtual int override(int mode) { return mode; }	// Defaults to mode supplied

        // Tells the system that we aren't starting a new creation
        virtual BOOL StartNewCreation() { return TRUE; }

		//Tells the system if ortho mode makes sense for this creation
		//In general this won't be true but splines and such...
		virtual BOOL TolerateOrthoMode() {	return FALSE; }
	};


class Object;
class ConstObject;
class MouseCallBack;
class IObjCreate;
class IObjParam;
class ModContext;
class ModContextList;
class INodeTab;

// Passed to getBkgFrameRange()
#define VIEWPORT_BKG_START			0
#define VIEWPORT_BKG_END			1

// Passed to setBkgORType().
#define VIEWPORT_BKG_BLANK			0
#define VIEWPORT_BKG_HOLD			1
#define VIEWPORT_BKG_LOOP			2

// Passed to setBkgImageAspect()
#define VIEWPORT_BKG_ASPECT_VIEW	0
#define VIEWPORT_BKG_ASPECT_BITMAP	1
#define VIEWPORT_BKG_ASPECT_OUTPUT	2


// Identify the transform tool buttons
#define MOVE_BUTTON		1
#define ROTATE_BUTTON	2
#define NUSCALE_BUTTON	3
#define USCALE_BUTTON	4
#define SQUASH_BUTTON	5
#define SELECT_BUTTON	6

// Axis constraints.
#define AXIS_XY		2
#define AXIS_ZX		1
#define AXIS_YZ		0
#define AXIS_X		3
#define AXIS_Y		4
#define AXIS_Z		5

// Origin modes		
#define ORIGIN_LOCAL		0	// Object's pivot
#define ORIGIN_SELECTION	1	// Center of selection set (or center of individual object for local or parent space)
#define ORIGIN_SYSTEM		2	// Center of the reference coord. system

// Reference coordinate system
#define COORDS_HYBRID	0
#define COORDS_SCREEN	1
#define COORDS_WORLD	2
#define COORDS_PARENT	3
#define COORDS_LOCAL	4
#define COORDS_OBJECT	5
#define COORDS_GIMBAL	6

// Task Modes
#define TASK_MODE_CREATE		1
#define TASK_MODE_MODIFY		2
#define TASK_MODE_HIERARCHY		3
#define TASK_MODE_MOTION		4
#define TASK_MODE_DISPLAY		5
#define TASK_MODE_UTILITY		6

// Max cursors
#define SYSCUR_MOVE			1
#define SYSCUR_ROTATE		2
#define SYSCUR_USCALE		3
#define SYSCUR_NUSCALE		4
#define SYSCUR_SQUASH		5
#define SYSCUR_SELECT		6
#define SYSCUR_DEFARROW		7
#define SYSCUR_MOVE_SNAP	8

// flags to pass to RedrawViews
#define REDRAW_BEGIN		(1<<0)
#define REDRAW_INTERACTIVE	(1<<1)
#define REDRAW_END			(1<<2)
#define REDRAW_NORMAL		(1<<3)

// Return values for GetNumAxis()
#define NUMAXIS_ZERO		0 	// Nothing to transform
#define NUMAXIS_ALL			1	// Use only one axis.
#define NUMAXIS_INDIVIDUAL	2	// Do all, one at a time

/*! \defgroup MAXDirIDs MAX Directory IDs
The following IDs represent a set of application system directories that are
used in various contexts.  The most common usage for these IDs is in the 
IPathConfigMgr::GetDir() and IPathConfigMgr::SetDir(int) methods.
*/
//@{
/*! The ID used to access the default Font files location.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int) 
*/
#define APP_FONT_DIR	 	         0
/*! The ID used to access the default location where Scene files
	are saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int) 
*/
#define APP_SCENE_DIR		      1
/*! The ID used to access the default location where import data
	is browsed from.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_IMPORT_DIR		      2
/*! The ID used to access the default location where export data
	is saved to.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int) 
*/
#define APP_EXPORT_DIR		      3
/*! The ID used to access the system location where help files are stored.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_HELP_DIR		         4
/*! The ID used to access the default location where data files relating
	to the Expression Controller are saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_EXPRESSION_DIR	      5
/*! The ID used to access the default location where preview rendering outputs
	are saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_PREVIEW_DIR		      6
/*! The ID used to access the default location where all non-material images
	used in scenes are browsed from.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_IMAGE_DIR		      7
/*! The ID used to access the default location where all sound files
	used in scenes are browsed from.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_SOUND_DIR		      8
/*! The ID used to access the system location where all plugin configuration
	files are saved and loaded.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_PLUGCFG_DIR		      9
/*! The ID used to access the system location where the application will look
	for default application startup scene.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_MAXSTART_DIR	      10
/*! The ID used to access the default location where all Video Post files
	used in scenes are browsed from.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_VPOST_DIR		      11
/*! The ID used to access the system location where the application will look
	for graphic driver specifc files.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_DRIVERS_DIR		      12
/*! The ID used to access the default location where the autobackup files are saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_AUTOBACK_DIR	      13
/*! The ID used to access the default location where the material library files 
	are loaded and saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_MATLIB_DIR		      14
/*! The ID used to access the system location where scripts are saved.  This location
	is also included in the maxscript search path.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_SCRIPTS_DIR		      15
/*! The ID used to access the system location where startup scripts are saved.  Scripts
	found in this folder will be run when the application starts up. This location
	is also included in the maxscript search path.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_STARTUPSCRIPTS_DIR   16
/*! The ID used to access the system location where a market defaults set is loaded from.  
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_MARKETDEFAULTS_DIR    17
/*! The ID used to access the default location where render preset files 
	are loaded and saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_RENDER_PRESETS_DIR   18

#ifndef RENDER_VER_CONFIG_PATHS // xavier robitaille | 03.01.24 | modify kahn's config. paths
#define APP_DIR_OFFSET			 0

#else // RENDER_VER_CONFIG_PATHS
#define APP_DIR_OFFSET			 1
/*! The ID used to access the default location where render catalog files 
	are loaded and saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_PLUGCFG_CATALOGS_DIR 19

#endif // RENDER_VER_CONFIG_PATHS
/*! The ID used to access the system location where files relating to the 
	User Interface files are read from.  
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_UI_DIR			      (APP_DIR_OFFSET + 19)	
/*! 
	The APP_MAXROOT_DIR ID has been removed and deprecated to force plugins using this 
	value to consider whether it is necessary to refer to the application install path.  
	With Windows Vista, plugins  will no longer have direct write access to the 
	application install path.  Application data and settings needs to be written to a user profile path.

	Plugins should follow the following rules from writing files:
	<ul>
	<li>Plugins that need to write configuration data should rely on APP_PLUGCFG_DIR.
	<li>Plugins that need to write miscellaneous data can use APP_MAXDATA_DIR.  This path is
	pointed to a user profile directory and is safe to write to.  Plugins may
	create subdirectories under this path.
	<li>Plugins that need to write out temporary files should rely on APP_TEMP_DIR.
	<li>Finally, plugins that need to dynamically look up the application install root can use the 
	APP_MAX_SYS_ROOT_DIR value.
	</ul>

	The ID used to access the system location where application executable resides.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_MAX_SYS_ROOT_DIR		(APP_DIR_OFFSET + 20)
#pragma deprecated("APP_MAXROOT_DIR")

/*! The ID used to access the default location where render output files are
	saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_RENDER_OUTPUT_DIR			(APP_DIR_OFFSET + 21)
/*! The ID used to access the default location where animation files are
	loaded and saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_ANIMATION_DIR				(APP_DIR_OFFSET + 22)
/*! The ID used to access the default location where archived scene files are
	saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_ARCHIVES_DIR				(APP_DIR_OFFSET + 23)
/*! The ID used to access the default location where photometric data files are
	loaded.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_PHOTOMETRIC_DIR				(APP_DIR_OFFSET + 24)
/*! The ID used to access the default location where render assets (such as 
	mental ray photon maps, etc.) are loaded and saved.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_RENDER_ASSETS_DIR			(APP_DIR_OFFSET + 25)
/*! The ID used to access the default location where a user can safely write or create
	a script file.  This location is also included in the maxscript search path.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_USER_SCRIPTS_DIR			(APP_DIR_OFFSET + 26)
/*! The ID used to access the default location where a user can safely write or create
	a macroscript file.  This is searched at startup to locate custom macroscript actions.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_USER_MACROS_DIR				(APP_DIR_OFFSET + 27)
/*! The ID used to access the default location where a user can safely write or create
	a script file which is run at startup.  
	Scripts found in this folder will be run when the application starts up. This location
	is also included in the maxscript search path.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_USER_STARTUPSCRIPTS_DIR		(APP_DIR_OFFSET + 28)
/*! The ID used to access the system location where a temp file can safely be written.  Note 
	that the application does no special action to clean up this folder on exit, that it
	is still the responsibility of the developer to remove temporary files.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_TEMP_DIR					(APP_DIR_OFFSET + 29)
/*! The ID used to access the default location where a user can safely create icon files
	that can be used by the Customizable User Interface system.
	This location is included in the search path when the application is attempting to 
	load an icon file by name.
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_USER_ICONS_DIR				(APP_DIR_OFFSET + 30)
/*! The ID used to access the system location where user specific configuration files
	should be written.  This value was introduced due to support Least Privilege Users.  
	\see APP_MAX_SYS_ROOT_DIR
	\see IPathConfigMgr::GetDir() 
	\see IPathConfigMgr::SetDir(int)
*/
#define APP_MAXDATA_DIR					(APP_DIR_OFFSET + 31)
/*! The ID used to access the root folder of the current system project.  Developers
	should prefer to access this value using the methods named below.
	\see IPathConfigMgr::GetCurrentProjectFolder() 
	\see IPathConfigMgr::SetCurrentProjectFolder(const MaxSDK::Util::Path&)
*/
#define APP_PROJECT_FOLDER_DIR		(APP_DIR_OFFSET + 32)
/*! The ID used to access the user defined location where downloaded files should
	be deposited by default.
	\see IPathConfigMgr::GetCurrentProjectFolder() 
	\see IPathConfigMgr::SetCurrentProjectFolder(const MaxSDK::Util::Path&)
*/
#define APP_DOWNLOAD_DIR		(APP_DIR_OFFSET + 33)
/*! The ID used to access the user defined location where proxies (mainly bitmaps)
	are stored.
	\see IPathConfigMgr::GetCurrentProjectFolder() 
	\see IPathConfigMgr::SetCurrentProjectFolder(const MaxSDK::Util::Path&)
*/
#define APP_PROXIES_DIR		(APP_DIR_OFFSET + 34)
//@}
// closes Max Directories group

// Types for status numbers
#define STATUS_UNIVERSE					1
#define STATUS_SCALE					2
#define STATUS_ANGLE					3
#define STATUS_OTHER					4
#define STATUS_UNIVERSE_RELATIVE		5
#define STATUS_POLAR					6
#define STATUS_POLAR_RELATIVE			7


/*! \name Extended display modes */
//@{
//! \brief Flags used for the extended display functions SetExtendedDisplayMode() and GetExtendedDisplayMode().
/*! These are flags used in the class Interface methods SetExtendedDisplayMode() and GetExtendedDisplayMode() for catching various display/selection/hittesting events
*/
#define EXT_DISP_NONE				0			//!< no flags
#define EXT_DISP_SELECTED			(1<<0)		//!< object is selected
#define EXT_DISP_TARGET_SELECTED	(1<<1)		//!< object's target is selected
#define EXT_DISP_LOOKAT_SELECTED	(1<<2)		//!< object's lookat node is selected
#define EXT_DISP_ONLY_SELECTED		(1<<3)		//!< object is only thing selected
#define EXT_DISP_DRAGGING			(1<<4)		//!< object is being "dragged"
#define EXT_DISP_ZOOM_EXT			(1<<5)		//!< object is being tested for zoom ext
#define EXT_DISP_GROUP_EXT			(1<<6)		//!< object is being tested for extents as member of group
#define EXT_DISP_ZOOMSEL_EXT		(1<<7)		//!< object is being tested for zoom selected ext
//@}

// Render time types passed to SetRendTimeType()
#define REND_TIMESINGLE		0
#define REND_TIMESEGMENT	1
#define REND_TIMERANGE		2
#define REND_TIMEPICKUP		3

// Flag bits for hide by category.
#define HIDE_OBJECTS	0x0001
#define HIDE_SHAPES		0x0002
#define HIDE_LIGHTS		0x0004
#define HIDE_CAMERAS	0x0008
#define HIDE_HELPERS	0x0010
#define HIDE_WSMS		0x0020
#define HIDE_SYSTEMS	0x0040
#define HIDE_PARTICLES	0x0080
#define HIDE_BONEOBJECTS 0x0100
#define HIDE_ALL		0xffff
#define HIDE_NONE		0



// viewport layout configuration
//   VP_LAYOUT_ LEGEND
//		# is number of viewports (total) in view panel
//		V = vertical split
//		H = horizontal split
//		L/R	= left/right placement
//		T/B = top/bottom placement
//   CONSTANT LEGEND
//		bottom nibble is total number of views
#define VP_LAYOUT_1			0x0001
#define VP_LAYOUT_2V		0x0012
#define VP_LAYOUT_2H		0x0022
#define VP_LAYOUT_2HT		0x0032
#define VP_LAYOUT_2HB		0x0042
#define VP_LAYOUT_3VL		0x0033
#define VP_LAYOUT_3VR		0x0043
#define VP_LAYOUT_3HT		0x0053
#define VP_LAYOUT_3HB		0x0063
#define VP_LAYOUT_4			0x0074
#define VP_LAYOUT_4VL		0x0084
#define VP_LAYOUT_4VR		0x0094
#define VP_LAYOUT_4HT		0x00a4
#define VP_LAYOUT_4HB		0x00b4
#define VP_LAYOUT_1C		0x00c1
#define VP_NUM_VIEWS_MASK	0x000f


// Node attribute flags used by Interface7::SetNodeAttribute
#define ATTRIB_HIDE									0
#define	ATTRIB_FREEZE								1
#define	ATTRIB_BOXMODE							2
#define	ATTRIB_BACKCULL							3
#define	ATTRIB_ALLEDGES							4
#define ATTRIB_LINKDISP							5
#define ATTRIB_LINKREPL							6
#define ATTRIB_UNSEL								7		// for internal use only; something is not selected
#define ATTRIB_HIDE_UNSEL						8		// for internal use only
#define ATTRIB_FREEZE_UNSEL					9		// for internal use only
#define ATTRIB_VERTTICKS						10
#define ATTRIB_UNHIDE								11	// for internal use only
#define ATTRIB_UNFREEZE							12	// for internal use only
#define ATTRIB_CVERTS								13
#define ATTRIB_SHADE_CVERTS					14
#define ATTRIB_XRAY									15
#define ATTRIB_IGNORE_EXT						16
#define ATTRIB_TRAJECTORY						17 // valid if DESIGN_VER
#define ATTRIB_FRZMTL								18
#define ATTRIB_HIDE_WITH_LAYER			19 // used by MXS - also unhides layer if hidden
#define ATTRIB_FREEZE_WITH_LAYER		20 // used by MXS - also unhides layer if hidden
#define ATTRIB_INHERITVISIBILITY		100
#define ATTRIB_CASTSHADOWS					101
#define ATTRIB_RECEIVESHADOWS				102
#define ATTRIB_ISTARGET							103
#define ATTRIB_RENDERABLE						104
#define ATTRIB_RENDEROCCLUDED				105
#define ATTRIB_RCVCAUSTICS					106
#define ATTRIB_GENERATECAUSTICS			107
#define ATTRIB_RCVGLOBALILLUM				108
#define ATTRIB_GENERATEGLOBALILLUM	109
#define ATTRIB_SETWIRECOLOR					110
#define ATTRIB_SETGBUFID						111
#define ATTRIB_PRIMARYVISIBILITY		112
#define ATTRIB_SECONDARYVISIBILITY	113
#define ATTRIB_MOTIONBLUR						114


class DWORDTab : public Tab<DWORD> {};


// A callback object passed to Execute
#define  I_EXEC_REGISTER_POSTSAVE_CB  1001
#define  I_EXEC_UNREGISTER_POSTSAVE_CB  1002
#define  I_EXEC_REGISTER_PRESAVE_CB  1003
#define  I_EXEC_UNREGISTER_PRESAVE_CB  1004

class GenericCallback: public MaxHeapOperators {
 public:
  virtual void Callme()=0;
 };

// A callback object passed to RegisterTimeChangeCallback()
class TimeChangeCallback: public MaxHeapOperators {
	public:
		virtual void TimeChanged(TimeValue t)=0;
	};


// A callback object passed to RegisterCommandModeChangeCallback()
class CommandModeChangedCallback: public MaxHeapOperators {
	public:
		virtual void ModeChanged(CommandMode *oldM, CommandMode *newM)=0;
	};

// A callback to allow plug-ins that aren't actually objects (such as utilities)
// to draw something in the viewports.
class ViewportDisplayCallback: public MaxHeapOperators {
	public:
		virtual void Display(TimeValue t, ViewExp *vpt, int flags)=0;		
		virtual void GetViewportRect( TimeValue t, ViewExp *vpt, Rect *rect )=0;
		virtual BOOL Foreground()=0; // return TRUE if the object changes a lot or FALSE if it doesn't change much		
	};

//! \brief A callback object that will get called before the program exits.
/*! Register an instance of this class with Interface::RegisterExitMAXCallback. 
During normal shutdowns, the Exit method will be called. The Exit method is not called 
during shutdown after crash recovery.
\see ExitMAXCallback2
*/
class ExitMAXCallback: public MaxHeapOperators {
	public:
		//! Method called on normal shutdown
		//! \param[in] hWnd - MAX main window handle is passed in. 
		//! \return FALSE to abort the exit, TRUE otherwise.
		virtual BOOL Exit(HWND hWnd)=0;
	};

//! \brief A callback object that will get called before the program exits.
/*! Register an instance of this class with Interface8::RegisterExitMAXCallback. 
During normal shutdowns, the ExitMAXCallback::Exit method will be called. During shutdown after crash recovery,
the CrashRecoverExit method will be called. Since it is being called after crash recovery, the 
scene may or may not be in a stable state, do only absolutely necessary actions here.
*/
class ExitMAXCallback2 : public ExitMAXCallback {
public:
	//! Method called on shutdown after crash recovery.
    /*! \param [in] hWnd - MAX main window handle is passed in.
	*/
	virtual void CrashRecoverExit(HWND hWnd)=0;
};

//! \brief A callback object called for each sequence file created with Interface8::CreateRendImgSeq()
/*! Pass an instance of this class to Interface8::CreateRendImgSeq().
For each sequence file created, the callback method will be called.  The method can modify
the name of the sequence file or return false to prevent creation of the file.
*/
class CreateRendImgSeqCallback: public MaxHeapOperators {
public:
	//! Method called for each sequence file to create
    /*! \param[in, out] filename - Full path and name of the sequence file; may be modified by the callee
        \param[in] objType - 0 for the main render output, or 1 for render elements
        \param[in] objPtr - NULL for the main render output, or IRenderElement* for render elements
	    \return Return FALSE to prevent creation of the sequence file, TRUE otherwise. */
	virtual BOOL CreateRendImgSeq( MSTR& filename, int objType, void* objPtr )=0;
};

class MAXFileOpenDialog: public MaxHeapOperators {
	public:
	virtual BOOL BrowseMAXFileOpen(MSTR& fileName, MSTR* defDir, MSTR* defFile) = 0;
	};

class MAXFileSaveDialog: public MaxHeapOperators {
	public:
	virtual BOOL BrowseMAXFileSave(MSTR& fileName) = 0;
	};

// A callback object to filter selection in the track view.
class TrackViewFilter : public InterfaceServer{
	public:
		// Return TRUE to accept the anim as selectable.
		virtual BOOL proc(Animatable *anim, Animatable *client, int subNum)=0;
		virtual BOOL TextColor(Animatable *anim, Animatable *client, int subNum, COLORREF& color) { return FALSE; }
	};

// Stores the result of a track view pick
class TrackViewPick: public MaxHeapOperators {
	public:
		ReferenceTarget *anim;
		ReferenceTarget *client;
		int subNum;

		TrackViewPick() {anim=NULL;client=NULL;subNum=0;}
	};

// A callback object passed to SetPickMode()
class PickModeCallback : public InterfaceServer {
	public:
		// Called when ever the pick mode needs to hit test. Return TRUE if something was hit
		virtual BOOL HitTest(IObjParam *ip,HWND hWnd,ViewExp *vpt,IPoint2 m,int flags)=0;
		
		// Called when the user picks something. The vpt should have the result of the hit test in it.
		// return TRUE to end the pick mode.
		virtual BOOL Pick(IObjParam *ip,ViewExp *vpt)=0;

		//Called when a node or controller is picked from Trackview or Schematic view
		//return TRUE if the Pick Mode callback accepts the animatable object.
		//the track will not be selected in trackview if the this function returns FALSE
		virtual BOOL PickAnimatable(Animatable* anim) { return TRUE; }

		// Called when the user right-clicks or presses ESC
		// return TRUE to end the pick mode, FALSE to continue picking
		virtual BOOL RightClick(IObjParam *ip,ViewExp *vpt)	{ return FALSE; }

		// Called when the mode is entered and exited.
		virtual void EnterMode(IObjParam *ip) {}
		virtual void ExitMode(IObjParam *ip) {}

		virtual HCURSOR GetDefCursor(IObjParam *ip) {return NULL;}
		virtual HCURSOR GetHitCursor(IObjParam *ip) {return NULL;}

		// If the user hits the H key while in your pick mode, you
		// can provide a filter to filter the name list.
		virtual PickNodeCallback *GetFilter() {return NULL;}

		// Return TRUE to allow the user to pick more than one thing.
		// In this case the Pick method may be called more than once.
		virtual BOOL AllowMultiSelect() {return FALSE;}
	};

// Not to be confused with a PickMODEcallback...
// Used to filter node's during a hit test (PickNode)
class PickNodeCallback: public MaxHeapOperators {
	public:
		// Return TRUE if this is an acceptable hit, FALSE otherwise.
		virtual BOOL Filter(INode *node)=0;
	};

// Used with DoHitByNameDialog();
class HitByNameDlgCallback: public MaxHeapOperators {
public:
	virtual MCHAR *dialogTitle()	{ return _M(""); }
	virtual MCHAR *buttonText() 	{ return _M(""); }
	virtual BOOL singleSelect()		{ return FALSE; }
	virtual BOOL useFilter()		{ return TRUE; }
	virtual int filter(INode *node)	{ return TRUE; }
	virtual BOOL useProc()			{ return TRUE; }
	virtual void proc(INodeTab &nodeTab) {}
	virtual BOOL doCustomHilite()	{ return FALSE; }
	virtual BOOL doHilite(INode *node)	{ return FALSE; }
	virtual BOOL showHiddenAndFrozen()	{ return FALSE; }
};


//watje new selection filters
//this is a call back for the selection filter drop down in the tab panel
//it allows plugins and maxscripts to add additional filters to this list
class SelectFilterCallback : public InterfaceServer
{
public:
	virtual MCHAR* GetName() = 0;  // the name that will appear in the drop down list when it is registered
	//the actual proc that does the filtering
	// the node, class id, and super class id are passed as parameter to check the filter again
	//return FALSE if the node is not selectable, true if it is selectable
	virtual BOOL IsFiltered(SClass_ID sid, Class_ID cid, INode *node) = 0;
};


//watje new display filters
//this is a call back for the display filter list in the display command panel
//it allows plugins and maxscripts to add additional filters to this list
class DisplayFilterCallback : public InterfaceServer
{
public:
	BOOL on;
	virtual MCHAR* GetName() = 0;  // the name that will appear in the drop down list when it is registered
	//the actual proc that does the filtering
	// the node, class id, and super class id are passed as parameter to check the filter again
	//return FALSE if the node is not selectable, true if it is selectable
	virtual BOOL IsVisible(SClass_ID sid, Class_ID cid, INode *node) = 0;
};


class Interface;

// A callback used with RegisterRedrawViewsCallback()
class RedrawViewsCallback: public MaxHeapOperators {
	public:
		// this will be called after all the viewport have completed drawing.
		virtual void proc(Interface *ip)=0;
	};

// A callback used with RegisterAxisChangeCallback()
class AxisChangeCallback: public MaxHeapOperators {
	public:
		// this will be called when the axis system is changed
		virtual void proc(Interface *ip)=0;
	};

// parameters for creation of a preview
class PreviewParams: public MaxHeapOperators {
public:
	BOOL	outputType;	// 0=default AVI codec, 1=user picks file, 2=user picks device
	int		pctSize;	// percent (0-100) of current rendering output res
	// frame limits
	int		start;
	int		end;
	int		skip;
	// frame rate
	int		fps;
	// display control
	BOOL	dspGeometry;
	BOOL	dspShapes;
	BOOL	dspLights;
	BOOL	dspCameras;
	BOOL	dspHelpers;
	BOOL	dspSpaceWarps;
	BOOL	dspGrid;
	BOOL	dspSafeFrame;
	BOOL	dspFrameNums;
	// rendering level
	int		rndLevel;	// 0=smooth/hi, 1=smooth, 2=facet/hi, 3=facet
						// 4=lit wire, 6=wire, 7=box
	int		dspBkg;
};

// Viewport transparency rendering modes
#define VPT_TRANS_NONE			0
#define VPT_TRANS_STIPPLE		1
#define VPT_TRANS_BLEND			2
#define VPT_TRANS_SORT_BLEND	3		

// Scene Display Flags	
#define DISPLAY_WIRE_AS_MTL      1
#define DISPLAY_SHADED_AS_MTL (1<<1)
#define DISPLAY_SELECTED_ONLY (1<<2)

// flag values for TrackViewPickMultiDlg and TrackViewPickDlg
#define PICKTRACK_FLAG_ANIMATED			(1<<0)  /*-- turn on display of animated tracks only--*/
#define PICKTRACK_FLAG_VISTRACKS		(1<<1)  /*-- turn off display of node Visibility track--*/
#define PICKTRACK_FLAG_SELOBJECTS		(1<<2)  /*-- turn on display of only selected nodes--*/
#define PICKTRACK_FLAG_WORLDMODS		(1<<3)  /*-- turn off display of World Space Modifiers--*/
#define PICKTRACK_FLAG_OBJECTMODS		(1<<4)  /*-- turn off display of Object Space Modifiers--*/
#define PICKTRACK_FLAG_TRANSFORM		(1<<5)  /*-- turn off display of node Transform track--*/
#define PICKTRACK_FLAG_BASEPARAMS		(1<<6)  /*-- turn off display of node base object--*/
#define PICKTRACK_FLAG_CONTTYPES		(1<<7)  /*-- turn off display of controller types--*/
#define PICKTRACK_FLAG_NOTETRACKS		(1<<8)  /*-- turn off display of note tracks--*/
#define PICKTRACK_FLAG_SOUND			(1<<9)  /*-- turn off display of the sound track--*/
#define PICKTRACK_FLAG_MATMAPS			(1<<10) /*-- turn off display of maps in materials--*/
#define PICKTRACK_FLAG_MATPARAMS		(1<<11) /*-- turn off display of material parameters--*/
#define PICKTRACK_FLAG_VISIBLE_OBJS		(1<<12) /*-- turn on display of hidden nodes--*/
#define PICKTRACK_FLAG_HIERARCHY		(1<<13) /*-- turn off display of hierarchy--*/
#define PICKTRACK_FLAG_KEYABLE			(1<<14) /*-- turn off display of non-keyable tracks--*/
#define PICKTRACK_FLAG_NODES			(1<<15) /*-- turn off display of nodes--*/
#define PICKTRACK_FLAG_GEOM				(1<<16) /*-- turn off display of geometry nodes--*/
#define PICKTRACK_FLAG_SHAPES			(1<<17) /*-- turn off display of shape nodes--*/
#define PICKTRACK_FLAG_LIGHTS			(1<<18) /*-- turn off display of light nodes--*/
#define PICKTRACK_FLAG_CAMERAS			(1<<19) /*-- turn off display of camera nodes--*/
#define PICKTRACK_FLAG_HELPERS			(1<<20) /*-- turn off display of helper nodes--*/
#define PICKTRACK_FLAG_WARPS			(1<<21) /*-- turn off display of warp nodes--*/
#define PICKTRACK_FLAG_POSITION			(1<<22) /*-- turn off display of position controllers--*/
#define PICKTRACK_FLAG_ROTATION			(1<<23) /*-- turn off display of rotation controllers--*/
#define PICKTRACK_FLAG_SCALE			(1<<24) /*-- turn off display of scale controllers--*/
#define PICKTRACK_FLAG_BONES			(1<<25) /*-- turn off display of bone nodes--*/
#define PICKTRACK_FLAG_FOCUS_SEL_NODES	(1<<26) /*-- set focus to first selected node found--*/
//! Only show the active animation layer, instead of all.
#define PICKTRACK_FLAG_ACTIVELAYER		(1<<27) /*-- show only the active animation layer--*/

#define MAIN_MAX_INTERFACE Interface_ID(0x64854123, 0x7b9e551c)

// Generic interface into Max
class Interface : public FPStaticInterface {
	public:
		virtual HFONT GetAppHFont()=0;
		virtual void RedrawViews(TimeValue t,DWORD vpFlags=REDRAW_NORMAL, ReferenceTarget *change=NULL)=0;		
		virtual BOOL SetActiveViewport(HWND hwnd)=0;
		virtual ViewExp *GetActiveViewport()=0; // remember to release ViewExp* with ReleaseViewport()
		virtual IObjCreate *GetIObjCreate()=0;
		virtual IObjParam *GetIObjParam()=0;
		virtual HWND GetMAXHWnd()=0;

		// This will cause all viewports to be completely redrawn.
		// This is extremely heavy handed and should only be used when
		// necessary.
		virtual void ForceCompleteRedraw(BOOL doDisabled=TRUE)=0;

		// This will draw the active view (if it is a camera view with a multi-pass effect
		// enabled) with that effect active.  Returns TRUE if the active view is a camera view 
		// with a multi-pass effect enabled, FALSE otherwise
		virtual BOOL DisplayActiveCameraViewWithMultiPassEffect() = 0;

		// Determines the way transparency will be displayed in the active viewport, based on transType setting
		//		VPT_TRANS_NONE => all objects are always opaque
		//		VPT_TRANS_STIPPLE => use screendoor transparency
		//		VPT_TRANS_BLEND => use hardware blending, based on drawing transparent objects last
		virtual BOOL SetActiveViewportTransparencyDisplay(int transType) = 0;

		// Turn scene redraw on/off.  Each call to disable redraw should be
		// matched by a call to enable it, since this is implemented with an
		// internal counter.  This can be used to stop scene redraws from 
		// happening during rendering, for example.
		virtual void DisableSceneRedraw()=0;
		virtual void EnableSceneRedraw()=0;
		virtual int	 IsSceneRedrawDisabled()=0;	// returns non-zero if disabled

		// Register a call back object that gets called evrytime
		// the viewports are redrawn.
		virtual void RegisterRedrawViewsCallback(RedrawViewsCallback *cb)=0;
		virtual void UnRegisterRedrawViewsCallback(RedrawViewsCallback *cb)=0;
//watje new selection filters
//these work just like all the other register callback function
//except they are used to register filter selection call backs
		virtual void RegisterSelectFilterCallback(SelectFilterCallback *cb)=0;
		virtual void UnRegisterSelectFilterCallback(SelectFilterCallback *cb)=0;

//watje new display filters
//these work just like all the other register callback function
//except they are used to register display selection call backs
		virtual void RegisterDisplayFilterCallback(DisplayFilterCallback *cb)=0;
		virtual void UnRegisterDisplayFilterCallback(DisplayFilterCallback *cb)=0;

		// set the selection type to automatic window/crossing (based on dir, as set by 2nd arg), 
		// or to manual window/crossing (based on second arg)
		//
		// if autoWinCross is TRUE, then Window/Crossing is chosen by direction
		//		if winCrossOrAutoDir is set to AC_DIR_RL_CROSS, then right-to-left implies crossing
		//		if winCrossOrAutoDir is set to AC_DIR_LR_CROSS, then left-to-right implies crossing
		// if autoWinCross is FALSE, then Window/Crossing is set explicitly, as follows:
		//		if winCrossOrAutoDir is TRUE, then you get crossing selection
		//		if winCrossOrAutoDir is FALSE, then you get window selection
		virtual BOOL SetSelectionType(BOOL autoWinCross, int winCrossOrAutoDir) = 0;

		// For use with extended views: 
		// - make the extended viewport active (set on mouse click, for example)
		// - put up the view type popup menu (put up on right-click, for example)
		virtual void MakeExtendedViewportActive(HWND hWnd)=0;
		virtual void PutUpViewMenu(HWND hWnd, POINT pt)=0;

		// Execute a track view pick dialog.
		virtual BOOL TrackViewPickDlg(HWND hParent, TrackViewPick *res, TrackViewFilter *filter=NULL, DWORD flags=0)=0;
//watje
		virtual BOOL TrackViewPickMultiDlg(HWND hParent, Tab<TrackViewPick> *res, TrackViewFilter *filter=NULL,DWORD flags=0)=0;

		// Command mode methods
		virtual void PushCommandMode( CommandMode *m )=0;
		virtual void SetCommandMode( CommandMode *m )=0;
		virtual void PopCommandMode()=0;		
		virtual CommandMode* GetCommandMode()=0;
		virtual void SetStdCommandMode( int cid )=0;
		virtual void PushStdCommandMode( int cid )=0;		
		virtual void RemoveMode( CommandMode *m )=0;
		virtual void DeleteMode( CommandMode *m )=0;

		// This will set the command mode to a standard pick mode.
		// The callback implements hit testing and a method that is
		// called when the user actually pick an item.
		virtual void SetPickMode(PickModeCallback *pc)=0;
		
		// makes sure no pick modes are in the command stack.
		virtual void ClearPickMode()=0;

		//added by AF (08/18/2000)
		//if a Pick Mode is on top of the command stack this function will return it 
		//otherwise it returns NULL
		virtual PickModeCallback* GetCurPickMode()=0;						

		// Puts up a hit by name dialog. If the callback is NULL it 
		// just does a standard select by name.
		// returns TRUE if the user OKs the dialog, FALSE otherwise.
		virtual BOOL DoHitByNameDialog(HitByNameDlgCallback *hbncb=NULL)=0;

		// status panel prompt stuff
		virtual void PushPrompt( MCHAR *s )=0;
		virtual void PopPrompt()=0;
		virtual void ReplacePrompt( MCHAR *s )=0;
		virtual void DisplayTempPrompt( MCHAR *s, int msec=1000)=0;
		virtual void RemoveTempPrompt()=0;

		// put up a directory choose dialog
		// hWnd = parent
		// title is dialog box title
		// dir is return value for chosen dir (empty on cancel)
		// desc, if non-null, puts up a description field and returns new desc.
		virtual void ChooseDirectory(HWND hWnd, MCHAR *title, MCHAR *dir, MCHAR *desc=NULL)=0;

		// auto-backup control -- times are in minutes
		virtual float GetAutoBackupTime()=0;
		virtual void SetAutoBackupTime(float minutes)=0;
		virtual BOOL AutoBackupEnabled()=0;
		virtual void EnableAutoBackup(BOOL onOff)=0;

		// status panel progress bar
		virtual BOOL ProgressStart(MCHAR *title, BOOL dispBar, LPTHREAD_START_ROUTINE fn, LPVOID arg)=0;
		virtual void ProgressUpdate(int pct, BOOL showPct = TRUE, MCHAR *title = NULL)=0;
		virtual void ProgressEnd()=0;
		virtual BOOL GetCancel()=0;
		virtual void SetCancel(BOOL sw)=0;

		// create preview from active view.
		// If pvp is NULL, this uses the parameters from the preview rendering dialog box.
		virtual void CreatePreview(PreviewParams *pvp=NULL)=0;
		
		// Some info about the current grid settings
		virtual float GetGridSpacing()=0;
		virtual int GetGridMajorLines()=0;

		// Write values to x,y,z status boxes. Before doing this, mouse
		// tracking must be disabled. Typically a plug-in would disable
		// mouse tracking on mouse down and enable it on mouse up.		
		virtual void DisableStatusXYZ()=0;
		virtual void EnableStatusXYZ()=0;
		virtual void SetStatusXYZ(Point3 xyz,int type)=0;
		virtual void SetStatusXYZ(AngAxis aa)=0; // this will convert the aa for status display

		// Extended display modes (such as camera cones that only appear when dragging a camera)
		virtual void SetExtendedDisplayMode(int flags)=0;
		virtual int GetExtendedDisplayMode()=0;

		// UI flyoff timing
		virtual void SetFlyOffTime(int msecs)=0;
		virtual int  GetFlyOffTime()=0;

		// Get standard Max cursors.
		virtual HCURSOR GetSysCursor( int id )=0;

		// Turn on or off a cross hair cursor which draws horizontal and vertical
		// lines the size of the viewport's width and height and intersect at
		// the mouse position.
		virtual void SetCrossHairCur(BOOL onOff)=0;
		virtual BOOL GetCrossHairCur()=0;

		// This pops all modes above the create or modify mode.
		// NOTE: This is obsolete with the new modifiy panel design.
		virtual void RealizeParamPanel()=0;

		// Snap an angle value (in radians)
		virtual float SnapAngle(float angleIn, BOOL fastSnap=TRUE, BOOL forceSnap=FALSE)=0;

		// Snap a percentage value (1.0 = 100%)
		virtual float SnapPercent(float percentIn)=0;

		// Get the snap switch state
		virtual BOOL GetSnapState()=0;

		// Get the snap type -- Absolute or Relative (grid.h)
		virtual int GetSnapMode()=0;

		// Set the snap mode -- Set to absolute will fail if not in screen space
		// Returns TRUE if succeeded
		virtual BOOL SetSnapMode(int mode)=0;

		// Hit tests the screen position for nodes and returns a 
		// INode pointer if one is hit, NULL otherwise.
		virtual INode *PickNode(HWND hWnd,IPoint2 pt,PickNodeCallback *filt=NULL)=0;

		// Region hit testing. To access the result, use the ViewExp funtions
		// GetClosestHit() or GetHit().		
		virtual void BoxPickNode(ViewExp *vpt,IPoint2 *pt,BOOL crossing,PickNodeCallback *filt=NULL)=0;
		virtual void CirclePickNode(ViewExp *vpt,IPoint2 *pt,BOOL crossing,PickNodeCallback *filt=NULL)=0;
		virtual void FencePickNode(ViewExp *vpt,IPoint2 *pt,BOOL crossing,PickNodeCallback *filt=NULL)=0;

		//----- Modify-related Methods--------------------------

		// Registers the sub-object types for a given plug-in object type.

		// This method is obsolete for the modifiy panel from 3DS MAX 4 on. 
		// All subobject types have to be registered through the new methods 
		// BaseObject::NumSubObjTypes() and BaseObject::GetSubObjType(int i). 
		// This method is still working for motion panel subobject types.
		// Please check the SDK help file for more information.

		virtual void RegisterSubObjectTypes( const MCHAR **types, int count,
                                             int startIndex = 0)=0;

		// Add sub-object named selection sets the named selection set drop down.
		// This should be done whenever the selection level changes.
		virtual void AppendSubObjectNamedSelSet(const MCHAR *set)=0;

		// Clear the named selections from the drop down.
		virtual void ClearSubObjectNamedSelSets()=0;

		// Clears the edit field of the named selection set drop down
		virtual void ClearCurNamedSelSet()=0;

		// Sets the edit field of the named selection set drop down
		virtual void SetCurNamedSelSet(MCHAR *setName)=0;

		// new for V2... tell the system that the named sets have changed at
		// that the drop down needs to be rebuilt.
		virtual void NamedSelSetListChanged()=0;

		// Returns the state of the sub object drop-down. 0 is object level
		// and >= 1 refer to the levels registered by the object.
		virtual int GetSubObjectLevel()=0;
		
		// Sets the sub-object drop down. This will cause the object being edited
		// to receive a notification that the current subobject level has changed.
        // if force == TRUE, the it will set the level even if the current
        // level is the same as the level requested.  This is to support
        // objects that change sub-object levels on the fly, like NURBS
		virtual void SetSubObjectLevel(int level, BOOL force = FALSE)=0;

		// Returns the number of entries in the sub-object drop down list.
		virtual int GetNumSubObjectLevels()=0;

		// Enables or disables sub object selection. Note that it
		// will already be disabled if there are no subobject levels
		// registered. In this case, it can not be enabled.
		virtual void EnableSubObjectSelection(BOOL enable)=0;
		virtual BOOL IsSubObjectSelectionEnabled()=0;

		// Notifies the system that the selection level in the pipeline has chaned.
		virtual void PipeSelLevelChanged()=0;

		// Returns the sub-object selection level at the point in the
		// pipeline  just before the current place in the history.
		virtual void GetPipelineSubObjLevel(DWORDTab &levels)=0;

		// Get's all instance contexts for the modifier at the current
		// place in the history.
		virtual void GetModContexts(ModContextList& list, INodeTab& nodes)=0;

		// Get the object (or modifier) that is currently being edited in the
		// modifier panel
		virtual BaseObject* GetCurEditObject()=0;

		// Hit tests the object currently being edited at the sub object level.
		virtual int SubObHitTest(TimeValue t, int type, int crossing, 
			int flags, IPoint2 *p, ViewExp *vpt)=0;

		// Is the selection set frozen?
		virtual BOOL SelectionFrozen()=0;
		virtual void FreezeSelection()=0;
		virtual void ThawSelection()=0;

		// Nodes in the current selection set.
		virtual INode *GetSelNode(int i)=0;
		virtual int GetSelNodeCount()=0;

		// Enable/disable, get/set show end result. 
		virtual void EnableShowEndResult(BOOL enabled)=0;
		virtual BOOL GetShowEndResult ()=0;
		virtual void SetShowEndResult (BOOL show)=0;

		// Returns the state of the 'crossing' preference for hit testing.
		virtual BOOL GetCrossing()=0;

		// Sets the state of one of the transform tool buttons.
		// TRUE indecates pressed, FALSE is not pressed.
		virtual void SetToolButtonState(int button, BOOL state )=0;
		virtual BOOL GetToolButtonState(int button)=0;
		virtual void EnableToolButton(int button, BOOL enable=TRUE )=0;

        // Enable and disable Undo/Redo.
        virtual void EnableUndo(BOOL enable)=0;

		// Get and set the command panel task mode
		virtual int GetCommandPanelTaskMode()=0;
		virtual void SetCommandPanelTaskMode(int mode)=0;

		// Finds the vpt given the HWND
		virtual ViewExp *GetViewport( HWND hwnd )=0;		
		virtual void ReleaseViewport( ViewExp *vpt )=0;		

		// Disables/Enables animate button
		virtual void EnableAnimateButton(BOOL enable)=0;
		virtual BOOL IsAnimateEnabled()=0;

		// Turns the animate button on or off
		virtual void SetAnimateButtonState(BOOL onOff)=0;

		// In some simpulation task, a time sequence of matrices are computed
		// and to be set to node via
		// INode::SetNodeTM(TimeValue t, Matrix3& tm). If the node
		// employs the standard PRS controller and the rotation is the
		// standard Euler XYZ controller, it would achieve better animation
		// result if we decides euler angles at this frame based on those at
		// the previous frame. The Progressive Mode tells the Euler
		// controller to derive angles based on the previous frame.
		// It is assumed that in this mode, SetNodeTM() are called
		// in strictly forward time order.
		// Synoposis:
		//   GetCOREInterface()->BeginProgressiveMode();
		//   for (t = start_time, t < end_time; t += time_per_frame) {
		//      node.SetNodeTM(t, tm_at_t);
		//   }
		//   GetCOREInterface()->EndProgressiveMode();
		//
		virtual bool InProgressiveMode() =0;
		virtual void BeginProgressiveMode() =0;
		virtual void EndProgressiveMode() =0;

		// Registers a callback that gets called whenever the axis
		// system is changed.
		virtual void RegisterAxisChangeCallback(AxisChangeCallback *cb)=0;
		virtual void UnRegisterAxisChangeCallback(AxisChangeCallback *cb)=0;
		 
		// Gets/Sets the state of the axis constraints.
		virtual int GetAxisConstraints()=0;
		virtual void SetAxisConstraints(int c)=0;
		virtual void EnableAxisConstraints(int c,BOOL enabled)=0;
		// An axis constraint stack
		virtual void PushAxisConstraints(int c) = 0;
		virtual void PopAxisConstraints() = 0;

		// Gets/Sets the state of the coordinate system center
		virtual int GetCoordCenter()=0;
		virtual void SetCoordCenter(int c)=0;
		virtual void EnableCoordCenter(BOOL enabled)=0;

		// Gets/Sets the reference coordinate systems
		virtual int GetRefCoordSys()=0;
		virtual void SetRefCoordSys(int c)=0;
		virtual void EnableRefCoordSys(BOOL enabled)=0;

	// watje Gets/Sets the selection filter
		virtual int GetSelectFilter()=0;
		virtual void SetSelectFilter(int c)=0;
		virtual int GetNumberSelectFilters()=0;
		virtual MCHAR* GetSelectFilterName(int index)=0;

		virtual BOOL GetDisplayFilter(int index) = 0;
		virtual void SetDisplayFilter(int index, BOOL on) = 0;
		virtual int GetNumberDisplayFilters() = 0;
		virtual BOOL DisplayFilterIsNodeVisible(int index, int sid, Class_ID cid, INode *node) = 0;
		virtual MCHAR* GetDisplayFilterName(int index)=0;



		// Gets the axis which define the space in which transforms should
		// take place. 
		// The node and subIndex refer to the object and sub object which the axis
		// system should be based on (this should be the thing the user clicked on)
		// If 'local' is not NULL, it will be set to TRUE if the center of the axis
		// is the pivot point of the node, FALSE otherwise.
		virtual Matrix3 GetTransformAxis(INode *node,int subIndex,BOOL* local = NULL)=0;

		// This returns the number of axis tripods in the scene. When transforming
		// multiple sub-objects, in some cases each sub-object is transformed in
		// a different space.
		// Return Values:
		// NUMAXIS_ZERO			- Nothing to transform
		// NUMAXIS_ALL			- Use only one axis.
		// NUMAXIS_INDIVIDUAL	- Do all, one at a time
		virtual int GetNumAxis()=0;

		// Locks axis tripods so that they will not be updated.
		virtual void LockAxisTripods(BOOL onOff)=0;
		virtual BOOL AxisTripodLocked()=0;

		// Registers a dialog window so IsDlgMesage() gets called for it.
		virtual void RegisterDlgWnd( HWND hDlg )=0;
		virtual int UnRegisterDlgWnd( HWND hDlg )=0;

		// Registers a keyboard accelerator table
        // These functions are obsolete.  Use the AcceleratorTable
        // funciton below to get tables that use the keyboard prefs dialog
		virtual void RegisterAccelTable( HWND hWnd, HACCEL hAccel )=0;
		virtual int UnRegisterAccelTable( HWND hWnd, HACCEL hAccel )=0;

        // The action manager is used to manage ActionTables which
        // plug-ins can use to export operations that can be tied to UI
        // elements like keyboard shortcuts, menus and toolbars.
        virtual IActionManager* GetActionManager() = 0;

        // Returns a pointer to the manager for cusomizable menus
        // see iMenuMan.h for a description of the menu manager
        virtual IMenuManager* GetMenuManager() = 0;
    
		// Adds rollup pages to the command panel. Returns the window
		// handle of the dialog that makes up the page.
		virtual HWND AddRollupPage( HINSTANCE hInst, MCHAR *dlgTemplate, 
				DLGPROC dlgProc, MCHAR *title, LPARAM param=0,DWORD flags=0, int category = ROLLUP_CAT_STANDARD )=0;

		virtual HWND AddRollupPage( HINSTANCE hInst, DLGTEMPLATE *dlgTemplate, 
				DLGPROC dlgProc, MCHAR *title, LPARAM param=0,DWORD flags=0, int category = ROLLUP_CAT_STANDARD )=0;

		// Removes a rollup page and destroys it.
		virtual void DeleteRollupPage( HWND hRollup )=0;

		// Replaces existing rollup with another. (and deletes the old one)
		virtual HWND ReplaceRollupPage( HWND hOldRollup, HINSTANCE hInst, MCHAR *dlgTemplate, 
						DLGPROC dlgProc, MCHAR *title, LPARAM param=0,DWORD flags=0, int category = ROLLUP_CAT_STANDARD )=0;
		
		virtual HWND ReplaceRollupPage( HWND hOldRollup, HINSTANCE hInst, DLGTEMPLATE *dlgTemplate, 
						DLGPROC dlgProc, MCHAR *title, LPARAM param=0,DWORD flags=0, int category = ROLLUP_CAT_STANDARD )=0;
		
		// Gets a rollup window interface to the command panel rollup
		virtual IRollupWindow *GetCommandPanelRollup()=0;

		// When the user mouses down in dead area, the plug-in should pass
		// mouse messages to this function which will pass them on to the rollup.
		virtual void RollupMouseMessage( HWND hDlg, UINT message, 
				WPARAM wParam, LPARAM lParam )=0;

		// get/set the current time.
		virtual TimeValue GetTime()=0;	
		virtual void SetTime(TimeValue t,BOOL redraw=TRUE)=0;

		// get/set the anim interval.
		virtual Interval GetAnimRange()=0;
		virtual void SetAnimRange(Interval range)=0;

		// Register a callback object that will get called every time the
		// user changes the frame slider.
		virtual void RegisterTimeChangeCallback(TimeChangeCallback *tc)=0;
		virtual void UnRegisterTimeChangeCallback(TimeChangeCallback *tc)=0;

		// Register a callback object that will get called when the user
		// causes the command mode to change
		virtual void RegisterCommandModeChangedCallback(CommandModeChangedCallback *cb)=0;
		virtual void UnRegisterCommandModeChangedCallback(CommandModeChangedCallback *cb)=0;

		// Register a ViewportDisplayCallback
		// If 'preScene' is TRUE then the callback will be called before object are rendered (typically, but not always).
		virtual void RegisterViewportDisplayCallback(BOOL preScene,ViewportDisplayCallback *cb)=0;
		virtual void UnRegisterViewportDisplayCallback(BOOL preScene,ViewportDisplayCallback *cb)=0;
		virtual void NotifyViewportDisplayCallbackChanged(BOOL preScene,ViewportDisplayCallback *cb)=0;

		// Register a ExitMAXCallback
		// Note: the Exit method of ExitMAXCallback is not called on shutdown after crash recovery
		// See Interface8 for an overriding method.
		virtual void RegisterExitMAXCallback(ExitMAXCallback *cb)=0;
		virtual void UnRegisterExitMAXCallback(ExitMAXCallback *cb)=0;

		virtual RightClickMenuManager* GetRightClickMenuManager()=0;

		// Delete key notitfication
		virtual void RegisterDeleteUser(EventUser *user)=0;		// Register & Activate
		virtual void UnRegisterDeleteUser(EventUser *user)=0;	// Deactivate & UnRegister

		//----- Creation-related Methods--------------------------
		
		virtual void MakeNameUnique(MSTR &name)=0;
		virtual INode *CreateObjectNode( Object *obj)=0;		
		virtual GenCamera *CreateCameraObject(int type) = 0;
		virtual Object   *CreateTargetObject() = 0;
		virtual GenLight *CreateLightObject(int type) = 0;
		virtual void *CreateInstance(SClass_ID superID, Class_ID classID)=0;
		virtual int BindToTarget(INode *laNode, INode *targNode)=0;
		virtual int IsCPEdgeOnInView()=0;		
		//!	\brief Deletes the specified node from the scene.
		/*!	Call this method to delete a specific node from the scene. 
		\li The children of the deleted node will maintain their positon. 
		\li Empty group heads resulting from the deletion of the node will also be deleted. 
		\li System nodes are not deleted by this method. Use the system's methods to
		modify its node structure. Use Interface9::DeleteNodes to delete all nodes of a
		system object.
		\li The selection state and deletion of the node will be undo-able after calling 
		this method, provided that the undo	system was engaged. For example, to delete 
		the first node in the current selection set, one could use the following code:
		\code
			theHold.Begin();
			Interface* ip = GetCOREInterface()
			ip->DeleteNode(ip->GetSelNode(0));
			theHold.Accept(_M("Delete"));
		\endcode
		The string used in the Accept() method appears in the undo list. 

		Note that the bject reference of the node is deleted as well if the only item referencing 
		that object is the node. This is because when the node is deleted, it first 
		deletes all its references. Whenever a reference is deleted, if the item is 
		the last reference, then the system deletes the item as well. If the node
		deletion occurs while the undo system was engaged, RestoreObjs will 
		hold references to the items removed from the node, preventing their 
		immediate deletion.

		\param [in] node The node to be deleted
		\param [in] redraw If FALSE the viewports will not be redrawn after the node is 
		deleted. This allows several nodes to be deleted at once without the viewports 
		having to be redrawn after each one.
		\param [in] overrideSlaves If TRUE, this method will delete nodes whose 
		TM controllers implement Control::PreventNodeDeletion() to return TRUE. 
		Control::PreventNodeDeletion() controls whether the node can be deleted from the 
		scene. This parameter allows a master controller to easily delete slaves nodes 
		if it needs to.
		\return The number of nodes that were deleted.
		*/
		virtual unsigned int DeleteNode(INode *node, BOOL redraw=TRUE, BOOL overrideSlaves=FALSE)=0;

		virtual INode *GetRootNode()=0;
		virtual void NodeInvalidateRect( INode *node )=0;
		virtual void SelectNode( INode *node, int clearSel = 1)=0;
		virtual void DeSelectNode(INode *node)=0;
		virtual void SelectNodeTab(INodeTab &nodes,BOOL sel,BOOL redraw=TRUE)=0;
		virtual void ClearNodeSelection(BOOL redraw=TRUE)=0;
		virtual void AddLightToScene(INode *node)=0; 
		virtual void AddGridToScene(INode *node) = 0;
		virtual void SetNodeTMRelConstPlane(INode *node, Matrix3& mat)=0;
		virtual void SetActiveGrid(INode *node)=0;
		virtual INode *GetActiveGrid()=0;

		// When a plug-in object implements it's own BeginCreate()/EndCreate()
		// it can cause EndCreate() to be called by calling this method.
		virtual void StopCreating()=0;

		// This creates a new object/node with out going throught the usual
		// create mouse proc sequence.
		// The matrix is relative to the contruction plane.
		virtual Object *NonMouseCreate(Matrix3 tm)=0;
		virtual void NonMouseCreateFinish(Matrix3 tm)=0;

		// directories
		virtual const MCHAR *GetDir(int which)=0;		// which = APP_XXX_DIR
		virtual int	GetPlugInEntryCount()=0;	// # of entries in PLUGIN.INI
		virtual const MCHAR *GetPlugInDesc(int i)=0;	// ith description
		virtual const MCHAR *GetPlugInDir(int i)=0;	// ith directory

		// bitmap path
		virtual int GetMapDirCount(BOOL xref=FALSE)=0;			// number of dirs in path
		virtual const MCHAR *GetMapDir(int i, BOOL xref=FALSE)=0;		// i'th dir of path
		virtual BOOL AddMapDir(const MCHAR *dir, BOOL xref=FALSE)=0;	// add a path to the list

		virtual BOOL AddSessionMapDir(const MCHAR *dir, int update=TRUE, BOOL xref=FALSE)=0;
		virtual int GetSessionMapDirCount(BOOL xref=FALSE)=0;
		virtual const MCHAR *GetSessionMapDir(int i, BOOL xref=FALSE)=0;
		virtual BOOL DeleteSessionMapDir(int i, int update=TRUE, BOOL xref=FALSE)=0;

		//These methods provide access to the combined list of permanent and temporary (session) map dirs,
		//therefore the current total set of map directories
		virtual int GetCurMapDirCount(BOOL xref=FALSE)=0;
		virtual const MCHAR *GetCurMapDir(int i, BOOL xref=FALSE)=0;

		virtual float GetLightConeConstraint()=0;

		// New Method for light exclusion/inclusion lists
		virtual int DoExclusionListDialog(ExclList *nl, BOOL doShadows=TRUE)=0;

		// Convert a NameTab to an ExclList ( the new format for Exclusion lists).
		virtual void ConvertNameTabToExclList(const NameTab *nt, ExclList *excList)=0;
		
		virtual MtlBase *DoMaterialBrowseDlg(HWND hParent,DWORD flags,BOOL &newMat,BOOL &cancel)=0;

		virtual void PutMtlToMtlEditor(MtlBase *mb, int slot=-1)=0;
		virtual MtlBase* GetMtlSlot(int slot)=0;
		virtual MtlBaseLib* GetSceneMtls()=0;

		// Before assigning material to scene, call this to avoid duplicate names.
		// returns 1:OK  0:Cancel
		virtual	BOOL OkMtlForScene(MtlBase *m)=0;

		// Access names of current files
		virtual MSTR &GetCurFileName()=0;
		virtual MSTR &GetCurFilePath()=0;
		virtual MCHAR *GetMatLibFileName()=0;

		// These may bring up file requesters
		virtual void FileOpen()=0;
		virtual BOOL FileSave()=0;
		virtual BOOL FileSaveAs()=0;
		virtual void FileSaveSelected()=0;
		virtual void FileReset(BOOL noPrompt=FALSE)=0;
		virtual void FileMerge()=0;
		virtual void FileHold()=0;
		virtual void FileFetch()=0;
		virtual void FileOpenMatLib(HWND hWnd)=0;  // Window handle is parent window
#ifndef NO_MATLIB_SAVING // orb 01-09-2002
		virtual void FileSaveMatLib(HWND hWnd)=0;
		virtual void FileSaveAsMatLib(HWND hWnd)=0;
#endif // NO_MATLIB_SAVING
		virtual BOOL FileImport()=0;
		virtual BOOL FileExport()=0;

		// This loads 3dsmax.mat (if it exists
		virtual void LoadDefaultMatLib()=0;

		// These do not bring up file requesters
		virtual int LoadFromFile(const MCHAR *name, BOOL refresh=TRUE)=0;
		// LAM - 8/8/03 - ECO 1125 - added clearNeedSaveFlag, useNewFile
		// if clearNeedSaveFlag is false, the scene 'dirty' flag is not cleared
		// if useNewFile is false, the file is not added to the MRU list and the current scene file is not set to the saved file
		virtual int SaveToFile(const MCHAR *fname, BOOL clearNeedSaveFlag = TRUE, BOOL useNewFile = TRUE)=0; 
		virtual void FileSaveSelected(MCHAR *fname)=0;
		virtual void FileSaveNodes(INodeTab* nodes, MCHAR *fname)=0;
		virtual int LoadMaterialLib(const MCHAR *name, MtlBaseLib *lib=NULL)=0;
#ifndef NO_MATLIB_SAVING // orb 01-09-2002
		virtual int SaveMaterialLib(const MCHAR *name, MtlBaseLib *lib=NULL)=0;
#endif // #ifndef NO_MATLIB_SAVING // orb 01-09-2002
		// dupMtlAction and reparentAction added for r6.0 - 8/5/03
		virtual int MergeFromFile(const MCHAR *name, 
				BOOL mergeAll=FALSE,    // when true, merge dialog is not put up
				BOOL selMerged=FALSE,   // select merged items?
				BOOL refresh=TRUE,      // refresh viewports ?
				int dupAction = MERGE_DUPS_PROMPT,  // what to do when duplicate node names are encountered
				NameTab* mrgList=NULL,  // names to be merged (mergeAll must be TRUE)
				int dupMtlAction = MERGE_DUP_MTL_PROMPT,  // what to do when duplicate material names are encountered
				int reparentAction = MERGE_REPARENT_PROMPT   // what to do when can reparent
				)=0;
		virtual BOOL ImportFromFile(const MCHAR *name, BOOL suppressPrompts=FALSE, Class_ID *importerID=NULL)=0;
		virtual BOOL ExportToFile(const MCHAR *name, BOOL suppressPrompts=FALSE, DWORD options=0, Class_ID *exporterID=NULL)=0;

		// Returns TRUE if this instance of MAX is in slave mode
		virtual BOOL InSlaveMode()=0;

		// Brings up the object color picker. Returns TRUE if the user
		// picks a color and FALSE if the user cancels the dialog.
		// If the user picks a color then 'col' will be set to the color.
		virtual BOOL NodeColorPicker(HWND hWnd,DWORD &col)=0;

		
		// The following gourping functions will operate on the table
		// of nodes passed in or the current selection set if the table is NULL
		
		// If name is NULL a dialog box will prompt the user to select a name. 
		// If sel group is TRUE, the group node will be selected after the operation completes.
		// returns a pointer to the group node created.
		virtual INode *GroupNodes(INodeTab *nodes=NULL,MSTR *name=NULL,BOOL selGroup=TRUE)=0;
		virtual void UngroupNodes(INodeTab *nodes=NULL)=0;
		virtual void ExplodeNodes(INodeTab *nodes=NULL)=0;
		virtual void OpenGroup(INodeTab *nodes=NULL,BOOL clearSel=TRUE)=0;
		virtual void CloseGroup(INodeTab *nodes=NULL,BOOL selGroup=TRUE)=0;

		// Flashes nodes (to be used to indicate the completion of a pick operation, for example)
		virtual void FlashNodes(INodeTab *nodes)=0;

		// If a plug-in needs to do a PeekMessage() and wants to actually remove the
		// message from the queue, it can use this method to have the message
		// translated and dispatched.
		virtual void TranslateAndDispatchMAXMessage(MSG &msg)=0;
		
		// This will go into a PeekMessage loop until there are no more
		// messages left. If this method returns FALSE then the user
		// is attempting to quit MAX and the caller should return.
		virtual BOOL CheckMAXMessages()=0;

		// Access viewport background image settings.
		virtual BOOL		setBkgImageName(MCHAR *name)=0;
		virtual MCHAR *		getBkgImageName(void)=0;
		virtual void		setBkgImageAspect(int t)=0;
		virtual int			getBkgImageAspect()=0;
		virtual void		setBkgImageAnimate(BOOL onOff)=0;
		virtual int			getBkgImageAnimate(void)=0;
		virtual void		setBkgFrameRange(int start, int end, int step=1)=0;
		virtual int			getBkgFrameRangeVal(int which)=0;
		virtual void		setBkgORType(int which, int type)=0; // which=0 => before start, which=1 =>	after end
		virtual int			getBkgORType(int which)=0;
		virtual void		setBkgStartTime(TimeValue t)=0;
		virtual TimeValue	getBkgStartTime()=0;
		virtual void		setBkgSyncFrame(int f)=0;
		virtual int			getBkgSyncFrame()=0;
		virtual int			getBkgFrameNum(TimeValue t)=0;

		// Gets the state of the real-time animation playback toggle.
		virtual BOOL GetRealTimePlayback()=0;
		virtual void SetRealTimePlayback(BOOL realTime)=0;
		virtual BOOL GetPlayActiveOnly()=0;
		virtual void SetPlayActiveOnly(BOOL playActive)=0;
		virtual void StartAnimPlayback(int selOnly=FALSE)=0;
		virtual void EndAnimPlayback()=0;
		virtual BOOL IsAnimPlaying()=0;
		virtual int GetPlaybackSpeed()=0;
		virtual void SetPlaybackSpeed(int s)=0;
		virtual BOOL GetPlaybackLoop()=0;
		virtual void SetPlaybackLoop(BOOL loop)=0;

		// mjm - 09.07.00
		// provide access to a global validity token, used essentially as a timestamp.
		// several areas of code may use it, one being interactive reshading.
		virtual void IncrementValidityToken()=0;
		virtual unsigned int CurrentValidityToken()=0;


		// The following APIs provide a simplistic method to call
		// the renderer and render frames. The renderer just uses the
		// current user specified parameters.
		// Note that the renderer uses the width, height, and aspect
		// of the specified bitmap so the caller can control the size
		// of the rendered image rendered.

		// Renderer must be opened before frames can be rendered.
		// Either camNode or view must be non-NULL but not both.
		// 
		// Returns the result of the open call on the current renderer.
		// 0 is fail and 1 is succeed.
		virtual int OpenCurRenderer(INode *camNode,ViewExp *view, RendType t = RENDTYPE_NORMAL, int w=0, int h=0)=0;

		// optional way to specify the view when Opening the renderer.
		virtual int OpenCurRenderer(ViewParams *vpar, RendType t = RENDTYPE_NORMAL, int w=0, int h=0)=0;

		// The renderer must be closed when you are done with it.
		virtual void CloseCurRenderer()=0;

		// Renders a frame to the given bitmap.
		// The RendProgressCallback is an optional callback (the base class is
		// defined in render.h).
		//
		// Returns the result of the render call on the current renderer.
		// 0 is fail and 1 is succeed.
		virtual int CurRendererRenderFrame(TimeValue t, Bitmap *bm, RendProgressCallback *prog=NULL, float frameDur = 1.0f, ViewParams *vp=NULL, RECT *regionRect = NULL)=0;

		// creates a default scanline renderer - must be deleted with IScanRenderer::DeleteThis()
		virtual IScanRenderer *CreateDefaultScanlineRenderer()=0;

		// [dl | 15may2003] Creates an instance of the default renderer for the given render setting. 
		// If the class ID of the default renderer (see SetDefaultRendererClassID()) does not 
		// exist, then an instance of the MAX scanline renderer is created.
		// The instance returned must be deleted with Renderer::DeleteThis().
		virtual Renderer* CreateDefaultRenderer(RenderSettingID renderSettingID) = 0;
		// [dl | 15may2003] Gets/sets the class ID of the default renderer for the given render setting. 
		// An instance of the default renderer can be created by using CreateDefaultRenderer()
		virtual Class_ID GetDefaultRendererClassID(RenderSettingID renderSettingID) = 0;
		virtual void SetDefaultRendererClassID(RenderSettingID renderSettingID, Class_ID classID) = 0;

		// a set of functions parallel to those above, to work with any Renderer instance
		virtual int OpenRenderer(Renderer *pRenderer, INode *camNode,ViewExp *view, RendType type = RENDTYPE_NORMAL, int w=0, int h=0)=0;
		virtual int OpenRenderer(Renderer *pRenderer, ViewParams *vpar, RendType type = RENDTYPE_NORMAL, int w=0, int h=0)=0;
		virtual int RendererRenderFrame(Renderer *pRenderer, TimeValue t, Bitmap *bm, RendProgressCallback *prog=NULL, float frameDur = 1.0f, ViewParams *vp=NULL, RECT *regionRect = NULL)=0;
		virtual void CloseRenderer(Renderer *pRenderer)=0;

		// To get more control over the renderer, the renderer can be called directly.
		// The following methods give access to the current renderer and the the user's
		// current rendering settings.

		// gets a pointer to the renderer being used with the currently active render settings
		virtual Renderer *GetCurrentRenderer()=0;
		// gets a pointer to the renderer being used for each of the render settings
		// GetRenderer(RenderSettingID) can be used to access all settings
		virtual Renderer *GetProductionRenderer()=0;
		virtual Renderer *GetDraftRenderer()=0;
		// this function can be used instead of the two above.
		// RS_MEdit: Returns the renderer in the MEdit slot. Does not consider the lock.
		virtual Renderer *GetRenderer(RenderSettingID renderSettingID)=0;				// mjm - 05.26.00

		// assigns a renderer to be used with the currently active render settings
		virtual void AssignCurRenderer(Renderer *rend)=0;
		// assigns a renderer to be used with each of the render settings
		// AssignRenderer(RenderSettingID, Renderer*) can be used to access all settings
		virtual void AssignProductionRenderer(Renderer *rend)=0;
		virtual void AssignDraftRenderer(Renderer *rend)=0;
		// this function can be used instead of the two above.
		// RS_MEdit: Assign the renderer to the MEdit slot. Does not consider the lock.
		virtual void AssignRenderer(RenderSettingID renderSettingID, Renderer *rend)=0;	// mjm - 05.26.00

		// in order to support more than just two render settings, the following two functions should no
		// longer be used. instead, use ChangeRenderSetting() below.

		// SetUseDraftRenderer() sets the CurrentRenderSetting to 'production'
		// if parameter (b == 0) and sets it to 'draft' if (b != 0)
		virtual void SetUseDraftRenderer(BOOL b)=0;

		// GetUseDraftRenderer() returns TRUE if the CurrentRenderSetting is 'draft'
		// it returns TRUE if the CurrentRenderSetting is not 'draft'
		// there is no way with this function to determine if the CurrentRenderSetting is anything but 'draft'
		virtual BOOL GetUseDraftRenderer()=0;

		// these functions can be used instead of the two above.
		// RS_MEdit cannot be the current render setting. Calling ChangeRenderSetting(RS_MEdit) has no effect.
		virtual void ChangeRenderSetting(RenderSettingID renderSettingID)=0;			// mjm - 05.26.00
		virtual RenderSettingID GetCurrentRenderSetting()=0;							// mjm - 05.26.00

		// [dl | 15may2003] Gets/sets the renderer assigned to the MEdit slot.
		// NOTE: This ignores the state of the MEdit renderer lock, and will not necesarily
		// get/set the renderer that is actually used for MEdit.
		virtual Renderer* GetMEditRenderer() = 0;
		virtual void AssignMEditRenderer(Renderer* renderer) = 0;
		// [dl | 15may2003] This returns the renderer to be used for MEdit.
		// This TAKES THE LOCK INTO ACCOUNT: this will return the Current renderer if
		// the MEdit lock is ON.
		virtual Renderer* GetActualMEditRenderer() = 0;
		// [dl | 15may2003] Gets/sets the status of the MEdit renderer lock.
		// When ON, the Current renderer should be used to render in MEdit.
		virtual bool GetMEditRendererLocked() = 0;
		virtual void SetMEditRendererLocked(bool locked) = 0;
		virtual bool GetMEditRendererLocked_DefaultValue() = 0;
		virtual void SetMEditRendererLocked_DefaultValue(bool locked) = 0;

// mjm - begin - 06.30.00
		// gets the current (production vs. draft) render element manager
		virtual IRenderElementMgr *GetCurRenderElementMgr()=0;
		// gets production or draft render element manager -- passing in renderSettingID = RS_IReshade will return NULL
		virtual IRenderElementMgr *GetRenderElementMgr(RenderSettingID renderSettingID)=0;
// mjm - end

		// Fills in a RendParams structure that can be passed to the
		// renderer with the user's current rendering settings.
		// A vpt pointer only needs to be passed in if the RendType
		// is RENDTYPE_REGION or RENDTYPE_BLOWUP. In these cases it will
		// set up the RendParams regxmin,regxmax,regymin,regymax from
		// values stored in the viewport.
		virtual void SetupRendParams(RendParams &rp, ViewExp *vpt, RendType t = RENDTYPE_NORMAL)=0;

		// fills in a ViewParams structure based on type of node passed (camera or light)
		// can be used when instantiating a renderer and calling Open(), Render(), and Close()
		// directly on it.
		virtual void GetViewParamsFromNode(INode* vnode, ViewParams& vp, TimeValue t)=0;

		// Call during render to check if user has cancelled render.  
		// Returns TRUE iff user has cancelled.
		virtual BOOL CheckForRenderAbort()=0;

    // Call during render to abort the render
    virtual void AbortRender()=0;

		// These give access to individual user specified render parameters
		// These are either parameters that the user specifies in the
		// render dialog or the renderer page of the preferences dialog.
		virtual int GetRendTimeType()=0;
		virtual void SetRendTimeType(int type)=0;
		virtual TimeValue GetRendStart()=0;
		virtual void SetRendStart(TimeValue start)=0;
		virtual TimeValue GetRendEnd()=0;
		virtual void SetRendEnd(TimeValue end)=0;
		virtual int GetRendNThFrame()=0;
		virtual void SetRendNThFrame(int n)=0;

		virtual BOOL GetRendShowVFB()=0;
#ifndef WEBVERSION //some properties are read-only method in Webversion, (have Get() but not Set())
		virtual void SetRendShowVFB(BOOL onOff)=0;
#endif
		virtual BOOL GetRendSaveFile()=0;
		virtual void SetRendSaveFile(BOOL onOff)=0;
		virtual BOOL GetRendUseDevice()=0;
#ifndef WEBVERSION 
		virtual void SetRendUseDevice(BOOL onOff)=0;
#endif
		virtual BOOL GetRendUseNet()=0;
#ifndef WEBVERSION 
		virtual void SetRendUseNet(BOOL onOff)=0;
#endif

		virtual BitmapInfo& GetRendFileBI()=0;
		virtual BitmapInfo& GetRendDeviceBI()=0;
		virtual int GetRendWidth()=0;
		virtual void SetRendWidth(int w)=0;
		virtual int GetRendHeight()=0;
		virtual void SetRendHeight(int h)=0;
		virtual float GetRendApect()=0;
#ifndef WEBVERSION 
		virtual void SetRendAspect(float a)=0;
#endif

		virtual float GetRendImageAspect()=0;

		virtual float GetRendApertureWidth()=0;	 // get aperture width in mm.
#ifndef WEBVERSION 
		virtual void SetRendApertureWidth(float aw)=0; // set aperture width in mm.
#endif
		virtual BOOL GetRendFieldRender()=0;
#ifndef WEBVERSION 
		virtual void SetRendFieldRender(BOOL onOff)=0;
#endif
		virtual BOOL GetRendColorCheck()=0;
#ifndef WEBVERSION 
		virtual void SetRendColorCheck(BOOL onOff)=0;
#endif
		virtual BOOL GetRendSuperBlack()=0;
#ifndef WEBVERSION 
		virtual void SetRendSuperBlack(BOOL onOff)=0;
#endif

		virtual BOOL GetRendHidden()=0;
		virtual void SetRendHidden(BOOL onOff)=0;
		virtual BOOL GetRendForce2Side()=0;
		virtual void SetRendForce2Side(BOOL onOff)=0;

		virtual BOOL GetRendAtmosphere()=0;
#ifndef WEBVERSION 
		virtual void SetRendAtmosphere(BOOL onOff)=0;
#endif
		virtual BOOL GetRendEffects()=0;
#ifndef WEBVERSION 
		virtual void SetRendEffects(BOOL onOff)=0;
#endif
		virtual BOOL GetRendDisplacement()=0;
#ifndef WEBVERSION 
		virtual void SetRendDisplacement(BOOL onOff)=0;
#endif

		virtual MSTR& GetRendPickFramesString()=0;
		virtual BOOL GetRendDitherTrue()=0;
		virtual void SetRendDitherTrue(BOOL onOff)=0;
		virtual BOOL GetRendDither256()=0;
		virtual void SetRendDither256(BOOL onOff)=0;
		virtual BOOL GetRendMultiThread()=0;
		virtual void SetRendMultiThread(BOOL onOff)=0;
		virtual BOOL GetRendNThSerial()=0;
		virtual void SetRendNThSerial(BOOL onOff)=0;
		virtual int GetRendVidCorrectMethod()=0; // 0->FLAG, 1->SCALE_LUMA 2->SCALE_SAT
		virtual void SetRendVidCorrectMethod(int m)=0;
		virtual int GetRendFieldOrder()=0; // 0->even, 1-> odd
		virtual void SetRendFieldOrder(int fo)=0;
		virtual int GetRendNTSC_PAL()=0; // 0 ->NTSC,  1 ->PAL
		virtual void SetRendNTSC_PAL(int np)=0;
		virtual int GetRendSuperBlackThresh()=0;
#ifndef WEBVERSION 
		virtual void SetRendSuperBlackThresh(int sb)=0;
#endif
		virtual int GetRendFileNumberBase()=0;
		virtual void SetRendFileNumberBase(int n)=0;
		virtual BOOL GetSkipRenderedFrames()=0;
#ifndef WEBVERSION 
		virtual void SetSkipRenderedFrames(BOOL onOff)=0;
#endif

		//Max 6.0
		virtual BOOL GetRendSimplifyAreaLights() = 0;
		virtual void SetRendSimplifyAreaLights(BOOL onOff) = 0;

		virtual DWORD GetHideByCategoryFlags()=0;
		virtual void SetHideByCategoryFlags(DWORD f)=0;

		virtual int GetViewportLayout()=0;
		virtual void SetViewportLayout(int layout)=0;
		virtual BOOL IsViewportMaxed()=0;
		virtual void SetViewportMax(BOOL max)=0;

		// Zoom extents the active viewport, or all
		virtual void ViewportZoomExtents(BOOL doAll, BOOL skipPersp=FALSE)=0;

	//watje this zooms the active viewport or all to the specified bounding box which is in world space
		virtual void ZoomToBounds(BOOL doAll, Box3 box) =0;

		// Gets the world space bounding box of the selection.
		virtual void GetSelectionWorldBox(TimeValue t,Box3 &box)=0;
		
		// Find an INode with the given name
		virtual INode *GetINodeByName(const MCHAR *name)=0;

		// Find an INode with the given handle
		virtual INode *GetINodeByHandle(ULONG handle)=0;

		// For use with gbuffer BMM_CHAN_NODE_RENDER_ID channel during video post
		virtual INode *GetINodeFromRenderID(UWORD id)=0;

		// Executes a MAX command. See maxcom.h for available commands
		virtual void ExecuteMAXCommand(int id)=0;

		// Returns a class used for efficiently creating unique names
		virtual NameMaker* NewNameMaker(BOOL initFromScene = TRUE)=0;

		// Get set the viewport background color.
		virtual void SetViewportBGColor(const Point3 &color)=0;
		virtual Point3 GetViewportBGColor()=0;

		// Get/Set the environment texture map, ambient light and other effects
		virtual Texmap *GetEnvironmentMap()=0;
		virtual void SetEnvironmentMap(Texmap *map)=0;
		virtual BOOL GetUseEnvironmentMap()=0;
		virtual void SetUseEnvironmentMap(BOOL onOff)=0;

		virtual Point3 GetAmbient(TimeValue t,Interval &valid)=0;
		virtual void SetAmbient(TimeValue t, Point3 col)=0;
		virtual Control *GetAmbientController()=0;
		virtual void SetAmbientController(Control *c)=0;

		virtual Point3 GetLightTint(TimeValue t,Interval &valid)=0;
		virtual void SetLightTint(TimeValue t, Point3 col)=0;
		virtual Control *GetLightTintController()=0;
		virtual void SetLightTintController(Control *c)=0;

		virtual float GetLightLevel(TimeValue t,Interval &valid)=0;
		virtual void SetLightLevel(TimeValue t, float lev)=0;
		virtual Control *GetLightLevelController()=0;
		virtual void SetLightLevelController(Control *c)=0;

		virtual int NumAtmospheric()=0;
		virtual Atmospheric *GetAtmospheric(int i)=0;
		virtual void SetAtmospheric(int i,Atmospheric *a)=0;
		virtual void AddAtmosphere(Atmospheric *atmos)=0;
		virtual void DeleteAtmosphere(int i)=0;
		virtual void EditAtmosphere(Atmospheric *a, INode *gizmo=NULL)=0;

		virtual int NumEffects()=0;
		virtual Effect *GetEffect(int i)=0;
		virtual void SetEffect(int i,Effect *e)=0;
		virtual void AddEffect(Effect *eff)=0;
		virtual void DeleteEffect(int i)=0;
		virtual void EditEffect(Effect *e, INode *gizmo=NULL)=0;

		virtual Point3 GetBackGround(TimeValue t,Interval &valid)=0;
		virtual void SetBackGround(TimeValue t,Point3 col)=0;
		virtual Control *GetBackGroundController()=0;
		virtual void SetBackGroundController(Control *c)=0;

		// Get/Set the current sound object.
		virtual SoundObj *GetSoundObject()=0;
		virtual void SetSoundObject(SoundObj *snd)=0;

#ifdef _OSNAP
		virtual IOsnapManager *GetOsnapManager()=0;
		virtual MouseManager *GetMouseManager()=0;
		virtual void InvalidateOsnapdraw()=0;
#endif

		// Access the current mat lib loaded.
		virtual MtlBaseLib& GetMaterialLibrary()=0;

		virtual BOOL IsNetServer()=0; // returns 1 iff is network server
		//-- GUPSTART
		virtual void SetNetServer()=0;
		//-- GUPEND


		//-- Logging Facilities (Replaces the old NetLog() stuff)
		// 
		//   Check log.h for methods

		virtual LogSys *Log()=0;


		// get ref to the central DLL directory
		virtual DllDir& GetDllDir()=0; 

		// Generic expansion function
		virtual INT_PTR Execute(int cmd, ULONG_PTR arg1=0, ULONG_PTR arg2=0, ULONG_PTR arg3=0, ULONG_PTR arg4=0, ULONG_PTR arg5=0, ULONG_PTR arg6=0)=0; 
									// WIN64 Cleanup: Martell and Shuler
		virtual void *GetInterface(DWORD id)=0;

		// Get pointer to the scene.
		virtual ReferenceTarget *GetScenePointer()=0;

		// Get a pointer to the Track View root node.
		virtual ITrackViewNode *GetTrackViewRootNode()=0;

		// Free all bitmaps used by the scene
		virtual void FreeSceneBitmaps()=0;

		// Access the DllDir
		virtual DllDir *GetDllDirectory()=0;

		// Enumerate Bitmap Files
		virtual void EnumAuxFiles(NameEnumCallback& nameEnum, DWORD flags)=0;

		// Render a 2D bitmap from a procedural texture
		virtual void RenderTexmap(Texmap *tex, Bitmap *bm, float scale3d=1.0f, BOOL filter=FALSE, BOOL display=FALSE, float z=0.0f)=0;

		// Activate and deactivate a texture map in the viewports. 
		// mtl is the TOP LEVEL of the material containing the texture map. If it
		// is a Multi-material, then subNum specifies which sub branch of the
		// multi contains tx.
		CoreExport void DeActivateTexture(MtlBase *tx, Mtl *mtl, int subNum=-1);
		CoreExport void ActivateTexture(MtlBase *tx, Mtl *mtl, int subNum=-1);

		// Access to named selection sets at the object level
		virtual int GetNumNamedSelSets()=0;
		virtual MCHAR *GetNamedSelSetName(int setNum)=0;
		virtual int GetNamedSelSetItemCount(int setNum)=0;
		virtual INode *GetNamedSelSetItem(int setNum,int i)=0;
		virtual void AddNewNamedSelSet(INodeTab &nodes,MSTR &name)=0;
		virtual void RemoveNamedSelSet(MSTR &name)=0;
		virtual void ReplaceNamedSelSet(INodeTab &nodes,int setNum)=0;
		virtual void GetNamedSelSetList(INodeTab &nodes,int setNum)=0;

		// Get new material and map names to maintain name uniqueness
		virtual void AssignNewName(Mtl *m)=0;
		virtual void AssignNewName(Texmap *m)=0;

		// rescale world units of entire scene, or selection
		virtual void RescaleWorldUnits(float f, BOOL selected)=0;

		// Initialize snap info structure with current snap settings
		// (Returns zero if snap is OFF)
		virtual int InitSnapInfo(SnapInfo *info)=0;

		// Time configuration dialog key step options
		virtual BOOL GetKeyStepsSelOnly()=0;
		virtual void SetKeyStepsSelOnly(BOOL onOff)=0;
		virtual BOOL GetKeyStepsUseTrans()=0;
		virtual void SetKeyStepsUseTrans(BOOL onOff)=0;
		virtual BOOL GetKeyStepsPos()=0;
		virtual void SetKeyStepsPos(BOOL onOff)=0;
		virtual BOOL GetKeyStepsRot()=0;
		virtual void SetKeyStepsRot(BOOL onOff)=0;
		virtual BOOL GetKeyStepsScale()=0;
		virtual void SetKeyStepsScale(BOOL onOff)=0;
		virtual BOOL GetKeyStepsUseTrackBar()=0;
		virtual void SetKeyStepsUseTrackBar(BOOL onOff)=0;

		// Enables/disables the use of Transform Gizmos
		virtual BOOL GetUseTransformGizmo()=0;
		virtual void SetUseTransformGizmo(BOOL onOff)=0;

		// Get/Set if the TGiz restores the previous axis when released.
		virtual void SetTransformGizmoRestoreAxis(BOOL bOnOff)=0;
		virtual BOOL GetTransformGizmoRestoreAxis()=0;

		// Turn off axis follows transform mode AI.
		virtual BOOL GetConstantAxisRestriction()=0;
		virtual void SetConstantAxisRestriction(BOOL onOff)=0;

		// Used to hittest transform Gizmos for sub-objects
		virtual int HitTestTransformGizmo(IPoint2 *p, ViewExp *vpt, int axisFlags) = 0;

		// Used to deactiveate the Transform Gizmo when it is released.
		virtual void DeactivateTransformGizmo() = 0;

		// put up dialog to let user configure the bitmap loading paths.
		// returns 1: OK ,  0: Cancel.
		virtual int ConfigureBitmapPaths()=0;

		// Puts up the space array dialog. If the callback is NULL it 
		// just does the standard space array tool.
		// returns TRUE if the user OKs the dialog, FALSE otherwise.
		virtual BOOL DoSpaceArrayDialog(SpaceArrayCallback *sacb=NULL)=0;

		// dynamically add plugin-class.
		// returns -1 if superclass was unknown
		// returns 0 if class already exists
		// returns 1 if class added successfully
		virtual int AddClass(ClassDesc *cd)=0;

		// dynamically delete plugin-class.
		// returns -1 if superclass was unknown
		// returns 0 if class does not exist
		// returns 1 if class deleted successfully
		virtual int DeleteClass(ClassDesc *cd)=0;

		// Number of CommandModes in the command mode stack
		virtual int GetCommandStackSize()=0;
		// Get the CommandMode at this position in the command mode stack (0 = current)
		virtual CommandMode* GetCommandStackEntry(int entry)=0;

		
		// This method should be called in an light's BeginEditParams, after adding rollups 
		// to the modify panel: it puts up a rollup containing a list of all Atmospherics 
		// and Effects that use the current selected node as a "gizmo"
		virtual void AddSFXRollupPage(ULONG flags=0)=0; // flags are for future use

		// This is called in a light's EndEditParams when removing rollups
		virtual void DeleteSFXRollupPage()=0;

		// This is called by an Atmospheric or Effect when it adds or removes a "gizmo" reference.
		virtual void RefreshSFXRollupPage()=0;

		// PropertySet access
		// Legal values for "int PropertySet" are defined above:
		//	PROPSET_SUMMARYINFO
		//	PROPSET_DOCSUMMARYINFO
		//	PROPSET_USERDEFINED
		virtual int					GetNumProperties(int PropertySet)=0;
		virtual int					FindProperty(int PropertySet, const PROPSPEC* propspec)=0;
		virtual const PROPVARIANT*	GetPropertyVariant(int PropertySet, int idx)=0;
		virtual const PROPSPEC*		GetPropertySpec(int PropertySet, int idx)=0;
		virtual void				AddProperty(int PropertySet, const PROPSPEC* propspec, const PROPVARIANT* propvar)=0;
		virtual void				DeleteProperty(int PropertySet, const PROPSPEC* propspec)=0;

		// register a window that can appear in a MAX viewport
		virtual BOOL RegisterViewWindow(ViewWindow *vw)=0;
		virtual BOOL UnRegisterViewWindow(ViewWindow *vw)=0;
		
		// Get and set the global shadow generator ( used by light.cpp)
		virtual ShadowType *GetGlobalShadowGenerator()=0;
		virtual void SetGlobalShadowGenerator(ShadowType *st)=0;

		// Get the Import zoom-extents flag
		virtual BOOL GetImportZoomExtents()=0;
		virtual void SetImportZoomExtents(BOOL onOff)=0;

		virtual unsigned int HardwareLockID()=0;
		virtual BOOL CheckForSave()=0;

		virtual ITrackBar*	GetTrackBar()=0;

		// For scene XRefs. Most of the time the XRef trees (whose root node is a child of the
		// client scene's root node) are skipped when traversing the hierarchy. When this option
		// is turned on, all root nodes will include child XRef scene root nodes in any traversal
		// related functions such as NumberOfChildren() and GetChildNode(i).
		// 
		// This option is turned on automatically before rendering and turned off after so that
		// scene XRefs appear in the production renderer. This option should not be left on if
		// it is turned on since it would cause scene XRef objects to be accessible to the user in the client scene.
		virtual void SetIncludeXRefsInHierarchy(BOOL onOff)=0;
		virtual BOOL GetIncludeXRefsInHierarchy()=0;

		// Use these two suspend automatic reloading of XRefs. 
		virtual BOOL IsXRefAutoUpdateSuspended()=0;
		virtual void SetXRefAutoUpdateSuspended(BOOL onOff)=0;

		// get the macroRecorder interface pointer
		virtual MacroRecorder* GetMacroRecorder()=0;


		// DS 2/2/99: 
		virtual void UpdateMtlEditorBrackets()=0;

		// PRS 2/4/99
		virtual bool IsTrialLicense() = 0;
		virtual bool IsNetworkLicense() = 0;
		virtual bool IsEmergencyLicense() = 0;

		// CCJ - 2/12/99
		virtual void SetMAXFileOpenDlg(MAXFileOpenDialog* dlg) = 0;
		virtual void SetMAXFileSaveDlg(MAXFileSaveDialog* dlg) = 0;

		virtual void RAMPlayer(HWND hWndParent, MCHAR* szChanA=NULL, MCHAR* szChanB=NULL) = 0;

		//KAE - 3/4/99
		virtual void FlushUndoBuffer() = 0;

		// CCJ 3/16/99
		virtual bool DeferredPluginLoadingEnabled() = 0;
		virtual void EnableDeferredPluginLoading(bool onOff) = 0;

		// RB: 3/30/99
		virtual BOOL IsSceneXRefNode(INode *node)=0;
	
		// MEP 4/19/99
		virtual bool CanImportFile(const MCHAR* filename)=0;
		virtual bool IsMaxFile(const MCHAR* filename)=0;
		virtual bool IsInternetCachedFile(const MCHAR* filename)=0;

		// MEP 6/21/99
		virtual bool CanImportBitmap(const MCHAR* filename)=0;

		//JH 8/07/99: implementing scheme to capture subobject registration
		virtual bool CaptureSubObjectRegistration(bool OnOff, Class_ID cid)=0;

		// MEP 8/06/99: easy method to download a file from a URL
		// JH 5/4/99: adding argument to control dialog appearance
		#define DOWNLOADDLG_NOPLACE		1<<0
		virtual bool DownloadUrl(HWND hwnd, const MCHAR* url, const MCHAR* filename, DWORD flags = 0)=0;

		// MEP 3/10/2000: if drag-and-dropped onto a node, you can retrieve the node with this
		virtual INode* GetImportCtxNode(void)=0;

		// child treeview creation - JBW 5.25.00
		virtual ITreeView* CreateTreeViewChild(ReferenceTarget* root, HWND hParent, DWORD style=0, ULONG id=0, int open=OPENTV_SPECIAL)=0;

		// CCJ 6.7.00 - Create a viewport material from a renderer material
		virtual void ConvertMtl(TimeValue t, Material &gm, Mtl *mtl,  BOOL doTex, int subNum, float vis, BOOL &needDecal, INode *node, BitArray *needTex, GraphicsWindow *gw)=0;

		// 000817  --prs.
		virtual int GetProductVersion()=0;
		virtual int GetLicenseBehavior()=0;
		// 000821  --prs.
		virtual bool IsFeatureLicensed(int subNum)=0;
		// 000824  --prs.
		virtual int GetLicenseDaysLeft()=0;

		// CCJ 8.23.00 - Clone a node hierarchy
		virtual bool CloneNodes(INodeTab& nodes, Point3& offset, bool expandHierarchies = true, CloneType cloneType = NODE_COPY, INodeTab* resultSource = NULL, INodeTab* resultTarget = NULL) = 0;

		// NS: 9/9/00
		virtual void CollapseNode(INode *node, BOOL noWarning = FALSE)=0;
		virtual BOOL CollapseNodeTo(INode *node, int modIndex, BOOL noWarning = FALSE)=0;
		virtual BOOL ConvertNode(INode *node, Class_ID &cid)=0;

		// TB 5/22/03
		virtual IRenderPresetsManager* GetRenderPresetsManager()=0;

		// LAM - 6/24/03
		//-- Default Action Facilities 
		//   Check DefaultActions.h for methods
		virtual DefaultActionSys *DefaultActions()=0;

		// Sets internal flag, returns old value. 
		// Set to TRUE to indicate that no dialogs should be displayed to user.
		virtual BOOL SetQuietMode(BOOL onOff)=0;

		// The return value from this method should be called before displaying any dialogs.
		// Returns internal flag set by SetQuietMode. If checkNetSlave == TRUE, returns internal 
		// flag OR-ed with whether max is running in network rendering server mode. 
		virtual BOOL GetQuietMode(BOOL checkNetSlave = TRUE)=0;

}; // class Interface

class IObjParam: public Interface{};

class IObjCreate: public IObjParam{};

// Forward declarations
class MAXScriptPrefs;
class IMenu;
class IMenuItem;
class FrameRendParams;
class DefaultLight;
class ITabPage;

//-----------------------------------------------------------------------------
/// - This class extends Max's previous version of core interface (class Interface) 
/// - "7" is the major version number of Max that exposes this interface.
/// - Call GetCOREInterface7 to acquire a pointer to this interface.
//-----------------------------------------------------------------------------
class Interface7 : public IObjCreate
{
public:
	/// --- Object Selection ------------------------------------------------ ///
	/// Retrieves the currently selected nodes into the supplied parameter.
	/// It clears the node tab supplied as parameter before using it.
	virtual void GetSelNodeTab(INodeTab& selectedNodes) const = 0;

	/// --- Transform Modes and Reference Coordinate System ----------------- ///
	/// Pivot modes the system can be in
	enum PivotMode {
		/// Transforms will affect the objects' world transform
		kPIV_NONE = PIV_NONE,
		/// Transforms will affect only the pivot point of objects
		kPIV_PIVOT_ONLY = PIV_PIVOT_ONLY,
		/// Transforms will affect only the objects and not their pivot points
		kPIV_OBJECT_ONLY = PIV_OBJECT_ONLY,
		/// Rotation and Scale will be applied to the hierarchy by rotating or 
		/// scaling the position of the pivot point without rotating or scaling 
		/// the pivot point itself. 
		kPIV_HIERARCHY_ONLY = PIV_HIERARCHY_ONLY
	};
	
	/// Returns the current pivot mode the system is in. 
	virtual PivotMode GetPivotMode() const = 0;
	/// Set the pivot mode of the system
	virtual void SetPivotMode(PivotMode pivMode) = 0;

	/// Returns true if the transforms applied to a node will affect its children,
	/// otherwise returns false
	virtual bool GetAffectChildren() const = 0;
	/// Sets whether the transforms applied to a node will affect its children.
	/// If bAffectChildren is true, the children of a node will be transformed when
	/// their parent node is transformed. Otherwise, they won't be transformed.
	virtual void SetAffectChildren(bool bAffectChildren) = 0;

	/// Given a reference coordinate system id, returns the name of it
	virtual void GetCurRefCoordSysName(MSTR& name) const = 0;

	/// Allows for setting the specified node's axis, as the current reference 
	/// coordiante system
	virtual void AddRefCoordNode(INode *node) = 0;
	virtual INode* GetRefCoordNode() = 0;

	/// --- Rendering ------------------------------------------------------- ///
	/// Only relevant for network rendering.  It is possible to set a job flag
	/// indicates that max should attempt to continue to render even when an
	/// "error" has been detected.  This method allows plug-ins to determine
	/// whether this flag has been set.
	virtual bool ShouldContinueRenderOnError() const = 0;

	virtual void SetupFrameRendParams(
		FrameRendParams &frp, 
		RendParams &rp, 
		ViewExp *vx, 
		RECT *r) = 0;
	virtual void SetupFrameRendParams_MXS(
		FrameRendParams &frp, 
		RendParams &rp, 
		ViewExp *vx, 
		RECT *r, 
		bool useSelBox) = 0;
	virtual int InitDefaultLights(
		DefaultLight *dl, 
		int maxn, 
		BOOL applyGlobalLevel = FALSE, 
		ViewExp *vx = NULL, 
		BOOL forRenderer = FALSE) = 0;

	/// Methods used to sync use with Material Editor sample rendering
	virtual void IncrRenderActive() = 0;  
	virtual void DecrRenderActive() = 0;
	virtual BOOL IsRenderActive() = 0;
	
	virtual BOOL XRefRenderBegin() = 0;
	virtual void XRefRenderEnd() = 0;

	virtual void OpenRenderDialog() = 0;
	virtual void CancelRenderDialog() = 0;
	virtual void CloseRenderDialog() = 0;
	virtual void CommitRenderDialogParameters() = 0;
	virtual void UpdateRenderDialogParameters() = 0;
	virtual BOOL RenderDialogOpen() = 0;

	virtual Bitmap* GetLastRenderedImage() = 0;

	/// --- General UI ------------------------------------------------------ ///
	/// Returns the window handle of the status panel window 
	/// (this holds the MAXScript mini-listener)
	virtual HWND GetStatusPanelHWnd() = 0;
	
	/// --- Maxscript ------------------------------------------------------- ///
	virtual void SetListenerMiniHWnd(HWND wnd) = 0;
	virtual HWND GetListenerMiniHWnd() = 0;
	
	/// Starts the MAXScript help
	virtual int MAXScriptHelp(MCHAR* keyword = NULL) = 0;

	/// Retrieves maxscript preferences
	virtual MAXScriptPrefs& GetMAXScriptPrefs() = 0;

	/// --- Trackview ------------------------------------------------------- ///
	virtual BOOL OpenTrackViewWindow(
		MCHAR* tv_name, 
		MCHAR* layoutName = NULL, 
		Point2 pos = Point2(-1.0f,-1.0f), 
		int width = -1, 
		int height = -1, 
		int dock = TV_FLOAT) = 0;
	
	/// Sets the fous to the specified track view window
	virtual BOOL BringTrackViewWindowToTop(MCHAR* tv_name) = 0;

	virtual BOOL TrackViewZoomSelected(MCHAR* tv_name) = 0;
	virtual BOOL TrackViewZoomOn(MCHAR* tv_name, Animatable* parent, int subNum) = 0;
	virtual BOOL CloseTrackView(MCHAR* tv_name) = 0;
	virtual int NumTrackViews() = 0;
	virtual MCHAR* GetTrackViewName(int i) = 0;
	/// The mask bits are defined in MAXSDK/INCLUDE/ITREEVW.H. Internally, the mask 
	/// bits are stored in two DWORDs The 'which' param tells which to work with - 
	/// valid values are 0 and 1.
	virtual BOOL SetTrackViewFilter(
		MCHAR* tv_name, 
		DWORD mask, 
		int which, 
		BOOL redraw = TRUE) = 0;
	virtual BOOL ClearTrackViewFilter(
		MCHAR* tv_name, 
		DWORD mask, 
		int which, 
		BOOL redraw = TRUE) = 0;
	virtual DWORD TestTrackViewFilter(MCHAR* tv_name, DWORD mask, int which) = 0;
	virtual void FlushAllTrackViewWindows() = 0;
	virtual void UnFlushAllTrackViewWindows() = 0;
	virtual void CloseAllTrackViewWindows() = 0;
	
	/// --- Command Panel Control ------------------------------------------- ///
	virtual void SetCurEditObject(BaseObject *obj, BaseNode *hintNode = NULL) = 0;
	/// Get the object or modifier that is currently being edited in the modifier panel
	virtual BaseObject* GetCurEditObject() = 0;
	virtual void AddModToSelection(Modifier* mod) = 0;
	virtual void InvalidateObCache(INode* node) = 0;
	virtual INode* FindNodeFromBaseObject(ReferenceTarget* obj) = 0;  
	virtual void SelectedHistoryChanged() = 0; 
	virtual BOOL CmdPanelOpen() = 0;
	virtual void CmdPanelOpen(BOOL openClose) = 0;
	
	/// Suspends\Resumes command pannels specified via bits set in whichPanels param
	virtual void SuspendEditing(
		DWORD whichPanels = (1<<TASK_MODE_MODIFY), 
		BOOL alwaysSuspend = FALSE) = 0;
	virtual void ResumeEditing(
		DWORD whichPanels = (1<<TASK_MODE_MODIFY), 
		BOOL alwaysSuspend = FALSE) = 0;
	virtual void SuspendMotionEditing() = 0;
	virtual void ResumeMotionEditing() = 0;

	/// This method expands Interface::AddClass. It allows for adding new ClassDesc 
	/// dynamically to create panel 
	virtual int AddClass(
		ClassDesc *cdesc, 
		int dllNum = -1, 
		int index = -1, 
		bool load = true) = 0;
	/// Rebuilds the list of groups and categories of the Create Panel
	virtual void ReBuildSuperList() = 0;
	
	/// Returns FALSE if the editing is stopped. While stopped, it shouldn't be resumed
	virtual BOOL IsEditing() = 0;

	/// Allows for changing the modifier panel's 
	virtual void ChangeHistory(int upDown) = 0;

	/// --- Object Creation ------------------------------------------------- ///
	virtual void StartCreatingObject(ClassDesc* cd) = 0;
	virtual BOOL IsCreatingObject(Class_ID& id) = 0;
	virtual BOOL IsCreatingObject() = 0; 
	/// Fast node creation for FileLink
	virtual void UpdateLockCheckObjectCounts() = 0;
	virtual INode *CreateObjectNode( Object *obj, const MCHAR* name) = 0;
	
	/// --- Configuration Paths/Directories --------------------------------- ///
	/// The 'which' parameter corresponds to the 'MAX Directories' defines in maxapi.h
	virtual BOOL SetDir(int which, MCHAR *dir) = 0;
	virtual BOOL AddMapDir(MCHAR *dir, int update=TRUE, BOOL xref=FALSE) = 0;
	virtual BOOL DeleteMapDir(int i, int update=TRUE, BOOL xref=FALSE) = 0;
	virtual void UpdateMapSection(BOOL xref=FALSE) = 0;
	
	/// Appends a string to the current file name and file path. It also updates 
	/// the string in the application's title bar. 
	virtual BOOL AppendToCurFilePath(const MCHAR* toAppend) = 0;

	/// Returns empty MSTR if locType == LOC_REGISTRY.
	virtual MSTR GetMAXIniFile() = 0; 

	/// --- Schematic View -------------------------------------------------- ///
#ifndef NO_SCHEMATICVIEW
	virtual BOOL OpenSchematicViewWindow(MCHAR* sv_name) = 0;
	virtual BOOL SchematicViewZoomSelected(MCHAR* sv_name) = 0;
	virtual BOOL CloseSchematicView(MCHAR* sv_name) = 0;
	virtual int  NumSchematicViews() = 0;
	virtual MCHAR* GetSchematicViewName(int i) = 0;
	virtual void CloseAllSchematicViewWindows() = 0;
	virtual void FlushAllSchematicViewWindows() = 0;
	virtual void UnFlushAllSchematicViewWindows() = 0;
#endif // NO_SCHEMATICVIEW

	/// --- Scene ----------------------------------------------------------- ///
	virtual BOOL DrawingEnabled() = 0;
	virtual void EnableDrawing(BOOL onOff) = 0;
	virtual BOOL SceneResetting() = 0;
	virtual BOOL QuitingApp() = 0;

	virtual BOOL GetHideFrozen() = 0;
	/// See 'Scene Display Flags' in maxapi.h for the possible values of 'flag'
	virtual void SetSceneDisplayFlag(DWORD flag, BOOL onOff, BOOL updateUI = TRUE) = 0;
	virtual BOOL GetSceneDisplayFlag(DWORD flag) = 0;

	/// Access to the scene interface
	virtual IScene* GetScene() = 0;
	
	/// ---  Materials and Material Editor ---------------------------------- ///
	virtual void SetMtlSlot(int i, MtlBase* m) = 0;
	virtual int GetActiveMtlSlot() = 0;
	virtual void SetActiveMtlSlot(int i) = 0;
	virtual int NumMtlSlots() = 0;
	virtual void FlushMtlDlg() = 0;
	virtual void UnFlushMtlDlg() = 0;
	virtual BOOL IsMtlInstanced(MtlBase* m) = 0;

	virtual Mtl *FindMtlNameInScene(MSTR &name) = 0;
	virtual void PutMaterial(
		MtlBase* mtl, 
		MtlBase* oldMtl, 
		BOOL delOld = 1, 
		RefMakerHandle skipThis = 0) = 0;

	virtual BOOL IsMtlDlgShowing() = 0;
	virtual void OpenMtlDlg() = 0;
	virtual void CloseMtlDlg() = 0;

	/// --- Viewport -------------------------------------------------------- ///
	/// Returns the window handle for the viewport frame
	virtual HWND GetViewPanelHWnd() = 0;  
	/// Viewport access by index
	virtual int  getActiveViewportIndex() = 0;
	virtual BOOL setActiveViewport(int index) = 0;
	virtual int getNumViewports() = 0;
	virtual ViewExp *getViewport(int i) = 0;
	virtual void resetAllViews() = 0;
	
	/// Viewport name access
	virtual MCHAR* getActiveViewportLabel() = 0;
	virtual MCHAR* getViewportLabel(int index) = 0;
	
	/// Viewport blow-up and sub-region access
	virtual void SetRegionRect(int index, Rect r) = 0;
	virtual Rect GetRegionRect(int index) = 0;
	virtual void SetBlowupRect(int index, Rect r) = 0;
	virtual Rect GetBlowupRect(int index) = 0;
	virtual void SetRegionRect2(int index, Rect r) = 0;
	virtual Rect GetRegionRect2(int index) = 0;
	virtual void SetBlowupRect2(int index, Rect r) = 0;
	virtual Rect GetBlowupRect2(int index) = 0;
	virtual int GetRenderType() = 0;
	virtual void SetRenderType(int rtype) = 0;
	virtual BOOL GetLockImageAspRatio() = 0;
	virtual void SetLockImageAspRatio(BOOL on) = 0;
	virtual float GetImageAspRatio() = 0;
	virtual void SetImageAspRatio(float on) = 0;
	virtual BOOL GetLockPixelAspRatio() = 0;
	virtual void SetLockPixelAspRatio(BOOL on) = 0;
	virtual float GetPixelAspRatio() = 0;
	virtual void SetPixelAspRatio(float on) = 0;

	virtual void SetViewportGridVisible(int index, BOOL state) = 0;
	virtual BOOL GetViewportGridVisible(int index) = 0;

	virtual void ViewportInvalidate(int index) = 0; 
	virtual void ViewportInvalidateBkgImage(int index) = 0; 
	virtual void InvalidateAllViewportRects() = 0;

	virtual void RedrawViewportsNow(
		TimeValue t, 
		DWORD vpFlags = VP_DONT_SIMPLIFY) = 0;
	virtual void RedrawViewportsLater(
		TimeValue t, 
		DWORD vpFlags = VP_DONT_SIMPLIFY ) = 0;

	/// See 'Viewport Rendering Levels' in maxapi.h for possible values of 'level' 
	virtual void SetActiveViewportRenderLevel(int level) = 0;
	virtual int GetActiveViewportRenderLevel() = 0;

	/// Access to viewport show edge faces states
	virtual void SetActiveViewportShowEdgeFaces(BOOL show) = 0;
	virtual BOOL GetActiveViewportShowEdgeFaces() = 0;

	/// There are 3 levels of transparency: 0, 1 and 2. 
	/// 0 - no transperancy
	/// 1 - if hardware rendring -> Blend, otherwise Stipple
	/// 2 - if hardware rendring -> Sorted Blend, otherwise Blend
	virtual void SetActiveViewportTransparencyLevel(int level) = 0;
	virtual int GetActiveViewportTransparencyLevel() = 0;

	/// Access dual plane settings
	virtual BOOL GetDualPlanes() = 0;
	virtual void SetDualPlanes(BOOL b) = 0;

	/// --- Hit Testing ----------------------------------------------------- ///
	/// When the flag is On, only frozen objects are hit tested during a pick
	virtual void SetTestOnlyFrozen(int onOff) = 0;

	/// --- Tool and Command Modes ------------------------------------------ ///
	/// Scale modes: CID_OBJSCALE, CID_OBJUSCALE, CID_OBJSQUASH (see cmdmode.h)
	virtual void SetScaleMode(int mode) = 0;
	/// Center modes - see 'Origin modes' in maxapi.h
	virtual void SetCenterMode(int mode) = 0;
	/// Manipulator related
	virtual BOOL InManipMode() = 0;
	virtual void StartManipulateMode() = 0;
	virtual void EndManipulateMode() = 0;
	virtual BOOL IsViewportCommandMode(CommandMode *m) = 0;
	
	/// --- XRefs ----------------------------------------------------------- ///
	/// See the 'ConvertFlagedNodesToXRefs() xflags bits' in maxapi.h for the 
	// possible values the 'xflag' parameter can take
	virtual	void ConvertFlagedNodesToXRefs(
		MSTR &fname,
		INode *rootNode,
		Tab<INode*> &nodes, 
		int xFlags) = 0;
	/// 'f' parameter can take values defined as 'Xref flag bits' in inode.h
	virtual void XRefSceneSetIgnoreFlag(int index, DWORD f, BOOL onOff) = 0;
	virtual void UpdateSceneXRefState() = 0;

	/// --- Object Snaps ---------------------------------------------------- ///
	virtual BOOL GetSnapActive() = 0;
	virtual void SetSnapActive(BOOL onOff) = 0;
	virtual int  GetSnapType() = 0;
	/// See 'Snap types' in snap.h for possible values of 'type'
	virtual void SetSnapType(int type) = 0;
	virtual void ToggleASnap() = 0;
	virtual int ASnapStatus() = 0;
	virtual void TogglePSnap() = 0;
	virtual int PSnapStatus() = 0;
	virtual float GetGridSpacing() = 0;
	virtual void SetGridSpacing(float newVal) = 0;
	virtual int GetGridMajorLines() = 0;
	virtual void SetGridMajorLines(float newVal) = 0;
	virtual float GetSnapAngle() = 0;
	virtual void SetSnapAngle(float newVal) = 0;
	virtual float GetSnapPercent() = 0;
	virtual void SetSnapPercent(float newVal) = 0;

	/// --- Node Properties ------------------------------------------------- ///
	virtual BOOL GetPrimaryVisibility(INode* node) = 0;
	virtual void SetPrimaryVisibility(INode* node, BOOL onOff) = 0;
	virtual BOOL GetSecondaryVisibility(INode* node) = 0;
	virtual void SetSecondaryVisibility(INode* node, BOOL onOff) = 0;
	/// 'whatAttrib' can take the valid values of the "Node attribute flags" 
	/// defined in maxapi.h
	virtual void SetNodeAttribute(INodeTab &nodes, int whatAttrib, int onOff) = 0;
	virtual void SetNodeAttribute(INode *node, int whatAttrib, int onOff) = 0;

	virtual void InvalidateNodeRect(INode* node, TimeValue t, BOOL oldPos=FALSE) = 0;

	/// --- Custom and General UI ------------------------------------------- ///
	virtual void SetExpertMode(int onOff) = 0;
	virtual int GetExpertMode() = 0;
	virtual void LoadCUIConfig(MCHAR* fileName) = 0;
	virtual void WriteCUIConfig() = 0;
	virtual void SaveCUIAs() = 0;
	virtual void LoadCUI() = 0;
	virtual void RevertToBackupCUI() = 0;
	virtual void ResetToFactoryDefaultCUI() = 0;
	virtual void DoUICustomization(int startPage) = 0;

	virtual int GetDefaultImageListBaseIndex(SClass_ID sid, Class_ID cid) = 0;
	virtual MSTR* GetDefaultImageListFilePrefix(SClass_ID sid, Class_ID cid) = 0;

	virtual float GetGridIntens() = 0;
	virtual void SetGridIntens(float f) = 0;
	virtual BOOL GetWhiteOrigin() = 0;
	virtual void SetWhiteOrigin(BOOL b) = 0;
	virtual BOOL GetUseGridColor() = 0;
	virtual void SetUseGridColor(BOOL b) = 0;
	virtual void UpdateColors(
		BOOL useGridColor, 
		int gridIntensity, 
		BOOL whiteOrigin) = 0;

	virtual IMenu* GetIMenu() = 0;
	virtual IMenuItem* GetIMenuItem() = 0;

	virtual void RepaintTimeSlider() = 0;

	virtual MSTR GetTabPageTitle(ITabPage *page) = 0;

	/// --- File Open/Save Dialogs ------------------------------------------ ///
	virtual BOOL DoMaxFileSaveAsDlg(MSTR &fileName, BOOL setAsCurrent=TRUE) = 0;
	virtual BOOL DoMaxFileOpenDlg(MSTR &fileName, MSTR *defDir, MSTR *defFile) = 0;
	virtual BOOL DoMaxFileMergeDlg(MSTR &fileName, MSTR *defDir, MSTR *defFile) = 0;
	virtual BOOL DoMaxFileXRefDlg(MSTR &fileName, MSTR *defDir, MSTR *defFile) = 0;

	/// --- Animation ------------------------------------------------------- ///
	/// Old style playback with no immediate return
	virtual void StartAnimPlayback2(int selOnly) = 0; 

	/// --- Modifiers ------------------------------------------------------- ///
	enum ResCode
	{
		kRES_INTERNAL_ERROR = -3,
		kRES_MOD_NOT_FOUND = -2,
		kRES_MOD_NOT_APPLICABLE = -1,
		kRES_SUCCESS = 0,
	};
	/// Tests of modifier applicability
	virtual BOOL IsValidModForSelection(ClassEntry* ce) = 0;
	/// Returns FALSE if the specied modifier cannot be applied to the object
	virtual BOOL IsValidModifier(INode& node, Modifier& mod) = 0;

	/// Adds the specified modifier to a node.
	/// INode& node - the node the modifier will be added to
	/// Modifier& mod - the modifier instance that will be applied to node
	/// int beforeIdx - the index in the modstack where the modifier should
	///									be inserted. 0 means at the top of the modstack 
	///									(just below the node)
	/// If the object's stack doesn't have beforeIdx number of modifiers in it,
	/// the modifier is added at the lowest possible index (just on top of the 
	/// base object)
	/// Returns one of these error codes:
	/// kRES_INTERNAL_ERROR - if an error such as invalid pointer is encountered
	/// kRES_MOD_NOT_APPLICABLE - if the modifier is not applicable
	/// kRES_SUCCESS - on success
	virtual ResCode AddModifier(INode& node, Modifier& mod, int beforeIdx = 0) = 0;

	/// Deletes the first occurance of the specified modifier from the node's 
	/// modifier stack. Returns one of the following values: kRES_INTERNAL_ERROR,
	/// kRES_MOD_NOT_FOUND or kRES_SUCCESS
	virtual ResCode DeleteModifier(INode& node, Modifier& mod) = 0;

	/// Deletes the modifier at the specified index from the node's modifier stack
	/// Returns one of the following values: kRES_INTERNAL_ERROR,
	/// kRES_MOD_NOT_FOUND or kRES_SUCCESS
	virtual ResCode DeleteModifier(INode& node, int modIdx) = 0;

	/// Finds the first occurance of the specified modifier on a node. 
	/// It returns a pointer to the derived object to which this modifier belongs,
	/// the index of the modifier within this derived object and the index within 
	/// the modifier stack. If the modifier is not found, returns NULL. 
	/// It searches the WS, then the OS part of the geom pipeline of the node.
	virtual IDerivedObject* FindModifier(
		INode& node, 
		Modifier& mod, 
		int& modStackIdx, 
		int& derivedObjIdx) = 0;

	/// Finds the modifier at the specified modstack index on a node. 
	/// It returns a pointer to the derived object to which this modifier belongs, 
	/// the index of the modifier within this derived object, and a pointer to the 
	/// modifier itself. If the modifier is not found, returns NULL. 
	/// It searches both WS and OS part of the geom pipeline of the object
	virtual IDerivedObject* FindModifier(
		INode& node, 
		int modIdx, 
		int& idx, 
		Modifier*& mod) = 0;

	/// Finds the index of a modifier instance in a modstack of a node, when 
	/// the node, the modifier it's corresponding cod context are given.
	/// It returns a pointer to the derived object to which this modifier belongs, 
	/// the index of the modifier within this derived object, and a pointer to the 
	/// modifier itself. If the modifier is not found, returns NULL. It searches 
	/// both WS and OS part of the geom pipeline of the object node, mod and mc 
	/// are input parameters, while modStackIdx and dobjidx are output params
	virtual IDerivedObject* FindModifier(
		INode& node, 
		Modifier& mod, 
		ModContext& mc, 
		int& modStackIdx, 
		int& dobjidx) = 0;

	virtual ResCode DoDeleteModifier(INode& node, IDerivedObject& dobj, int idx) = 0;

	/// A node's reference to its object should be replaced using this method. 
	/// Returns NULL when the operation cannot be completed, such as for Actively 
	/// (File) Linked objects or their user created clone-instances\references.
	virtual Object* GetReplaceableObjRef(INode& node) = 0;

	/// --- Render Effects -------------------------------------------------- ///
#ifndef NO_RENDEREFFECTS 
	virtual void OpenEnvEffectsDialog() = 0;
	virtual void CloseEnvEffectsDialog() = 0;
	virtual BOOL EnvEffectsDialogOpen() = 0;
#endif // NO_RENDEREFFECTS

}; // class Interface7

//! \brief The interface class for max version 8.
/*! This interface should always be 
	retrieved over Interface or Interface7 when programming against version 8
	of the application.  It features new functionality and deprecates obsolete functions. */
class Interface8 : public Interface7
{
public:
	// ---------- Rendering ------------
	//! \name Rendering
	//@{

	//! \brief Launches a Quick Render, as though pressing the Quick Render button
	/*! \param[in] t The time to render the image.
	    Pass TIME_PosInfinity or TIME_NegInfinity to use the current slider time
	    \param[in] rendbm The bitmap to render to, or NULL for default handling.
	    If a bitmap is provided, the caller is responsible for displaying the VFB and saving the file.
	    \param[in] prog The RendProgressCallback is an optional callback.
	    Pass NULL for the default render progress dialog
	    \return The result of the render - Nonzero if success; otherwise 0. */
	virtual int QuickRender( TimeValue t=TIME_PosInfinity, Bitmap* rendbm=NULL, RendProgressCallback* prog=NULL )=0;
	//! \brief Enumerates the frames for rendering, as specified in the Render Dialog
	/*! \param[out] frameNums The frame number list; this is resized and set by the callee */
	virtual void GetRendFrameList(IntTab& frameNums)=0;
	//! \brief Returns the custom progress callback, if any, used for rendering through the UI.
	/*! If NULL, the renderer will use its default progress dialog */
	virtual RendProgressCallback* GetRendProgressCallback()=0;
	//! \brief Sets a custom progress callback, used for rendering through the UI.
	/*! This will be used when performing a render through the UI.
	    If set to NULL, the renderer will use its default progress dialog
	    \param[in] prog The custom progress callback */
	virtual void SetRendProgressCallback( RendProgressCallback* prog )=0;
	//! \brief Returns the custom camera node, if any, used for rendering through the UI.
	/*! If NULL, the renderer will render from a viewport by default */
	virtual INode* GetRendCamNode()=0;
	//! \brief Sets a custom camera node, used for rendering through the UI.
	/*! If NULL, the renderer will render from a viewport by default, but
	    this allows for specifying a perspective other than one of the viewports.
	    \param[in] camNode The custom camera node */
	virtual void SetRendCamNode( INode* camNode )=0;
	//! \brief Returns the "Put Image File List(s) in Output Path(s)" setting in the render dialog.
	virtual BOOL GetRendUseImgSeq()=0;
	//! \brief Sets the "Put Image File List(s) in Output Path(s)" setting in the render dialog.
	/*! When enabled, a sequence file (in .imsq or .ifl format) will be saved for frames stored
	    during the render, including separate sequence files for the frames of each render element
	    \param[in] onOff TRUE to enable writing of sequence files, FALSE to disable */
	virtual void SetRendUseImgSeq(BOOL onOff)=0;
	//! \brief Returns the file format used when the renderer creates image sequence files.
	/*! For .imsq files this is 0, otherwise for .ifl files this is 1 */
	virtual int GetRendImgSeqType()=0;
	//! \brief Sets the file format used when the renderer creates image sequence files.
	/*! \param[in] type The format type. For .imsq files pass 0, otherwise for .ifl files pass 1 */
	virtual void SetRendImgSeqType(int type)=0;
	//! \brief Saves image sequence files (in .imsq or .ifl format) based on the current output settings.
	/*! Equivalent to pressing the "Create Now" button for image sequence files in the render dialog
	    \param[in] cb An optional callback object to be called for each sequence file created */
	virtual void CreateRendImgSeq( CreateRendImgSeqCallback* cb=NULL )=0;
	//! \brief Returns the filename of the Pre-Render Script as indicated in the render dialog
	virtual MSTR GetPreRendScript()=0;
	//! \brief Sets the filename of the Pre-Render Script as shown in the render dialog.
	/*! For non-network rendering, the script is executed once before rendering begins.
	    For network rendering, if the Execute Locally is enabled, the script is executed once, before submission;
	    otherwise the script is executed once on each remote machine the job is assigned to.
	    The script is executed before any pre-render notifcations, but after sequence files (.imsq or .ifl) are written
	    \param[in] script The filename to set for the the pre-render script */
	virtual void SetPreRendScript( const MSTR& script )=0;
	//! \brief Returns the Enable setting for the Pre-Render Script in the render dialog
	virtual BOOL GetUsePreRendScript()=0;
	//! \brief Sets the Enable setting for the Pre-Render Script in the render dialog.
	/*! \param[in] onOff TRUE to enable the pre-render script, FALSE to disable */
	virtual void SetUsePreRendScript( BOOL onOff )=0;
	//! \brief Returns the Execute Locally setting for the Pre-Render Script in the render dialog
	virtual BOOL GetLocalPreRendScript()=0;
	//! \brief Sets the Execute Locally setting for the Pre-Render Script in the render dialog.
	/*! This setting affects network rendering, causing the script to be executed once before submission,
	    instead of once per machine the job is assigned to.
	    \param[in] onOff TRUE to enable local execution of the pre-render script, FALSE to disable */
	virtual void SetLocalPreRendScript( BOOL onOff )=0;
	//! \brief Returns the filename of the Post-Render Script as indicated in the render dialog
	virtual MSTR GetPostRendScript()=0;
	//! \brief Sets the filename of the Post-Render Script as shown in the render dialog.
	/*! The script is executed after any post-render notifcations.
	    \param[in] script The filename to set for the the post-render script */
	virtual void SetPostRendScript( const MSTR& script )=0;
	//! \brief Returns the Enable setting for the Post-Render Script in the render dialog
	virtual BOOL GetUsePostRendScript()=0;
	//! \brief Sets the Enable setting for the Post-Render Script in the render dialog.
	/*! \param[in] onOff TRUE to enable the post-render script, FALSE to disable */
	virtual void SetUsePostRendScript( BOOL onOff )=0;
	//@}

	// ---------- Animation Preferences ------------

	//! \name Animation
	//@{
	//! \brief Retrieves whether or not newly created controllers will respect animation range
	/*!	Retrieves the default value of the animation preference which determines 
		whether the active range of parametric controllers will be respected or not.
		This preference does not affect keyable controllers.
		When range is respected, the controller evaluation at a time before the
		activation range start time will return the value at start time and 
		evaluation after activation range end time, the value at end time.
		Users can see the activation range of controllers as a black line in the 
		Track View, in Dope Sheet mode, when Edit Ranges is on.

		\return - TRUE if by default, the animation range of controllers is not respected, FALSE otherwise */
	virtual BOOL GetControllerOverrideRangeDefault() = 0;
	//! \brief Sets whether or not newly created controllers will respect animation range
	/*! Sets the default value of the animation preference which determines whether 
		the active range of parametric controllers will be respected or not
		\see GetControllerOverrideRangeDefault()
		\param override - If FALSE, the active range of parametric controllers will 
		be respected by default, otherwise it won't */
	virtual void SetControllerOverrideRangeDefault(BOOL override) = 0;

	/*! Get the default tangent type for both the "In" and the "Out" tangent.
		This tangent type is the one that gets applied to any new animation key created in Max.
		\param[out] dfltInTangentType - default type for the "In" tangent.
		\param[out] dfltOutTangentType - default type for the "Out" tangent. */
	virtual void GetDefaultTangentType(int &dfltInTangentType, int &dfltOutTangentType) = 0;
	/*! Set the default tangent type for both the "In" and the "Out" tangent.
		This tangent type will get set on any animation key created in Max.
		\param[in] dfltInTangentType - default type for the "In" tangent.
		\param[in] dfltOutTangentType - default type for the "Out" tangent.
		\param[in] writeInCfgFile - TRUE if tangent type values have to be written in the config file, FALSE otherwise. */
	virtual void SetDefaultTangentType(int dfltInTangentType, int dfltOutTangentType, BOOL writeInCfgFile = TRUE) = 0;

	//! \brief Returns whether quick manipulation mode for spring controllers is on
	/*!	Retrieves the animation preference controlling whether spring systems
		used in spring controllers are in quick edit mode.  The default is to
		be OFF.  If turned on, then when something invalidates a spring 
		controller, instead of recomputing the results from start as would be
		required to get the correct results, it resets the system a certain
		number of frames back (see GetSpringRollingStart() below).  This can
		make a big difference in interactivity.
		\return - TRUE if the spring controllers are in quick edit mode */
	virtual BOOL GetSpringQuickEditMode() const = 0;
	//! \brief Sets quick manipulation mode for spring controllers 
	/*!	Sets the animation preference controlling whether spring systems used
		in spring controllers will be accurate at all times.
		\param[in] in_quickEdit - turn on spring quick edit mode. */
	virtual void SetSpringQuickEditMode(BOOL in_quickEdit) = 0;

	//! \brief Sets the rolling start value of the quick manipulation mode for spring controllers 
	/*!	Sets the animation preference controlling how many frames back the 
		spring controllers will use as a rolling starting point for simulation 
		if invalidated, if the Quick Edit option is on (see SetSpringQuickEditMode
		above).
		\param[in] in_start - the number of frames back when restarting.
		\see SetSpringQuickEditMode */
	virtual void SetSpringRollingStart(int in_start) = 0;
	//! \brief Returns the rolling start value of the quick manipulation mode for spring controllers 
	/*!	Returns the animation preference controlling how many frames back the
		spring controllers will use as a rolling starting point for simulation 
		\see GetSpringQuickEditMode */
	virtual int GetSpringRollingStart() const = 0;
	//@}

	//! \brief This method returns the color corresponding to the id in input

	/*! This function represents the color-id mapping that is done for :
	 \li The Material Id render element
	 \li The Object Id render element
	 \li The Material Effects and Object channels of the rpf format
	 \param[in] id - the id for which we want the color
	 \param[out] c - the color corresponding to the id
	*/
	virtual void ColorById(DWORD id, Color& c) = 0 ;

	//! Register a callback object to be called during max shutdown.
	//! \see ExitMAXCallback2, ExitMAXCallback
	//! \param[in] cb - the ExitMAXCallback2 object
	virtual void RegisterExitMAXCallback(ExitMAXCallback2 *cb)=0;

	//! Unregister a callback object to be called during max shutdown.
	//! \param[in] cb - the ExitMAXCallback2 object
	virtual void UnRegisterExitMAXCallback(ExitMAXCallback2 *cb)=0;

	//! \brief Opens a max-style File Save As dialog for generic file types.

	//! Launches a generic File Save As dialog which supports arbitrary file
	//! types.  The dialog includes max-browsing features such as browse
	//! history.  
	//! \post filename contains the long filename user selection if this function
	//! returns "true"; initialDir contains the path of the file selected, if this
	//! function returns \b true, otherwise it contains the original contents passed-in
	//! \param[in] parentWnd The window handle which should be this dialog's parent window.
	//! \param[in] title The string to be set as the title of this dialog.
	//! \param[in, out] filename As an in parameter, can contain the long or short name
	//! of the file that should be default selection of the dialog.  As an out parameter,
	//! filename contains the long name of the file selected by the user, if this function
	//! returns \b true
	//! \param[in, out] initialDir As an in parameter, contains the initial dialog 
	//! directory.  If the user clicks OK, then this parameter contains the user
	//! selected directory path.
	//! \param[in] extensionList A list of extensions supported by this dialog.  See
	//! FilterList documentation for details.
	//! \return \b true if the user makes an acceptable choice, false if canceled
	virtual bool DoMaxSaveAsDialog(	HWND parentWnd, 
									const MSTR &title, 
									MSTR &filename, 
									MSTR &initialDir, 
									const FilterList &extensionList) =0;

	//! \brief Opens a max-style Open dialog for generic file types.

	//! Launches a generic Open dialog which supports arbitrary file
	//! types.  The dialog includes max-browsing features such as browse
	//! history.  
	//! \post filename contains the long filename user selection if this function
	//! returns \b true; initialDir contains the path of the file selected, if this
	//! function returns "true", otherwise it contains the original contents passed-in
	//! \param[in] parentWnd The window handle which should be this dialog's parent window.
	//! \param[in] title The string to be set as the title of this dialog.
	//! \param[in, out] filename As an in parameter, can contain the long or short name
	//! of the file that should be default selection of the dialog.  As an out parameter,
	//! filename contains the long name of the file selected by the user, if this function
	//! returns \b true
	//! \param[in, out] initialDir As an in parameter, contains the initial dialog 
	//! directory.  If the user clicks OK, then this parameter contains the user
	//! selected directory path.
	//! \param[in] extensionList A list of extensions supported by this dialog.  See
	//! FilterList documentation for details.
	//! \return \b true if the user makes an acceptable choice, false if canceled
	virtual bool DoMaxOpenDialog(	HWND parentWnd, 
									const MSTR &title,
									MSTR &filename, 
									MSTR &initialDir, 
									const FilterList &extensionList) =0;

	//! \brief Makes a window modeless during a render.

	//! When a render is in progress, window messages to all windows but the virtual frame buffer
	//! and the progress window are supressed in order to make the render operation modal.
	//! This method may be used to make a window modeless during the render operation.
	//! All messages sent to the given window handle will no longer be supressed by the render
	//! executer.
	//!
	//! Note: A modeless render dialog should, ideally, only display certain statistics or messages.
	//! It is unsafe to do any complex operation or user interaction from a modeless dialog while
	//! rendering. That is why 3ds max blocks most window messages while rendering by default.
	//!
	//! PS: Be sure to un-register your window with UnRegisterModelessRenderWindow() when
	//! it is destroyed.
	//! \param[in] hWnd Handle to the window to be registered.
	virtual void RegisterModelessRenderWindow(HWND hWnd) =0;
	//! \brief Un-registers a window registered with RegisterModelessRenderWindow().
	//! See the documentation of RegisterModelessRenderWindow() for more details.
	//! \param[in] hWnd Handle to the window to be un-registered.
	virtual void UnRegisterModelessRenderWindow(HWND hWnd) =0;

	//! \brief Returns whether a file save operation is currently in progress.
	//! \return true if a file save operation is in progress.
	virtual bool IsSavingToFile() = 0;

	//! \brief Flags for LoadFromFile method

	/*! \see Interface8::LoadFromFile
	*/
	enum LoadFromFileFlags {
		/*! redraw the viewports once the file is loaded */
		kRefreshViewports	= 0x01,
		/*! will suppress the any file units or obsolete file dialogs */
		kSuppressPrompts	= 0x02,
		/*! Is only valid in conjunction with kSuppressPrompts. If the new file's 
			units differ, setting kUseFileUnits will cause the System's unit scale to
			be changed to the file's unit scale.  Otherwise, the file's objects will be 
			rescaled to the current System's unit scale.
		*/
		kUseFileUnits		= 0x04
	};

	//! \brief Loads the specified scene file.

	//! \see Interface8::LoadFromFileFlags
	//! \param[in] szFilename the file to load
	//! \param[in] lFlags combination of Interface8::LoadFromFileFlags flags.
	//! \return true if success, false if there was an error
	virtual bool LoadFromFile( const MCHAR* szFilename, unsigned long lFlags ) = 0;

	//! \brief Opens a Windows Explorer window pointing to the passed in path

	//! This is a utility function for opening a Windows Explorer window with
	//! a given path.  The path can point to a folder, or it can be a full path
	//! to a file.
	//! If the file path doesn't exist, a warning dialog will open indicating that the path
	//! does not exist and the explorer will not open.
	//! The explorer is executed using a ShellExecute command, and is independent of
	//! the calling application.  In other words, shutting down max will not result in the 
	//! explorer app being shut down.
	//! \pre An absolute path value <= 256 characters.  A zero length path will result in the explorer
	//! opening to the system-default drive (most likely C:\).  An invalid path will result
	//! in a warning dialog, as described above.
	//! \post An explorer window opens to the location specified by the path.
	//! \param[in] path An absolute path for the location used as the default browse location of the explorer.
	//! \return true if shell function returns a success code
	virtual bool RevealInExplorer(const MSTR& path) =0;

protected:

	//! \name Path Configuration deprecated functions
	//@ { 
	//! \brief This function has been deprecated.  See IPathConfigMgr.

	/*! Path Configuration - see IPathConfigMgr
		The following methods are now grouped under the new interface IPathConfigMgr
		See this class for usage details and new functionality.
	*/
	virtual const MCHAR *GetDir(int which)=0;		
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual int	GetPlugInEntryCount()=0;	
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual const MCHAR *GetPlugInDesc(int i)=0;	
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual const MCHAR *GetPlugInDir(int i)=0;	
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual int GetMapDirCount(BOOL xref=FALSE)=0;			
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual const MCHAR *GetMapDir(int i, BOOL xref=FALSE)=0;		
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual BOOL AddMapDir(const MCHAR *dir, BOOL xref=FALSE)=0;	
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual BOOL AddSessionMapDir(const MCHAR *dir, int update=TRUE, BOOL xref=FALSE)=0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual int GetSessionMapDirCount(BOOL xref=FALSE)=0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual const MCHAR *GetSessionMapDir(int i, BOOL xref=FALSE)=0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual BOOL DeleteSessionMapDir(int i, int update=TRUE, BOOL xref=FALSE)=0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual int GetCurMapDirCount(BOOL xref=FALSE)=0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual const MCHAR *GetCurMapDir(int i, BOOL xref=FALSE)=0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual BOOL SetDir(int which, MCHAR *dir) = 0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual BOOL AddMapDir(MCHAR *dir, int update=TRUE, BOOL xref=FALSE) = 0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual BOOL DeleteMapDir(int i, int update=TRUE, BOOL xref=FALSE) = 0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual void UpdateMapSection(BOOL xref=FALSE) = 0;
	//! \brief This function has been deprecated.  See IPathConfigMgr.
	virtual MSTR GetMAXIniFile() = 0; 
	//@}
}; // Interface8

//! \brief The interface class for max version 9.
/*! This interface should always be 
	retrieved over Interface, Interface7, or Interface8 when programming against version 9
	of the application.  It features new functionality and deprecates obsolete functions. */
class Interface9 : public Interface8	{
public:
	//! \brief Opens a application-style Open dialog for generic file types.
	/*!	Launches the standard Browse For Folder dialog in max.
		\param aOwner[in] The handle to the window to whom this browse
		dialog should be a child to.
		\param aInstructions[in] An parameter for specifying user instructions
		which will appear above the main selection area.  These instructions can span
		multiple lines.  If this parameter is an empty string, then default instructions 
		will be specified.
		\param aDir[in, out] This parameter is used to specify both the initial 
		browse directory, and the user selection.  If the user cancels the selection,
		then this variable will not be modified.  If this parameter is an empty string, 
		or if a non-existing directory is passed in, the Windows control will default to
		the My Documents directory.
		\return true if the user accepts a selection, false if the user cancels
	 */
	virtual bool DoMaxBrowseForFolder(
		HWND aOwner, 
		const MSTR& aInstructions,
		MSTR& aDir) = 0;

	//!	\brief Deletes the specified nodes from the scene.
	/*!	Call this method to delete the specific nodes from the scene. The result of
	this operation is the same as deleting the nodes interactively or via maxscript's
	"delete <node>" command. If you need to delete individual nodes, use Interface::DeleteNode.
	\li The children of the deleted nodes will maintain their positon. 
	\li Empty group heads resulting from the deletion of the nodes will be deleted. 
	\li If a system node is deleted, the whole system is deleted.
	\li If target light\camera nodes are deleted, their target nodes are also deleted.
	\li If the target of a target light\camera node is deleted, the light\camera node is also deleted.
	\li If a closed group\assembly head node is deleted, the whole group\assembly 
	is deleted.
	\li The selection state and deletion of the nodes will be undo-able after calling 
	this method, provided that the undo	system was engaged.
	\param [in] aNodes The nodes to be deleted. This array is expanded with other nodes
	based on the criteria described above. After this method returns, the nodes in this 
	array are invalid.
	\param [in] aKeepChildTM If true, the child nodes of each deleted node keep their 
	world transformation. Note that this modifies each child node's transformation matrix.
	If false, the node's transformation matrix is not modified, but that means they'll
	get transformed relative to the world since their parent node is changing.
	\param [in] aRedraw If false the viewports will not be redrawn after the nodes  
	are deleted. This allows client code to optimize the viewport redraw.
	\param [in] overrideSlaveTM If true, this method will delete nodes whose 
	TM controllers implement Control::PreventNodeDeletion() to return TRUE. 
	Control::PreventNodeDeletion() controls whether the node can be deleted from 
	the scene. This parameter allows a master controller to easily delete slaves nodes 
	if it needs to.
	\return The number of nodes that were deleted. 
	*/
	virtual unsigned int DeleteNodes(
		INodeTab& aNodes,			
		bool aKeepChildTM = true,
		bool aRedraw = true,
		bool overrideSlaveTM = false
	) = 0;
}; // Interface9

/// Methods that control an internal optimization related to reference message 
/// propagation. The optimization consists of preventing the same ref message 
/// from being sent repeatedly to the same refmaker. This optimization improves
/// the performance of interactive manipulation of complex IK rigs, but it is 
/// not completely reliable in the presence of some objects such as VIZWalls.
///
/// Call this method to turn the optimization On (value = true) or Off 
/// (value = false) at runtime. When 'commitToInit' is true, 'value' is saved
/// to the current market default file as 'DontRepeatRefMsg=<value>'
CoreExport void SetDontRepeatRefMsg(bool value, bool commitToInit);
/// Call this method to find out if the optimization is active or not
CoreExport bool DontRepeatRefMsg();
/// In order to apply the optimization to a notification session, the 
/// Pre\PostRefNotifyDependents methods have to bracket it.
/// IMPORTANT: The notification session should not contain GetValue calls.
CoreExport void PreRefNotifyDependents();
CoreExport void PostRefNotifyDependents();


/// Methods that control optimization for evaluating the transforms of complex rigs,
/// especially of those with the expose transform helper. Note that this optimization
/// may not work completely reliable with all objects and controllers
CoreExport void SetInvalidateTMOpt(bool value, bool commitToInit);
CoreExport bool GetInvalidateTMOpt();


#endif // __MAXAPI__

