// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//This method is used to find memory leaks

//To find memory Leaks and peak memory, add MemoryLeak.h
//Copy below mentioned code at start of functional code
//-----------------------------------------------------------------------------------
//SetInsertMemoryInfo(true);
//m_allocatedMemory = 0;
//m_peakMemory = 0;
//-----------------------------------------------------------------------------------
//At end of functional code call method dumpMemoryLeak(const char *fileName)
//or Copy below mentioned code at end of functional code
//SetInsertMemoryInfo(false);
//double div = 1024*1024;
//printf("Memory Leaks\tPeak memory \n%0.2fMB\t\t%0.2fMB\n", m_allocatedMemory/div, m_peakMemory/div);

#ifndef _MEMORY_LEAK_H_
#define _MEMORY_LEAK_H_

#ifdef MEMORY_TRACKING_ON

#ifdef REVLIBDLL_EXPORTS
#define DLLLink __declspec( dllexport )
#else
#define DLLLink __declspec( dllimport ) 
#endif

#include <Windows.h>

DLLLink extern CRITICAL_SECTION CriticalSection;

void* operator new (size_t size);

void operator delete(void* p);

void* operator new[] (size_t size);

void operator delete[](void* p);

DLLLink void DumpMemoryLeak(const char *fileName);

DLLLink void SetInsertMemoryInfo(bool flag);

DLLLink unsigned __int64 GetAllocatedMemory();

DLLLink unsigned __int64 GetPeakMemory();

DLLLink void ResetMemory();

#endif

//-----------------------------------------------------------------------------------
#endif