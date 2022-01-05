// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// Include files
#include "CUMemoryTracker.h"
#include <process.h>
#include "CUSystemInfo.h"

namespace CommonUtil
{

unsigned int __stdcall ThreadMemoryFunc(void* memoryTrackerObj)
{
	if(!memoryTrackerObj)
		return 0;	

	MemoryTracker* memoryTracker = (MemoryTracker*)memoryTrackerObj;

	memoryTracker->Execute();

	return 0;
}

MemoryTracker::MemoryTracker()
{
	m_waitTime = 5;	// Seconds

	m_startPhysicalMemory = m_startTotalMemory = 0;
	m_peakPhysicalMemory = m_peakTotalMemory = 0;
	m_endPhysicalMemory = m_endTotalMemory = 0;
}

//-----------------------------------------------------------------------------

MemoryTracker::~MemoryTracker()
{
	m_startPhysicalMemory = 0;
	m_startTotalMemory = 0;
	m_peakPhysicalMemory = 0;
	m_peakTotalMemory = 0;
	m_endPhysicalMemory = 0;
	m_endTotalMemory = 0;
}

//-----------------------------------------------------------------------------

void MemoryTracker::Start()
{
	m_analyzerStartEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
	m_analyzerFinishEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

	unsigned threadID;
	m_analyzerThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadMemoryFunc, this, 0, &threadID);    // creating analyzer thread

	SetEvent(m_analyzerStartEvent);
	ResetEvent(m_analyzerFinishEvent);

	getCurrentConsumedMemory(m_startPhysicalMemory, m_startTotalMemory);

	m_peakPhysicalMemory = m_startPhysicalMemory;
	m_peakTotalMemory = m_startTotalMemory;

	m_endPhysicalMemory = m_endTotalMemory = 0;
}

//-----------------------------------------------------------------------------

void MemoryTracker::Execute()
{
	DWORD dwWaitStartResult, dwWaitFinishResult;
	while(1)
	{ 
		dwWaitStartResult = WaitForSingleObject(m_analyzerStartEvent, 0);
		dwWaitFinishResult = WaitForSingleObject(m_analyzerFinishEvent, 0);

		if(dwWaitStartResult == WAIT_OBJECT_0 && dwWaitFinishResult != WAIT_OBJECT_0)
		{
			trackMemoryData();
			Sleep(m_waitTime*1000);
		}
		else if(dwWaitFinishResult == WAIT_OBJECT_0 && dwWaitStartResult != WAIT_OBJECT_0)
		{
			SetEvent(m_analyzerStartEvent);
			return;
		}	
  	}// while(1) ends here
}

//-----------------------------------------------------------------------------

void MemoryTracker::getCurrentConsumedMemory(unsigned __int64& consumedPhysicalMemory, unsigned __int64& consumedTotalMemory)
{
	consumedPhysicalMemory = consumedTotalMemory = 0;

	unsigned __int64 consumedPageFileMemory, consumedVirtualMemory ;
	SystemInfo::GetSystemMemoryInfo(consumedPhysicalMemory, consumedPageFileMemory, consumedVirtualMemory);

	consumedTotalMemory = consumedPhysicalMemory + consumedPageFileMemory + consumedVirtualMemory;
}

//-----------------------------------------------------------------------------

void MemoryTracker::trackMemoryData()
{	
	unsigned __int64 consumedPhysicalMemory, consumedTotalMemory;
	getCurrentConsumedMemory(consumedPhysicalMemory, consumedTotalMemory);

	if(consumedPhysicalMemory > m_peakPhysicalMemory)
		m_peakPhysicalMemory = consumedPhysicalMemory;

	if(consumedTotalMemory > m_peakTotalMemory)
		m_peakTotalMemory = consumedTotalMemory;
}

//-----------------------------------------------------------------------------

void MemoryTracker::Terminate()
{
	getCurrentConsumedMemory(m_endPhysicalMemory, m_endTotalMemory);

	if(m_endPhysicalMemory > m_peakPhysicalMemory)
		m_peakPhysicalMemory = m_endPhysicalMemory;

	if(m_endTotalMemory > m_peakTotalMemory)
		m_peakTotalMemory = m_endTotalMemory;

	SetEvent(m_analyzerFinishEvent);
	ResetEvent(m_analyzerStartEvent);
	WaitForMultipleObjects(1, &m_analyzerStartEvent, TRUE, INFINITE);
	ResetEvent(m_analyzerStartEvent);

	CloseHandle(m_analyzerStartEvent);
	CloseHandle(m_analyzerFinishEvent);
}

//-----------------------------------------------------------------------------

void MemoryTracker::DumpOperationInfo(const char* operationName)
{
	FILE* fp = fopen("D:\\MemoryDump.txt","a");
	double div = 1024 *1024;
	fprintf(fp,"\n**** Operation Info ****\n");
	fprintf(fp, operationName );
	fprintf(fp, "\n");
	fprintf(fp, "Physical Memory\n");
	fprintf(fp, "Start : %0.2f MB\tPeak  : %0.2f MB\tEnd   : %0.2f MB\n", m_startPhysicalMemory/div,  m_peakPhysicalMemory/div,  m_endPhysicalMemory/div);
	fprintf(fp, "Total Memory\n");
	fprintf(fp, "Start : %0.2f MB\tPeak  : %0.2f MB\tEnd   : %0.2f MB\n", m_startTotalMemory/div,	m_peakTotalMemory/div,	m_endTotalMemory/div);
	fprintf(fp, "Consumed Memory\n");
	fprintf(fp, "Physical Memory: %0.2f MB\t Total Memory: %0.2f MB\n", (m_peakPhysicalMemory-m_startPhysicalMemory)/div, (m_peakTotalMemory-m_startTotalMemory)/div);
	fprintf(fp, "\n***************************\n");
}

//-----------------------------------------------------------------------------
}