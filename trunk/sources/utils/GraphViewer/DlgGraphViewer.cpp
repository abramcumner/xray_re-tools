// DlgGraphViewer.cpp : implementation file
//

#include "stdafx.h"
#include "GraphViewer.h"
#include "DlgGraphViewer.h"
#include "Graph.h"
#include "DlgLoad.h"
#include "DlgInfo.h"
#include "DlgSettings.h"
#include "DlgWayObjects.h"
#include  <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphViewer dialog

CDlgGraphViewer::CDlgGraphViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphViewer::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pDlgInfo = NULL;
    m_pDlgWays = NULL;
    m_pGraph = NULL;
    m_pLevel = NULL;
    m_uLevel = -1;

    // масштаб отображения, колличество пиксел между соседними ячейками
    m_iZoom = 2;
    m_iCoverMode = 0;

    m_bDrag = FALSE;
    m_bBitmap = FALSE;
}

void CDlgGraphViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphViewer)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgGraphViewer, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphViewer)
	ON_WM_PAINT()
    ON_WM_ERASEBKGND()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_COMMAND(IDM_OPEN, OnMenuLoad)
	ON_COMMAND(IDM_CLOSE, OnMenuClose)
	ON_COMMAND(IDM_SHOW_SETTINGS, OnShowSettings)
	ON_COMMAND(IDM_SHOW_INFO, OnMenuShowInfo)
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDM_SHOW_WAYS, OnShowWays)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgGraphViewer::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
    m_pGraph = new CGraph;
    if (!m_pGraph)
    {
        AfxMessageBox("Ошибка инициализации!");
        EndDialog(IDABORT);
        return TRUE;
    }

    m_pLevel = new CLevel;
    if (!m_pLevel)
    {
        AfxMessageBox("Ошибка инициализации!");
        EndDialog(IDABORT);
        return TRUE;
    }

    m_pDlgInfo = new CDlgInfo;
    if (!m_pDlgInfo)
    {
        AfxMessageBox("Ошибка инициализации!");
        EndDialog(IDABORT);
        return TRUE;
    }
    m_pDlgInfo->m_pMainDlg = this;
    m_pDlgInfo->m_pGraph = m_pGraph;
    m_pDlgInfo->m_pLevel = m_pLevel;
    if (!m_pDlgInfo->Create(CDlgInfo::IDD, this))
    {
        AfxMessageBox("Ошибка инициализации!");
        EndDialog(IDABORT);
        return TRUE;
    }
	
    m_pDlgWays = new CDlgWayObjects;
    m_pDlgWays->m_pDlgMain = this;
    m_pDlgWays->m_pLtxReader = &m_oLtxWay;
    m_pDlgWays->m_pWayObject = &m_oWay[0];
    m_pDlgWays->m_iWayObject = 5;
    m_pDlgWays->Create(CDlgWayObjects::IDD, this);

    //m_oLtxWay.Open("C:\\Documents and Settings\\igor\\My Documents\\stalker\\mods\\acdc_24aug\\dec\\way_l01_escape.ltx");
    //m_oWay[0].Read(&m_oLtxWay, m_oLtxWay.GetSectionName(6));
    //m_oWay[1].Read(&m_oLtxWay, m_oLtxWay.GetSectionName(7));
    //m_oWay[2].Read(&m_oLtxWay, m_oLtxWay.GetSectionName(8));

	return TRUE;
}

void CDlgGraphViewer::OnDestroy() 
{
	CDialog::OnDestroy();
	
    if (m_pDlgWays)
    {
        m_pDlgWays->DestroyWindow();
        delete m_pDlgWays;
        m_pDlgWays = NULL;
    }

    if (m_pDlgInfo)
    {
        m_pDlgInfo->DestroyWindow();
        delete m_pDlgInfo;
        m_pDlgInfo = NULL;
    }

	if (m_pGraph)
    {
        delete m_pGraph;
        m_pGraph = NULL;
    }

    if (m_pLevel)
    {
        delete m_pLevel;
        m_pLevel = NULL;
    }
}

void CDlgGraphViewer::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CPaintDC dc(this);
        Draw(&dc);
	}
}

