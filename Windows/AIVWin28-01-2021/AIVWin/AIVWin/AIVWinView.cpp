
// AIVWinView.cpp : implementation of the CAIVWinView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AIVWin.h"
#endif

#include "AIVWinDoc.h"
#include "AIVWinView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <fstream>
#include <vector>

using namespace std;

// CAIVWinView

IMPLEMENT_DYNCREATE(CAIVWinView, CView)

BEGIN_MESSAGE_MAP(CAIVWinView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAIVWinView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYUP()
//	ON_WM_TIMER()
END_MESSAGE_MAP()

// CAIVWinView construction/destruction

CAIVWinView::CAIVWinView()
{
	// TODO: add construction code here

}

CAIVWinView::~CAIVWinView()
{
}

BOOL CAIVWinView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAIVWinView drawing

void CAIVWinView::OnDraw(CDC* /*pDC*/)
{
	CAIVWinDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CAIVWinView printing


void CAIVWinView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CAIVWinView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAIVWinView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAIVWinView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CAIVWinView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAIVWinView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CAIVWinView diagnostics

#ifdef _DEBUG
void CAIVWinView::AssertValid() const
{
	CView::AssertValid();
}

void CAIVWinView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAIVWinDoc* CAIVWinView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAIVWinDoc)));
	return (CAIVWinDoc*)m_pDocument;
}
#endif //_DEBUG


// CAIVWinView message handlers


void CAIVWinView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_RETURN && GetDocument()->returnConnectionStatus() == false)
	{
		GetDocument()->connectToPi();
		GetDocument()->recieveFullScannedData();
	}
	else if (nChar == VK_RETURN && GetDocument()->returnConnectionStatus() == true)
	{
		GetDocument()->endConnection();
	}
	else if (nChar = VK_SHIFT)
	{
		//GetDocument()->recieveFullScannedData();
	}
	else
		return;

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
