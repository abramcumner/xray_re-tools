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
// FILE:        custcont.h
// DESCRIPTION: Custom Controls for Max
// AUTHOR:      Rolf Berteig
// HISTORY:     created 17 November, 1994
//**************************************************************************/

#ifndef __CUSTCONT__
#define __CUSTCONT__

#include "maxheap.h"
#include "winutil.h"
#include "buildver.h"


// Define CHILDWND_CUI to build MAX with a child-window based CUI system
#define CHILDWND_CUI 

// Standard Category for rollups
#define ROLLUP_CAT_SYSTEM 2000
#define ROLLUP_CAT_STANDARD 4000
#define ROLLUP_CAT_CUSTATTRIB 6000

CoreExport void InitCustomControls( HINSTANCE hInst );

#ifdef DESIGN_VER
// VIZ window tinting APIs
CoreExport void EnableWindowTinting(bool flag = true);
CoreExport void SetWindowTint(const HWND hwnd, const COLORREF color);
CoreExport void ChangeWindowTint(const HWND hwnd, const COLORREF color);
CoreExport COLORREF GetWindowTint(const HWND hwnd);
CoreExport COLORREF GetAncestorTint(const HWND hwnd);
#endif

// DS 9/29/00  -- Turnoff CustButton borders.
#define I_EXEC_CB_NO_BORDER					0xA000 // Pass this to CustButton::Execute() to turnoff borders, like in the toolbar

#define I_EXEC_CS_NO_BORDER					0xA001 // Pass this to CustStatus::Execute() to turnoff borders, like in the toolbar

#define I_EXEC_SPINNER_RESET				0xA002 // Set a spinner back to its Reset value


#define I_EXEC_SPINNER_IS_RESET_CHANGE  	0xA003	// When called during a CC_SPINNER_xxx messages, it will return true if 
													// the msg was triggered by a right-click reset

#define I_EXEC_SPINNER_ALT_DISABLE			0xA004 // laurent r : disable the alt key spinner behaviour 
#define I_EXEC_SPINNER_ALT_ENABLE			0xA005 // laurent r : enable the alt key spinner behaviour 

#define I_EXEC_SPINNER_ONE_CLICK_DISABLE	0xA006 // laurent r : diable the one click spinner behaviour with alt or ctl down
#define I_EXEC_SPINNER_ONE_CLICK_ENABLE		0xA007 // laurent r : enable the one clcik spinner behaviour with alt or ctl down

#define I_EXEC_BUTTON_DAD_ENABLE			0xA008 // enable or disable button drag & drop within a toolbar

// Values returned by DADMgr::SlotOwner()
#define OWNER_MEDIT_SAMPLE 		0
#define OWNER_NODE 				1
#define OWNER_MTL_TEX			2  //  button in mtl or texture
#define OWNER_SCENE				3  //  button in light, modifier, atmospheric, etc
#define OWNER_BROWSE_NEW		4
#define OWNER_BROWSE_LIB		5
#define OWNER_BROWSE_MEDIT		6
#define OWNER_BROWSE_SCENE		7

class ReferenceTarget;
class IParamBlock;
class IParamBlock2;
class FPInterface;

class DADMgr : public InterfaceServer {
	public:
		// Called on the source to see what if anything can be dragged from this x,y
		// returns 0 if can't drag anything from this point
		virtual SClass_ID GetDragType(HWND hwnd, POINT p)=0;


		// Return TRUE if creating instance witb "new", rather than returning
		// a pointer to an existing entity.
		// If GetInstance creates a new instance every time it is called, then IsNew should
		// return TRUE. This prevents GetInstance from being called repeatedly as the
		// drag progresses.
		virtual BOOL IsNew(HWND hwnd, POINT p, SClass_ID type) { return FALSE; } 

		// called on potential target to see if can drop type at this x,y
		virtual BOOL OkToDrop(ReferenceTarget *dropThis, HWND hfrom, HWND hto, POINT p, SClass_ID type, BOOL isNew = FALSE)=0;

		// called on potential target to allow it to substitute custom cursors. (optional)
		virtual HCURSOR DropCursor(ReferenceTarget *dropThis, HWND hfrom, HWND hto, POINT p, SClass_ID type, BOOL isNew = FALSE){ return NULL;}

		// Return one of the OWNER_ values above
		virtual int SlotOwner() { return OWNER_MTL_TEX; } 
		 
		// This should return a pointer to the drag source.  HWND is the window the mouse
		// down occured in, and p is the position in that window.  Type tells the expected
		// type of object.
		virtual ReferenceTarget *GetInstance(HWND hwnd, POINT p, SClass_ID type)=0;

		// This routine is called on the target with the pointer returned by the source's GetInstance,
		// or possibly a clone of it as the dropThis.   hwnd is where the mouse was released
		// for the drop, p is the position within hwnd, and type is the type of object
		// being dropped.
		virtual void  Drop(ReferenceTarget *dropThis, HWND hwnd, POINT p, SClass_ID type)=0;

		// This is called when the source and target WINDOW are the same
		virtual void  SameWinDragAndDrop(HWND h1, POINT p1, POINT p2) {}

		// This lets the manager know whether to call LocalDragAndDrop when the
		// same DADMgr handles both source and target windows.
		virtual BOOL  LetMeHandleLocalDAD() { return 0; }

		// This is called if the same DADMgr is handling both the source and target windows,
		// if LetMeHandleLocalDAD() returned true.
		virtual void  LocalDragAndDrop(HWND h1, HWND h2, POINT p1, POINT p2){}

		// If this returns true, the CustButtons that have this DADManager
		// will automatically make their text a tooltip
		virtual BOOL AutoTooltip(){ return FALSE; }

		// If a drag source doesn't want any references being made to the instance returned,
		// then this method should return true: it will force a copy to be made.
		virtual BOOL CopyOnly(HWND hwnd, POINT p, SClass_ID type) { return FALSE; } 

		// Normally the mouse down and mouse up messages are not sent to the 
		// source window when doing DAD, but if you need them, return TRUE
		virtual BOOL AlwaysSendButtonMsgsOnDrop(){ return FALSE; }

		// Generic expansion function
		virtual INT_PTR Execute(int cmd, ULONG_PTR arg1=0, ULONG_PTR arg2=0, ULONG_PTR arg3=0) { return 0; } 

		// called on potential target to see if can instance "dropThis" at this x,y
		virtual BOOL OkToDropInstance(ReferenceTarget *dropThis, HWND hfrom, HWND hto, POINT p, SClass_ID type) { return TRUE; }
	};


class ICustomControl : public InterfaceServer {
	public:
		virtual HWND GetHwnd()=0;
		virtual void Enable(BOOL onOff=TRUE)=0;
		virtual void Disable()=0;
		virtual BOOL IsEnabled()=0;
		// this second enable function is used to disable and enable custom controls
		// when the associated parameter has a non-keyframable parameter.
		// The effective enable state is the AND of these two enable bits.
		virtual void Enable2(BOOL onOff=TRUE)=0;
		// Generic expansion function
		virtual INT_PTR Execute(int cmd, ULONG_PTR arg1=0, ULONG_PTR arg2=0, ULONG_PTR arg3=0) { return 0; } 
	};

// This is a bitmap brush where the bitmap is a gray and white checker board.
CoreExport HBRUSH GetLTGrayBrush();
CoreExport HBRUSH GetDKGrayBrush();

// Makes the grid pattern brushes solid for screen shots
CoreExport void MakeBrushesSolid(BOOL onOff);

// The standard font...
// The fonts are available in two versions:
// 1. A fixed character set, dependent on the running build of 3ds max. An english build of 3ds max
//    will always generate a font that supports the english character set.
// 2. The local character set, dependent on the language of the operating system. If running 3ds max
//    on a Japanese version of Windows, this font will support the Japanese character set.
//    This font is used, for example, on buttons created by MessageBox() on a Japanese build
//    of Windows with an English build of 3ds max. In such cases, using GetFixedFont() will 
//    return a font that is unable to display the Japanese characters. GetFixedFont_LocalCharSet()
//    will return a font that is able to display Japanese character, but which may be different
//    in typeface and size than GetFixedFont().
//    You should usually not have to worry about the LocalCharSet() version, it is mostly for internal use.
//
CoreExport HFONT GetFixedFont();
CoreExport HFONT GetFixedFont_LocalCharSet();
CoreExport HFONT GetFixedFontBold();
CoreExport HFONT GetFixedFontBold_LocalCharSet();

