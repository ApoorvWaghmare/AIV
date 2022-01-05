// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include <map>
#include <fstream>
#include "CUMemoryLeak.h"

using namespace std;

#ifdef MEMORY_TRACKING_ON

//counter for Finding memory leak
static unsigned __int64 m_allocatedMemory = 0;

//counter for Finding peak memory
static unsigned __int64 m_peakMemory = 0;

//This bool must be "true" at start of functional code of
// which memory leaks has to find
static bool m_insertMemoryInfo = false;

CRITICAL_SECTION CriticalSection;
//-----------------------------------------------------------------------------------

void DumpMemoryLeak(const char *fileName)
{
	m_insertMemoryInfo = false;	
	double div = 1024*1024;
	printf("Memory Leaks\tPeak memory \n%0.2fMB\t\t%0.2fMB\n", m_allocatedMemory/div, m_peakMemory/div);

	FILE *fp = 0;
	fopen_s(&fp, fileName, "w");
	if(!fp)
		return;	

	fwprintf(fp, L"Memory Leaks\tPeak memory \n%0.2fMB\t\t%0.2fMB\n", m_allocatedMemory/div, m_peakMemory/div);	
	fclose(fp);
}

//-----------------------------------------------------------------------------------

void* operator new (size_t size)
{
	size_t newSize = size + 32;

	void * p = malloc(newSize);
	if (p == 0)  throw "allocation failure";

	int* intPtr = (int*)p;
	*intPtr = (int)size;

	if(m_insertMemoryInfo && p)
	{
		// Request ownership of the critical section.
		EnterCriticalSection(&CriticalSection); 

		m_allocatedMemory += size;
		if(m_allocatedMemory > m_peakMemory)
			m_peakMemory = m_allocatedMemory;		

		// Release ownership of the critical section.
		LeaveCriticalSection(&CriticalSection);
	}

	intPtr = intPtr + 1;
	p = (void*)intPtr;
	return p;
}

//-----------------------------------------------------------------------------------

void operator delete(void* p)
{	
	int* intPtr = (int*)p;
	intPtr = intPtr-1;

	if(m_insertMemoryInfo && p)
	{		
		// Request ownership of the critical section.
		EnterCriticalSection(&CriticalSection);
		
		int size = *intPtr;
		m_allocatedMemory -= size;
	
		// Release ownership of the critical section.
		LeaveCriticalSection(&CriticalSection);		
	}	
	p = (void*)intPtr;
	free(p);
	p = 0;
}

//-----------------------------------------------------------------------------------

void* operator new[] (size_t size)
{
	size_t newSize = size + 32;

	void * p = malloc(newSize);
	if (p == 0)  throw "allocation failure";

	int* intPtr = (int*)p;
	*intPtr = (int)size;

	if(m_insertMemoryInfo && p)
	{
		// Request ownership of the critical section.
		EnterCriticalSection(&CriticalSection); 

		m_allocatedMemory += size;
		if(m_allocatedMemory > m_peakMemory)
			m_peakMemory = m_allocatedMemory;		

		// Release ownership of the critical section.
		LeaveCriticalSection(&CriticalSection);
	}

	intPtr = intPtr + 1;
	p = (void*)intPtr;
	return p;
}

//-----------------------------------------------------------------------------------

void operator delete[](void* p)
{	
	int* intPtr = (int*)p;
	intPtr = intPtr-1;

	if(m_insertMemoryInfo && p)
	{		
		// Request ownership of the critical section.
		EnterCriticalSection(&CriticalSection);
		
		int size = *intPtr;
		m_allocatedMemory -= size;
	
		// Release ownership of the critical section.
		LeaveCriticalSection(&CriticalSection);		
	}	
	p = (void*)intPtr;
	free(p);
	p = 0;
}

//-----------------------------------------------------------------------------------

void SetInsertMemoryInfo(bool flag)
{
	m_insertMemoryInfo = flag;
}

//-----------------------------------------------------------------------------------

unsigned __int64 GetAllocatedMemory()
{
	return m_allocatedMemory;
}

//-----------------------------------------------------------------------------------

unsigned __int64 GetPeakMemory()
{
	return m_peakMemory;
}

//-----------------------------------------------------------------------------------

void ResetMemory()
{
	m_allocatedMemory = 0;
	m_peakMemory = 0;
}

//-----------------------------------------------------------------------------------

#endif