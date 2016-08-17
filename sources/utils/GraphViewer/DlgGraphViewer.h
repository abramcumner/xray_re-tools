// DlgGraphViewer.h : header file
//

#if !defined(AFX_DLGGRAPHVIEWER_H__7DAD9D74_FDA2_431A_BFB9_DC84BB29F050__INCLUDED_)
#define AFX_DLGGRAPHVIEWER_H__7DAD9D74_FDA2_431A_BFB9_DC84BB29F050__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphViewer dialog
class CGraph;
class CLevel;
class CDlgInfo;
class CDlgWayObjects;

#include "LtxReader.h"

class CDlgGraphViewer : public CDialog
{
// Construction
public:
	CDlgGraphViewer(CWnd* pParent = NULL);	// standard constructor

    void Draw(CDC* pDC);
    void DrawWay(CDC* pDC, CWayObject* pWay);
    void DrawSelection(CDC* pDC);
    void CreateBitmap();

// Dialog Data
	//{{AFX_DATA(CDlgGraphViewer)
	enum { IDD = IDD_GRAPHVIEWER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON            m_hIcon;
    CDlgInfo*        m_pDlgInfo;
    CDlgWayObjects*  m_pDlgWays;
    CGraph*          m_pGraph;
    CLevel*          m_pLevel;
    unsigned         m_uLevel;

    CLtxReader  m_oLtxWay;
    CWayObject  m_oWay[5];

    int       m_iCX;
    int       m_iCY;
    int       m_iViewCX;
    int       m_iViewCY;
    int       m_iZoom;
    float     m_fMin;
    float     m_fMax;
    float     m_fCurMin;
    float     m_fCurMax;
    int       m_iCoverMode;

    BOOL      m_bDrag;
    POINT     m_sDrag;
    POINT     m_sDragPos;

    BOOL      m_bBitmap;
    CBitmap   m_oBitmap;

	// Generated message map functions
	//{{AFX_MSG(CDlgGraphViewer)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMenuLoad();
	afx_msg void OnMenuClose();
	afx_msg void OnShowSettings();
	afx_msg void OnMenuShowInfo();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWays();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHVIEWER_H__7DAD9D74_FDA2_431A_BFB9_DC84BB29F050__INCLUDED_)