// These routines provide access to the underlying parts of a font if
// needed to do your own manipulation.
//! \brief Returns the standard font height for UI objects
/*! 
		\return Font height (14 for English & European languages, 12 for Asian languages)
*/
CoreExport LONG GetFixedFontHeight();
//! \brief Returns the small font height for UI object
/*! 
		\return Font height (8 for English & European languages, 9 for Asian languages)
*/
CoreExport LONG GetFixedFontHeightSmall();
//! \brief Returns character set for the current language
/*! 
		\return character set for the current language
*/
CoreExport DWORD GetFixedFontCharset();
//! \brief Returns the font face string for the standard font.
/*! 
		\return Font face string for the standard font.
*/
CoreExport char* GetFixedFontFace();


// The hand cursor used for panning.
CoreExport HCURSOR GetPanCursor();

// Used to update the new mouse-tracking outlined buttons
CoreExport void UpdateButtonOutlines();

//----------------------------------------------------------------------------//
// Customizable UI Frame 

#define CUIFRAMECLASS _M("CUIFrame")

// CUI Frame content types
#define CUI_TOOLBAR			(1<<0)		// set if frame holds toolbars and/or tool palettes
#define CUI_MENU			(1<<1)		// set if frame holds a menu
#define CUI_HWND			(1<<2)		// set if frame hold a generic hWnd

// CUI Frame position types
#define CUI_TOP_DOCK		(1<<0)
#define CUI_BOTTOM_DOCK		(1<<1)
#define CUI_LEFT_DOCK		(1<<2)
#define CUI_RIGHT_DOCK		(1<<3)
#define CUI_ALL_DOCK		(CUI_TOP_DOCK|CUI_BOTTOM_DOCK|CUI_LEFT_DOCK|CUI_RIGHT_DOCK)
#define CUI_HORIZ_DOCK		(CUI_TOP_DOCK|CUI_BOTTOM_DOCK)
#define CUI_VERT_DOCK		(CUI_LEFT_DOCK|CUI_RIGHT_DOCK)
#define CUI_FLOATABLE		(1<<4)
#define CUI_FLOATING		(1<<4)	// synonym
#define CUI_CONNECTABLE		(1<<5)
#define CUI_SM_HANDLES		(1<<6)		// set if frame should display size/move handles
// defined for release 4.5 and later only
#define CUI_SLIDING			(1<<7)		// the frame doesn't butt up against the one next to it.
#define CUI_MAX_SIZED		(1<<8)		// the frame takes up the entire row.  Nothing can be docked next to it.
#define CUI_DONT_SAVE		(1<<9)		// Don't save this CUI frame in the .cui file
#define CUI_HAS_MENUBAR		(1<<10)		// CUI frames that have a menu bar need to be treated differently

// CUI Docking Panel locations
#define CUI_NO_PANEL		0
#define CUI_TOP_PANEL		CUI_TOP_DOCK
#define CUI_BOTTOM_PANEL	CUI_BOTTOM_DOCK
#define CUI_LEFT_PANEL		CUI_LEFT_DOCK
#define CUI_RIGHT_PANEL		CUI_RIGHT_DOCK
#define CUI_FIXED_PANELS	(CUI_TOP_PANEL|CUI_BOTTOM_PANEL|CUI_LEFT_PANEL|CUI_RIGHT_PANEL)
#define CUI_FLOATING_PANELS (1<<4)
#define CUI_ALL_PANELS		(CUI_FIXED_PANELS|CUI_FLOATING_PANELS)

#define CUI_POSDATA_MSG				(WM_APP + 0x3412)	// used for retrieving CUIFrame position data
#define CUI_SUBFRAME_ADDED_MSG		(WM_APP + 0x3413)	// tells a parent window that an ICUIFrame has been added
#define CUI_SUBFRAME_REMOVED_MSG	(WM_APP + 0x3414)	// tells a parent window that an ICUIFrame has been removed
#define CUI_PRESET_MACROBUTTONS		(WM_APP + 0x3415)	// Set MacroButtonStates is about to be called on the toolbar.
#define CUI_SUBFRAME_ACTIVATE_MSG	(WM_APP + 0x3416)	// tells a parent window that a subframe's active state has changed

// orientation parameters
#define CUI_NONE			0
#define CUI_HORIZ			CUI_HORIZ_DOCK
#define CUI_VERT			CUI_VERT_DOCK
#define CUI_FLOAT			CUI_FLOATING

#define CUI_MIN_TB_WIDTH	25		// minimum width of a CUIFrame-based toolbar

#define CUI_MENU_HIDE			0
#define CUI_MENU_SHOW_ENABLED	1
#define CUI_MENU_SHOW_DISABLED	2

// CUI size parameters
#define CUI_MIN_SIZE			0
#define CUI_MAX_SIZE			1
#define CUI_PREF_SIZE			2

// CUI bitmap button image size (in pixels: 16x15 or 24x24)
#define CUI_SIZE_16				16
#define CUI_SIZE_24				24

// CUI bitmap button image mask options
#define CUI_MASK_NONE			0	// no mask -- MAX should generate one
#define CUI_MASK_MONO			1	// normal Windows convention
#define CUI_MASK_ALPHA			2	// 8-bit alpha channel present
#define CUI_MASK_ALPHA_PREMULT	3	// 8-bit pre-multiplied alpha channel present

// CUI edit types -- not all implemented (yet?)
#define CUI_EDIT_NONE			0
#define CUI_EDIT_KBD			(1<<0)
#define CUI_EDIT_SCRIPT			(1<<1)
#define CUI_EDIT_MACRO			(CUI_EDIT_KBD | CUI_EDIT_SCRIPT)
#define CUI_EDIT_ORDER			(1<<2)

class CUIPosData: public MaxHeapOperators 
{
public:
	virtual ~CUIPosData()									{}
	virtual int GetWidth(int sizeType, int orient)			{ return 50; }
	virtual int GetHeight(int sizeType, int orient)			{ return 50; }
};

// Provides a way for messages received by the CUIFrame to be processed
// in a context-specific fashion.  ProcessMessage should return TRUE 
// if the message is handled and FALSE if not.  If FALSE is returned (or
// no handler is defined), then the CUIFrame simply passes WM_COMMAND
// messages on to its parent.  Window position messages are passed from
// the CUIFrame to the HWND of the 'content' (either toolbar or menu)
// Other messages are passed on to DefaultWndProc.

class CUIFrameMsgHandler: public MaxHeapOperators
{
public:
	virtual int ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) { return FALSE; }
};

class ICustButton;
class ICustStatus;

