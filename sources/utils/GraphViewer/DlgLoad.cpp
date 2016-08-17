// DlgLoad.cpp : implementation file
//

#include "stdafx.h"
#include "GraphViewer.h"
#include "DlgLoad.h"
#include "Graph.h"
#include "LtxReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoad dialog


CDlgLoad::CDlgLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLoad)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_pGraph = NULL;
    m_pLevel = NULL;
    m_uLevel = -1;
}


void CDlgLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLoad)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CDlgLoad::UpdateGraphInfo()
{
    if (m_pGraph->m_pLevels)
    {
        m_pFile->SetWindowText(m_pGraph->m_chFilename);

        m_pLevels->EnableWindow(TRUE);
        m_pLevels->ResetContent();
        m_pLevels->SetCurSel(-1);
        for (unsigned i=0; i<m_pGraph->m_sHeader.level_count; i++)
            m_pLevels->AddString(m_pGraph->m_pLevels[i].level_name);

        CString str;
        str.Format("version = %d\nguid = {%s}\nlevel_count = %d\nvertex_count = %d\nedge_count = %d\nlevel_point_count = %d",
            (unsigned) m_pGraph->m_sHeader.version,
                       guid2str(m_pGraph->m_sHeader.guid),
            (unsigned) m_pGraph->m_sHeader.level_count,
            (unsigned) m_pGraph->m_sHeader.vertex_count,
            (unsigned) m_pGraph->m_sHeader.edge_count,
            (unsigned) m_pGraph->m_sHeader.level_point_count);
        m_pHeaderInfo->SetWindowText(str);

        UpdateLevelInfo();
    }
    else if (m_pLevel->m_pPoints)
    {
        m_pFile->SetWindowText(m_pLevel->m_chFilename);

        m_pLevels->EnableWindow(FALSE);
        m_pLevels->ResetContent();
        m_pLevels->SetCurSel(-1);

        CString str;
        str.Format("version = %d\nvertex_count = %d\ncell_size = %f\nfactor_y = %f\nbox = {%.f,%.f,%.f}-{%.f,%.f,%.f}\nguid = {%s}",
            (unsigned) m_pLevel->m_sHeader.version,
            (unsigned) m_pLevel->m_sHeader.vertex_count,
	                   m_pLevel->m_sHeader.cell_size,
	                   m_pLevel->m_sHeader.factor_y,
                       m_pLevel->m_sHeader.box.p1.x, m_pLevel->m_sHeader.box.p1.y, m_pLevel->m_sHeader.box.p1.z,
                       m_pLevel->m_sHeader.box.p2.x, m_pLevel->m_sHeader.box.p2.y, m_pLevel->m_sHeader.box.p2.z,
                       guid2str(m_pLevel->m_sHeader.guid));
        m_pHeaderInfo->SetWindowText(str);

        m_pLevelInfo->SetWindowText(NULL);
        m_pSelect->EnableWindow(TRUE);
    }
    else
    {
        m_pFile->SetWindowText(NULL);
        m_pLevels->ResetContent();
        m_pLevels->SetCurSel(-1);
        m_pHeaderInfo->SetWindowText(NULL);
        m_pLevelInfo->SetWindowText(NULL);
        m_pSelect->EnableWindow(FALSE);
    }
}

void CDlgLoad::UpdateLevelInfo()
{
    int sel = m_pLevels->GetCurSel();
    if (m_pGraph->m_pLevels && (sel >= 0))
    {
        CString str;
        str.Format("level_name = %s\noffset = %.f,%.f,%.f\nlevel_id = %d\nsection_name = %s\nguid = {%s}",
            m_pGraph->m_pLevels[sel].level_name,
            m_pGraph->m_pLevels[sel].offset.x, m_pGraph->m_pLevels[sel].offset.y, m_pGraph->m_pLevels[sel].offset.z,
            (unsigned) m_pGraph->m_pLevels[sel].level_id,
            m_pGraph->m_pLevels[sel].section_name,
            guid2str(m_pGraph->m_pLevels[sel].guid));
        m_pLevelInfo->SetWindowText(str);
        m_pSelect->EnableWindow(TRUE);
    }
    else
    {
        m_pLevelInfo->SetWindowText(NULL);
        m_pSelect->EnableWindow(FALSE);
    }
}

CString CDlgLoad::guid2str(unsigned __int8 uGUID[16])
{
    CString str;
    str.Format("%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        uGUID[0], uGUID[1], uGUID[2], uGUID[3],
        uGUID[4], uGUID[5],
        uGUID[6], uGUID[7],
        uGUID[8], uGUID[9],
        uGUID[10],uGUID[11],uGUID[12],uGUID[13],uGUID[14],uGUID[15]);
    return str;
}

BEGIN_MESSAGE_MAP(CDlgLoad, CDialog)
	//{{AFX_MSG_MAP(CDlgLoad)
	ON_BN_CLICKED(IDC_LOAD_BROWS, OnLoadBrows)
	ON_BN_CLICKED(IDC_LOAD_SELECT, OnLoadSelect)
	ON_LBN_SELCHANGE(IDC_LOAD_LEVELS, OnSelchangeLoadLevels)
	ON_LBN_DBLCLK(IDC_LOAD_LEVELS, OnDblclkLoadLevels)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoad message handlers

BOOL CDlgLoad::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_pFile       = (CEdit*)    GetDlgItem(IDC_LOAD_FILE);
    m_pLevels     = (CListBox*) GetDlgItem(IDC_LOAD_LEVELS);
    m_pHeaderInfo = (CStatic*)  GetDlgItem(IDC_LOAD_HEADER_INFO);
    m_pLevelInfo  = (CStatic*)  GetDlgItem(IDC_LOAD_LEVEL_INFO);
    m_pSelect     = (CButton*)  GetDlgItem(IDC_LOAD_SELECT);

    UpdateGraphInfo();

	return TRUE;
}

void CDlgLoad::OnOK() 
{
	//CDialog::OnOK();
}

void CDlgLoad::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgLoad::OnLoadBrows() 
{
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Ôאיכû 'graph' ט 'ai'|*.graph;*.ai|Ôאיכ דנאפא (*.graph)|*.graph|Ôאיכ ai (*.ai)|*.ai||");
    if (dlg.DoModal() == IDOK)
    {
        CString sFilename = dlg.GetPathName();

        //CLtxReader test;
        //test.Open(sFilename);
        //CWayObject way;
        //way.Read(&test, test.GetSectionName(0));

        if (sFilename.Right(3) == ".ai")
        {
            m_pGraph->Clean();
            m_pLevel->Load(sFilename);
        }
        else
        {
            m_pLevel->Clean();
            m_pGraph->Load(sFilename);
        }

        UpdateGraphInfo();
    }
}

void CDlgLoad::OnSelchangeLoadLevels() 
{
    UpdateLevelInfo();
}

void CDlgLoad::OnDblclkLoadLevels() 
{
    OnLoadSelect();
}

void CDlgLoad::OnLoadSelect() 
{
    if (m_pGraph->m_pVertexs)
    {
        int sel = m_pLevels->GetCurSel();
        if (m_pGraph->m_pLevels && (sel >= 0))
        {
            m_uLevel = (unsigned) sel;
            CDialog::OnOK();
        }
    }
    else if (m_pLevel->m_pPoints)
    {
        m_uLevel = -1;
        CDialog::OnOK();
    }
}
