// DlgWayObjects.cpp : implementation file
//

#include "stdafx.h"
#include "GraphViewer.h"
#include "DlgWayObjects.h"
#include "DlgGraphViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWayObjects dialog


CDlgWayObjects::CDlgWayObjects(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWayObjects::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWayObjects)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_pLtxReader = NULL;
    m_pWayObject = NULL;
    m_iWayObject = 0;
}


void CDlgWayObjects::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWayObjects)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWayObjects, CDialog)
	//{{AFX_MSG_MAP(CDlgWayObjects)
	ON_BN_CLICKED(IDC_WAYOBJECTS_BROWS, OnWayobjectsBrows)
	ON_LBN_DBLCLK(IDC_WAYOBJECTS_LIST, OnDblclkWayobjectsList)
	ON_LBN_DBLCLK(IDC_WAYOBJECTS_SELECTED, OnDblclkWayobjectsSelected)
	ON_LBN_SELCHANGE(IDC_WAYOBJECTS_LIST, OnSelchangeWayobjectsList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWayObjects message handlers

void CDlgWayObjects::OnOK() 
{
	//CDialog::OnOK();
}

void CDlgWayObjects::OnCancel() 
{
	//CDialog::OnCancel();
    ShowWindow(SW_HIDE);
    CMenu* pMenu = m_pDlgMain->GetMenu();
    pMenu->CheckMenuItem(IDM_SHOW_WAYS, MF_BYCOMMAND | MF_UNCHECKED);
}

BOOL CDlgWayObjects::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_pFile = (CEdit*)    GetDlgItem(IDC_WAYOBJECTS_FILE);
    m_pInfo = (CEdit*)    GetDlgItem(IDC_WAYOBJECTS_INFO);
    m_pAll  = (CListBox*) GetDlgItem(IDC_WAYOBJECTS_LIST);
    m_pSel  = (CListBox*) GetDlgItem(IDC_WAYOBJECTS_SELECTED);
/*
    if (m_pLtxReader->m_pSections)
    {
    }
    else
    {
        m_pFile->SetWindowText(NULL);
        m_pInfo->SetWindowText(NULL);
        //m_pAll  = (CListBox*) GetDlgItem(IDC_WAYOBJECTS_LIST);
        //m_pSel  = (CListBox*) GetDlgItem(IDC_WAYOBJECTS_SELECTED);
    }
*/
	return TRUE;
}

void CDlgWayObjects::OnWayobjectsBrows() 
{
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Файлы путей (*.ltx)|*.ltx||");
    if (dlg.DoModal() == IDOK)
    {
        m_pLtxReader->Close();
        for (int i=0; i<m_iWayObject; i++)
            m_pWayObject[i].Clean();

        CString sFilename = dlg.GetPathName();

        m_pFile->SetWindowText(sFilename);
        m_pInfo->SetWindowText(NULL);
        m_pAll->ResetContent();
        m_pSel->ResetContent();

        if (!m_pLtxReader->Open(sFilename))
        {
            AfxMessageBox("Ошыбка открытия файла!");
        }
        else
        {
            //CAnyObject obj;
            //obj.Read(m_pLtxReader, m_pLtxReader->GetSectionName(49));

            for (int j=0; j<m_pLtxReader->m_iSections; j++)
            {
                m_pAll->SetItemData(m_pAll->AddString(m_pLtxReader->GetSectionName(j)), (DWORD) j);
            }
        }
    }
}

void CDlgWayObjects::OnDblclkWayobjectsList() 
{
    int sel = m_pAll->GetCurSel();
    if (sel >= 0)
    {
        int ind = (int) m_pAll->GetItemData(sel);

        for (int i=0; i<m_iWayObject; i++)
        {
            if (!m_pWayObject[i].m_pPoints)
            {
                if (!m_pWayObject[i].Read(m_pLtxReader, m_pLtxReader->GetSectionName(ind)))
                {
                    AfxMessageBox("Ошибка чтения пути!");
                    return;
                }
                else
                {
                    m_pSel->SetItemData(m_pSel->AddString(m_pLtxReader->GetSectionName(ind)), (DWORD) ind);
                    m_pDlgMain->Draw(NULL);
                    return;
                }
            }
        }

        AfxMessageBox("Максимальное число отображаемых путей!\nЧтобы добавить новый путь, нужно удалить\nчто-либо из списка (двойной клик).");
    }
}

void CDlgWayObjects::OnSelchangeWayobjectsList() 
{
    int sel = m_pAll->GetCurSel();
    if (sel >= 0)
    {
        int ind = (int) m_pAll->GetItemData(sel);

        CWayObject way;
        if (!way.Read(m_pLtxReader, m_pLtxReader->GetSectionName(ind)))
        {
            m_pInfo->SetWindowText(NULL);
            AfxMessageBox("Ошибка чтения секции!");
        }
        else
        {
            CString str, s;
            str.Format("[%s]\r\npoints = ", way.m_pName);
            for (int i=0; i<way.m_iPoints; i++)
            {
                if (i==0)
                    s.Format("p%d", i);
                else
                    s.Format(",p%d", i);
                str += s;
            }
            str += "\r\n";
            for (i=0; i<way.m_iPoints; i++)
            {
                s.Format("p%d:name = %s\r\n", i, way.m_pPoints[i].pName);
                str += s;
                if (way.m_pPoints[i].flags > 0)
                {
                    s.Format("p%d:flags = 0x%x\r\n", i, way.m_pPoints[i].flags);
                    str += s;
                }
                s.Format("p%d:position = %f,%f,%f\r\n", i, way.m_pPoints[i].x, way.m_pPoints[i].y, way.m_pPoints[i].z);
                str += s;
                s.Format("p%d:game_vertex_id = %d\r\n", i, way.m_pPoints[i].gvid);
                str += s;
                s.Format("p%d:level_vertex_id = %d\r\n", i, way.m_pPoints[i].lvid);
                str += s;

                int k = 0;
                for (int j=0; j<way.m_iLinks; j++)
                {
                    if (way.m_pLinks[j].from == i)
                    {
                        if (k++==0)
                            s.Format("p%d:links = p%d(%d)", i, way.m_pLinks[j].to, way.m_pLinks[j].prob);
                        else
                            s.Format(",p%d(%d)", way.m_pLinks[j].to, way.m_pLinks[j].prob);
                        str += s;
                    }
                }
                str += (k == 0) ? "\r\n" : "\r\n\r\n";
            }

            m_pInfo->SetWindowText(str);
        }
    }
}

void CDlgWayObjects::OnDblclkWayobjectsSelected() 
{
    int sel = m_pSel->GetCurSel();
    if (sel >= 0)
    {
        int ind = (int) m_pSel->GetItemData(sel);

        for (int i=0; i<m_iWayObject; i++)
        {
            if (m_pWayObject[i].m_pPoints)
            {
                if (strcmp(m_pWayObject[i].m_pName, m_pLtxReader->GetSectionName(ind)) == 0)
                {
                    m_pWayObject[i].Clean();
                    m_pSel->DeleteString(sel);
                    m_pDlgMain->Draw(NULL);
                    return;
                }
            }
        }
    }
}