class ICUIFrame : public ICustomControl {
public:
	// CUIFrame attributes
	virtual	void SetPosType(DWORD t)=0;
	virtual	DWORD GetPosType()=0;
	virtual void SetPosRank(int rank, int subrank=0)=0;
	virtual int GetPosRank()=0;
	virtual int GetPosSubrank()=0;
	virtual BOOL IsFloating()=0;
	virtual void Hide(BOOL b)=0;
	virtual BOOL IsHidden()=0;
	virtual void SetCurPosition(DWORD pos)=0;
	virtual DWORD GetCurPosition()=0;
	virtual void SetContentType(DWORD t)=0;
	virtual DWORD GetContentType()=0;
	virtual void SetContentHandle(HWND hContent)=0;
	virtual HWND GetContentHandle()=0;
	virtual void SetTabbedToolbar(BOOL b)=0;
	virtual BOOL GetTabbedToolbar()=0;
	virtual void AddToolbarTab(HWND hTBar, CUIFrameMsgHandler *msgHandler, MCHAR *name, int pos = -1)=0;
	virtual void DeleteToolbarTab(int pos)=0;
	virtual int GetToolbarCount()=0;
	virtual HWND GetToolbarHWnd(int pos)=0;
	virtual MCHAR *GetTabName(int pos)=0;
	virtual void SetCurrentTab(int pos)=0;
	virtual int GetCurrentTab()=0;
	virtual int GetSize(int sizeType, int dir /* width or height */, int orient)=0;
	virtual BOOL InstallMsgHandler(CUIFrameMsgHandler *msgHandler)=0;
	virtual void SetName(MCHAR *name)=0;	// name is used to store position info
	virtual MCHAR *GetName()=0;
	virtual BOOL SetMenuDisplay(int md)=0;
	virtual int GetMenuDisplay()=0;
	virtual void SetSystemWindow(BOOL b)=0;	// set to TRUE for main UI, only works if parent is the main MAX hWnd
	virtual BOOL GetSystemWindow()=0;
	virtual BOOL ReadConfig(MCHAR *cfg, int startup=FALSE)=0;	// returns FALSE if no position has been saved
	virtual void WriteConfig(MCHAR *cfg)=0;
};

CoreExport ICUIFrame *GetICUIFrame( HWND hCtrl );
CoreExport void ReleaseICUIFrame( ICUIFrame *icf );


CoreExport HWND CreateCUIFrameWindow(HWND hParent, MCHAR *title, int x, int y, int cx, int cy);

#define CUI_MODE_NORMAL		0
#define CUI_MODE_EDIT		1

class CUIFrameMgr : public BaseInterfaceServer {
private:
	HWND	hApp;
	void	RecalcPanel(int panel);
	int		GetMaxRank(int panel);
	int		GetMaxSubrank(int panel, int rank);
	void	GetDockRect(int panel, int rank, RECT *rp);
	int		GetDockingRank(int panel, RECT *rp = NULL);
	int		GetDockingSubrank(int panel, int rank, RECT *rp = NULL);
	void	AdjustRanks(int panel, int start, int incr);
	void	AdjustSubranks(int panel, int rank, int start, int incr);
	int		resSize[4];
	int		mode;
	int		horizTextBtns;
	int		fixedWidthTextBtns;
	int		btnWidth;
	int		imageSize;
	int		lockLayout;
	CUIFrameMsgHandler *defMsgHandler;
protected:
	MSTR	cfgFile;
	//! \brief Constructor made protected to prevent instantiation
	/*! Blocking default constructor - use GetCUIFrameMgr() accessor
	*/
	CoreExport CUIFrameMgr();
	//! \brief Copy Constructor made protected to prevent instantiation
	/*! Blocking Copy constructor - use GetCUIFrameMgr() accessor
	*/
	CUIFrameMgr(CUIFrameMgr& frame);
	CoreExport virtual ~CUIFrameMgr();

public:
	CoreExport void	SetAppHWnd(HWND hApp);
	HWND			GetAppHWnd() { return hApp; }
	CoreExport MCHAR *GetCUIDirectory();

	CoreExport void ProcessCUIMenu(HWND hWnd, int x, int y);
	CoreExport void DockCUIWindow(HWND hWnd, int panel, RECT *rp = NULL, int init = FALSE);
	CoreExport void FloatCUIWindow(HWND hWnd, RECT *rp = NULL, int init = FALSE);

	CoreExport void SetReservedSize(int panel, int size);
	CoreExport int GetReservedSize(int panel);
	CoreExport int GetPanelSize(int panel, int incReserved = FALSE);
	CoreExport int GetPanelWidth(int panel);  // R4.5 and later only

	CoreExport void RecalcLayout(int entireApp=FALSE);
	CoreExport void DrawCUIWindows(int panels=CUI_ALL_PANELS);
    CoreExport void SetMacroButtonStates(BOOL force);
	CoreExport void ResetIconImages();

	CoreExport int OverDockRegion(LPPOINT pt, DWORD posType, int override = FALSE);
	CoreExport void SetMode(int md);
	CoreExport int GetMode();

	CoreExport void ExpertMode(int onOff);

	CoreExport void HideFloaters(int onOff);
	CoreExport HWND GetItemHwnd(int id);
	CoreExport ICustButton *GetICustButton( int id );
	CoreExport ICustStatus *GetICustStatus( int id );

	CoreExport void HorizTextButtons(BOOL b);
	CoreExport int GetHorizTextButtons();
	CoreExport void FixedWidthTextButtons(BOOL b);
	CoreExport int GetFixedWidthTextButtons();
	CoreExport void SetTextButtonWidth(int w);
	CoreExport int GetTextButtonWidth();

	CoreExport int GetCount();
	CoreExport ICUIFrame *GetICUIFrame(int i);
	CoreExport ICUIFrame *GetICUIFrame(MCHAR *name);
	CoreExport ICUIFrame *GetICUIFrame(int panel, int rank, int subrank);

	CoreExport int SetConfigFile(MCHAR *cfg);
	CoreExport MCHAR *GetConfigFile();

	CoreExport int DeleteSystemWindows(int toolbarsOnly = TRUE);
	CoreExport int CreateSystemWindows(int reset = FALSE);
	//Release 5.0 and later only
	CoreExport int GetSystemWindowCount();

	CoreExport void SetImageSize(int size)	{ imageSize = size; }
	CoreExport int GetImageSize()			{ return imageSize; }
	CoreExport int GetButtonHeight(int sz=0) { if(!sz) sz=imageSize; return sz==CUI_SIZE_16 ? 22 : 31; }
	CoreExport int GetButtonWidth(int sz=0)  { if(!sz) sz=imageSize; return sz==CUI_SIZE_16 ? 23 : 32; }

	CoreExport void SetDefaultData(CUIFrameMsgHandler *msg, HIMAGELIST img16, HIMAGELIST img24=NULL);

	CoreExport int GetDefaultImageListBaseIndex(SClass_ID sid, Class_ID cid);
	CoreExport MSTR* GetDefaultImageListFilePrefix(SClass_ID sid, Class_ID cid);

	CoreExport int AddToRawImageList(MCHAR* pFilePrefix, int sz, HBITMAP image, HBITMAP mask);

	CoreExport int LoadBitmapFile(MCHAR *filename);
	CoreExport int LoadBitmapImages();

	CoreExport CUIFrameMsgHandler *GetDefaultMsgHandler()	{ return defMsgHandler; }

	CoreExport int ReadConfig();
	CoreExport int WriteConfig();

	CoreExport void SetLockLayout(BOOL lock)	{ lockLayout = lock; }
	CoreExport BOOL GetLockLayout()				{ return lockLayout; }

	CoreExport void EnableAllCUIWindows(int enabled);

	//! \brief Given a configuration filename, will attempt to find the best match
	/*! If the application is configured to use User Profiles, this function will attempt
	    to match the filename in the user profile UI directory.  If this fails, it will
		check the system directory.  
		\see IPathConfigMgr::IsUsingProfileDirectories()
		\see IPathConfigMgr::IsUsingRoamingProfiles()
		\param aFilename [in] the filename to match, with extension
		\param aResult [out] the resulting absolute path for the matched file, if found
		\return true if a match is found, false otherwise
	*/
	CoreExport virtual bool ResolveReadPath(const MSTR& aFilename, MSTR& aResult) = 0;
	//! \brief Given a configuration filename, will resolve the correct write absolute path
	/*! If the application is configured to use User Profiles, this function map this configuration
		file to a user profile directory.  Otherwise, the configuration file will be resolved to
		the legacy system UI directory.
		\see IPathConfigMgr::IsUsingProfileDirectories()
		\see IPathConfigMgr::IsUsingRoamingProfiles()
		\param aFilename [in] the filename to match, with extension
		\param aResult [out] the resulting absolute path to which a client should write a config file
		\return true if resolved correctly, false if any error is encountered
	*/
	CoreExport virtual bool ResolveWritePath(const MSTR& aFilename, MSTR& aResult) = 0;

};

