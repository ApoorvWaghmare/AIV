// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include <vector>
#include <string>

namespace CommonUtil
{
class DebugUtil
{
	std::vector<std::string>m_debugStrings;
	std::vector<std::string>m_dumpStrings;
	static DebugUtil* m_instance;

	DebugUtil();
	~DebugUtil();
public:
	static DebugUtil* GetInstance();
	void DeleteInstance();

	void AddDebugString(const std::string& str);
	void AddDumpString(const std::string& str);

	void AddDebugStrings(const std::vector<std::string>& strs);
	void AddDumpStrings(const std::vector<std::string>& strs);

	const std::vector<std::string>& GetDebugStrings()const;
	const std::vector<std::string>& GetDumpStrings()const;

	void DumpStrings(const std::string& path)const;
	void DebugStrings(const std::string& path)const;

	void ClearDumpStrings();
	void ClearDebugStrings();
};
}