// DlgSettings.cpp : implementation file
//

#include "stdafx.h"
#include "GraphViewer.h"
#include "DlgSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSettings dialog

CDlgSettings::CDlgSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSettings)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSettings)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSettings, CDialog)
	//{{AFX_MSG_MAP(CDlgSettings)
	ON_BN_CLICKED(IDC_SETTINGS_SETMIN, OnSettingsSetmin)
	ON_BN_CLICKED(IDC_SETTINGS_SETMAX, OnSettingsSetmax)
	ON_CBN_SELCHANGE(IDC_SETTINGS_ZOOM, OnSelchangeSettingsZoom)
	ON_CBN_SELCHANGE(IDC_SETTINGS_COVER, OnSelchangeSettingsCover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSettings message handlers

void CDlgSettings::OnOK() 
{
	CDialog::OnOK();
}

void CDlgSettings::OnCancel() 
{
	CDialog::OnCancel();
}

BOOL CDlgSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_pZoom   = (CComboBox*)   GetDlgItem(IDC_SETTINGS_ZOOM);
    m_pCover  = (CComboBox*)   GetDlgItem(IDC_SETTINGS_COVER);
    m_pRange  = (CSliderCtrl*) GetDlgItem(IDC_SETTINGS_RANGE);
    m_pMin    = (CEdit*)       GetDlgItem(IDC_SETTINGS_MIN);
    m_pMax    = (CEdit*)       GetDlgItem(IDC_SETTINGS_MAX);
    m_pCurMin = (CEdit*)       GetDlgItem(IDC_SETTINGS_CURMIN);
    m_pCurMax = (CEdit*)       GetDlgItem(IDC_SETTINGS_CURMAX);

    if (m_iZoom > 4)
        m_iZoom = 4;

    m_pZoom->SetCurSel(m_iZoom-1);
    m_pCover->SetCurSel(m_iCoverMode);
    m_pRange->SetRange(0, 200);
    m_pRange->SetSelection(200*(m_fCurMin-m_fMin)/(m_fMax - m_fMin), 200*(m_fCurMax-m_fMin)/(m_fMax - m_fMin));

    char chText[256];
    sprintf(chText, "%.1f", m_fMin);
    m_pMin->SetWindowText(chText);
    sprintf(chText, "%.1f", m_fMax);
    m_pMax->SetWindowText(chText);
    sprintf(chText, "%.1f", m_fCurMin);
    m_pCurMin->SetWindowText(chText);
    sprintf(chText, "%.1f", m_fCurMax);
    m_pCurMax->SetWindowText(chText);

	return TRUE;
}

void CDlgSettings::OnSettingsSetmin() 
{
    m_fCurMin = m_fMin + m_pRange->GetPos()*(m_fMax - m_fMin)/200;

    char chText[256];
    sprintf(chText, "%.1f", m_fCurMin);
    m_pCurMin->SetWindowText(chText);

    m_pRange->SetSelection(200*(m_fCurMin-m_fMin)/(m_fMax - m_fMin), 200*(m_fCurMax-m_fMin)/(m_fMax - m_fMin));
    m_pRange->Invalidate();
}

void CDlgSettings::OnSettingsSetmax() 
{
    m_fCurMax = m_fMin + m_pRange->GetPos()*(m_fMax - m_fMin)/200;

    char chText[256];
    sprintf(chText, "%.1f", m_fCurMax);
    m_pCurMax->SetWindowText(chText);

    m_pRange->SetSelection(200*(m_fCurMin-m_fMin)/(m_fMax - m_fMin), 200*(m_fCurMax-m_fMin)/(m_fMax - m_fMin));
    m_pRange->Invalidate();
}

void CDlgSettings::OnSelchangeSettingsZoom() 
{
	m_iZoom = m_pZoom->GetCurSel() + 1;
    if (m_iZoom == 4)
        m_iZoom = 6;
}

void CDlgSettings::OnSelchangeSettingsCover() 
{
	m_iCoverMode = m_pCover->GetCurSel();
}