CoreExport CUIFrameMgr *GetCUIFrameMgr();
CoreExport void DoCUICustomizeDialog();
CoreExport BOOL AllFloatersAreHidden();
CoreExport void ResizeFloatingTB(HWND hWnd);


#define MB_TYPE_KBD			        1
#define MB_TYPE_SCRIPT		        2
#define MB_TYPE_ACTION		        3
#define MB_TYPE_ACTION_CUSTOM		4

#define MB_FLAG_ENABLED           (1 << 0)
#define MB_FLAG_CHECKED           (1 << 1)

typedef DWORD ActionTableId;
typedef DWORD ActionContextId;
class ActionItem;

class MacroButtonData: public MaxHeapOperators {
public:
    CoreExport	MacroButtonData()	{ label = tip = imageName = NULL; imageID = -1; }
    // constructor for kbd-based macro buttons
    CoreExport	MacroButtonData(long tID, int cID, MCHAR *lbl, MCHAR *tp=NULL, int imID=-1, MCHAR *imName=NULL);
    CoreExport	MacroButtonData(int msID, MCHAR *lbl, MCHAR *tp=NULL, int imID=-1, MCHAR *imName=NULL)
        {
            macroType=MB_TYPE_SCRIPT; macroScriptID=msID; imageID=imID; 
            label=NULL; SetLabel(lbl); tip=NULL; SetTip(tp); imageName=NULL; SetImageName(imName);
        }
    CoreExport	~MacroButtonData();
    
    CoreExport	MacroButtonData & operator=(const MacroButtonData& mbd);
    
    CoreExport	void SetLabel(MCHAR *lbl);
    MCHAR *GetLabel()		{ return label; }
    CoreExport	void SetTip(MCHAR *tp);
    MCHAR *GetTip()			{ return tip; }
    void SetCmdID(int id)	{ cmdID = id; }
    int GetCmdID()			{ return cmdID; }
    void SetScriptID(int id){ macroScriptID = id; }
    int GetScriptID()		{ return macroScriptID; }
    CoreExport	void SetImageName(MCHAR *imName);
    MCHAR *GetImageName()	{ return imageName; }
    void SetImageID(int id)	{ imageID = id; }
    int GetImageID()		{ return imageID; }
    
    void SetTblID(ActionTableId id) { tblID = id; }
    ActionTableId GetTblID() { return tblID; }
    
    void SetActionItem(ActionItem* pAction) { actionItem = pAction; }
    ActionItem* GetActionItem() { return actionItem; }
    
    CoreExport BOOL IsActionButton() { return macroType == MB_TYPE_ACTION_CUSTOM ||
                                           macroType == MB_TYPE_ACTION; }
    
    int			  macroType;
    ActionTableId tblID; 
    int			  cmdID;
    int			  macroScriptID;
    MCHAR *		  label;
    MCHAR *		  tip;
    
    MCHAR *		  imageName;
    int			  imageID;
    ActionItem*   actionItem;


    // flags constains the last state when redrawing
    DWORD         flags;
};

//---------------------------------------------------------------------------//
// Spinner control


#define SPINNERWINDOWCLASS	_M("SpinnerControl")


// LOWORD(wParam) = ctrlID, 
// HIWORD(wParam) = TRUE if user is dragging the spinner interactively.
// lParam = pointer to ISpinnerControl
#define CC_SPINNER_CHANGE  		WM_USER + 600	

// LOWORD(wParam) = ctrlID, 
// lParam = pointer to ISpinnerControl
#define CC_SPINNER_BUTTONDOWN	WM_USER + 601

// LOWORD(wParam) = ctrlID, 
// HIWORD(wParam) = FALSE if user cancelled - TRUE otherwise
// lParam = pointer to ISpinnerControl
#define CC_SPINNER_BUTTONUP		WM_USER + 602


enum EditSpinnerType {
	EDITTYPE_INT, 
	EDITTYPE_FLOAT, 
	EDITTYPE_UNIVERSE, 
	EDITTYPE_POS_INT, 
	EDITTYPE_POS_FLOAT, 
	EDITTYPE_POS_UNIVERSE,
	EDITTYPE_TIME
	};

class ISpinnerControl : public ICustomControl {
	public:
		virtual float GetFVal()=0;
		virtual int GetIVal()=0;
		virtual void SetAutoScale(BOOL on=TRUE)=0;
		virtual void SetScale( float s )=0;
		virtual void SetValue( float v, int notify )=0;
		virtual void SetValue( int v, int notify )=0;
		virtual void SetLimits( int min, int max, int limitCurValue = TRUE )=0;
		virtual void SetLimits( float min, float max, int limitCurValue = TRUE )=0;
		virtual void LinkToEdit( HWND hEdit, EditSpinnerType type )=0;
		virtual void SetIndeterminate(BOOL i=TRUE)=0;
		virtual BOOL IsIndeterminate()=0;
		virtual void SetResetValue(float v)=0;
		virtual void SetResetValue(int v)=0;
		virtual void SetKeyBrackets(BOOL onOff)=0;
	};

CoreExport ISpinnerControl *GetISpinner( HWND hCtrl );
CoreExport void ReleaseISpinner( ISpinnerControl *isc );

CoreExport void SetSnapSpinner(BOOL b);
CoreExport BOOL GetSnapSpinner();
CoreExport void SetSnapSpinValue(float f);
CoreExport float GetSnapSpinValue();

CoreExport void SetSpinnerPrecision(int p);
CoreExport int GetSpinnerPrecision();

CoreExport void SetSpinnerWrap(int w);
CoreExport int GetSpinnerWrap();


// begin - mjm 12.18.98
//---------------------------------------------------------------------------
// Slider control

#define SLIDERWINDOWCLASS	_M("SliderControl")

// LOWORD(wParam) = ctrlID, 
// HIWORD(wParam) = TRUE if user is dragging the slider interactively.
// lParam = pointer to ISliderControl
#define CC_SLIDER_CHANGE  		WM_USER + 611

// LOWORD(wParam) = ctrlID, 
// lParam = pointer to ISliderControl
#define CC_SLIDER_BUTTONDOWN	WM_USER + 612

// LOWORD(wParam) = ctrlID, 
// HIWORD(wParam) = FALSE if user cancelled - TRUE otherwise
// lParam = pointer to ISliderControl
#define CC_SLIDER_BUTTONUP		WM_USER + 613

class ISliderControl : public ICustomControl
{
public:
	virtual float GetFVal()=0;
	virtual int GetIVal()=0;
	virtual void SetNumSegs( int num )=0;
	virtual void SetValue( float v, int notify )=0;
	virtual void SetValue( int v, int notify )=0;
	virtual void SetLimits( int min, int max, int limitCurValue = TRUE )=0;
	virtual void SetLimits( float min, float max, int limitCurValue = TRUE )=0;
	virtual void LinkToEdit( HWND hEdit, EditSpinnerType type )=0;
	virtual void SetIndeterminate(BOOL i=TRUE)=0;
	virtual BOOL IsIndeterminate()=0;
	virtual void SetResetValue(float v)=0;
	virtual void SetResetValue(int v)=0;
	virtual void SetKeyBrackets(BOOL onOff)=0;
};

CoreExport ISliderControl *GetISlider( HWND hCtrl );
CoreExport void ReleaseISlider( ISliderControl *isc );

// mjm - 3.1.99 - use spinner precision for edit boxes linked to slider controls
/*
CoreExport void SetSliderPrecision(int p);
CoreExport int  GetSliderPrecision();
*/

// routines for setting up sliders.
CoreExport ISliderControl *SetupIntSlider(HWND hwnd, int idSlider, int idEdit,  int min, int max, int val, int numSegs);
CoreExport ISliderControl *SetupFloatSlider(HWND hwnd, int idSlider, int idEdit,  float min, float max, float val, int numSegs);
CoreExport ISliderControl *SetupUniverseSlider(HWND hwnd, int idSlider, int idEdit,  float min, float max, float val, int numSegs);

