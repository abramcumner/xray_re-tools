#if !defined(AFX_DLGLOAD_H__771C85AE_8B46_4022_993A_7A11ABCEB0E1__INCLUDED_)
#define AFX_DLGLOAD_H__771C85AE_8B46_4022_993A_7A11ABCEB0E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLoad.h : header file
//

class CGraph;
class CLevel;

class CDlgLoad : public CDialog
{
public:
    CGraph*   m_pGraph;
    CLevel*   m_pLevel;
    unsigned  m_uLevel;

protected:
    CEdit*     m_pFile;
    CListBox*  m_pLevels;
    CStatic*   m_pHeaderInfo;
    CStatic*   m_pLevelInfo;
    CButton*   m_pSelect;

public:
	CDlgLoad(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDlgLoad)
	enum { IDD = IDD_LOAD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    void UpdateGraphInfo();
    void UpdateLevelInfo();
    CString guid2str(unsigned __int8 uGUID[16]);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLoad)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLoadBrows();
	afx_msg void OnLoadSelect();
	afx_msg void OnSelchangeLoadLevels();
	afx_msg void OnDblclkLoadLevels();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOAD_H__771C85AE_8B46_4022_993A_7A11ABCEB0E1__INCLUDED_)