BOOL CDlgGraphViewer::OnEraseBkgnd(CDC *pDC)
{
    return TRUE;
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgGraphViewer::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDlgGraphViewer::Draw(CDC* pDC)
{
    CDC* pTempDC = NULL;
    if (!pDC)
    {
        pTempDC = new CClientDC(this);
        if (!pTempDC)
            return;
        pDC = pTempDC;
    }

    RECT rc;
    GetClientRect(&rc);

    if (m_bBitmap)
    {
        CDC mdc1;
        if (mdc1.CreateCompatibleDC(pDC))
        {
            CBitmap mbm1;
            if (mbm1.CreateCompatibleBitmap(pDC, rc.right, rc.bottom))
            {
                CBitmap* pOldBitmap1 = mdc1.SelectObject(&mbm1);

                mdc1.FillSolidRect(&rc, RGB(255,255,255));

                CDC mdc2;
                if (mdc2.CreateCompatibleDC(pDC))
                {
                    CBitmap* pOldBitmap2 = mdc2.SelectObject(&m_oBitmap);
                    int x = GetScrollPos(SB_HORZ);
                    int y = GetScrollPos(SB_VERT);
                    mdc1.BitBlt(0,0,rc.right,rc.bottom, &mdc2, x*m_iZoom, y*m_iZoom, SRCCOPY);
                    mdc2.SelectObject(pOldBitmap2);
                }

                DrawWay(&mdc1, &m_oWay[0]);
                DrawWay(&mdc1, &m_oWay[1]);
                DrawWay(&mdc1, &m_oWay[2]);
                DrawWay(&mdc1, &m_oWay[3]);
                DrawWay(&mdc1, &m_oWay[4]);

                DrawSelection(&mdc1);

                pDC->BitBlt(0,0,rc.right,rc.bottom, &mdc1,0,0,SRCCOPY);

                mdc1.SelectObject(pOldBitmap1);
            }
        }
    }
    else
    {
        CDC mdc1;
        if (mdc1.CreateCompatibleDC(pDC))
        {
            CBitmap mbm1;
            if (mbm1.CreateCompatibleBitmap(pDC, rc.right, rc.bottom))
            {
                CBitmap* pOldBitmap1 = mdc1.SelectObject(&mbm1);

                mdc1.FillSolidRect(&rc, RGB(255,255,255));

                if (m_pGraph->m_pVertexs)
                {
                    DRAW_DATA dd;
                    dd.pDC = &mdc1;
                    dd.x = GetScrollPos(SB_HORZ);
                    dd.y = GetScrollPos(SB_VERT);
                    dd.cx = m_iViewCX;
                    dd.cy = m_iViewCY;
                    dd.iZoom = m_iZoom;
                    dd.fMinY = m_fCurMin;
                    dd.fMaxY = m_fCurMax;
                    dd.iCoverMode = m_iCoverMode;

                    m_pGraph->DrawLevelPoints(&dd);
                    m_pGraph->DrawEdges(&dd);
                    m_pGraph->DrawVertexs(&dd);
                }
                else if (m_pLevel->m_pPoints)
                {
                    DRAW_DATA dd;
                    dd.pDC = &mdc1;
                    dd.x = GetScrollPos(SB_HORZ);
                    dd.y = GetScrollPos(SB_VERT);
                    dd.cx = m_iViewCX;
                    dd.cy = m_iViewCY;
                    dd.iZoom = m_iZoom;
                    dd.fMinY = m_fCurMin;
                    dd.fMaxY = m_fCurMax;
                    dd.iCoverMode = m_iCoverMode;

                    m_pLevel->DrawVertexs(&dd);
                }

                DrawWay(&mdc1, &m_oWay[0]);
                DrawWay(&mdc1, &m_oWay[1]);
                DrawWay(&mdc1, &m_oWay[2]);
                DrawWay(&mdc1, &m_oWay[3]);
                DrawWay(&mdc1, &m_oWay[4]);

                DrawSelection(&mdc1);

                pDC->BitBlt(0,0,rc.right,rc.bottom, &mdc1,0,0,SRCCOPY);

                mdc1.SelectObject(pOldBitmap1);
            }
        }
/*
        pDC->FillSolidRect(&rc, RGB(255,255,255));

        if (m_pGraph->m_pVertexs)
        {
            DRAW_DATA dd;
            dd.pDC = pDC;
            dd.x = GetScrollPos(SB_HORZ);
            dd.y = GetScrollPos(SB_VERT);
            dd.cx = m_iViewCX;
            dd.cy = m_iViewCY;
            dd.iZoom = m_iZoom;
            dd.fMinY = m_fCurMin;
            dd.fMaxY = m_fCurMax;
            dd.iCoverMode = m_iCoverMode;

            m_pGraph->DrawLevelPoints(&dd);
            m_pGraph->DrawEdges(&dd);
            m_pGraph->DrawVertexs(&dd);
        }
        else if (m_pLevel->m_pPoints)
        {
            DRAW_DATA dd;
            dd.pDC = pDC;
            dd.x = GetScrollPos(SB_HORZ);
            dd.y = GetScrollPos(SB_VERT);
            dd.cx = m_iViewCX;
            dd.cy = m_iViewCY;
            dd.iZoom = m_iZoom;
            dd.fMinY = m_fCurMin;
            dd.fMaxY = m_fCurMax;
            dd.iCoverMode = m_iCoverMode;

            m_pLevel->DrawVertexs(&dd);
        }

        DrawWay(pDC, &m_oWay[0]);
        DrawWay(pDC, &m_oWay[1]);
        DrawWay(pDC, &m_oWay[2]);
        DrawWay(pDC, &m_oWay[3]);
        DrawWay(pDC, &m_oWay[4]);

        DrawSelection(pDC);
*/
    }

    if (pTempDC)
        delete pTempDC;
}

void CDlgGraphViewer::DrawWay(CDC* pDC, CWayObject* pWay)
{
    if (!pWay->m_pPoints)
        return;

    BOX_3D* pBox = NULL;

    if (m_pGraph->m_pVertexs)
    {
        pBox = &m_pGraph->m_sGridBox;
    }
    else if (m_pLevel->m_pPoints)
    {
        pBox = &m_pLevel->m_sHeader.box;
    }

    if (!pBox)
        return;

    COLORREF cr = RGB(128,0,128);
    CPen pen(PS_SOLID, 1, cr);
    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush br(cr);
    CBrush* pOldBrush = pDC->SelectObject(&br);
    int iOldPolyMode = pDC->SetPolyFillMode(WINDING);

    int offx = GetScrollPos(SB_HORZ);
    int offy = GetScrollPos(SB_VERT);

    int i,x,y,d;
    for (i=0; i<pWay->m_iPoints; i++)
    {
        x = ((pWay->m_pPoints[i].x - pBox->p1.x) / 0.7 - offx) * m_iZoom;
        y = ((pBox->p2.z - pWay->m_pPoints[i].z) / 0.7 - offy - 1) * m_iZoom;
        pDC->FillSolidRect(x-1, y-1, 3, 3, cr);
        pWay->m_pPoints[i].pt.x = x;
        pWay->m_pPoints[i].pt.y = y;
    }


    POINT* pSrc;
    POINT* pDst;
    for (i=0; i<pWay->m_iLinks; i++)
    {
        pSrc = &pWay->m_pPoints[pWay->m_pLinks[i].from].pt;
        pDst = &pWay->m_pPoints[pWay->m_pLinks[i].to].pt;

        pDC->MoveTo(*pSrc);
        pDC->LineTo(*pDst);

        x = pSrc->x - pDst->x;
        y = pSrc->y - pDst->y;
        d = x*x + y*y;
        if (d > 15*15) // 10 секунд
        {
            POINT pt[3];
            pt[0].x = pDst->x;
            pt[0].y = pDst->y;

            //double dd = sqrt(d);
            double d2 = sqrt(d*2);

            double xx = pSrc->x - (pDst->y - pSrc->y)/4.0;
            double yy = pSrc->y + (pDst->x - pSrc->x)/4.0;
            xx = pDst->x + (xx - pDst->x)*14/d2;
            yy = pDst->y + (yy - pDst->y)*14/d2;
            //pDC->MoveTo(pDst->x, pDst->y);
            //pDC->LineTo(xx, yy);
            pt[1].x = xx;
            pt[1].y = yy;

            xx = pSrc->x + (pDst->y - pSrc->y)/4.0;
            yy = pSrc->y - (pDst->x - pSrc->x)/4.0;
            xx = pDst->x + (xx - pDst->x)*14/d2;
            yy = pDst->y + (yy - pDst->y)*14/d2;
            //pDC->MoveTo(pDst->x, pDst->y);
            //pDC->LineTo(xx, yy);
            pt[2].x = xx;
            pt[2].y = yy;

            pDC->Polygon(pt, 3);
        }
    }

    pDC->SetPolyFillMode(iOldPolyMode);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}

void CDlgGraphViewer::DrawSelection(CDC* pDC)
{
    unsigned uIndex = m_pDlgInfo->m_uIndex;

    if (uIndex != -1)
    {
        DRAW_DATA dd;
        dd.pDC = pDC;
        dd.x = GetScrollPos(SB_HORZ);
        dd.y = GetScrollPos(SB_VERT);
        dd.cx = m_iViewCX;
        dd.cy = m_iViewCY;
        dd.iZoom = m_iZoom;
        dd.fMinY = m_fCurMin;
        dd.fMaxY = m_fCurMax;
        dd.iCoverMode = m_iCoverMode;

        if (m_pDlgInfo->m_uExType == 0)
        {
            if (m_pGraph->m_pVertexs)
            {
                if (uIndex & 0x80000000)
                {
                    m_pGraph->DrawSelVertexs(&dd, uIndex);
                }
                else
                {
                    m_pGraph->DrawSelLevelPoints(&dd, uIndex);
                }
            }
            else if (m_pLevel->m_pPoints)
            {
                m_pLevel->DrawSelVertexs(&dd, uIndex);
            }
        }
        else if (m_pDlgInfo->m_uExType == 1)
        {
            if (m_pGraph->m_pVertexs)
            {
                m_pGraph->DrawSelLevelPoints(&dd, uIndex);
            }
        }
        else if (m_pDlgInfo->m_uExType == 2)
        {
            if (m_pGraph->m_pVertexs)
            {
                m_pGraph->DrawSelEdges(&dd, uIndex);
            }
        }
    }
}

void CDlgGraphViewer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (!pScrollBar)
    {
        int minpos;
        int maxpos;
        GetScrollRange(SB_HORZ, &minpos, &maxpos); 
        maxpos = GetScrollLimit(SB_HORZ);

        if (minpos < maxpos)
        {
            int curpos = GetScrollPos(SB_HORZ);

            switch (nSBCode)
            {
                case SB_LEFT:
                    curpos = minpos;
                    break;

                case SB_RIGHT:
                    curpos = maxpos;
                    break;

                case SB_ENDSCROLL:
                    break;

                case SB_LINELEFT:
                    if (curpos > minpos)
                        curpos--;
                    break;

                case SB_LINERIGHT:
                    if (curpos < maxpos)
                        curpos++;
                    break;

                case SB_PAGELEFT:
                    if (curpos > minpos)
                        curpos = max(minpos, curpos - 20);
                    break;

                case SB_PAGERIGHT:
                    if (curpos < maxpos)
                        curpos = min(maxpos, curpos + 20);
                    break;

                case SB_THUMBPOSITION:
                    curpos = nPos;
                    break;

                case SB_THUMBTRACK:
                    curpos = nPos;
                    break;
            }

            if (curpos != GetScrollPos(SB_HORZ))
            {
                SetScrollPos(SB_HORZ, curpos);
                Draw(NULL);
            }
        }
    }
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgGraphViewer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (!pScrollBar)
    {
        int minpos;
        int maxpos;
        GetScrollRange(SB_VERT, &minpos, &maxpos); 
        maxpos = GetScrollLimit(SB_VERT);

        if (minpos < maxpos)
        {
            int curpos = GetScrollPos(SB_VERT);

            switch (nSBCode)
            {
                case SB_TOP:
                    curpos = minpos;
                    break;

                case SB_BOTTOM:
                    curpos = maxpos;
                    break;

                case SB_ENDSCROLL:
                    break;

                case SB_LINEUP:
                    if (curpos > minpos)
                        curpos--;
                    break;

                case SB_LINEDOWN:
                    if (curpos < maxpos)
                        curpos++;
                    break;

                case SB_PAGEUP:
                    if (curpos > minpos)
                        curpos = max(minpos, curpos - 20);
                    break;

                case SB_PAGEDOWN:
                    if (curpos < maxpos)
                        curpos = min(maxpos, curpos + 20);
                    break;

                case SB_THUMBPOSITION:
                    curpos = nPos;
                    break;

                case SB_THUMBTRACK:
                    curpos = nPos;
                    break;
            }

            if (curpos != GetScrollPos(SB_VERT))
            {
                SetScrollPos(SB_VERT, curpos);
                Draw(NULL);
            }
        }
    }
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDlgGraphViewer::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
    if (m_pGraph)
    {
        if (!m_pGraph->m_pVertexs && !m_pLevel->m_pPoints)
        {
            m_iCX = 0;
            m_iCY = 0;
        }

        m_iViewCX = cx / m_iZoom;
        m_iViewCY = cy / m_iZoom;

        SetScrollRange(SB_HORZ, 0, (m_iCX > m_iViewCX) ? m_iCX - m_iViewCX : 0);
        SetScrollRange(SB_VERT, 0, (m_iCY > m_iViewCY) ? m_iCY - m_iViewCY : 0);

        Draw(NULL);
    }
}