// controls whether or not sliders send notifications while the user adjusts them with the mouse
CoreExport void SetSliderDragNotify(BOOL onOff);
CoreExport BOOL GetSliderDragNotify();
// end - mjm 12.18.98


//---------------------------------------------------------------------------//
// Rollup window control

#define WM_CUSTROLLUP_RECALCLAYOUT WM_USER+876

#define ROLLUPWINDOWCLASS _M("RollupWindow")

typedef void *RollupState;

// Flags passed to AppendRollup
#define APPENDROLL_CLOSED		(1<<0)	// Starts the page out rolled up.
#define DONTAUTOCLOSE    		(1<<1)	// Dont close this rollup when doing Close All
#define ROLLUP_SAVECAT    		(1<<2)	// Save the catory field in the RollupOrder.cfg
#define ROLLUP_USEREPLACEDCAT	(1<<3)	// In case of ReplaceRollup, use the replaced rollups category

class IRollupWindow;
class IRollupPanel;

class IRollupCallback : public InterfaceServer
{
public:
	virtual BOOL HandleDrop(IRollupPanel *src,IRollupPanel *targ, bool before){ return FALSE; }
	virtual BOOL GetEditObjClassID(SClass_ID &sid,Class_ID &cid){ return FALSE;}
	virtual BOOL HandleOpenAll(){return FALSE;}
	virtual BOOL HandleCloseAll(){ return FALSE;}
};

class IRollupPanel : public InterfaceServer
{
public:
	virtual HINSTANCE GetHInst()=0;
	virtual DWORD_PTR GetResID()=0;
	virtual BOOL operator==(const IRollupPanel& id)=0;
	virtual int GetCategory()=0;
	virtual void SetCategory(int cat)=0;
	virtual HWND GetHWnd()=0;
	virtual HWND GetRollupWindowHWND()=0;
	virtual HWND GetTitleWnd()=0;
	//! This function gets the main panel window handle.
	//! \return Returns the panel window handle
	virtual HWND GetPanelWnd()=0;

	//! This functions sets the height of this panel.
	//! \param[in] height  The height of the panel dlg.
	virtual void SetDlgHeight(int height)=0;

	//! This function gets the height of the panel.
	//! \return Returns the panel height.
	virtual int GetDlgHeight()=0;
};

class IRollupRCMenuItem : public InterfaceServer {
public:
	virtual MCHAR*		RRCMMenuText()=0;
	virtual void		RRCMExecute()=0;
	virtual bool		RRCMShowChecked()=0;
	virtual bool		RRCMHasSeparator()=0;
};

class IRollupWindow : public ICustomControl {
	public:
		// Shows or hides all
		virtual void Show()=0;
		virtual void Hide()=0;

		// Shows or hides by index
		virtual void Show(int index)=0;
		virtual void Hide(int index)=0;

		virtual HWND GetPanelDlg(int index)=0;
		virtual int GetPanelIndex(HWND hWnd)=0;
		virtual void SetPanelTitle(int index,MCHAR *title)=0;

		// returns index of new panel
		virtual int AppendRollup( HINSTANCE hInst, MCHAR *dlgTemplate, 
				DLGPROC dlgProc, MCHAR *title, LPARAM param=0,DWORD flags=0, int category = ROLLUP_CAT_STANDARD )=0;
		virtual int AppendRollup( HINSTANCE hInst, DLGTEMPLATE *dlgTemplate, 
				DLGPROC dlgProc, MCHAR *title, LPARAM param=0,DWORD flags=0, int category = ROLLUP_CAT_STANDARD )=0;
		virtual int ReplaceRollup( int index, HINSTANCE hInst, MCHAR *dlgTemplate, 
				DLGPROC dlgProc, MCHAR *title, LPARAM param=0,DWORD flags=0, int category = ROLLUP_CAT_STANDARD)=0;
		virtual int ReplaceRollup( int index, HINSTANCE hInst, DLGTEMPLATE *dlgTemplate, 
				DLGPROC dlgProc, MCHAR *title, LPARAM param=0,DWORD flags=0, int category = ROLLUP_CAT_STANDARD)=0;
		virtual void DeleteRollup( int index, int count )=0;
		virtual void SetPageDlgHeight(int index,int height)=0;

		virtual void SaveState( RollupState *hState )=0;
		virtual void RestoreState( RollupState *hState )=0;

		// Passing WM_LBUTTONDOWN, WM_MOUSEMOVE, and WM_LBUTTONUP to
		// this function allows scrolling with unused areas in the dialog.
		virtual void DlgMouseMessage( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )=0;

		virtual int GetNumPanels()=0;
		virtual BOOL IsPanelOpen(int index) = 0;
		virtual void SetPanelOpen(int index, BOOL isOpen, BOOL ignoreFlags = TRUE) =0;
		virtual int GetScrollPos()=0;
		virtual void SetScrollPos(int spos)=0;

		// This methods moves a RollupPanel to another RollupWindow. It either inserts it
		// at the top, or appends it at the end (depending on the top parameter)

		virtual void MoveRollupPanelFrom(IRollupWindow *from, HWND hPanel, BOOL top)=0;
		
		// Returns the Height of a RollupPanel
		virtual int GetPanelHeight(int index)=0;
		
		// Returns the Height of a RollupWindow, that it is longer than the visible area
		virtual int GetScrollHeight()=0;
		
		// Used internally
		virtual void UpdateLayout()=0;
		
		virtual IRollupPanel *GetPanel(HWND hWnd)=0;

		virtual void RegisterRollupCallback( IRollupCallback *callb)=0;
		virtual void UnRegisterRollupCallback( IRollupCallback *callb)=0;

		virtual void RegisterRCMenuItem( IRollupRCMenuItem *item)=0;
		virtual void UnRegisterRCMenuItem( IRollupRCMenuItem *item)=0;

		virtual void ResetCategories(bool update = true)=0;
	};

// This function returns TRUE if a particular rollup panel is open given
// a handle to the dialog window in the panel.
CoreExport BOOL IsRollupPanelOpen(HWND hDlg);

CoreExport IRollupWindow *GetIRollup( HWND hCtrl );
CoreExport void ReleaseIRollup( IRollupWindow *irw );

//----------------------------------------------------------------------------//
// CustEdit control

#define CUSTEDITWINDOWCLASS _M("CustEdit")

// Sent when the user hits the enter key in an edit control.
// wParam = cust edit ID
// lParam = HWND of cust edit control.
#define WM_CUSTEDIT_ENTER	(WM_USER+685)

class ICustEdit : public ICustomControl {
	public:
		virtual void GetText( MCHAR *text, int ct )=0;
		virtual void SetText( MCHAR *text )=0;	
		virtual void SetText( int i )=0;
		virtual void SetText( float f, int precision=3 )=0;
		virtual int GetInt(BOOL *valid=NULL)=0;
		virtual float GetFloat(BOOL *valid=NULL)=0;
		virtual void SetLeading(int lead)=0;
		virtual void WantReturn(BOOL yesNo)=0;
		virtual BOOL GotReturn()=0;		// call this on receipt of EN_CHANGE
		virtual void GiveFocus()=0;
		virtual BOOL HasFocus()=0;
		virtual void WantDlgNextCtl(BOOL yesNo)=0;
		virtual void SetNotifyOnKillFocus(BOOL onOff)=0;
		virtual void SetBold(BOOL onOff)=0;
		virtual void SetParamBlock(ReferenceTarget* pb, int subNum)=0;
	};

CoreExport ICustEdit *GetICustEdit( HWND hCtrl );
CoreExport void ReleaseICustEdit( ICustEdit *ice );

#define CUSTSTATUSEDITWINDOWCLASS _M("CustStatusEdit")

