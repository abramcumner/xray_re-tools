// View2D.cpp : implementation file
//

#include "stdafx.h"
#include "GraphViewer.h"
#include "View2D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView2D

CView2D::CView2D()
{
    m_pGraph = NULL;
    m_pLevel = NULL;
}

CView2D::~CView2D()
{
}


BEGIN_MESSAGE_MAP(CView2D, CStatic)
	//{{AFX_MSG_MAP(CView2D)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView2D message handlers

void CView2D::OnPaint() 
{
	CPaintDC dc(this);

    Draw(&dc);
}

void CView2D::Clean()
{
    m_pGraph = NULL;
    m_pLevel = NULL;

    if (m_pVertex)
    {
        delete[] m_pVertex;
        m_pVertex = NULL;
    }

    if (m_pPoint)
    {
        delete[] m_pPoint;
        m_pPoint = NULL;
    }

    m_uVertexCount = m_uPointCount = 0;

    m_fMinX = m_fMaxX = \
    m_fMinY = m_fMaxY = \
    m_fMinZ = m_fMaxZ = 0.0f;
}

void CView2D::SetGraph(CGraph* pGraph, unsigned uLevel)
{
    Clean();

    if (!pGraph)
        return;

    m_pGraph = pGraph;

    unsigned i,j,index;
    unsigned level_id = m_pGraph->m_pLevels[uLevel].level_id;

    // считаем колличество точек
    for (i=0; i<m_pGraph->m_sHeader.vertex_count; i++)
    {
        if (m_pGraph->m_pVertexs[i].level_id == level_id)
        {
            m_uVertexCount++;
            m_uPointCount += m_pGraph->m_pVertexs[i].level_point_count;
        }
    }

    // выделяем память
    m_pVertex = new unsigned[m_uVertexCount];
    if (!m_pVertex)
    {
        Clean();
        AfxMessageBox("Ошибка выделения памяти");
        return false;
    }

    if (m_uPointCount > 0)
    {
        m_pPoint  = new unsigned[m_uPointCount];
        if (!m_pPoint)
        {
            Clean();
            AfxMessageBox("Ошибка выделения памяти");
            return false;
        }
    }

    // считаем колличество точек
    unsigned uVertex = 0;
    unsigned uPoint  = 0;
    for (i=0; i<m_pGraph->m_sHeader.vertex_count; i++)
    {
        if (m_pGraph->m_pVertexs[i].level_id == level_id)
        {
            m_pVertex[uVertex++] = i;

            for (j=0; j<m_pGraph->m_pVertexs[i].level_point_count; j++)
                m_pPoint[uPoint++] = m_pGraph->m_pVertexs[i].level_point_offset + j;
        }
    }

    // поиск минимальных и максимальных координат уровня
    GG_VERTEX*      pVertexs = m_pGraph->m_pVertexs;
    GG_LEVEL_POINT* pPoints  = m_pGraph->m_pLevelPoints;

    index = m_pVertex[0];
    m_fMinX = m_fMaxX = pVertexs[index].level_point.x;
    m_fMinY = m_fMaxY = pVertexs[index].level_point.y;
    m_fMinZ = m_fMaxZ = pVertexs[index].level_point.z;

    for (i=1; i<m_uVertexCount; i++)
    {
        index = m_pVertex[i];

        if (m_fMinX > pVertexs[index].level_point.x)
            m_fMinX = pVertexs[index].level_point.x;
        if (m_fMinY > pVertexs[index].level_point.y)
            m_fMinY = pVertexs[index].level_point.y;
        if (m_fMinZ > pVertexs[index].level_point.z)
            m_fMinZ = pVertexs[index].level_point.z;

        if (m_fMaxX < pVertexs[index].level_point.x)
            m_fMaxX = pVertexs[index].level_point.x;
        if (m_fMaxY < pVertexs[index].level_point.y)
            m_fMaxY = pVertexs[index].level_point.y;
        if (m_fMaxZ < pVertexs[index].level_point.z)
            m_fMaxZ = pVertexs[index].level_point.z;
    }

    for (i=0; i<m_uPointCount; i++)
    {
        index = m_pPoint[i];

        if (m_fMinX > pPoints[index].point.x)
            m_fMinX = pPoints[index].point.x;
        if (m_fMinY > pPoints[index].point.y)
            m_fMinY = pPoints[index].point.y;
        if (m_fMinZ > pPoints[index].point.z)
            m_fMinZ = pPoints[index].point.z;

        if (m_fMaxX < pPoints[index].point.x)
            m_fMaxX = pPoints[index].point.x;
        if (m_fMaxY < pPoints[index].point.y)
            m_fMaxY = pPoints[index].point.y;
        if (m_fMaxZ < pPoints[index].point.z)
            m_fMaxZ = pPoints[index].point.z;
    }

    // колличество точек по осям X и Z для загруженного уровня
    m_iCX = (int) abs((m_fMaxX - m_fMinX) / 0.700012);
    m_iCZ = (int) abs((m_fMaxZ - m_fMinZ) / 0.700012);

    // колличество точек по осям X и Z отображаемых в текущем окне
    CRect rc;
    GetClientRect(rc);
    m_iViewCX = rc.Width()  / m_iZoom;
    m_iViewCZ = rc.Height() / m_iZoom;

    //
    SetScrollRange(SB_HORZ, 0, (m_iCX > m_iViewCX) ? m_iCX - m_iViewCX : 0);
    SetScrollRange(SB_VERT, 0, (m_iCZ > m_iViewCZ) ? m_iCZ - m_iViewCZ : 0);
    Draw(NULL);

    return true;
}