void CDlgGraphViewer::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if (!m_bDrag)
    {
        SetCapture();
        m_bDrag = TRUE;
        m_sDrag = point;
        m_sDragPos.x = GetScrollPos(SB_HORZ);
        m_sDragPos.y = GetScrollPos(SB_VERT);
    }

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgGraphViewer::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if (m_bDrag)
        ReleaseCapture();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgGraphViewer::OnRButtonDown(UINT nFlags, CPoint point) 
{
    int x = GetScrollPos(SB_HORZ) + point.x / m_iZoom;
    int y = (m_iCY-1) - GetScrollPos(SB_VERT) - point.y / m_iZoom;
    m_pDlgInfo->SetSelection(x, y);
    CMenu* pMenu = GetMenu();
    pMenu->CheckMenuItem(IDM_SHOW_INFO, MF_BYCOMMAND | MF_CHECKED);
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CDlgGraphViewer::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CDialog::OnRButtonUp(nFlags, point);
}

void CDlgGraphViewer::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pGraph->m_pVertexs || m_pLevel->m_pPoints)
    {
        int x = GetScrollPos(SB_HORZ) + point.x / m_iZoom;
        int y = (m_iCY-1) - GetScrollPos(SB_VERT) - point.y / m_iZoom;
        m_pDlgInfo->SetPosition(x, y);
    }
	
    if (m_bDrag)
    {
        int x = m_sDragPos.x + (m_sDrag.x - point.x) / m_iZoom;
        int y = m_sDragPos.y + (m_sDrag.y - point.y) / m_iZoom;

        int minpos;
        int maxpos;

        GetScrollRange(SB_VERT, &minpos, &maxpos); 
        if (x<minpos)
            x = minpos;
        if (x>maxpos)
            x = maxpos;

        GetScrollRange(SB_VERT, &minpos, &maxpos); 
        if (y<minpos)
            y = minpos;
        if (y>maxpos)
            y = maxpos;

        SetScrollPos(SB_HORZ, x);
        SetScrollPos(SB_VERT, y);

        Draw(NULL);
        //Invalidate(FALSE);
    }
	
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgGraphViewer::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
    if (m_bDrag)
    {
        m_bDrag = FALSE;
        Draw(NULL);
    }
	
	CDialog::OnCaptureChanged(pWnd);
}