class ICustStatusEdit : public ICustomControl {
	public:
		virtual void GetText( MCHAR *text, int ct )=0;
		virtual void SetText( MCHAR *text )=0;	
		virtual void SetText( int i )=0;
		virtual void SetText( float f, int precision=3 )=0;
		virtual int GetInt(BOOL *valid=NULL)=0;
		virtual float GetFloat(BOOL *valid=NULL)=0;
		virtual void SetLeading(int lead)=0;
		virtual void WantReturn(BOOL yesNo)=0;
		virtual BOOL GotReturn()=0;		// call this on receipt of EN_CHANGE
		virtual void GiveFocus()=0;
		virtual BOOL HasFocus()=0;
		virtual void WantDlgNextCtl(BOOL yesNo)=0;
		virtual void SetNotifyOnKillFocus(BOOL onOff)=0;
		virtual void SetBold(BOOL onOff)=0;
		virtual void SetReadOnly(BOOL onOff)=0;
	};

CoreExport ICustStatusEdit *GetICustStatusEdit( HWND hCtrl );
CoreExport void ReleaseICustStatusEdit( ICustStatusEdit *ice );

//----------------------------------------------------------------------------//
// CustButton control

#define CUSTBUTTONWINDOWCLASS _M("CustButton")

#define CC_COMMAND  		WM_USER + 700
// send these with CC_COMMAND: wParam = CC_???
#define CC_CMD_SET_TYPE  		23		// lParam = CBT_PUSH, CBT_CHECK
#define CC_CMD_SET_STATE		24		// lParam = 0/1 for popped/pushed
#define CC_CMD_HILITE_COLOR		25		// lParam = RGB packed int

#define RED_WASH	RGB(255,192,192)
#define GREEN_WASH	(ColorMan()->GetColor(kActiveCommand))
#define BLUE_WASH	(ColorMan()->GetColor(kPressedHierarchyButton))
#define SUBOBJ_COLOR (ColorMan()->GetColor(kSubObjectColor))

enum CustButType { CBT_PUSH, CBT_CHECK };

// If the button is set to notify on button down, it will send a WM_COMMAND
// with this notify code when the user touches the button.
#define BN_BUTTONDOWN 	8173
// It will also send this message when the mouse is released regardless
// if the mouse is released inside the toolbutton rectangle
#define BN_BUTTONUP		8174

// If a button is set to notify on right clicks,  it will send a WM_COMMAND
// with this notify code when the user right clicks on the button.
#define BN_RIGHTCLICK 	8183

// When the user chooses a new fly-off item, this notify code will be sent.
#define BN_FLYOFF		8187


// When the user presses a button a WM_MENUSELECT message is sent so that
// the client can display a status prompt describing the function of
// the tool. The fuFlags parameter is set to this value:
#define CMF_TOOLBUTTON	9274

class MaxBmpFileIcon;

class FlyOffData {
	public:
		int iOutEn;
		int iInEn;
		int iOutDis;
		int iInDis;
        MaxBmpFileIcon* mpIcon;
        MaxBmpFileIcon* mpInIcon;
	};

// Directions the fly off will go.
#define FLY_VARIABLE	1
#define FLY_UP			2
#define FLY_DOWN		3
#define FLY_HVARIABLE	4 // horizontal variable
#define FLY_LEFT		5
#define FLY_RIGHT		6

typedef LRESULT CALLBACK PaintProc(HDC hdc, Rect rect, BOOL in, BOOL checked, BOOL enabled);

class ICustButton : public ICustomControl {
	public:
		virtual void GetText( MCHAR *text, int ct )=0;
		virtual void SetText( MCHAR *text )=0;			

		virtual void SetImage( HIMAGELIST hImage, 
                               int iOutEn, int iInEn, int iOutDis, int iInDis,
                               int w, int h )=0;
        // Alternate way to set an image on a button.   
        virtual void SetIcon ( MaxBmpFileIcon* pIcon, int w, int h) = 0;
        virtual void SetInIcon ( MaxBmpFileIcon* pInIcon, int w, int h) = 0;

		virtual void SetType( CustButType type )=0;
		virtual void SetFlyOff(int count,FlyOffData *data,int timeOut,
                               int init,int dir=FLY_VARIABLE, int columns=1)=0;

		virtual void SetCurFlyOff(int f,BOOL notify=FALSE)=0;
		virtual int GetCurFlyOff()=0;

		virtual BOOL IsChecked()=0;
		virtual void SetCheck( BOOL checked )=0;
		virtual void SetCheckHighlight( BOOL highlight )=0;

		virtual void SetButtonDownNotify(BOOL notify)=0;
		virtual void SetRightClickNotify(BOOL notify)=0;

		virtual void SetHighlightColor(COLORREF clr)=0;
		virtual COLORREF GetHighlightColor()=0;

		virtual void SetTooltip(BOOL onOff, LPSTR text)=0;

		virtual void SetDADMgr(DADMgr *dad)=0;
		virtual DADMgr *GetDADMgr()=0;

		virtual void SetMacroButtonData(MacroButtonData *md)=0;

		virtual MacroButtonData *GetMacroButtonData()=0;

		virtual void SetDisplayProc(PaintProc *proc)=0;

		virtual MCHAR* GetCaptionText(void)=0;
		virtual bool SetCaptionText(const MCHAR* text)=0;
	};

CoreExport ICustButton *GetICustButton( HWND hCtrl );
CoreExport void ReleaseICustButton( ICustButton *icb );


//---------------------------------------------------------------------------//
// CustStatus

#define CUSTSTATUSWINDOWCLASS _M("CustStatus")

enum StatusTextFormat {
	STATUSTEXT_LEFT,
	STATUSTEXT_CENTERED,
	STATUSTEXT_RIGHT };


class ICustStatus : public ICustomControl {
	public:
		virtual void SetText(MCHAR *text)=0;
		virtual void SetTextFormat(StatusTextFormat f)=0;
		virtual void GetText(MCHAR *text, int ct)=0;
		virtual void SetTooltip(BOOL onOff, LPSTR text)=0;
	};

CoreExport ICustStatus *GetICustStatus( HWND hCtrl );
CoreExport void ReleaseICustStatus( ICustStatus *ics );

//---------------------------------------------------------------------------//
// CustSeparator -- for use on toolbars

#define CUSTSEPARATORWINDOWCLASS _M("CustSeparator")

class ICustSeparator : public ICustomControl {
	public:
		virtual void SetVisibility(BOOL onOff)=0;
		virtual BOOL GetVisibility()=0;
	};

CoreExport ICustSeparator *GetICustSeparator( HWND hCtrl );
CoreExport void ReleaseICustSeparator( ICustSeparator *ics );


//----------------------------------------------------------------------------//
// CustToolbar control

#define CUSTTOOLBARWINDOWCLASS _M("CustToolbar")

#ifdef _OSNAP 
	#define VERTTOOLBARWINDOWCLASS _M("VertToolbar")

#endif

// Sent in a WM_COMMAND when the user right clicks in open space
// on a toolbar.
#define TB_RIGHTCLICK 	0x2861

enum ToolItemType { 
	CTB_PUSHBUTTON, 
	CTB_CHECKBUTTON, 
	CTB_MACROBUTTON,	// DB 10/27/98
	CTB_SEPARATOR,
	CTB_STATUS,
	CTB_OTHER
#ifdef _OSNAP
	, CTB_IMAGE
#endif
	};

// toolbar orientation
#define CTB_NONE		CUI_NONE
#define CTB_HORIZ		CUI_HORIZ
#define CTB_VERT		CUI_VERT
#define CTB_FLOAT		CUI_FLOAT

class ToolItem: public MaxHeapOperators {
	public: 
		ToolItemType type;
		int id;
		DWORD helpID;
		int w, h;
		int orient;	// which orientations does this item apply to?
		virtual ~ToolItem() {}
	};

