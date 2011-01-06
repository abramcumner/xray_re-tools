/**********************************************************************
 *<
	FILE: winutil.h

	DESCRIPTION: Misc. windows related functions

	CREATED BY: Rolf Berteig

	HISTORY: 1-6-95 file created

 *>	Copyright (c) 1994-2003 All Rights Reserved.
 **********************************************************************/

#ifndef __WINUTIL__
#define __WINUTIL__

#include "maxheap.h"
#include <limits>
#include <locale.h>

CoreExport float GetWindowFloat(HWND hwnd,BOOL *valid=NULL);
CoreExport int GetWindowInt(HWND hwnd,BOOL *valid=NULL);
CoreExport BOOL SetWindowTextInt( HWND hwnd, int i );
CoreExport BOOL SetWindowTextFloat( HWND hwnd, float f, int precision=3 );
CoreExport BOOL SetDlgItemFloat( HWND hwnd, int idControl, float val );
CoreExport float GetDlgItemFloat( HWND hwnd, int idControl, BOOL *valid=NULL );
CoreExport void SetDlgFont( HWND hDlg, HFONT hFont );
CoreExport void SlideWindow( HWND hwnd, int x, int y );
CoreExport void StretchWindow( HWND hwnd, int w, int h );
CoreExport BOOL CenterWindow(HWND hWndChild, HWND hWndParent);
CoreExport void GetClientRectP( HWND hwnd, Rect *rect );
CoreExport void DrawIconButton( HDC hdc, HBITMAP hBitmap, Rect& wrect, Rect& brect, BOOL in );
CoreExport int GetListHieght( HWND hList );
CoreExport void ShadedVertLine( HDC hdc, int x, int y0, int y1, BOOL in );
CoreExport void ShadedHorizLine( HDC hdc, int y, int x0, int x1, BOOL in );
CoreExport void ShadedRect( HDC hdc, RECT& rect );
CoreExport void Rect3D( HDC hdc, RECT& rect, BOOL in );
CoreExport void WhiteRect3D( HDC hdc, RECT rect, BOOL in );
CoreExport void DrawButton( HDC hdc, RECT rect, BOOL in );
CoreExport void XORDottedRect( HWND hwnd, IPoint2 p0, IPoint2 p1, int solidToRight = 0 );
CoreExport void XORDottedCircle( HWND hwnd, IPoint2 p0, IPoint2 p1, int solidToRight = 0 );
CoreExport void XORDottedPolyline( HWND hwnd, int count, IPoint2 *pts, int solid = 0);
CoreExport void XORRect(HDC hdc, RECT& r, int border=1);
CoreExport void MakeButton2State(HWND hCtrl);
CoreExport void MakeButton3State(HWND hCtrl);
CoreExport int GetCheckBox(HWND hw, int id);
				// WIN64 Cleanup: Shuler
CoreExport void SetCheckBox(HWND hw, int id, BOOL b);
CoreExport BOOL DoesFileExist(const TCHAR *file);
CoreExport int GetBitsPerPixel();

// Delete superfluous zeroes from float string: 1.2300000 -> 1.23
CoreExport void StripTrailingZeros(TCHAR* buf);

template<class T> void LimitValue( T& value, T min, T max )
	{
	if ( value < min ) value = min;
	if ( value > max ) value = max;
	}

// mjm - 1.26.99 - safely casts double to float - valid flag will indicate overflow
inline float Dbl2Flt(double val, BOOL *valid = NULL)
{
	if ( val < 0.0f )
	{
		if ( val < -FLT_MAX )
		{
			if (valid) *valid = FALSE;
			return -FLT_MAX;
		}
		if ( val > -FLT_MIN )
		{
			if (valid) *valid = FALSE;
			return -FLT_MIN;
		}
		if (valid) *valid = TRUE;
		return (float)val;
	}

	if ( val > FLT_MAX )
	{
		if (valid) *valid = FALSE;
		return FLT_MAX;
	}
	if ( val < FLT_MIN && val != 0.0 )
	{
		if (valid) *valid = FALSE;
		return FLT_MIN;
	}
	if (valid) *valid = TRUE;
	return (float)val;
}

// mjm - 1.26.99 - safely casts double to int - valid flag will indicate overflow
inline int Dbl2Int(double val, BOOL *valid = NULL)
{
	if ( val > INT_MAX )
	{
		if (valid) *valid = FALSE;
		return INT_MAX;
	}
	if ( val < INT_MIN )
	{
		if (valid) *valid = FALSE;
		return INT_MIN;
	}
	if (valid) *valid = TRUE;
	return (int)val;
}