void CDlgGraphViewer::OnMenuLoad() 
{
    BOOL bVisible = m_pDlgInfo->IsWindowVisible();

    if (bVisible)
        m_pDlgInfo->ShowWindow(SW_HIDE);

    m_pDlgInfo->SetPosition(-1,-1);
    m_pDlgInfo->SetSelection(-1,-1);
    //m_pGraph->Clean();
    //m_pLevel->Clean();

    CDlgLoad dlg(this);
    dlg.m_pGraph = m_pGraph;
    dlg.m_pLevel = m_pLevel;
    dlg.m_uLevel = m_uLevel;
    if (dlg.DoModal() == IDOK)
        m_uLevel = dlg.m_uLevel;

    {
        if (m_pGraph->m_pVertexs && (m_uLevel != -1))
        {
            m_pGraph->GridInit(m_uLevel);
            // колличество точек по осям X и Z для загруженного уровня
            m_iCX = (int) m_pGraph->m_uGridCX;
            m_iCY = (int) m_pGraph->m_uGridCZ;

            m_fMin = m_pGraph->m_sGridBox.p1.y;
            m_fMax = m_pGraph->m_sGridBox.p2.y;
            m_fCurMin = m_fMin;
            m_fCurMax = m_fMax;

            CString str;
            str.Format("GraphViewer [file=%s] [level=%s]", m_pGraph->m_chFilename, m_pGraph->m_pLevels[m_uLevel].level_name);
            SetWindowText(str);

            CMenu* pMenu = GetMenu();
            pMenu->EnableMenuItem(IDM_SHOW_SETTINGS, MF_BYCOMMAND | MF_ENABLED);
        }
        else if (m_pLevel->m_pPoints)
        {
            m_pLevel->GridInit();
            // колличество точек по осям X и Z для загруженного уровня
            m_iCX = (int) m_pLevel->m_uGridCX;
            m_iCY = (int) m_pLevel->m_uGridCZ;

            m_fMin = m_pLevel->m_sHeader.box.p1.y;
            m_fMax = m_pLevel->m_sHeader.box.p2.y;
            m_fCurMin = m_fMin;
            m_fCurMax = m_fMax;

            CString str;
            str.Format("GraphViewer [file=%s]", m_pLevel->m_chFilename);
            SetWindowText(str);

            CMenu* pMenu = GetMenu();
            pMenu->EnableMenuItem(IDM_SHOW_SETTINGS, MF_BYCOMMAND | MF_ENABLED);
        }
        else
        {
            SetWindowText("GraphViewer");

            CMenu* pMenu = GetMenu();
            pMenu->EnableMenuItem(IDM_SHOW_SETTINGS, MF_BYCOMMAND | MF_GRAYED);
        }

        // колличество точек по осям X и Z отображаемых в текущем окне
        CRect rc;
        GetClientRect(rc);
        m_iViewCX = rc.Width()  / m_iZoom;
        m_iViewCY = rc.Height() / m_iZoom;

        // размерность скролов
        SetScrollRange(SB_HORZ, 0, (m_iCX > m_iViewCX) ? m_iCX - m_iViewCX : 0);
        SetScrollRange(SB_VERT, 0, (m_iCY > m_iViewCY) ? m_iCY - m_iViewCY : 0);

        CreateBitmap();
    }

    Draw(NULL);

    if (bVisible)
        m_pDlgInfo->ShowWindow(SW_SHOW);
}