void CView2D::SetLevel(CLevel* pLevel)
{
}

void CView2D::SetOffset(int x, int y)
{
}

void CView2D::Draw(CDC* pDC)
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
    pDC->FillSolidRect(&rc, RGB(255,255,255));

    if (m_pGraph && m_pGraph->m_pVertexs)
    {
        DrawGraph(pDC);
    }
    else if (m_pLevel && m_pLevel->m_pPoints)
    {
        DrawLevel(pDC);
    }

    if (pTempDC)
        delete pTempDC;
}

void CView2D::DrawGraph(CDC* pDC)
{
    DrawGraphPoints(pDC);
    DrawGraphEdges(pDC);
    DrawGraphVertexs(pDC);
}

void CView2D::DrawGraphPoints(CDC* pDC)
{
    if (m_uPointCount == 0)
        return;

    int dx, dz;
    int x = GetScrollPos(SB_HORZ);
    int z = GetScrollPos(SB_VERT);
    int pt = (m_iZoom < 3) ? 1 : m_iZoom/2; // размер точки
    COLORREF cr = RGB(0,0,92);              // цвет точки

    float fMinX = m_fMinX + x * 0.700012f;
    float fMaxX = fMinX + m_iViewCX * 0.700012f;
    float fMaxZ = m_fMaxZ - z * 0.700012f;
    float fMinZ = fMaxZ - m_iViewCZ * 0.700012f;

    POINT_3D* pPoint;
    unsigned* pIndexs = m_pPoint;
    GG_LEVEL_POINT* pPoints = m_pGraph->m_pLevelPoints;

    for (unsigned i=0; i<m_uPointCount; i++)
    {
        pPoint = &pPoints[*pIndexs++].point;

        if ((pPoint->x >= fMinX) && (pPoint->x <= fMaxX) && (pPoint->z >= fMinZ) && (pPoint->z <= fMaxZ))
        {
            dx = (int) (abs((pPoint->x - m_fMinX) / 0.700012) - x);
            dz = (int) (abs((m_fMaxZ - pPoint->z) / 0.700012) - z);

            pDC->FillSolidRect(dx*m_iZoom, dz*m_iZoom, pt, pt, cr);
        }
    }
}

