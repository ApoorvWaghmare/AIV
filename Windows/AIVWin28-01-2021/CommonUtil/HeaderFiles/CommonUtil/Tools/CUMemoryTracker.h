// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _MEMORY_TRACKER_H_
#define _MEMORY_TRACKER_H_

#include <stdio.h>
#include <windows.h>
#include <vector>

namespace CommonUtil
{

class MemoryTracker
{
	int m_waitTime;

	HANDLE m_analyzerThread;
	HANDLE m_analyzerStartEvent;
	HANDLE m_analyzerFinishEvent;
	
	unsigned __int64 m_startPhysicalMemory;
	unsigned __int64 m_startTotalMemory;
	unsigned __int64 m_peakPhysicalMemory;
	unsigned __int64 m_peakTotalMemory;
	unsigned __int64 m_endPhysicalMemory;
	unsigned __int64 m_endTotalMemory;

	void getCurrentConsumedMemory(unsigned __int64& consumedPhysicalMemory, unsigned __int64& consumedTotalMemory);

	void trackMemoryData();

public:
	// Constructor
	MemoryTracker();

	// Destructor
	~MemoryTracker();	

	void SetWaitTime(int waitTime){ m_waitTime = waitTime; }

	void Start();

	void Execute();

	void GetConsumedMemoryStatus(unsigned __int64& startPhysicalMemory, unsigned __int64& startTotalMemory,
								 unsigned __int64& peakPhysicalMemory, unsigned __int64& peakTotalMemory,
								 unsigned __int64& endPhysicalMemory, unsigned __int64& endTotalMemory)const
	{
		startPhysicalMemory = m_startPhysicalMemory;
		startTotalMemory = m_startTotalMemory;

		peakPhysicalMemory = m_peakPhysicalMemory;
		peakTotalMemory = m_peakTotalMemory;

		endPhysicalMemory = m_endPhysicalMemory;
		endTotalMemory = m_endTotalMemory;
	}

	void Terminate();	
	void DumpOperationInfo(const char* operationName);
};
}
#endif