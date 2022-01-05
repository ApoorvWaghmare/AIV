#include <Windows.h>
#include <vector>
#include "CUSystemInfo.h"


namespace CommonUtil
{
SystemInfo::SystemInfo(void)
{
}

//---------------------------------------------------------------------------------------------------

SystemInfo::~SystemInfo(void)
{
}

//---------------------------------------------------------------------------------------------------

void SystemInfo::GetSystemTime(unsigned short& hour, unsigned short& minute, unsigned short& second)
{
	hour = minute = second = 0;

	 _SYSTEMTIME time;
    GetLocalTime(&time);

	hour = time.wHour;
	minute = time.wMinute;
	second = time.wSecond;
}

//---------------------------------------------------------------------------------------------------

void SystemInfo::GetSystemMemoryInfo(unsigned __int64& consumedPhysicalMemory,
									 unsigned __int64& consumedPageFileMemory,
									 unsigned __int64& consumedVirtualMemory)
{
	consumedPhysicalMemory = consumedPageFileMemory = consumedVirtualMemory = 0;

	MEMORYSTATUSEX memStatus;
	memStatus.dwLength = sizeof(MEMORYSTATUSEX);
	
	GlobalMemoryStatusEx(&memStatus);

	consumedPhysicalMemory = memStatus.ullTotalPhys-memStatus.ullAvailPhys;
	consumedPageFileMemory = memStatus.ullTotalPageFile-memStatus.ullAvailPageFile;
	consumedVirtualMemory = memStatus.ullTotalVirtual-memStatus.ullAvailVirtual;
}

//---------------------------------------------------------------------------------------------------

}
