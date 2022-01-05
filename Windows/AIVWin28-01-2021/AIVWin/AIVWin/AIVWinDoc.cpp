
// AIVWinDoc.cpp : implementation of the CAIVWinDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AIVWin.h"
#endif

#include "AIVWinDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAIVWinDoc

IMPLEMENT_DYNCREATE(CAIVWinDoc, CDocument)

BEGIN_MESSAGE_MAP(CAIVWinDoc, CDocument)
END_MESSAGE_MAP()


// CAIVWinDoc construction/destruction

CAIVWinDoc::CAIVWinDoc()
{
	// TODO: add one-time construction code here

}

CAIVWinDoc::~CAIVWinDoc()
{
}

BOOL CAIVWinDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CAIVWinDoc serialization

void CAIVWinDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAIVWinDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAIVWinDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAIVWinDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAIVWinDoc diagnostics

#ifdef _DEBUG
void CAIVWinDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAIVWinDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAIVWinDoc commands

// CAIVWinDoc operations
void CAIVWinDoc::connectToPi()
{
	TCPClient::TCPConnectionStatus update = m_client.connectToPi();
	if (update == TCPClient::WSA_DATA_STARTUP_FAIL)
		AfxMessageBox(_T("WSA data stratUp fail"));
	else if (update == TCPClient::GET_ADD_INFO_FAIL)
		AfxMessageBox(_T("Get address info fail"));
	else if (update == TCPClient::SOCKET_FAIL)
		AfxMessageBox(_T("Socket fail"));
	else if (update == TCPClient::SERVER_CONNECTION_FAIL)
		AfxMessageBox(_T("Server connection fail"));
	else if (update == TCPClient::INITIAL_DATA_SEND_FAIL)
		AfxMessageBox(_T("Initial data sent fail"));
	else if (update == TCPClient::CONNECTION_SUCCESSFUL)
	{
		AfxMessageBox(_T("Connection with Chris successful"));
		m_connectionStatus = true;
	}
}

void CAIVWinDoc::endConnection()
{
	if (m_connectionStatus == true)
	{
		m_client.endConnection();
		AfxMessageBox(_T("Connection with Pi has ended"));
	}
}

bool CAIVWinDoc::returnConnectionStatus()
{
	return m_connectionStatus;
}

void CAIVWinDoc::recieveSinglePoint()
{
	double x = m_client.recieveDouble();
	//double y = m_client.recieveDouble();

	m_xReadings.push_back(x);
}

void CAIVWinDoc::recieveFullScannedData()
{
	int totalPoints = m_client.recieveInt();

	for (int i = 0; i < totalPoints; i++)
	{
		recieveSinglePoint();
	}

	CString strMsg;
	strMsg.Format(_T("Total data points recieved = %d"), m_xReadings.size());
	AfxMessageBox(strMsg);
}
