#if !defined(AFX_DLGSETTINGS_H__037EE9E3_C0F1_4C17_95FF_82DE82E37DA2__INCLUDED_)
#define AFX_DLGSETTINGS_H__037EE9E3_C0F1_4C17_95FF_82DE82E37DA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgSettings : public CDialog
{
public:
    int    m_iZoom;
    float  m_fMin;
    float  m_fMax;
    float  m_fCurMin;
    float  m_fCurMax;
    int    m_iCoverMode;

public:
	CDlgSettings(CWnd* pParent = NULL);

	//{{AFX_DATA(CDlgSettings)
	enum { IDD = IDD_SETTINGS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
    CComboBox*    m_pZoom;
    CComboBox*    m_pCover;
    CSliderCtrl*  m_pRange;
    CEdit*        m_pMin;
    CEdit*        m_pMax;
    CEdit*        m_pCurMin;
    CEdit*        m_pCurMax;

	//{{AFX_MSG(CDlgSettings)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSettingsSetmin();
	afx_msg void OnSettingsSetmax();
	afx_msg void OnSelchangeSettingsZoom();
	afx_msg void OnSelchangeSettingsCover();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETTINGS_H__037EE9E3_C0F1_4C17_95FF_82DE82E37DA2__INCLUDED_)
