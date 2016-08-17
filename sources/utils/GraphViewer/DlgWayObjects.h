#if !defined(AFX_DLGWAYOBJECTS_H__27009DED_CCD4_4042_B52D_8E14345CA51B__INCLUDED_)
#define AFX_DLGWAYOBJECTS_H__27009DED_CCD4_4042_B52D_8E14345CA51B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWayObjects.h : header file
//

class CDlgGraphViewer;

#include "LtxReader.h"

class CDlgWayObjects : public CDialog
{
public:
    CDlgGraphViewer*  m_pDlgMain;
    CLtxReader*       m_pLtxReader;
    CWayObject*       m_pWayObject;
    int               m_iWayObject;

public:
	CDlgWayObjects(CWnd* pParent = NULL);

	//{{AFX_DATA(CDlgWayObjects)
	enum { IDD = IDD_WAYOBJECTS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgWayObjects)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
    CEdit*     m_pFile;
    CEdit*     m_pInfo;
    CListBox*  m_pAll;
    CListBox*  m_pSel;

	//{{AFX_MSG(CDlgWayObjects)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnWayobjectsBrows();
	afx_msg void OnDblclkWayobjectsList();
	afx_msg void OnDblclkWayobjectsSelected();
	afx_msg void OnSelchangeWayobjectsList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWAYOBJECTS_H__27009DED_CCD4_4042_B52D_8E14345CA51B__INCLUDED_)
