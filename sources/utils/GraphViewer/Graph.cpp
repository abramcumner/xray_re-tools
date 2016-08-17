//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Graph.h"



///////////////////////////////////////////////////////////////////////////////////////////////////
// game.graph
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
CGraph::CGraph()
{
    m_chFilename[0] = 0;
    m_pLevels = NULL;
    m_pVertexs = NULL;
    m_pEdges = NULL;
    m_pLevelPoints = NULL;
    m_pGrid = NULL;
}

//-------------------------------------------------------------------------------------------------
CGraph::~CGraph()
{
    Clean();
}

//-------------------------------------------------------------------------------------------------
void CGraph::Clean()
{
    m_chFilename[0] = 0;

    if (m_pLevels)
    {
        delete[] m_pLevels;
        m_pLevels = NULL;
    }

    if (m_pVertexs)
    {
        delete[] m_pVertexs;
        m_pVertexs = NULL;
    }

    if (m_pEdges)
    {
        delete[] m_pEdges;
        m_pEdges = NULL;
    }

    if (m_pLevelPoints)
    {
        delete[] m_pLevelPoints;
        m_pLevelPoints = NULL;
    }

    if (m_pGrid)
    {
        delete[] m_pGrid;
        m_pGrid = NULL;
    }
}

//-------------------------------------------------------------------------------------------------
bool CGraph::Load(LPCSTR pFilename)
{
    DWORD dwReaded;
    CString str;
    unsigned u;

    Clean();

    HANDLE hFile = CreateFile(pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        AfxMessageBox("Ошибка открытия файла!");
        Clean();
        return false;
    }

    // заголовок
    if (!ReadFile(hFile, &m_sHeader, sizeof(GG_HEADER), &dwReaded, NULL) || (dwReaded!=sizeof(GG_HEADER)))
    {
        AfxMessageBox("Ошибка чтения заголовка!");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    // уровни
    if (m_sHeader.level_count <= 0)
    {
        AfxMessageBox("Ошибка: level_count <= 0");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    m_pLevels = new GG_LEVEL[m_sHeader.level_count];
    if (!m_pLevels)
    {
        AfxMessageBox("Ошибка выделения памяти!");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    for (u=0; u<m_sHeader.level_count; u++)
    {
        if (!LoadLevel(hFile, &m_pLevels[u]))
        {
            str.Format("Ошибка чтения уровня №%d", u);
            AfxMessageBox(str);
            CloseHandle(hFile);
            Clean();
            return false;
        }
    }

    // game vertex
    if (m_sHeader.vertex_count <= 0)
    {
        AfxMessageBox("Ошибка: vertex_count <= 0");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    m_pVertexs = new GG_VERTEX[m_sHeader.vertex_count];
    if (!m_pVertexs)
    {
        AfxMessageBox("Ошибка выделения памяти!");
        CloseHandle(hFile);
        Clean();
        return false;
    }
    if (!ReadFile(hFile, m_pVertexs, m_sHeader.vertex_count*sizeof(GG_VERTEX), &dwReaded, NULL) || (dwReaded!=m_sHeader.vertex_count*sizeof(GG_VERTEX)))
    {
        AfxMessageBox("Ошибка чтения GG_VERTEX!");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    // пересчитываем смещения в индексы
    unsigned edges_offset = m_sHeader.vertex_count * 0x2a;
    unsigned level_points_offset = edges_offset + m_sHeader.edge_count * 0x06;
    for (u=0; u<m_sHeader.vertex_count; u++)
    {
        if (m_pVertexs[u].edge_count > 0)
            m_pVertexs[u].edge_offset = (m_pVertexs[u].edge_offset - edges_offset) / 0x06;

        if (m_pVertexs[u].level_point_count > 0)
            m_pVertexs[u].level_point_offset = (m_pVertexs[u].level_point_offset - level_points_offset) / 0x14;
    }

    // edge
    if (m_sHeader.edge_count <= 0)
    {
        AfxMessageBox("Ошибка: edge_count <= 0");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    m_pEdges = new GG_EDGE[m_sHeader.edge_count];
    if (!m_pEdges)
    {
        AfxMessageBox("Ошибка выделения памяти!");
        CloseHandle(hFile);
        Clean();
        return false;
    }
    if (!ReadFile(hFile, m_pEdges, m_sHeader.edge_count*sizeof(GG_EDGE), &dwReaded, NULL) || (dwReaded!=m_sHeader.edge_count*sizeof(GG_EDGE)))
    {
        AfxMessageBox("Ошибка чтения GG_EDGE!");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    // level vertex
    if (m_sHeader.level_point_count > 0)
    {
        m_pLevelPoints = new GG_LEVEL_POINT[m_sHeader.level_point_count];
        if (!m_pLevelPoints)
        {
            AfxMessageBox("Ошибка выделения памяти!");
            CloseHandle(hFile);
            Clean();
            return false;
        }
        if (!ReadFile(hFile, m_pLevelPoints, m_sHeader.level_point_count*sizeof(GG_LEVEL_POINT), &dwReaded, NULL) || (dwReaded!=m_sHeader.level_point_count*sizeof(GG_LEVEL_POINT)))
        {
            AfxMessageBox("Ошибка чтения GG_LEVEL_POINT!");
            CloseHandle(hFile);
            Clean();
            return false;
        }
    }
    else
    {
        //AfxMessageBox("Ошибка: level_point_count <= 0");
        //CloseHandle(hFile);
        //Clean();
        //return false;
    }

    CloseHandle(hFile);
    strcpy(m_chFilename, pFilename);
    return true;
}

//-------------------------------------------------------------------------------------------------
bool CGraph::LoadLevel(HANDLE hFile, GG_LEVEL* pLevel)
{
    DWORD dwReaded;

    DWORD u=0;
    if (!ReadFile(hFile, &pLevel->level_name[u], 1, &dwReaded, NULL) || (dwReaded != 1))
    {
        AfxMessageBox("Ошибка при чтении имени уровня!");
        return false;
    }
    while (pLevel->level_name[u++] != 0)
    {
        if (!ReadFile(hFile, &pLevel->level_name[u], 1, &dwReaded, NULL) || (dwReaded != 1))
        {
            AfxMessageBox("Ошибка при чтении имени уровня!");
            return false;
        }
    }

    if (!ReadFile(hFile, &pLevel->offset, sizeof(POINT_3D), &dwReaded, NULL) || (dwReaded != sizeof(POINT_3D)))
    {
        AfxMessageBox("Ошибка при чтении смещения уровня!");
        return false;
    }

    if (!ReadFile(hFile, &pLevel->level_id, 1, &dwReaded, NULL) || (dwReaded != 1))
    {
        AfxMessageBox("Ошибка при чтении id уровня!");
        return false;
    }

    u=0;
    if (!ReadFile(hFile, &pLevel->section_name[u], 1, &dwReaded, NULL) || (dwReaded != 1))
    {
        AfxMessageBox("Ошибка при чтении секции уровня!");
        return false;
    }
    while (pLevel->section_name[u++] != 0)
    {
        if (!ReadFile(hFile, &pLevel->section_name[u], 1, &dwReaded, NULL) || (dwReaded != 1))
        {
            AfxMessageBox("Ошибка при чтении секции уровня!");
            return false;
        }
    }

    if (!ReadFile(hFile, &pLevel->guid, 16, &dwReaded, NULL) || (dwReaded != 16))
    {
        AfxMessageBox("Ошибка при чтении guid уровня!");
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
void CGraph::GridClean()
{
    m_uGridCX = m_uGridCY = m_uGridCZ = 0;

    if (m_pGrid)
    {
        delete[] m_pGrid;
        m_pGrid = NULL;
    }
}

//-------------------------------------------------------------------------------------------------
bool CGraph::GridInit(unsigned uLevel)
{
    unsigned u, i, index, size, x, z, k;
    POINT_3D* ptr;

    unsigned level_id = m_pLevels[uLevel].level_id;

    // размерность матрицы по X и Z
    m_sGridBox.p1.x = m_sGridBox.p1.y = m_sGridBox.p1.z =  10000;
    m_sGridBox.p2.x = m_sGridBox.p2.y = m_sGridBox.p2.z = -10000;

    for (u=0; u<m_sHeader.vertex_count; u++)
    {
        if (m_pVertexs[u].level_id == level_id)
        {
            ptr = &m_pVertexs[u].level_point;

            if (m_sGridBox.p1.x > ptr->x)
                m_sGridBox.p1.x = ptr->x;
            if (m_sGridBox.p1.y > ptr->y)
                m_sGridBox.p1.y = ptr->y;
            if (m_sGridBox.p1.z > ptr->z)
                m_sGridBox.p1.z = ptr->z;

            if (m_sGridBox.p2.x < ptr->x)
                m_sGridBox.p2.x = ptr->x;
            if (m_sGridBox.p2.y < ptr->y)
                m_sGridBox.p2.y = ptr->y;
            if (m_sGridBox.p2.z < ptr->z)
                m_sGridBox.p2.z = ptr->z;

            index = m_pVertexs[u].level_point_offset;
            for (i=0; i<m_pVertexs[u].level_point_count; i++)
            {
                ptr = &m_pLevelPoints[index+i].point;

                if (m_sGridBox.p1.x > ptr->x)
                    m_sGridBox.p1.x = ptr->x;
                if (m_sGridBox.p1.y > ptr->y)
                    m_sGridBox.p1.y = ptr->y;
                if (m_sGridBox.p1.z > ptr->z)
                    m_sGridBox.p1.z = ptr->z;

                if (m_sGridBox.p2.x < ptr->x)
                    m_sGridBox.p2.x = ptr->x;
                if (m_sGridBox.p2.y < ptr->y)
                    m_sGridBox.p2.y = ptr->y;
                if (m_sGridBox.p2.z < ptr->z)
                    m_sGridBox.p2.z = ptr->z;
            }
        }
    }

    m_uGridCX = (unsigned) ((m_sGridBox.p2.x - m_sGridBox.p1.x) / 0.7 + 1.501);
    m_uGridCZ = (unsigned) ((m_sGridBox.p2.z - m_sGridBox.p1.z) / 0.7 + 1.501);

    // размерность матрицы по Y
    m_uGridCY = 1;
    size = m_uGridCX * m_uGridCZ;
    m_pGrid = new unsigned[size];
    if (!m_pGrid)
    {
        AfxMessageBox("Ошибка выделения памяти!");
        return false;
    }
    memset(m_pGrid, 0, size * sizeof(unsigned));

    for (u=0; u<m_sHeader.vertex_count; u++)
    {
        if (m_pVertexs[u].level_id == level_id)
        {
            ptr = &m_pVertexs[u].level_point;

            //x = (unsigned) ((ptr->x - m_sGridBox.p1.x) / 0.7 + 0.501);
            //z = (unsigned) ((ptr->z - m_sGridBox.p1.z) / 0.7 + 0.501);
            x = (unsigned) ((ptr->x - m_sGridBox.p1.x) / 0.7);
            z = (unsigned) ((ptr->z - m_sGridBox.p1.z) / 0.7);
            k = x * m_uGridCZ + z;
            m_pGrid[k]++;
            if (m_uGridCY < m_pGrid[k])
                m_uGridCY = m_pGrid[k];

            index = m_pVertexs[u].level_point_offset;
            for (i=0; i<m_pVertexs[u].level_point_count; i++)
            {
                ptr = &m_pLevelPoints[index+i].point;

                //x = (unsigned) ((ptr->x - m_sGridBox.p1.x) / 0.7 + 0.501);
                //z = (unsigned) ((ptr->z - m_sGridBox.p1.z) / 0.7 + 0.501);
                x = (unsigned) ((ptr->x - m_sGridBox.p1.x) / 0.7);
                z = (unsigned) ((ptr->z - m_sGridBox.p1.z) / 0.7);
                k = x * m_uGridCZ + z;
                m_pGrid[k]++;
                if (m_uGridCY < m_pGrid[k])
                    m_uGridCY = m_pGrid[k];
            }
        }
    }

    delete[] m_pGrid;

    // заполнение матрицы индексами вертексов, для game_vertex-ов ставим 1 в старший бит
    size = m_uGridCX * m_uGridCZ * m_uGridCY;
    m_pGrid = new unsigned[size];
    if (!m_pGrid)
    {
        AfxMessageBox("Ошибка выделения памяти!");
        return false;
    }
    memset(m_pGrid, -1, size * sizeof(unsigned));

    for (u=0; u<m_sHeader.vertex_count; u++)
    {
        if (m_pVertexs[u].level_id == level_id)
        {
            ptr = &m_pVertexs[u].level_point;

            //x = (unsigned) ((ptr->x - m_sGridBox.p1.x) / 0.7 + 0.501);
            //z = (unsigned) ((ptr->z - m_sGridBox.p1.z) / 0.7 + 0.501);
            x = (unsigned) ((ptr->x - m_sGridBox.p1.x) / 0.7);
            z = (unsigned) ((ptr->z - m_sGridBox.p1.z) / 0.7);
            k = (x * m_uGridCZ + z) * m_uGridCY;
            while (m_pGrid[k] != -1)
                k++;
            m_pGrid[k] = u | 0x80000000;

            index = m_pVertexs[u].level_point_offset;
            for (i=0; i<m_pVertexs[u].level_point_count; i++)
            {
                ptr = &m_pLevelPoints[index+i].point;

                //x = (unsigned) ((ptr->x - m_sGridBox.p1.x) / 0.7 + 0.501);
                //z = (unsigned) ((ptr->z - m_sGridBox.p1.z) / 0.7 + 0.501);
                x = (unsigned) ((ptr->x - m_sGridBox.p1.x) / 0.7);
                z = (unsigned) ((ptr->z - m_sGridBox.p1.z) / 0.7);
                k = (x * m_uGridCZ + z) * m_uGridCY;
                while (m_pGrid[k] != -1)
                    k++;
                m_pGrid[k] = index+i;
            }
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
void CGraph::DrawVertexs(DRAW_DATA* pDD)
{
    if (!m_pVertexs || !m_pGrid)
        return;

    GG_VERTEX* ptr;
    int pt = ((pDD->iZoom < 3) ? 1 : pDD->iZoom / 2)+1; // размер точки
    COLORREF cr = RGB(0,192,0);                         // цвет точки

    unsigned i, v, gx, gy;
    for (unsigned x=0; x<(unsigned)pDD->cx; x++)
    {
        for (unsigned y=0; y<(unsigned)pDD->cy; y++)
        {
            gx = x + pDD->x;
            gy = (m_uGridCZ-1) - pDD->y - y;

            if ((gx < m_uGridCX) && (gy < m_uGridCZ))
            {
                i = (gx * m_uGridCZ + gy) * m_uGridCY;

                for (unsigned k=0; k<m_uGridCY; k++)
                {
                    v = m_pGrid[i + k];

                    if (v != -1)
                    {
                        if (v & 0x80000000)
                        {
                            ptr = &m_pVertexs[v & 0x7fffffff];
                            if ((ptr->level_point.y >= pDD->fMinY) && (ptr->level_point.y <= pDD->fMaxY))
                            {
                                pDD->pDC->FillSolidRect(x*pDD->iZoom-1, y*pDD->iZoom-1, pt, pt, cr);
                                break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void CGraph::DrawEdges(DRAW_DATA* pDD)
{
    if (!m_pEdges || !m_pGrid)
        return;

    GG_VERTEX* ptr;
    int pt = ((pDD->iZoom < 3) ? 1 : pDD->iZoom / 2)+3; // размер точки
    COLORREF cr = RGB(0,192,0);                         // цвет точки

    CPen pen(PS_SOLID, 1, cr);
    CPen* pOldPen = pDD->pDC->SelectObject(&pen);
    pDD->pDC->SetBkMode(OPAQUE);
    pDD->pDC->SetROP2(R2_COPYPEN);

    unsigned i, v, gx, gy;
    for (unsigned x=0; x<(unsigned)pDD->cx; x++)
    {
        for (unsigned y=0; y<(unsigned)pDD->cy; y++)
        {
            gx = x + pDD->x;
            gy = (m_uGridCZ-1) - pDD->y - y;

            if ((gx < m_uGridCX) && (gy < m_uGridCZ))
            {
                i = (gx * m_uGridCZ + gy) * m_uGridCY;

                for (unsigned k=0; k<m_uGridCY; k++)
                {
                    v = m_pGrid[i + k];

                    if (v != -1)
                    {
                        if (v & 0x80000000)
                        {
                            ptr = &m_pVertexs[v & 0x7fffffff];
                            if ((ptr->level_point.y >= pDD->fMinY) && (ptr->level_point.y <= pDD->fMaxY))
                            {
                                for (unsigned l=0; l<m_pVertexs[v].edge_count; l++)
                                {
                                    unsigned  gvid = m_pEdges[m_pVertexs[v].edge_offset+l].game_vertex_id;

                                    if (m_pVertexs[v].level_id == m_pVertexs[gvid].level_id)
                                    {
                                        POINT_3D* pDst = &m_pVertexs[gvid].level_point;
                                        unsigned  x2 = (unsigned) (abs((pDst->x - m_sGridBox.p1.x) / 0.7) - pDD->x);
                                        unsigned  y2 = (unsigned) ((m_uGridCZ-1) - abs((pDst->z - m_sGridBox.p1.z) / 0.7) - pDD->y);

                                        pDD->pDC->MoveTo(x*pDD->iZoom, y*pDD->iZoom);
                                        pDD->pDC->LineTo(x2*pDD->iZoom, y2*pDD->iZoom);
                                    }
                                    else
                                    {
                                        pDD->pDC->FillSolidRect(x*pDD->iZoom-1, y*pDD->iZoom-1, pt, pt, cr);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    pDD->pDC->SelectObject(pOldPen);
}

//-------------------------------------------------------------------------------------------------
void CGraph::DrawLevelPoints(DRAW_DATA* pDD)
{
    if (!m_pLevelPoints || !m_pGrid)
        return;

    GG_LEVEL_POINT* ptr;
    int pt = 1;//(pDD->iZoom < 3) ? 1 : pDD->iZoom / 2; // размер точки
    COLORREF cr = RGB(0,0,92);                      // цвет точки

    unsigned i, v, gx, gy;
    for (unsigned x=0; x<(unsigned)pDD->cx; x++)
    {
        for (unsigned y=0; y<(unsigned)pDD->cy; y++)
        {
            gx = x + pDD->x;
            gy = (m_uGridCZ-1) - pDD->y - y;

            if ((gx < m_uGridCX) && (gy < m_uGridCZ))
            {
                i = (gx * m_uGridCZ + gy) * m_uGridCY;

                for (unsigned k=0; k<m_uGridCY; k++)
                {
                    v = m_pGrid[i + k];

                    if (v != -1)
                    {
                        if (!(v & 0x80000000))
                        {
                            ptr = &m_pLevelPoints[v];
                            if ((ptr->point.y >= pDD->fMinY) && (ptr->point.y <= pDD->fMaxY))
                            {
                                pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, cr);
                                break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void CGraph::DrawSelVertexs(DRAW_DATA* pDD, unsigned uVertex)
{
    if (!m_pVertexs || !m_pGrid)
        return;

    GG_VERTEX* ptr = &m_pVertexs[uVertex & 0x7fffffff];
    int pt = ((pDD->iZoom < 3) ? 1 : pDD->iZoom / 2)+1; // размер точки
    COLORREF cr = RGB(192,0,0);                         // цвет точки

    unsigned x = (unsigned) (abs((ptr->level_point.x - m_sGridBox.p1.x) / 0.7) - pDD->x);
    unsigned y = (unsigned) ((m_uGridCZ-1) - abs((ptr->level_point.z - m_sGridBox.p1.z) / 0.7) - pDD->y);

    if ((ptr->level_point.y >= pDD->fMinY) && (ptr->level_point.y <= pDD->fMaxY))
    {
        pDD->pDC->FillSolidRect(x*pDD->iZoom-1, y*pDD->iZoom-1, pt, pt, cr);
    }
}

//-------------------------------------------------------------------------------------------------
void CGraph::DrawSelEdges(DRAW_DATA* pDD, unsigned uVertex)
{
    if (!m_pEdges || !m_pGrid)
        return;

    GG_VERTEX* ptr = &m_pVertexs[uVertex & 0x7fffffff];
    int pt = ((pDD->iZoom < 3) ? 1 : pDD->iZoom / 2)+3; // размер точки
    COLORREF cr = RGB(192,0,0);                         // цвет точки

    CPen pen(PS_SOLID, 1, cr);
    CPen* pOldPen = pDD->pDC->SelectObject(&pen);
    pDD->pDC->SetBkMode(OPAQUE);
    pDD->pDC->SetROP2(R2_COPYPEN);

    unsigned x = (unsigned) (abs((ptr->level_point.x - m_sGridBox.p1.x) / 0.7) - pDD->x);
    unsigned y = (unsigned) ((m_uGridCZ-1) - abs((ptr->level_point.z - m_sGridBox.p1.z) / 0.7) - pDD->y);

    if ((ptr->level_point.y >= pDD->fMinY) && (ptr->level_point.y <= pDD->fMaxY))
    {
        for (unsigned l=0; l<m_pVertexs[uVertex].edge_count; l++)
        {
            unsigned gvid = m_pEdges[m_pVertexs[uVertex].edge_offset+l].game_vertex_id;

            if (m_pVertexs[uVertex].level_id == m_pVertexs[gvid].level_id)
            {
                POINT_3D* pDst = &m_pVertexs[gvid].level_point;
                unsigned  x2 = (unsigned) (abs((pDst->x - m_sGridBox.p1.x) / 0.7) - pDD->x);
                unsigned  y2 = (unsigned) ((m_uGridCZ-1) - abs((pDst->z - m_sGridBox.p1.z) / 0.7) - pDD->y);

                pDD->pDC->MoveTo(x*pDD->iZoom, y*pDD->iZoom);
                pDD->pDC->LineTo(x2*pDD->iZoom, y2*pDD->iZoom);
            }
            else
            {
                pDD->pDC->FillSolidRect(x*pDD->iZoom-1, y*pDD->iZoom-1, pt, pt, cr);
            }
        }
    }

    pDD->pDC->SelectObject(pOldPen);
}

//-------------------------------------------------------------------------------------------------
void CGraph::DrawSelLevelPoints(DRAW_DATA* pDD, unsigned uVertex)
{
    if (!m_pLevelPoints || !m_pGrid)
        return;

    POINT_3D* ptr;
    int pt = 2;//(pDD->iZoom < 3) ? 2 : pDD->iZoom / 2 + 1; // размер точки
    COLORREF cr = RGB(192,0,0);                         // цвет точки

    if (uVertex & 0x80000000)
    {
        uVertex &= 0x7fffffff;

        unsigned index = m_pVertexs[uVertex].level_point_offset;
        for (unsigned i=0; i<m_pVertexs[uVertex].level_point_count; i++)
        {
            ptr = &m_pLevelPoints[index+i].point;

            unsigned x = (unsigned) (abs((ptr->x  - m_sGridBox.p1.x) / 0.7) - pDD->x);
            unsigned y = (unsigned) ((m_uGridCZ-1) - abs((ptr->z - m_sGridBox.p1.z) / 0.7) - pDD->y);

            if ((ptr->y >= pDD->fMinY) && (ptr->y <= pDD->fMaxY))
            {
                pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, cr);
            }
        }
    }
    else
    {
        ptr = &m_pLevelPoints[uVertex].point;

        unsigned x = (unsigned) (abs((ptr->x  - m_sGridBox.p1.x) / 0.7) - pDD->x);
        unsigned y = (unsigned) ((m_uGridCZ-1) - abs((ptr->z - m_sGridBox.p1.z) / 0.7) - pDD->y);

        if ((ptr->y >= pDD->fMinY) && (ptr->y <= pDD->fMaxY))
        {
            pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, cr);
        }
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// level.ai
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
CLevel::CLevel()
{
    m_chFilename[0] = 0;
    memset(&m_sHeader, 0, sizeof(AI_HEADER));
    m_pPoints = NULL;

    m_pGrid = NULL;
}

//-------------------------------------------------------------------------------------------------
CLevel::~CLevel()
{
    Clean();
}

//-------------------------------------------------------------------------------------------------
void CLevel::Clean()
{
    m_chFilename[0] = 0;
    memset(&m_sHeader, 0, sizeof(AI_HEADER));

    if (m_pPoints)
    {
        delete[] m_pPoints;
        m_pPoints = NULL;
    }

    if (m_pGrid)
    {
        delete[] m_pGrid;
        m_pGrid = NULL;
    }
}

//-------------------------------------------------------------------------------------------------
bool CLevel::Load(LPCSTR pFilename)
{
    DWORD dwReaded;

    Clean();

    HANDLE hFile = CreateFile(pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        AfxMessageBox("Ошибка открытия файла!");
        Clean();
        return false;
    }

    // заголовок
    if (!ReadFile(hFile, &m_sHeader, sizeof(AI_HEADER), &dwReaded, NULL) || (dwReaded!=sizeof(AI_HEADER)))
    {
        AfxMessageBox("Ошибка чтения заголовка!");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    // вертексы
    if (m_sHeader.vertex_count <= 0)
    {
        AfxMessageBox("Ошибка: vertex_count <= 0");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    m_pPoints = new AI_POINT[m_sHeader.vertex_count];
    if (!m_pPoints)
    {
        AfxMessageBox("Ошибка выделения памяти!");
        CloseHandle(hFile);
        Clean();
        return false;
    }
    if (!ReadFile(hFile, m_pPoints, m_sHeader.vertex_count*sizeof(AI_POINT), &dwReaded, NULL) || (dwReaded!=m_sHeader.vertex_count*sizeof(AI_POINT)))
    {
        AfxMessageBox("Ошибка чтения AI_POINT!");
        CloseHandle(hFile);
        Clean();
        return false;
    }

    CloseHandle(hFile);
    strcpy(m_chFilename, pFilename);
    return true;
}

//-------------------------------------------------------------------------------------------------
void CLevel::GridClean()
{
    m_uGridCX = m_uGridCY = m_uGridCZ = 0;

    if (m_pGrid)
    {
        delete[] m_pGrid;
        m_pGrid = NULL;
    }
}

//-------------------------------------------------------------------------------------------------
bool CLevel::GridInit()
{
    unsigned u, size, i;

    // размерность матрицы по X и Z
    m_uGridCX = (unsigned) ((m_sHeader.box.p2.x - m_sHeader.box.p1.x) / m_sHeader.cell_size + 1.501);
    m_uGridCZ = (unsigned) ((m_sHeader.box.p2.z - m_sHeader.box.p1.z) / m_sHeader.cell_size + 1.501);

    // размерность матрицы по Y
    m_uGridCY = 1;
    size = m_uGridCX * m_uGridCZ;
    m_pGrid = new unsigned[size];
    if (!m_pGrid)
    {
        AfxMessageBox("Ошибка выделения памяти!");
        return false;
    }
    memset(m_pGrid, 0, size * sizeof(unsigned));

    for (u=0; u<m_sHeader.vertex_count; u++)
    {
        i = m_pPoints[u].packed_xz;
        m_pGrid[i]++;
        if (m_pGrid[i] > (unsigned) m_uGridCY)
            m_uGridCY = m_pGrid[i];
    }

    delete[] m_pGrid;

    // заполнение матрицы индексами вертексов
    size = m_uGridCX * m_uGridCZ * m_uGridCY;
    m_pGrid = new unsigned[size];
    if (!m_pGrid)
    {
        AfxMessageBox("Ошибка выделения памяти!");
        return false;
    }
    memset(m_pGrid, -1, size * sizeof(unsigned));

    for (u=0; u<m_sHeader.vertex_count; u++)
    {
        i = m_pPoints[u].packed_xz * m_uGridCY;
        while (m_pGrid[i] != -1)
            i++;
        m_pGrid[i] = u;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
void CLevel::DrawVertexs(DRAW_DATA* pDD)
{
    if (!m_pPoints || !m_pGrid)
        return;

    float f;
    int pt = 1;//(pDD->iZoom < 3) ? 1 : pDD->iZoom / 2; // размер точки
    COLORREF cr = RGB(0,0,92);                      // цвет точки

    if (pDD->iCoverMode == 0)
    {
        unsigned i, v, gx, gy;
        for (unsigned x=0; x<(unsigned)pDD->cx; x++)
        {
            for (unsigned y=0; y<(unsigned)pDD->cy; y++)
            {
                gx = x + pDD->x;
                gy = (m_uGridCZ-1) - pDD->y - y;

                if ((gx < m_uGridCX) && (gy < m_uGridCZ))
                {
                    i = (gx * m_uGridCZ + gy) * m_uGridCY;

                    for (unsigned k=0; k<m_uGridCY; k++)
                    {
                        v = m_pGrid[i + k];
                        if (v != -1)
                        {
                            f = m_sHeader.box.p1.y + (float) m_pPoints[v].packed_y * m_sHeader.factor_y / 65535;
                            if ((f >= pDD->fMinY) && (f <= pDD->fMaxY))
                            {
                                pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, cr);
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
    else
    {
        unsigned _cr[] = {
            RGB(0,  0,128),
            RGB(0, 32,128),
            RGB(0, 64,128),
            RGB(0,128,128),

            RGB( 0,128,64),
            RGB( 0,128,32),
            RGB( 0,128,0),
            RGB(32,128,0),

            RGB( 64,128,0),
            RGB(128,128,0),
            RGB(128, 64,0),
            RGB(128, 32,0),

            RGB(128,0,0),
            RGB(128,0,0),
            RGB(128,0,0),
            RGB(128,0,0)
        };

        unsigned _cr2[] = {
            RGB(0,  0,128),
            RGB(0, 16,128),
            RGB(0, 32,128),
            RGB(0, 64,128),

            RGB(0,128,128),
            RGB(0,128,64),
            RGB(0,128,32),
            RGB(0,128,16),

            RGB( 16,128,0),
            RGB( 32,128,0),
            RGB( 64,128,0),
            RGB(128,128,0),

            RGB(128,64,0),
            RGB(128,32,0),
            RGB(128,16,0),
            RGB(128, 0,0)
        };

        unsigned i, v, gx, gy, ugol;
        for (unsigned x=0; x<(unsigned)pDD->cx; x++)
        {
            for (unsigned y=0; y<(unsigned)pDD->cy; y++)
            {
                gx = x + pDD->x;
                gy = (m_uGridCZ-1) - pDD->y - y;

                if ((gx < m_uGridCX) && (gy < m_uGridCZ))
                {
                    i = (gx * m_uGridCZ + gy) * m_uGridCY;

                    for (unsigned k=0; k<m_uGridCY; k++)
                    {
                        v = m_pGrid[i + k];
                        if (v != -1)
                        {
                            f = m_sHeader.box.p1.y + (float) m_pPoints[v].packed_y * m_sHeader.factor_y / 65535;
                            if ((f >= pDD->fMinY) && (f <= pDD->fMaxY))
                            {
                                switch (pDD->iCoverMode)
                                {
                                    case 1:
                                        pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, _cr[m_pPoints[v].cover1]);
                                        break;
                                    case 2:
                                        pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, _cr[m_pPoints[v].cover2]);
                                        break;
                                    case 3:
                                        pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, _cr[m_pPoints[v].cover3]);
                                        break;
                                    case 4:
                                        pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, _cr[m_pPoints[v].cover4]);
                                        break;
                                    //case 5:
                                    default:
                                        //перевод сферических координат в декартовы:
                                        // x = R*sin(Fi)*cos(Teta)
                                        // y = R*sin(Fi)*sin(Teta)
                                        // z = R*cos(Fi)
                                        //double pi = 3.1415926535;
                                        //double v = ((int)((pPoint->plane >> 7) & 0x03f)) * 0.5*pi / 64;
                                        //double u = ((int)((pPoint->plane >> 0) & 0x07f)) * 0.5*pi / 128;
                                        //double nx = sin(v)*cos(u);
                                        //double nz = sin(v)*sin(u);
                                        //double ny = cos(v);
                                        ugol = ((m_pPoints[v].plane >> 7) & 0x03f);
                                        if (ugol > 31)
                                            ugol = 31;
                                        pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, _cr2[ugol/2]);
                                        break;
                                };
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void CLevel::DrawSelVertexs(DRAW_DATA* pDD, unsigned uVertex)
{
    if (!m_pPoints || !m_pGrid)
        return;

    int pt = 2;//(pDD->iZoom < 3) ? 2 : pDD->iZoom / 2 + 1; // размер точки
    COLORREF cr = RGB(192,0,0);                         // цвет точки

    unsigned x = (m_pPoints[uVertex].packed_xz / m_uGridCZ) - pDD->x;
    unsigned y = (m_uGridCZ-1) - (m_pPoints[uVertex].packed_xz % m_uGridCZ) - pDD->y;

    float f = m_sHeader.box.p1.y + (float) m_pPoints[uVertex].packed_y * m_sHeader.factor_y / 65535;
    if ((f >= pDD->fMinY) && (f <= pDD->fMaxY))
    {
        pDD->pDC->FillSolidRect(x*pDD->iZoom, y*pDD->iZoom, pt, pt, cr);
    }
}

//-------------------------------------------------------------------------------------------------
