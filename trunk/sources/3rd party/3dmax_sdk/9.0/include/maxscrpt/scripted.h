/************************************************************************	
 *		ScriptEd.h - wrapper classes for script editor windows			*
 *																		*
 *		Author: Ravi Karra												*
 ************************************************************************/

#ifndef _SCRIPTEDITOR_H
#define _SCRIPTEDITOR_H


#include "MaxScrpt.h"
#include "Listener.h"

// defines for script editor window menu items
#define	IDM_NEW			10
#define IDM_OPEN		11
#define IDM_EVAL_ALL	40026
#define IDM_CLOSE		40024

// wrapper class for script editor windows
class ScriptEditor
{
		MCHAR*			editScript;
		MSTR			title;
	protected:
		WNDPROC			originalWndProc;		
		IntTab			disable_menus;
		edit_window		*ew;
		HWND			hScript;
	
	public:
		ScriptEditor(MCHAR* ititle=NULL) : 
			title(ititle), 
			ew(NULL), 
			hScript(NULL), 
			editScript(NULL) { }

		ScripterExport ~ScriptEditor();
		
		virtual LRESULT APIENTRY proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {	
							return CallWindowProc(originalWndProc, hwnd, uMsg, wParam, lParam); 
							}

		ScripterExport virtual	HWND	DisplayWindow(HWND hParent=NULL/*for future use*/);
		ScripterExport virtual void		CloseWindow(bool notify=false);
		ScripterExport virtual MCHAR*	GetEditScript();
		ScripterExport virtual void		SetEditScript(MCHAR* script);		
		ScripterExport virtual void		SetTitle(MCHAR* t) { title = t; }						
		ScripterExport virtual bool		OnFileOpen(HWND hwnd);
		ScripterExport virtual bool		OnClose(HWND hwnd);
		
		virtual MCHAR*	GetTitle()			{ return title; }
		virtual Value*	GetValueTitle()		{ return (ew) ? ew->file_name : NULL; }
		virtual bool	OnExecute(HWND hwnd){ return false; } // return false to default handling
		virtual	bool	IsDisplayed()		{ return ew!=NULL; }
		virtual	IntTab&	GetDisabledMenuTab(){ return disable_menus; }
};

// open new editor on existing file, pop openfilename dialog if no filename supplied
// if ew is NULL, a new editor window is opened. If openfilename dialog is opened, initial
// path will be as specified by path arg, current path if NULL
ScripterExport void open_script(MCHAR* filename=NULL, edit_window *ew=NULL, MCHAR* path=NULL);

// open editor on existing file, jump to specified character offset
ScripterExport edit_window* show_editor_pos(MCHAR* file_name, int pos);

// returns existing edit_window for specified file name.
ScripterExport edit_window* get_edit_window(MCHAR* file_name);

#endif //_SCRIPTEDITOR_H