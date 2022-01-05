// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _PROGRESS_INTERFACE_H_
#define _PROGRESS_INTERFACE_H_

#include <string>

namespace CommonUtil
{
class ProgressInterface
{
public:

	//Constructor
	ProgressInterface(){};

	//Destructor
	virtual ~ProgressInterface(){}

	virtual void Enable(std::string& message) = 0;

	virtual void SetText(std::string& message) = 0;

	virtual void Disable() = 0;

	//Sets percentage of completion of a process
	virtual bool SetPercentageCompletion(double percentComplete) = 0;

	//Breaks process
	virtual bool BreakProcess() = 0;

	virtual void SetMultithreadFlag(bool flag) = 0;

	//increase current percentage by given percent amount
	virtual bool IncreamentPercentage(double deltaPercent) = 0;

	virtual int GetPercentCompletion() = 0;

	virtual void HandleWindowsMessage() = 0;

	virtual void DisplayMsg(const std::string& msg) = 0;
};
}
#endif
