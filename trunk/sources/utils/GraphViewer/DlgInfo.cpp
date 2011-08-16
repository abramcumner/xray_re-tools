// DlgInfo.cpp : implementation file
//

#include "stdafx.h"
#include "GraphViewer.h"
#include "DlgInfo.h"
#include "Graph.h"
#include "DlgGraphViewer.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo dialog


CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_pMainDlg = NULL;
    m_pGraph = NULL;
    m_pLevel = NULL;

    //m_uGridX;
    //m_uGridZ;
    //m_uGridY;
    m_uIndex = -1;
    m_uExType = -1; // -1 - пусто, 0 - general, 1 - lvids, 2 - edges, 3 - edge
}


void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgInfo)
	ON_CBN_SELCHANGE(IDC_INFO_SEL, OnSelchangeInfoSel)
	ON_CBN_SELCHANGE(IDC_INFO_SELEX, OnSelchangeInfoSelex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo message handlers

void CDlgInfo::OnOK() 
{
	//CDialog::OnOK();
}

void CDlgInfo::OnCancel() 
{
    ShowWindow(SW_HIDE);
	//CDialog::OnCancel();
    CMenu* pMenu = m_pMainDlg->GetMenu();
    pMenu->CheckMenuItem(IDM_SHOW_INFO, MF_BYCOMMAND | MF_UNCHECKED);
}

BOOL CDlgInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_pPosition = (CEdit*)     GetDlgItem(IDC_INFO_POSITION);
    m_pSel      = (CComboBox*) GetDlgItem(IDC_INFO_SEL);
    m_pSelEx    = (CComboBox*) GetDlgItem(IDC_INFO_SELEX);
    m_pSelText  = (CEdit*)     GetDlgItem(IDC_INFO_SEL_TEXT);

	return TRUE;
}

void CDlgInfo::SetPosition(int x, int y)
{
    if (x<0 || y<0)
    {
        m_pPosition->SetWindowText(NULL);

    }
    else
    {
        char chText[64];
        if (m_pGraph->m_pVertexs)
        {
            double dbX = m_pGraph->m_sGridBox.p1.x + x*0.7;
            double dbY = m_pGraph->m_sGridBox.p1.z + y*0.7;
            sprintf(chText, "%.1f, %.1f", dbX, dbY);
            m_pPosition->SetWindowText(chText);
        }
        else if (m_pLevel->m_pPoints)
        {
            double dbX = m_pLevel->m_sHeader.box.p1.x + x*0.7;
            double dbY = m_pLevel->m_sHeader.box.p1.z + y*0.7;
            sprintf(chText, "%.1f, %.1f", dbX, dbY);
            m_pPosition->SetWindowText(chText);
        }
        else
        {
            m_pPosition->SetWindowText(NULL);
        }
    }
}

