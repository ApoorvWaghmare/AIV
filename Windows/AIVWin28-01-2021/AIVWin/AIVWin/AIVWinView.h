
// AIVWinView.h : interface of the CAIVWinView class
//

#pragma once


class CAIVWinView : public CView
{
protected: // create from serialization only
	CAIVWinView();
	DECLARE_DYNCREATE(CAIVWinView)

// Attributes
public:
	CAIVWinDoc* GetDocument() const;

// Operations
private:
	
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CAIVWinView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in AIVWinView.cpp
inline CAIVWinDoc* CAIVWinView::GetDocument() const
   { return reinterpret_cast<CAIVWinDoc*>(m_pDocument); }
#endif

