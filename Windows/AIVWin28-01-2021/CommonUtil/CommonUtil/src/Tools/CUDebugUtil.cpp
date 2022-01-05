// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include "CUDebugUtil.h"

#define DEBUG_DATA

namespace CommonUtil
{
DebugUtil* DebugUtil::m_instance = nullptr;
//-----------------------------------------------------------------------------

DebugUtil::DebugUtil()
{}

//-----------------------------------------------------------------------------

DebugUtil::~DebugUtil()
{}

//-----------------------------------------------------------------------------

DebugUtil* DebugUtil::GetInstance()
{
#ifdef DEBUG_DATA
	if (!m_instance)
		m_instance = new DebugUtil();
#endif
	return m_instance;
}

//-----------------------------------------------------------------------------

void DebugUtil::DeleteInstance()
{
	if (m_instance)
		delete m_instance;
}

//-----------------------------------------------------------------------------

void DebugUtil::AddDebugString(const std::string& str)
{
#ifdef DEBUG_DATA
	m_debugStrings.push_back(str);
#endif
}

//-----------------------------------------------------------------------------

void DebugUtil::AddDumpString(const std::string& str)
{
#ifdef DEBUG_DATA
	m_dumpStrings.push_back(str);
#endif
}

//-----------------------------------------------------------------------------

void DebugUtil::AddDebugStrings(const std::vector<std::string>& strs)
{
#ifdef DEBUG_DATA
	m_debugStrings.insert(m_debugStrings.end(), strs.begin(), strs.end());
#endif
}

//-----------------------------------------------------------------------------

void DebugUtil::AddDumpStrings(const std::vector<std::string>& strs)
{
#ifdef DEBUG_DATA
	m_dumpStrings.insert(m_dumpStrings.end(), strs.begin(), strs.end());
#endif
}

//-----------------------------------------------------------------------------

const std::vector<std::string>& DebugUtil::GetDebugStrings()const
{
	return m_debugStrings;
}

//-----------------------------------------------------------------------------

const std::vector<std::string>& DebugUtil::GetDumpStrings()const
{
	return m_dumpStrings;
}

//-----------------------------------------------------------------------------

void DebugUtil::DumpStrings(const std::string& path)const
{
#ifdef DEBUG_DATA
	FILE* fp = fopen(path.c_str(), "w");
	std::string str;
	for (auto str : m_dumpStrings)
	{
		fputs(str.c_str(), fp);
	}
	fclose(fp);
#endif
}

//-----------------------------------------------------------------------------

void DebugUtil::DebugStrings(const std::string& path)const
{
#ifdef DEBUG_DATA
	FILE* fp = fopen(path.c_str(), "w");
	std::string str;
	for (auto str : m_debugStrings)
	{
		fputs(str.c_str(), fp);
	}
	fclose(fp);
#endif
}

//-----------------------------------------------------------------------------

void DebugUtil::ClearDumpStrings()
{
	m_dumpStrings.clear();
}

//-----------------------------------------------------------------------------

void DebugUtil::ClearDebugStrings()
{
	m_debugStrings.clear();
}

//-----------------------------------------------------------------------------
}