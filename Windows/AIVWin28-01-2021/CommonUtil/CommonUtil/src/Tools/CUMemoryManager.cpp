// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// Include Files
#include "CUMemoryManager.h"

namespace CommonUtil
{

MemManager :: MemManager(int unitSize, int numUnits) : m_unitSize(unitSize), m_numUnits(numUnits)
{
	m_currentIndex = 0;

	void* memBlock = (void *)malloc(m_numUnits * m_unitSize);

	m_initialPointers.push_back(memBlock);
	m_freePointers.clear();
}

//---------------------------------------------------------------------

void MemManager :: Cleanup()
{
	for(int i = 0; i < m_initialPointers.size(); i++)
	{
		void* p =  m_initialPointers[i];
		if(p)
			free(p);
	}
	m_initialPointers.clear();
	m_freePointers.clear();
	m_currentIndex = -1;
}

//---------------------------------------------------------------------

MemManager :: ~MemManager()
{
	Cleanup();
}


//---------------------------------------------------------------------

void MemManager::ClearFreePointers()
{
	m_freePointers.clear();
}

//---------------------------------------------------------------------

bool MemManager :: AllocateMemory(void* &allocatedPointer)
{
	bool stat = false;

	if(m_freePointers.size())
	{
		allocatedPointer = m_freePointers[m_freePointers.size() - 1];
		m_freePointers.pop_back();
		stat = true;
	}
	else
	{
		if(m_currentIndex >= m_numUnits)
		{
			void* memBlock = (void *)malloc(m_numUnits * m_unitSize);
			m_initialPointers.push_back(memBlock);
			m_currentIndex = 0;
		}
		allocatedPointer = (char*)m_initialPointers[m_initialPointers.size() - 1]
					+ (m_currentIndex * m_unitSize);
		m_currentIndex++;
		stat = true;
	}

	return stat;
}

//---------------------------------------------------------------------

void MemManager :: FreeMemory(void* &allocatedPointer)
{
	m_freePointers.push_back(allocatedPointer);
}

//---------------------------------------------------------------------

}