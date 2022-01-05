// ============================================================================
//			Copyright (c) 2013 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMON_DATAHANDLER_INTERFACE_H_
#define _COMMON_DATAHANDLER_INTERFACE_H_

namespace CommonUtil
{
// Forword declaration
class BlockInterface;

//  ***** DESCRIPTION of class DataHandlerInterface
//	This class is used in "Parallel Processing Utility".
//	This is an interface class that interacts with both utility and application. 
//	Using DataHandlerInterface, 
//		1. Utility tells application to process given block.
//		2. Utility gets information from application about how many blocks are assigned 
//		   (means selected for processing)
//		3. Utility gets a new block from application for processing
//
//  ***** HOW TO USE THIS CLASS AT APPLICATION SIDE ?
//	Inherit a class from this class. Override all virtual functions in child class 
//  as per need of the application

class DataHandlerInterface
{
public:

	// Constructor
	DataHandlerInterface(){}

	// Destructor
	virtual ~DataHandlerInterface(){}

	// Returns unchecked block for processing
	// The argument threadIndex will tell us which thread is asking for the block
	virtual BlockInterface* GetBlockForProcessing(size_t threadIndex)const = 0;

	// Process block through particular thread
	virtual void ProcessBlock(size_t threadIndex, BlockInterface* block) = 0;

	// Tells if it is required to set progress of given thread
	// Returns true if thread index is 0, else false
	virtual bool IsProgressSettingRequiredForThread(size_t threadIndex)const = 0;

	// Sets progress of application
	virtual void SetProgress(bool progressFlag = false) = 0;
};
}

#endif