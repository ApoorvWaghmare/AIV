// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _MEMORYMNGR_H_
#define _MEMORYMNGR_H_

// Std Include Files
#pragma warning (disable: 4786)
#include <vector>

namespace CommonUtil
{

class MemManager
{
	unsigned long m_currentIndex;
	const unsigned int m_unitSize;
	const unsigned long m_numUnits;
	std::vector<void *> m_initialPointers;
	std::vector<void *> m_freePointers;

public:
	
	// Constructor
	MemManager(int unitSize, int numUnits);

	// Destructor
	~MemManager();

	void Cleanup();

	bool AllocateMemory(void* &allocatedPointer);

	void FreeMemory(void* &allocatedPointer);

	void ClearFreePointers();
};

}
#endif