void CDlgGraphViewer::CreateBitmap() 
{
    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = m_iCX * m_iZoom;
    rc.bottom = m_iCY * m_iZoom;

    m_bBitmap = FALSE;
    m_oBitmap.DeleteObject();

    CDC* pDC = GetDC();
    if (pDC)
    {
        CDC mdc;
        if (mdc.CreateCompatibleDC(pDC))
        {
            if (m_oBitmap.CreateCompatibleBitmap(pDC, rc.right, rc.bottom))
            {
                BITMAP bm;
                m_oBitmap.GetObject(sizeof(BITMAP), &bm);

                CBitmap* pOldBitmap = mdc.SelectObject(&m_oBitmap);

                mdc.FillSolidRect(&rc, RGB(255,255,255));

                DRAW_DATA dd;
                dd.pDC = &mdc;
                dd.x = 0;
                dd.y = 0;
                dd.cx = m_iCX;
                dd.cy = m_iCY;
                dd.iZoom = m_iZoom;
                dd.fMinY = m_fCurMin;
                dd.fMaxY = m_fCurMax;
                dd.iCoverMode = m_iCoverMode;

                if (m_pGraph->m_pVertexs)
                {
                    m_pGraph->DrawLevelPoints(&dd);
                    m_pGraph->DrawEdges(&dd);
                    m_pGraph->DrawVertexs(&dd);
                }
                else if (m_pLevel->m_pPoints)
                {
                    m_pLevel->DrawVertexs(&dd);
                }

                mdc.SelectObject(pOldBitmap);

                m_bBitmap = TRUE;
            }
        }
        ReleaseDC(pDC);
    }
}

