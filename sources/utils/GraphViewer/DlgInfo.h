#if !defined(AFX_DLGINFO_H__E2FF09F8_770C_479E_84D9_46B6767159A9__INCLUDED_)
#define AFX_DLGINFO_H__E2FF09F8_770C_479E_84D9_46B6767159A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInfo.h : header file
//

class CGraph;
class CLevel;
class CDlgGraphViewer;

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo dialog

class CDlgInfo : public CDialog
{
public:
    CDlgGraphViewer* m_pMainDlg;
    CGraph*  m_pGraph;
    CLevel*  m_pLevel;

    unsigned m_uGridX;
    unsigned m_uGridZ;
    unsigned m_uGridY;
    unsigned m_uIndex;
    unsigned m_uExType; // -1 - пусто, 0 - general, 1 - lvids, 2 - edges

public:
	CDlgInfo(CWnd* pParent = NULL);   // standard constructor

    void SetPosition(int x, int y);
    void SetSelection(int x, int y);

// Dialog Data
	//{{AFX_DATA(CDlgInfo)
	enum { IDD = IDD_INFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CEdit*      m_pPosition;
    CComboBox*  m_pSel;
    CComboBox*  m_pSelEx;
    CEdit*      m_pSelText;

	// Generated message map functions
	//{{AFX_MSG(CDlgInfo)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeInfoSel();
	afx_msg void OnSelchangeInfoSelex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINFO_H__E2FF09F8_770C_479E_84D9_46B6767159A9__INCLUDED_)