#define MAKEPOINT( lparam, pt ) { pt.x = (short)LOWORD(lparam); pt.y = (short)HIWORD(lparam); }

// The following two functions extend list boxes. Set the list box to be
// owner draw and then call these two methods in response to the
// WM_MEASUREITEM and WM_DRAWITEM messages.
// 

// Flags to pass to CustListDrawItem
#define CUSTLIST_DISABLED		(1<<0)		// Text is gray
#define CUSTLIST_MED_DISABLED	(1<<1)		// Test is darker gray
#define CUSTLIST_SEPARATOR		(1<<2)		// Draws a separator instead of text
#define CUSTLIST_DBL_SERPARATOR	(1<<3)		// Draw a double line seperator
#define CUSTLIST_RED			(1<<4)      // Text is red

CoreExport void CustListMeasureItem(HWND hList,WPARAM wParam, LPARAM lParam);
CoreExport void CustListDrawItem(HWND hList,WPARAM wParam, LPARAM lParam,DWORD flags);


// MAX extended message box functionality  DB 7/98

#define MAX_MB_HOLD				0x0001		// add "Hold" button
#define MAX_MB_DONTSHOWAGAIN	0x0002		// add "Don't show this dialog again" checkbox

// The first four parameters are just like the Win32 MessageBox routine (but not
// all MessageBox functionality is supported!)
//
// The last two optional args add the functionality listed above -- exType is used
// for adding the additional buttons, and exRet is used for getting the extra
// return info.  For example, if exType includes MAX_MB_DONTSHOWAGAIN, and exRet
// is non-NULL, then exRet will have MAX_MB_DONTSHOWAGAIN set if that checkbox was
// checked by the user.

CoreExport INT_PTR MaxMsgBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT type, UINT exType=0, DWORD *exRet=NULL);
			// WIN64 Cleanup: Shuler

/**********************************************************************
 *
 * alpha blended icon support...
 *
 **********************************************************************/
#include "plugapi.h"

enum LoadMAXIconErrors
	{
	LMI_Ok,
	LMI_ResourceNotFound,
	LMI_ResourceLoadFailed,
	LMI_ImageAndMaskNotCompatible,
	};

CoreExport LoadMAXIconErrors LoadMAXIcon(HINSTANCE hInstance, LPCTSTR resID, LPCTSTR resMaskID, COLORREF bkColor,
		HIMAGELIST imageList, int imageIndex, int preMultAlpha=TRUE);

// returns index of first image into existing imageList
CoreExport int LoadMAXIconFromBMI(LPBITMAPINFOHEADER biImage, LPBITMAPINFOHEADER biMask, COLORREF bkColor, TCHAR* pFilePrefix, int preMultAlpha=TRUE, HIMAGELIST* pImageList=NULL);
CoreExport BITMAPINFOHEADER *LoadBitmapFromFile(TCHAR *filename);

CoreExport void DrawMAXIcon(HDC hDC, Rect &r, HIMAGELIST hList32, HIMAGELIST hList16, int index);

// Compute a good color to use for drawing XOR lines over a particular background color
CoreExport COLORREF ComputeXORDrawColor(COLORREF bkgColor);
// Compute a good color to use for drawing XOR lines over a viewport
CoreExport COLORREF ComputeViewportXORDrawColor();


/**********************************************************************

Dialog resizing and positioning. A generic class for resizing a dialog.
Includes functions to save and restore the initial size and position
of the dialog.

This class makes it easy to add resizability to a dialog. You'll need
one instance of this class for each instance of a dialog. Set up
various parameters during your dialog's WM_INITDIALOG processing,
then let this class handle all WM_SIZE events.

Include library: core.lib

HOW TO USE THIS CLASS:

1. Create either a static instance in your DlgProc, or a live instance
   in the object that is driving the dialog.
2. In your WM_INITDIALOG handling, call Initialize().
3. Optionally, call SetMinimumDlgSize(). However, the default minimum size
   for a dialog is it's size at the time Initialize() is called.
4. For each control in your dialog, call SetControlInfo(); you only need 
   to do this if you want to override the default behavior for the control.
   For most, the default is to be locked to the top left corner, with a
   fixed size. Some controls may default to resizing; SysListView32 has
   some specialized behaviors you may not need to override.
5. Process the WM_SIZING message, and call Process_WM_SIZING().
6. Process the WM_SIZE message, and call Process_WM_SIZE().

**********************************************************************/

class DialogItemSizeData: public MaxHeapOperators
{
public:
	HWND hwnd;
	RECT rect;
	DWORD flags;