void CDlgInfo::SetSelection(int x, int y)
{
    CString str;

    m_uGridX = x;
    m_uGridZ = y;
    m_uGridY = -1;
    m_uIndex = -1;

    BOOL bInvalid = TRUE;

    m_pSel->ResetContent();
    m_pSel->EnableWindow(FALSE);

    if ((x >= 0) && (y >= 0))
    {
        if (m_pGraph->m_pVertexs)
        {
            if ((m_uGridX < m_pGraph->m_uGridCX) && (m_uGridZ < m_pGraph->m_uGridCZ))
            {
                m_uGridY = 0;
                unsigned index = (m_uGridX * m_pGraph->m_uGridCZ + m_uGridZ) * m_pGraph->m_uGridCY;
                unsigned i = m_pGraph->m_pGrid[index];
                if (i != -1)
                {
                    for (unsigned u=0; u<m_pGraph->m_uGridCY; u++)
                    {
                        i = m_pGraph->m_pGrid[index + u];
                        if (i == -1)
                            break;

                        if (i & 0x80000000)
                        {
                            str.Format("gvid: %d", i & 0x7fffffff);
                            m_pSel->SetItemData(m_pSel->AddString(str), i);
                            //m_uGridY = u;
                        }
                        else
                        {
                            str.Format("lvid: %d", i);
                            m_pSel->SetItemData(m_pSel->AddString(str), i);
                        }
                    }
                }

                m_pSel->SetCurSel(m_uGridY);
            }
        }
        else if (m_pLevel->m_pPoints)
        {
            if ((m_uGridX < m_pLevel->m_uGridCX) && (m_uGridZ < m_pLevel->m_uGridCZ))
            {
                m_uGridY = 0;
                unsigned index = (m_uGridX * m_pLevel->m_uGridCZ + m_uGridZ) * m_pLevel->m_uGridCY;
                unsigned i = m_pLevel->m_pGrid[index];
                if (i != -1)
                {
                    for (unsigned u=0; u<m_pLevel->m_uGridCY; u++)
                    {
                        i = m_pLevel->m_pGrid[index + u];
                        if (i == -1)
                            break;

                        str.Format("lvid: %d", i);
                        m_pSel->SetItemData(m_pSel->AddString(str), i);
                    }
                }

                m_pSel->SetCurSel(m_uGridY);
            }
        }
        else
        {
        }

        if (m_pSel->GetCount() > 1)
            m_pSel->EnableWindow(TRUE);
    }
/*
    if (m_pSel->GetCount() <= 0)
    {
        for (int xx=-1; xx<=1; xx++)
        {
            m_uGridX = x+xx;
            for (int yy=-1; yy<=1; yy++)
            {
                m_uGridZ = y+yy;

                if (m_pGraph->m_pVertexs)
                {
                    if ((m_uGridX < m_pGraph->m_uGridCX) && (m_uGridZ < m_pGraph->m_uGridCZ))
                    {
                        m_uGridY = 0;
                        unsigned index = (m_uGridX * m_pGraph->m_uGridCZ + m_uGridZ) * m_pGraph->m_uGridCY;
                        unsigned i = m_pGraph->m_pGrid[index];
                        if (i != -1)
                        {
                            for (unsigned u=0; u<m_pGraph->m_uGridCY; u++)
                            {
                                i = m_pGraph->m_pGrid[index + u];
                                if (i == -1)
                                    break;

                                if (i & 0x80000000)
                                {
                                    str.Format("gvid: %d", i & 0x7fffffff);
                                    m_pSel->SetItemData(m_pSel->AddString(str), i);
                                    //m_uGridY = u;
                                }
                                else
                                {
                                    str.Format("lvid: %d", i);
                                    m_pSel->SetItemData(m_pSel->AddString(str), i);
                                }
                            }
                        }

                        m_pSel->SetCurSel(m_uGridY);
                    }
                }
                else if (m_pLevel->m_pPoints)
                {
                    if ((m_uGridX < m_pLevel->m_uGridCX) && (m_uGridZ < m_pLevel->m_uGridCZ))
                    {
                        m_uGridY = 0;
                        unsigned index = (m_uGridX * m_pLevel->m_uGridCZ + m_uGridZ) * m_pLevel->m_uGridCY;
                        unsigned i = m_pLevel->m_pGrid[index];
                        if (i != -1)
                        {
                            for (unsigned u=0; u<m_pLevel->m_uGridCY; u++)
                            {
                                i = m_pLevel->m_pGrid[index + u];
                                if (i == -1)
                                    break;

                                str.Format("lvid: %d", i);
                                m_pSel->SetItemData(m_pSel->AddString(str), i);
                            }
                        }

                        m_pSel->SetCurSel(m_uGridY);
                    }
                }
                else
                {
                }

                if (m_pSel->GetCount() > 1)
                    m_pSel->EnableWindow(TRUE);

                if (m_pSel->GetCount() > 0)
                    break;
            }

            if (m_pSel->GetCount() > 0)
                break;
        }
    }
*/
    if (m_pSel->GetCount() > 0)
    {
        if (!IsWindowVisible())
        {
            ShowWindow(SW_SHOW);
        }
    }

    OnSelchangeInfoSel();
}

void CDlgInfo::OnSelchangeInfoSel() 
{
    CString str;

    m_pSelEx->ResetContent();
    m_pSelEx->EnableWindow(FALSE);

    int sel = m_pSel->GetCurSel();
    if (sel >= 0)
    {
        m_uIndex = m_pSel->GetItemData(sel);

        if (m_uIndex != -1)
        {
            if (m_pGraph->m_pVertexs)
            {
                if (m_uIndex & 0x80000000)
                {
                    m_pSelEx->EnableWindow(TRUE);

                    str.Format("общая");
                    m_pSelEx->SetItemData(m_pSelEx->AddString(str), 0);
                    m_pSelEx->SetCurSel(0);

                    str.Format("lvids");
                    m_pSelEx->SetItemData(m_pSelEx->AddString(str), 1);

                    str.Format("edges");
                    m_pSelEx->SetItemData(m_pSelEx->AddString(str), 2);
                }
                else
                {
                    str.Format("общая");
                    m_pSelEx->SetItemData(m_pSelEx->AddString(str), 0);
                    m_pSelEx->SetCurSel(0);
                }
            }
            else if (m_pLevel->m_pPoints)
            {
                str.Format("общая");
                m_pSelEx->SetItemData(m_pSelEx->AddString(str), 0);
                m_pSelEx->SetCurSel(0);
            }
        }
    }

    OnSelchangeInfoSelex();
}