void CDlgGraphViewer::OnMenuClose() 
{
    OnCancel();
}

void CDlgGraphViewer::OnShowSettings() 
{
    BOOL bInfoVisible = m_pDlgInfo->IsWindowVisible();
    BOOL bWaysVisible = m_pDlgWays->IsWindowVisible();

    if (bInfoVisible)
        m_pDlgInfo->ShowWindow(SW_HIDE);
    if (bWaysVisible)
        m_pDlgWays->ShowWindow(SW_HIDE);

    m_pDlgInfo->SetPosition(-1,-1);
    m_pDlgInfo->SetSelection(-1,-1);

    CDlgSettings dlg(this);
    dlg.m_iZoom = m_iZoom;
    dlg.m_fMin = m_fMin;
    dlg.m_fMax = m_fMax;
    dlg.m_fCurMin = m_fCurMin;
    dlg.m_fCurMax = m_fCurMax;
    dlg.m_iCoverMode = m_iCoverMode;
    if (dlg.DoModal() == IDOK)
    {
        m_iZoom = dlg.m_iZoom;
        m_fMin = dlg.m_fMin;
        m_fMax = dlg.m_fMax;
        m_fCurMin = dlg.m_fCurMin;
        m_fCurMax = dlg.m_fCurMax;
        m_iCoverMode = dlg.m_iCoverMode;

        // колличество точек по осям X и Z отображаемых в текущем окне
        CRect rc;
        GetClientRect(rc);
        m_iViewCX = rc.Width()  / m_iZoom;
        m_iViewCY = rc.Height() / m_iZoom;

        // размерность скролов
        SetScrollRange(SB_HORZ, 0, (m_iCX > m_iViewCX) ? m_iCX - m_iViewCX : 0);
        SetScrollRange(SB_VERT, 0, (m_iCY > m_iViewCY) ? m_iCY - m_iViewCY : 0);

        CreateBitmap();

        Draw(NULL);
    }

    if (bInfoVisible)
        m_pDlgInfo->ShowWindow(SW_SHOW);
    if (bWaysVisible)
        m_pDlgWays->ShowWindow(SW_SHOW);
}

void CDlgGraphViewer::OnMenuShowInfo() 
{
    CMenu* pMenu = GetMenu();
    if (m_pDlgInfo->IsWindowVisible())
    {
        m_pDlgInfo->ShowWindow(SW_HIDE);
        pMenu->CheckMenuItem(IDM_SHOW_INFO, MF_BYCOMMAND | MF_UNCHECKED);
    }
    else
    {
        m_pDlgInfo->ShowWindow(SW_SHOW);
        pMenu->CheckMenuItem(IDM_SHOW_INFO, MF_BYCOMMAND | MF_CHECKED);
    }
}

void CDlgGraphViewer::OnShowWays() 
{
    CMenu* pMenu = GetMenu();
    if (m_pDlgWays->IsWindowVisible())
    {
        m_pDlgWays->ShowWindow(SW_HIDE);
        pMenu->CheckMenuItem(IDM_SHOW_WAYS, MF_BYCOMMAND | MF_UNCHECKED);
    }
    else
    {
        m_pDlgWays->ShowWindow(SW_SHOW);
        pMenu->CheckMenuItem(IDM_SHOW_WAYS, MF_BYCOMMAND | MF_CHECKED);
    }
}
