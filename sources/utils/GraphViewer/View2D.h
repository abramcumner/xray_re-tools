#if !defined(AFX_VIEW2D_H__87A69507_4814_497E_80EE_C4651C6F2F5C__INCLUDED_)
#define AFX_VIEW2D_H__87A69507_4814_497E_80EE_C4651C6F2F5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View2D.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView2D window

class CView2D : public CStatic
{
public:
    CGraph*   m_pGraph;
    CLevel*   m_pLevel;

    unsigned  m_uVertexCount;
    unsigned  m_uPointCount;
    unsigned* m_pVertex;
    unsigned* m_pPoint;
    float     m_fMinX, m_fMinY, m_fMinZ;
    float     m_fMaxX, m_fMaxY, m_fMaxZ;
    int       m_iX;
    int       m_iZ;
    int       m_iCX;
    int       m_iCZ;
    int       m_iViewCX;
    int       m_iViewCZ;
    int       m_iZoom;

public:
	CView2D();
	virtual ~CView2D();

    void Clean();
    void SetGraph(CGraph* pGraph, unsigned uLevel);
    void SetLevel(CLevel* pLevel);
    void SetOffset(int x, int z);

    void Draw(CDC* pDC);

    void DrawGraph(CDC* pDC);
    void DrawGraphPoints(CDC* pDC);
    void DrawGraphEdges(CDC* pDC);
    void DrawGraphVertexs(CDC* pDC);

    void DrawLevel(CDC* pDC);
    void DrawLevelPoints(CDC* pDC);

	//{{AFX_VIRTUAL(CView2D)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CView2D)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_VIEW2D_H__87A69507_4814_497E_80EE_C4651C6F2F5C__INCLUDED_)