void CDlgInfo::OnSelchangeInfoSelex() 
{
    CString str, s;

    if (m_uIndex != -1)
    {
        if (m_uIndex & 0x80000000)
        {
            int sel = m_pSelEx->GetCurSel();
            if (sel >= 0)
                m_uExType = m_pSelEx->GetItemData(sel);

            GG_VERTEX* pVertex = &m_pGraph->m_pVertexs[m_uIndex & 0x7fffffff];

            if (m_uExType == 1)
            {
                str.Format("count: %d", (unsigned) pVertex->level_point_count);
                for (unsigned u=0; u<pVertex->level_point_count; u++)
                {
                    unsigned i = pVertex->level_point_offset + u;

                    s.Format("\r\nlvid=%d  {%.1f,%.1f,%.1f}",
                        m_pGraph->m_pLevelPoints[i].level_vertex_id,
                        m_pGraph->m_pLevelPoints[i].point.x, m_pGraph->m_pLevelPoints[i].point.y, m_pGraph->m_pLevelPoints[i].point.z);

                    str += s;
                }
            }
            else if (m_uExType == 2)
            {
                str.Format("count: %d", (unsigned) pVertex->edge_count);
                for (unsigned u=0; u<pVertex->edge_count; u++)
                {
                    unsigned i = pVertex->edge_offset + u;

                    s.Format("\r\ngvid=%d  {dist=%.1f}",
                        m_pGraph->m_pEdges[i].game_vertex_id,
                        m_pGraph->m_pEdges[i].distance);

                    str += s;

                    i = m_pGraph->m_pEdges[i].game_vertex_id;
                    if (pVertex->level_id != m_pGraph->m_pVertexs[i].level_id)
                    {
                        for (unsigned j=0; j<m_pGraph->m_sHeader.level_count; j++)
                        {
                            if (m_pGraph->m_pLevels[j].level_id == m_pGraph->m_pVertexs[i].level_id)
                            {
                                s.Format("  '%s'", m_pGraph->m_pLevels[j].level_name);
                                str += s;
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                str.Format("level_point: %.f, %.f, %.f\r\ngame_point: %.f, %.f, %.f\r\nlevel_id: %d\r\nlevel_vertex_id: %d\r\ntype: %02x,%02x,%02x,%02x\r\nedge_count: %d\r\nlevel_point_count: %d",
                    pVertex->level_point.x, pVertex->level_point.y, pVertex->level_point.z,
                    pVertex->game_point.x, pVertex->game_point.y, pVertex->game_point.z,
                    (unsigned) pVertex->level_id,
                    (unsigned) pVertex->level_vertex_id,
                    (unsigned) pVertex->vertex_type_1, (unsigned) pVertex->vertex_type_2, (unsigned) pVertex->vertex_type_3, (unsigned) pVertex->vertex_type_4,
                    (unsigned) pVertex->edge_count,
                    (unsigned) pVertex->level_point_count);
            }
        }
        else
        {
            m_uExType = 0;

            if (m_pGraph->m_pVertexs)
            {
                GG_LEVEL_POINT* pPoint = &m_pGraph->m_pLevelPoints[m_uIndex];

                str.Format("point: %.f, %.f, %.f\r\nlevel_vertex_id: %d\r\ndistance: %.2f",
                    pPoint->point.x, pPoint->point.y, pPoint->point.z,
                    pPoint->level_vertex_id,
                    pPoint->distance);
            }
            else if (m_pLevel->m_pPoints)
            {
                AI_POINT* pPoint = &m_pLevel->m_pPoints[m_uIndex];

                double x = m_pLevel->m_sHeader.box.p1.x + m_uGridX * m_pLevel->m_sHeader.cell_size;
                double z = m_pLevel->m_sHeader.box.p1.z + m_uGridZ * m_pLevel->m_sHeader.cell_size;
                double y = m_pLevel->m_sHeader.box.p1.y + (double) pPoint->packed_y * m_pLevel->m_sHeader.factor_y / 65535;

                //перевод сферических координат в декартовы:
                // x = R*sin(Fi)*cos(Teta)
                // y = R*sin(Fi)*sin(Teta)
                // z = R*cos(Fi)
                double pi = 3.1415926535;
                double v = ((int)((pPoint->plane >> 7) & 0x03f)) * 0.5*pi / 64;
                double u = ((int)((pPoint->plane >> 0) & 0x07f)) * 0.5*pi / 128;
                double nx = sin(v)*cos(u);
                double nz = sin(v)*sin(u);
                double ny = cos(v);
                if (pPoint->plane & 0x8000)
                    nx = -nx;
                if (pPoint->plane & 0x4000)
                    ny = -ny;
                if (pPoint->plane & 0x2000)
                    nz = -nz;

                str.Format("соседи: %d, %d, %d, %d\r\nукрытия: %d, %d, %d, %d\r\nсвет: 0x%x\r\nнормаль:  {%.3f, %.3f, %.3f}\r\nпозиция: {%.1f, %.1f, %.1f}",
                    (unsigned) (pPoint->neighbours1 == 0x7fffff) ? -1 : pPoint->neighbours1,
                    (unsigned) (pPoint->neighbours2 == 0x7fffff) ? -1 : pPoint->neighbours2,
                    (unsigned) (pPoint->neighbours3 == 0x7fffff) ? -1 : pPoint->neighbours3,
                    (unsigned) (pPoint->neighbours4 == 0x7fffff) ? -1 : pPoint->neighbours4,
                    (unsigned) pPoint->cover1,
                    (unsigned) pPoint->cover2,
                    (unsigned) pPoint->cover3,
                    (unsigned) pPoint->cover4,
                    (unsigned) pPoint->light,
	                nx,ny,nz, //(unsigned) pPoint->plane,
                    x,y,z);
            }
        }
    }

    m_pSelText->SetWindowText(str);

    if (m_pMainDlg)
        m_pMainDlg->Invalidate();
}

