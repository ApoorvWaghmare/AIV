
// AIVWinDoc.h : interface of the CAIVWinDoc class
//


#pragma once

#include <vector>
#include "TCPClient.h"

using namespace std;

class CAIVWinDoc : public CDocument
{
protected: // create from serialization only
	CAIVWinDoc();
	DECLARE_DYNCREATE(CAIVWinDoc)

// Attributes
public:

// Operations
private:
	TCPClient m_client;
	bool m_connectionStatus = false;
	std::vector<double> m_xReadings;
	void recieveSinglePoint();

public:
	void connectToPi();
	bool returnConnectionStatus();
	void endConnection();
	void recieveFullScannedData();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CAIVWinDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
