// GraphViewer.h : main header file for the GRAPHVIEWER application
//

#if !defined(AFX_GRAPHVIEWER_H__BEE60628_FE1B_442B_BBFD_63EAF8E9F6F4__INCLUDED_)
#define AFX_GRAPHVIEWER_H__BEE60628_FE1B_442B_BBFD_63EAF8E9F6F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAppGraphViewer:
// See GraphViewer.cpp for the implementation of this class
//

class CAppGraphViewer : public CWinApp
{
public:
	CAppGraphViewer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppGraphViewer)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAppGraphViewer)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHVIEWER_H__BEE60628_FE1B_442B_BBFD_63EAF8E9F6F4__INCLUDED_)
