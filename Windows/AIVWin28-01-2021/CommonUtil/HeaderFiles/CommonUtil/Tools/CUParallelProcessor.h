// ============================================================================
//			Copyright (c) 2013 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _CU_PARALLEL_PROCESSOR_H_
#define _CU_PARALLEL_PROCESSOR_H_

#include <Windows.h>
#include <vector>

namespace CommonUtil
{

//Forword declaration
class DataHandlerInterface;
class BlockInterface;
class ParallelProcessor;

struct ThreadData
{
	size_t m_index;
	ParallelProcessor* m_processor;
	BlockInterface* m_block;
	bool m_processDataFlag;	//If false, threads return immediately(used for closing threads)
	FILE* m_fpLog;	// Log file

	ThreadData()
	{
		m_index = -1;
		m_processor = 0;
		m_block = 0;
		m_processDataFlag = false;
		m_fpLog = 0;
	}
};

//  ***** DESCRIPTION of class ParallelProcessor
//	This class is used in "Parallel Processing Utility".
//	This is most important class of this utility as it handles Parallel Processing
//	This class is responsible for 
//		1. Creating and deleting thread events
//		2. Starting and finishing of threads
//		3. Processing of blocks (BlockInterface) using DataHandlerInterface
//
//  ***** HOW TO USE THIS CLASS AT APPLICATION SIDE ???
//	To process any data parallaly, the application just needs to call ParallelProcessor::Execute()
//  provided classes DataHandlerInterface and BlockInterface are inherited in the application

class ParallelProcessor
{
	CommonUtil::DataHandlerInterface* m_dataHandler;

	size_t m_numThreads;

	size_t m_numBlocks;

	HANDLE* m_threads;
	HANDLE* m_startEvent;
	HANDLE* m_finishEvent;

	std::vector<ThreadData*> m_threadData;

	bool m_dumpThreadLog;

	//This method is aaded in order to get number of threads from system
	size_t getNumThreadsFromSystem()const;

	void startThreadEvent(size_t threadIndex);

	void finishThreadEvent(size_t threadIndex);

	void createThreadEvents();

	void deleteThreadEvents();

	void dumpThreadLog(FILE* fp, const char* infoStr);

	void createThreadData();

	void deleteThreadData();

public:

	// Constructor
	// if numCPUs = 0 or less, numCPUs decided inside by getting info from system
	ParallelProcessor(DataHandlerInterface* dataHandler, size_t numBlocks, size_t numCPUs);

	// Destructor
	~ParallelProcessor();

	//If returns false if begin thread fails, in that case user should
	// use serial processing
	bool Execute();

	void ProcessThread(ThreadData* threadData);

	size_t GetNumThreads()const
	{
		return m_numThreads;
	}
};
}
#endif