	DialogItemSizeData(HWND h, DWORD f = 0)
		:	hwnd(h), flags(f)
	{
		WINDOWPLACEMENT wpControl;
		wpControl.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwnd, &wpControl);
		rect = wpControl.rcNormalPosition;
	}
	// Compiler-generated destructor, copy constructor, and assignment 
	// operator should all be fine.
private:
	// Ensure no default constructor is generated or used.
	DialogItemSizeData();
};

class DialogResizer: public MaxHeapOperators
{
public:
	DialogResizer() : mhDlg(NULL)
	{
		mMinDlgSize.left = mMinDlgSize.top = 0;
		mMinDlgSize.right = mMinDlgSize.bottom = 50;
		mOriginalClientRect.left = mOriginalClientRect.top = 0;
		mOriginalClientRect.right = mOriginalClientRect.bottom = 50;
	}
	// Compiler-generated destructor, copy constructor, and assignment 
	// operator should all be fine.

	CoreExport void Initialize(HWND hDlg);
	CoreExport void SetMinimumDlgSize(LONG wid, LONG ht);

	// If you have a control you want to have fixed width and height, and locked to 
	// top left, you do not have to call SetControlInfo, as that is the default 
	// behavior.
	enum PositionControl
		{ kLockToTopLeft=1, kLockToTopRight, kLockToBottomLeft, kLockToBottomRight, kPositionsOnly=0xff };
	enum ControlFlags
		{ kDefaultBehavior=0, kWidthChangesWithDialog=1<<8, kHeightChangesWithDialog=1<<9 };
	CoreExport void SetControlInfo(int resID, PositionControl pos, DWORD flags = kDefaultBehavior);
	CoreExport void SetControlInfo(HWND hwnd, PositionControl pos, DWORD flags = kDefaultBehavior);

	CoreExport void Process_WM_SIZING(WPARAM wParam, LPARAM lParam);
	CoreExport void Process_WM_SIZE(WPARAM wParam, LPARAM lParam);

	// Static methods to let you save and restore the size and position of your dialog
	// (whether it's resizable or not).
	// If the section name is not specified, it will default to
	// [DialogResizer_SizeAndPositions].
	// If the ini filename is not specified, it will default to the main application ini
	// file, typically 3dsmax.ini.
	CoreExport static void SaveDlgPosition(HWND hDlg, const TCHAR* keyname, 
				const TCHAR* section = NULL, const TCHAR* inifn = NULL);
	CoreExport static void LoadDlgPosition(HWND hDlg, const TCHAR* keyname, 
				const TCHAR* section = NULL, const TCHAR* inifn = NULL);

	friend static BOOL CALLBACK GetInitialPositionECP(HWND hwnd, LPARAM lParam);

private:
	Tab<DialogItemSizeData> mControls;
	RECT mMinDlgSize;
	RECT mOriginalClientRect;
	HWND mhDlg;
};

//! \brief MaxLocaleHandler handles Globalization

//! This is a helper class that switches the Locale settings to a temporary
//! value and restores the previous locale setting in the destructor.
//! To use this class simply instanciate a variable of it's class at the beginning
//! of a method. The given locale setting will be set until the method is left and
//! the destructor of the class is called automatically.
class MaxLocaleHandler: public MaxHeapOperators {

public:

    //! \brief Constructor

    //! \param[in] category - The category affected by locale. See _tsetlocale in MSDN help for more info
	//! \param[in] localeSetting - The locale name. See _tsetlocale in MSDN help for more info
	MaxLocaleHandler(int category = LC_ALL, const TCHAR* localeSetting = _T("C"));
	//! \brief Destructor, resets locale setting to original value
	~MaxLocaleHandler();

   //! \brief Resets locale setting to original value, before the object goes out of scope.
   void RestoreLocale();

private:

	int m_category;
    TSTR m_oldLocale;
};

inline MaxLocaleHandler::MaxLocaleHandler(int category, const TCHAR* localeSetting) 
: m_oldLocale(_tsetlocale(category, NULL)),		// Save old locale
  m_category(category)
{
	// Set new locale
	_tsetlocale(category, localeSetting);
}

inline MaxLocaleHandler::~MaxLocaleHandler() {
   if( !m_oldLocale.isNull() )
   {
      // Restore old locale
      _tsetlocale(m_category, m_oldLocale.data());
   }
}

inline void MaxLocaleHandler::RestoreLocale() {
   if( !m_oldLocale.isNull() )
   {
      // Restore old locale
      _tsetlocale(m_category, m_oldLocale.data());

      // Prevent double restores
      m_oldLocale = _T("");
   }
}



#endif