class ToolButtonItem : public ToolItem {
	public:		
		int iOutEn, iInEn;
		int iOutDis, iInDis;
		int iw;
		int ih;		
		MCHAR *label;
        MaxBmpFileIcon* mpIcon, *mpInIcon;
		ToolButtonItem(ToolItemType t,
			int iOE, int iIE, int iOD, int iID,
			int iW, int iH, int wd,int ht, int ID, DWORD hID=0, MCHAR *lbl = NULL,
			int or = CTB_HORIZ|CTB_VERT|CTB_FLOAT)
			{ 
			type = t; 
			orient = or;
			iOutEn = iOE; iInEn = iIE; iOutDis = iOD; iInDis = iID;
			iw = iW; ih = iH; w = wd; h = ht; id = ID; helpID = hID;
			label = lbl;
            mpIcon = mpInIcon = NULL;
			}		
		ToolButtonItem(ToolItemType t,
                       MaxBmpFileIcon* pIcon,
			int iW, int iH, int wd,int ht, int ID, DWORD hID=0, MCHAR *lbl = NULL,
			int or = CTB_HORIZ|CTB_VERT|CTB_FLOAT)
			{ 
			type = t; 
			orient = or;
            mpIcon = pIcon;
            mpInIcon = NULL;
			iOutEn = iInEn = iOutDis = iInDis = -1;
			iw = iW; ih = iH; w = wd; h = ht; id = ID; helpID = hID;
			label = lbl;
			}		
		ToolButtonItem(ToolItemType t,
                       MaxBmpFileIcon* pIcon,
                       MaxBmpFileIcon* pInIcon,
			int iW, int iH, int wd,int ht, int ID, DWORD hID=0, MCHAR *lbl = NULL,
			int or = CTB_HORIZ|CTB_VERT|CTB_FLOAT)
			{ 
			type = t; 
			orient = or;
            mpIcon = pIcon;
            mpInIcon = pInIcon;
			iOutEn = iInEn = iOutDis = iInDis = -1;
			iw = iW; ih = iH; w = wd; h = ht; id = ID; helpID = hID;
			label = lbl;
			}		
	};

class ToolMacroItem : public ToolItem {
	public:		
		MacroButtonData md;
		ToolMacroItem(int wd, int ht, MacroButtonData *data, int or = CTB_HORIZ|CTB_VERT|CTB_FLOAT)
			{ 
			type = CTB_MACROBUTTON;
			md = *data; 
			orient = or;
			w = wd; h = ht; id = 0; helpID = 0;
			}
	};

class ToolSeparatorItem : public ToolItem {
	public:
		int vis;
		ToolSeparatorItem(int w, int h=16, BOOL vis=TRUE, int or=CTB_HORIZ|CTB_VERT|CTB_FLOAT) {
			type = CTB_SEPARATOR;
			id = 0;
			helpID = 0;
			this->w = w;
			this->h = h;
			h = 0;
			this->vis = vis;
			orient = or;
			} 
	};

class ToolStatusItem : public ToolItem {
	public:
		BOOL fixed;
		ToolStatusItem(int w, int h,BOOL f,int id, DWORD hID=0, int or = CTB_HORIZ|CTB_FLOAT) {
			type = CTB_STATUS;
			this->w = w;
			this->h = h;
			this->id = id;
			this->helpID = hID;
			fixed = f;
			orient = or;
			}
	};

#define CENTER_TOOL_VERTICALLY	0xffffffff

class ToolOtherItem : public ToolItem {
	public:
		int	  y;
		DWORD_PTR style;
		MCHAR *className;
		MCHAR *windowText;
		ToolOtherItem(MCHAR *cls,int w,int h,int id,DWORD_PTR style=WS_CHILD|WS_VISIBLE,
					int y=CENTER_TOOL_VERTICALLY,MCHAR *wt=NULL,DWORD hID=0, int or=CTB_HORIZ|CTB_FLOAT) {
			type = CTB_OTHER;
			this->y = y;
			this->w = w;
			this->h = h;
			this->id = id;
			this->helpID = hID;
			this->style = style;
			orient = or;
			className = cls;
			windowText = wt;
			}		
	};


#ifdef _OSNAP  //allow image controls on toolbars
class ToolImageItem : public ToolItem {
	public:
		int	  y;
		int	il_index;
		ToolImageItem(int w,int h,int k,int id, int y=CENTER_TOOL_VERTICALLY,DWORD hID=0, int or=CTB_HORIZ|CTB_FLOAT) {
			type = CTB_IMAGE;
			this->y = y;
			this->w = w;
			this->h = h;
			this->il_index  = k;
			this->id = id;
			this->helpID = hID;
			orient = or;
			}		
	};
#endif

class ICustToolbar : public ICustomControl {
	public:
		virtual void SetImage( HIMAGELIST hImage )=0;
		virtual void AddTool( ToolItem& entry, int pos=-1)=0;
        virtual void AddTool2(ToolItem& entry, int pos=-1)=0; // Adds caption buttons to toolbars
		virtual void DeleteTools( int start, int num=-1 )=0;  // num = -1 deletes 'start' through count-1 tools
		virtual void SetBottomBorder(BOOL on)=0;
		virtual void SetTopBorder(BOOL on)=0;
		virtual int	 GetNeededWidth(int rows)=0;	// return width needed for specified # of rows
		virtual void SetNumRows(int rows)=0;
		virtual ICustButton *GetICustButton( int id )=0;
		virtual ICustStatus *GetICustStatus( int id )=0;
		virtual HWND GetItemHwnd(int id)=0;
		virtual int GetNumItems()=0;
		virtual int GetItemID(int index)=0;
		virtual int FindItem(int id)=0;
		virtual void DeleteItemByID(int id)=0;
		virtual void LinkToCUIFrame( HWND hCUIFrame, CUIFrameMsgHandler *msgHandler)=0;
		virtual void GetFloatingCUIFrameSize(SIZE *sz, int rows=1)=0;
		virtual ICustStatusEdit *GetICustStatusEdit(int id)=0;
        virtual void ResetIconImages() = 0;
	};

CoreExport ICustToolbar *GetICustToolbar( HWND hCtrl );
CoreExport void ReleaseICustToolbar( ICustToolbar *ict );

#ifdef _OSNAP
class IVertToolbar : public ICustomControl {
	public:
		virtual void SetImage( HIMAGELIST hImage )=0;
		virtual void AddTool( const ToolItem& entry, int pos=-1 )=0;
		virtual void DeleteTools( int start, int num=-1 )=0;  // num = -1 deletes 'start' through count-1 tools
		virtual void SetBottomBorder(BOOL on)=0;
		virtual void SetTopBorder(BOOL on)=0;
		virtual ICustButton *GetICustButton( int id )=0;
		virtual ICustStatus *GetICustStatus( int id )=0;		
		virtual HWND GetItemHwnd(int id)=0;
		virtual void Enable(BOOL onOff=TRUE){};
	};

CoreExport IVertToolbar *GetIVertToolbar( HWND hCtrl );
CoreExport void ReleaseIVertToolbar( IVertToolbar *ict );

#endif


//---------------------------------------------------------------------------//
// CustImage


#define CUSTIMAGEWINDOWCLASS _M("CustImage")

class ICustImage : public ICustomControl {
	public:
		virtual void SetImage( HIMAGELIST hImage,int index, int w, int h )=0;		
	};

CoreExport ICustImage *GetICustImage( HWND hCtrl );
CoreExport void ReleaseICustImage( ICustImage *ici );

#ifdef _OSNAP
//---------------------------------------------------------------------------//
// CustImage 2D Version for displaying osnap icons


#define CUSTIMAGEWINDOWCLASS2D  _M("CustImage2D")

class ICustImage2D : public ICustomControl {
	public:
		virtual void SetImage( HIMAGELIST hImage,int index, int w, int h )=0;		
	};

#endif


//------------------------------------------------------------------------
// Off Screen Buffer

class IOffScreenBuf: public MaxHeapOperators {
	public:
		virtual HDC GetDC()=0;
		virtual void Erase(Rect *rct=NULL)=0;
		virtual void Blit(Rect *rct=NULL)=0;
		virtual void Resize()=0;
		virtual void SetBkColor(COLORREF color)=0;
		virtual COLORREF GetBkColor()=0;
	};

CoreExport IOffScreenBuf *CreateIOffScreenBuf(HWND hWnd);
CoreExport void DestroyIOffScreenBuf(IOffScreenBuf *iBuf);


