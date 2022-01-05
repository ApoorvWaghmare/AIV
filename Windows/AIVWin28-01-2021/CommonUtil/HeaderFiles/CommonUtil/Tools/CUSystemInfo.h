// ============================================================================
//			Copyright (c) 2010 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _SYSTEM_INFO_H_
#define _SYSTEM_INFO_H_

namespace CommonUtil
{
class SystemInfo
{
public:
	SystemInfo(void);
	~SystemInfo(void);

  static void GetSystemTime(unsigned short& hour, unsigned short& minute, unsigned short& second);

  static void GetSystemMemoryInfo(unsigned __int64& consumedPhysicalMemory,
								  unsigned __int64& consumedPageFileMemory,
								  unsigned __int64& consumedVirtualMemory);

};
}

#endif