void CView2D::DrawGraphEdges(CDC* pDC)
{
    if (m_uVertexCount == 0)
        return;

    int dx1, dz1, dx2, dz2;
    int x = GetScrollPos(SB_HORZ);
    int z = GetScrollPos(SB_VERT);
    int pt = ((m_iZoom < 3) ? 1 : m_iZoom/2)+3; // размер точки
    COLORREF cr = RGB(0,192,0);                 // цвет линии
    CPen pen(PS_SOLID, 1, cr);
    CPen* pOldPen = pDC->SelectObject(&pen);

    float fMinX = m_fMinX + x * 0.700012f;
    float fMaxX = fMinX + m_iViewCX * 0.700012f;
    float fMaxZ = m_fMaxZ - z * 0.700012f;
    float fMinZ = fMaxZ - m_iViewCZ * 0.700012f;

    unsigned*  pIndexs = m_pVertex;
    GG_VERTEX* pVertex = m_pGraph->m_pVertexs;
    GG_EDGE*   pEdge   = m_pGraph->m_pEdges;
    GG_VERTEX* pV1;
    GG_VERTEX* pV2;
    POINT_3D*  pP1;
    POINT_3D*  pP2;

    for (unsigned i=0; i<m_uVertexCount; i++)
    {
        pV1 = &pVertex[*pIndexs++];
        if (pV1->edge_count > 0)
        {
            pP1 = &pV1->level_point;
            if ((pP1->x >= fMinX) && (pP1->x <= fMaxX) && (pP1->z >= fMinZ) && (pP1->z <= fMaxZ))
            {
                dx1 = (int) (abs((pP1->x - m_fMinX) / 0.700012) - x);
                dz1 = (int) (abs((m_fMaxZ - pP1->z) / 0.700012) - z);

                for (unsigned j=0; j<pV1->edge_count; j++)
                {
                    pV2 = &pVertex[pEdge[pV1->edge_offset+j].game_vertex_id];
                    if (pV2->level_id == pV1->level_id)
                    {
                        pP2 = &pV2->level_point;
                        dx2 = (int) (abs((pP2->x - m_fMinX) / 0.700012) - x);
                        dz2 = (int) (abs((m_fMaxZ - pP2->z) / 0.700012) - z);

                        pDC->MoveTo(dx1*m_iZoom, dz1*m_iZoom);
                        pDC->LineTo(dx2*m_iZoom, dz2*m_iZoom);
                    }
                    else
                    {
                        pDC->FillSolidRect(dx1*m_iZoom, dz1*m_iZoom, pt, pt, cr);
                    }
                }
            }
        }
    }

    pDC->SelectObject(pOldPen);
}

void CView2D::DrawGraphVertexs(CDC* pDC)
{
    if (m_uVertexCount == 0)
        return;

    int dx, dz;
    int x = GetScrollPos(SB_HORZ);
    int z = GetScrollPos(SB_VERT);
    int pt = ((m_iZoom < 3) ? 1 : m_iZoom/2)+1; // размер точки
    COLORREF cr = RGB(192,0,0);                 // цвет точки

    float fMinX = m_fMinX + x * 0.700012f;
    float fMaxX = fMinX + m_iViewCX * 0.700012f;
    float fMaxZ = m_fMaxZ - z * 0.700012f;
    float fMinZ = fMaxZ - m_iViewCZ * 0.700012f;

    POINT_3D*  pPoint;
    unsigned*  pIndexs = m_pVertex;
    GG_VERTEX* pVertex = m_pGraph->m_pVertexs;

    for (unsigned i=0; i<m_uVertexCount; i++)
    {
        pPoint = &pVertex[*pIndexs++].level_point;

        if ((pPoint->x >= fMinX) && (pPoint->x <= fMaxX) && (pPoint->z >= fMinZ) && (pPoint->z <= fMaxZ))
        {
            dx = (int) (abs((pPoint->x - m_fMinX) / 0.700012) - x);
            dz = (int) (abs((m_fMaxZ - pPoint->z) / 0.700012) - z);

            pDC->FillSolidRect(dx*m_iZoom, dz*m_iZoom, pt, pt, cr);
        }
    }
}

void CView2D::DrawLevel(CDC* pDC)
{
    DrawLevelVertexs(pDC);
}