//------------------------------------------------------------------------
// Color swatch control
// Puts up the ColorPicker when user right clicks on it.
//

// This message is sent as the color is being adjusted in the 
// ColorPicker.
// LOWORD(wParam) = ctrlID, 
// HIWORD(wParam) = 1 if button UP 
//                = 0 if mouse drag.
// lParam = pointer to ColorSwatchControl
#define CC_COLOR_CHANGE			WM_USER + 603

// LOWORD(wParam) = ctrlID, 
// lParam = pointer to ColorSwatchControl
#define CC_COLOR_BUTTONDOWN		WM_USER + 606

// LOWORD(wParam) = ctrlID, 
// HIWORD(wParam) = FALSE if user cancelled - TRUE otherwise
// lParam = pointer to ColorSwatchControl
#define CC_COLOR_BUTTONUP		WM_USER + 607

// This message is sent if the color has been clicked on, before 
// bringing up the color picker.
// LOWORD(wParam) = ctrlID, 
// HIWORD(wParam) = 0 
// lParam = pointer to ColorSwatchControl
#define CC_COLOR_SEL			WM_USER + 604


// This message is sent if another color swatch has been dragged and dropped
// on this swatch. 
// LOWORD(wParam) = toCtrlID, 
// HIWORD(wParam) = 0
// lParam = pointer to ColorSwatchControl
#define CC_COLOR_DROP			WM_USER + 605

// This message is sent when the color picker is closed
// LOWORD(wParam) = ctrlID, 
// HIWORD(wParam) = 0 
// lParam = pointer to ColorSwatchControl
// The following macro has been added
// in 3ds max 4.2.  If your plugin utilizes this new
// mechanism, be sure that your clients are aware that they
// must run your plugin with 3ds max version 4.2 or higher.
#define CC_COLOR_CLOSE			WM_USER + 608 //RK: 05/14/00, added this
// End of 3ds max 4.2 Extension

#define COLORSWATCHWINDOWCLASS _M("ColorSwatch")

class IColorSwatch: public ICustomControl {
	public:
		// sets only the varying color of the color picker if showing
		virtual COLORREF SetColor(COLORREF c, int notify=FALSE)=0;  // returns old color
		COLORREF SetColor(Color c, int notify=FALSE) {return SetColor(c.toRGB(), notify);}  // returns old color
		virtual AColor SetAColor(AColor c, int notify=FALSE)=0;  // returns old color

		// sets both the varying color and the "reset"color of the color picker
		virtual COLORREF InitColor(COLORREF c, int notify=FALSE)=0;  // returns old color
		COLORREF InitColor(Color c, int notify=FALSE) {return InitColor(c.toRGB(), notify);}  // returns old color
		virtual AColor InitAColor(AColor c, int notify=FALSE)=0;  // returns old color

		virtual void SetUseAlpha(BOOL onOff)=0;
		virtual BOOL GetUseAlpha()=0;

		virtual COLORREF GetColor()=0;
		virtual AColor GetAColor()=0;
		virtual void ForceDitherMode(BOOL onOff)=0;
		virtual void SetModal()=0;
		virtual void Activate(int onOff)=0;
		virtual void EditThis(BOOL startNew=TRUE)=0;
		virtual void SetKeyBrackets(BOOL onOff)=0;
	};

CoreExport IColorSwatch *GetIColorSwatch( HWND hCtrl, COLORREF col, MCHAR *name);
CoreExport IColorSwatch *GetIColorSwatch( HWND hCtrl, Color col, MCHAR *name);
CoreExport IColorSwatch *GetIColorSwatch( HWND hCtrl, AColor col, MCHAR *name);
CoreExport IColorSwatch *GetIColorSwatch(HWND hCtrl);
CoreExport void ReleaseIColorSwatch( IColorSwatch *ics );

CoreExport void RefreshAllColorSwatches();

// This class is only available in versions 5.1 and later.
#define COLOR_SWATCH_RENAMER_INTERFACE_51 Interface_ID(0x5a684953, 0x1fc043dc)

class IColorSwatchRenamer : public BaseInterface {
public:
	// The method we needed the interface for:
	virtual void SetName (MCHAR *name) { }

	// Interface ID
	Interface_ID GetID() {return COLOR_SWATCH_RENAMER_INTERFACE_51;}
};

//---------------------------------------------------------------------------//
// DragAndDrop Window


#define DADWINDOWCLASS	_M("DragDropWindow")

typedef LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class IDADWindow : public ICustomControl {
	public:
		// Installing this makes it do drag and drop.
		virtual void SetDADMgr( DADMgr *dadMgr)=0;
		virtual DADMgr *GetDADMgr()=0;
		
		// Install Window proc called to do all the normal things after 
		// drag/and/drop processing is done.
		virtual void SetWindowProc( WindowProc *proc)=0;

	};

CoreExport IDADWindow *GetIDADWindow( HWND hWnd);
CoreExport void ReleaseIDADWindow( IDADWindow *idw );



//------------------------------------------------------------------------
// Window thumb tack

// This function installs a thumb tack in the title bar of a window
// which allows the user to make it an always on top window.
// NOTE: The window class for the window should have 4 extra bytes in 
// the window structure for SetWindowLongPtr().
CoreExport void InstallThumbTack(HWND hwnd);
CoreExport void RemoveThumbTack(HWND hwnd);

// Handy routines for setting up Spinners.
CoreExport ISpinnerControl *SetupIntSpinner(HWND hwnd, int idSpin, int idEdit,  int min, int max, int val);
CoreExport ISpinnerControl *SetupFloatSpinner(HWND hwnd, int idSpin, int idEdit,  float min, float max, float val, float scale = 0.1f);
CoreExport ISpinnerControl *SetupUniverseSpinner(HWND hwnd, int idSpin, int idEdit,  float min, float max, float val, float scale = 0.1f);

// Controls whether or not spinners send notifications while the user adjusts them with the mouse
CoreExport void SetSpinDragNotify(BOOL onOff);
CoreExport BOOL GetSpinDragNotify();

//---------------------------------------------------------------------------
//

CoreExport void DisableAccelerators();
CoreExport void EnableAccelerators();
CoreExport BOOL AcceleratorsEnabled();

//! \brief Sets both the save and autosave required flags. This is called after performing
//! certain events like saving a file, loading a file, etc... Note that requireAutoBackupSave
//! is only set as FALSE in one place in the entire code base: after an auto 
//! backup operation occurs.
CoreExport void SetSaveRequiredFlag(BOOL requireSave = TRUE, BOOL requireAutoBackupSave = TRUE);
//! \brief Gets the internal save required flag without coupling it with the undo
//! operation count. This should not be used to truly see if the 3ds max scene needs
//! to be saved to file.
CoreExport BOOL GetSaveRequiredFlag();
//! \brief This tells whether the anything in the 3dsmax scene has changed such 
//! that the the scene is different from the saved version file. 
/*! Internally, this queries the undo manager's operation count, which returns a 
	number that roughly corresponds to how many actions were performed in the 
	3dsMax scene, or roughly proportional to the number of items are on the undo stack.
	Once an autosave, or save operation, is executed, the operation count number 
	goes back to zero again. This is the method that File > Reset queries to see
	whether the scene needs saving or not.
*/ 
CoreExport BOOL IsSaveRequired();
//!	\brief This function tells if an autosave is required. 
/*! This is similar or almost identical to the save required flag, but was 
	differentiated for the purpose of making an autobackup happen only once when
	a 3ds max session is left unattended. Thus, an autobackup occurs, the 
	autobackupSaveRequired flag is set to FALSE, and the undo operation count is 
	reset to zero. Thus at another autosave timer message, it will query this 
	function which then	returns FALSE, and thence a consecutive autosave will not 
	occur. However, once an action occurs in the scene, the opcount is incremented, 
	and an autosave will then occur.
*/
CoreExport BOOL IsAutoSaveRequired();


#endif // __CUSTCONT__

