// ============================================================================
//			Copyright (c) 2013 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// Include files
#include "CUParallelProcessor.h"
#include <time.h>
#include <process.h>
#include "CUDataHandlerInterface.h"
#include "CUSecureUtil.h"

//#define PARALLELPROCESSOR_DEBUG
namespace CommonUtil
{

unsigned int __stdcall ThreadFunc(void* threadData)
{
	if(!threadData)
	{
#ifdef PARALLELPROCESSOR_DEBUG
		printf("No thread data\n");
#endif
		return 0;
	}

	ThreadData* thrData = (ThreadData*)threadData;
#ifdef PARALLELPROCESSOR_DEBUG
	printf("ThreadFunc: threadIndex %d\n", (int)thrData->m_index);
#endif
	ParallelProcessor* processor = (ParallelProcessor*)thrData->m_processor;
	processor->ProcessThread(thrData);

	return 0;
}

ParallelProcessor::ParallelProcessor(DataHandlerInterface* dataHandler,
									 size_t numBlocks, size_t numCPUs)
{
	m_dataHandler = dataHandler;
	m_numBlocks = numBlocks;
	
	// get number of threads from system
	// assign it to m_numThreads
	size_t numThreadsAvailableWithSystem = getNumThreadsFromSystem();
	m_numThreads = numThreadsAvailableWithSystem;

	if(numCPUs > 0 && m_numThreads > numCPUs)
	{
		m_numThreads = numCPUs;
	}

	if(m_numBlocks < m_numThreads)
		m_numThreads = m_numBlocks;

	createThreadEvents();

	m_dumpThreadLog = false;
}

//-----------------------------------------------------------------------------

ParallelProcessor::~ParallelProcessor()
{
	deleteThreadEvents();
}

//-----------------------------------------------------------------------------

size_t ParallelProcessor::getNumThreadsFromSystem()const
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	size_t numThreadsAvailableWithSystem = (size_t)sysInfo.dwNumberOfProcessors;
	return numThreadsAvailableWithSystem;
}

//-----------------------------------------------------------------------------

void ParallelProcessor::startThreadEvent(size_t threadIndex)
{
    if(!ResetEvent(m_finishEvent[threadIndex]))
    {
#ifdef PARALLELPROCESSOR_DEBUG
	    printf("\nReset failed for finish event tid : %d index : %d Wait Error : %d\n", 
				GetCurrentThreadId(), (int)threadIndex, GetLastError());
#endif
    }

    if(!SetEvent(m_startEvent[threadIndex]))
    {
#ifdef PARALLELPROCESSOR_DEBUG
	    printf("\nSet failed for start event tid : %d index : %d Wait Error : %d\n", 
				GetCurrentThreadId(), (int)threadIndex, GetLastError());
#endif
    }
	/*else	// Activate only for development
	{
		unsigned short hr, min, sec;
		RevLibUtil::GetSystemTime(hr, min, sec);
		printf("\n%d::%d::%d : Thread %d started\n", hr, min, sec, threadIndex);
	}*/
}

//-----------------------------------------------------------------------------

void ParallelProcessor::finishThreadEvent(size_t threadIndex)
{
    if(!ResetEvent(m_startEvent[threadIndex]))
	{
#ifdef PARALLELPROCESSOR_DEBUG
        printf("\nReset failed for start event tid : %d index : %d Wait Error : %d\n", 
				GetCurrentThreadId(), (int)threadIndex, GetLastError());
#endif
	}

	if(!SetEvent(m_finishEvent[threadIndex]))
	{
#ifdef PARALLELPROCESSOR_DEBUG
        printf("\nSet failed for finish event tid : %d index : %d Wait Error : %d\n", 
				GetCurrentThreadId(), (int)threadIndex, GetLastError());
#endif
	}
	/*else	// Activate only for development
	{
		unsigned short hr, min, sec;
		RevLibUtil::GetSystemTime(hr, min, sec);
		printf("\n%d::%d::%d : Thread %d finished\n", hr, min, sec, threadIndex);
	}*/
}

//-----------------------------------------------------------------------------

void ParallelProcessor::createThreadEvents()
{
	m_threads = new HANDLE[m_numThreads];
	m_startEvent = new HANDLE[m_numThreads];
	m_finishEvent = new HANDLE[m_numThreads];
	
	for(size_t i = 0; i < m_numThreads; i++)
	{
		m_startEvent[i] = CreateEventW(NULL, TRUE, FALSE, NULL);

		if(m_startEvent[i] == NULL)
		{
#ifdef PARALLELPROCESSOR_DEBUG
			printf("\n Create Event Failed (%d) \n",GetLastError());
#endif
		}

		m_finishEvent[i] = CreateEventW(NULL, TRUE, FALSE, NULL);

		if(m_finishEvent[i] == NULL)
		{
#ifdef PARALLELPROCESSOR_DEBUG
			printf("\n Create Event Failed (%d) \n",GetLastError());
#endif
		}
	}
}

//-----------------------------------------------------------------------------

void ParallelProcessor::deleteThreadEvents()
{
	if(m_threads)
	{
		delete [] m_threads;
		m_threads = 0;
	}
	if(m_startEvent)
	{
		for(size_t i = 0; i < m_numThreads; i++)
			CloseHandle(m_startEvent[i]);

		delete [] m_startEvent;
		m_startEvent = 0;
	}
	if(m_finishEvent)
	{
		for(size_t i = 0; i < m_numThreads; i++)
			CloseHandle(m_finishEvent[i]);

		delete [] m_finishEvent;
		m_finishEvent = 0;
	}
}

//-----------------------------------------------------------------------------
void ParallelProcessor::dumpThreadLog(FILE* fp, const char* infoStr)
{
	if(!fp)
		return;

	unsigned short hr, min, sec;
	 _SYSTEMTIME time;
    GetLocalTime(&time);

	hr = time.wHour;
	min = time.wMinute;
	sec = time.wSecond;

	DWORD curThreadId = GetCurrentThreadId();

    fprintf_s(fp, "%d\t%d::%d::%d : %s\n", (int) curThreadId, hr, min, sec, infoStr);
    fflush(fp);
}
//-----------------------------------------------------------------------------

void ParallelProcessor::createThreadData()
{
	m_threadData.resize(m_numThreads);
	for(size_t iThrd = 0; iThrd < m_numThreads; ++iThrd)
	{
		ThreadData* data = new ThreadData;

		data->m_index = iThrd;
		data->m_processor = this;
		data->m_block = 0;
		data->m_processDataFlag = true;
        data->m_fpLog = 0;

#ifdef PARALLELPROCESSOR_DEBUG
		if(m_dumpThreadLog)
		{
			char logFile[1024];
			sprintf_s(logFile, "C:\\Temp\\Thread%d.log", (int)iThrd);
			s_fopen(data->m_fpLog, logFile, "w");
		}
#endif

		m_threadData[iThrd] = data;
	}
}

//-----------------------------------------------------------------------------

void ParallelProcessor::deleteThreadData()
{
	for(size_t iThrd = 0; iThrd < m_numThreads; ++iThrd)
	{
		if(!m_threadData[iThrd])
			continue;

        if(m_threadData[iThrd]->m_fpLog)
          fclose(m_threadData[iThrd]->m_fpLog);

		delete m_threadData[iThrd];
	}

	m_threadData.clear();
}

//-----------------------------------------------------------------------------

bool ParallelProcessor::Execute()
{
	createThreadData();

    // Begin thread
	for(size_t iThrd = 0; iThrd < m_numThreads; ++iThrd)
	{
		unsigned threadID;
		m_threads[iThrd] = (HANDLE)_beginthreadex(NULL, 0, &ThreadFunc, m_threadData[iThrd], 0, &threadID);    // creating thread

		if(!m_threads[iThrd])
		{
#ifdef PARALLELPROCESSOR_DEBUG
			printf("Could not create Thread %d Exiting..\n",(int)iThrd);
#endif
			
			deleteThreadData();

			for(size_t i = 0; i < iThrd; ++i)
			{
				CloseHandle(m_threads[i]);
			}
			return false;
		}
	}

    // Start threads
#ifdef PARALLELPROCESSOR_DEBUG
    char str[1024];
#endif
	size_t numAssignedBlocks = 0;
	for(size_t iThrd = 0; iThrd < m_numThreads; ++iThrd)
	{
		m_threadData[iThrd]->m_block = m_dataHandler->GetBlockForProcessing(m_threadData[iThrd]->m_index);
		// Even though thread has no block ,thread event get started and if thread has block
		// numassigned blocks are increased.
		if(m_threadData[iThrd]->m_block)
			numAssignedBlocks++;

        startThreadEvent(iThrd);
		
#ifdef PARALLELPROCESSOR_DEBUG
        sprintf_s(str, 1024, "Set start event for thread %d", iThrd);
        dumpThreadLog(m_threadData[iThrd]->m_fpLog, str);
#endif
	}

	while(numAssignedBlocks < m_numBlocks)
	{
		for (size_t iThread = 0; iThread < m_numThreads && numAssignedBlocks < m_numBlocks; iThread++)
		{
			DWORD ret = WaitForSingleObject(m_finishEvent[iThread], 0);
			if (ret == WAIT_OBJECT_0)
			{
				size_t threadIndex = m_threadData[iThread]->m_index;
				if (m_dataHandler->IsProgressSettingRequiredForThread(threadIndex))
				{
					// Set Progress
					m_dataHandler->SetProgress(true);
				}

				BlockInterface* block = m_dataHandler->GetBlockForProcessing(m_threadData[iThread]->m_index);
				if (block)
				{
					numAssignedBlocks++;
					m_threadData[iThread]->m_block = block;
					startThreadEvent(iThread);
#ifdef PARALLELPROCESSOR_DEBUG
					sprintf_s(str, 1024, "Set start event for thread %d", firstSignaled);
					dumpThreadLog(m_threadData[iThread]->m_fpLog, str);
#endif
				}
			}
		}
	}
	
    // Wait till all threads finish their operation
	WaitForMultipleObjects((int)m_numThreads,m_finishEvent,TRUE,INFINITE);

	// This is required to make sure all thread events are closed properly
	for(size_t iThrd = 0; iThrd < m_numThreads; ++iThrd)
	{
		m_threadData[iThrd]->m_processDataFlag = false;
		
        startThreadEvent(iThrd);

#ifdef PARALLELPROCESSOR_DEBUG
        sprintf_s(str, 1024, "Set start event for thread %d", iThrd);
        dumpThreadLog(m_threadData[iThrd]->m_fpLog, str);
#endif
	}

	WaitForMultipleObjects((int)m_numThreads, m_finishEvent, TRUE, INFINITE);

	// This is required to make sure all threads are closed properly
	WaitForMultipleObjects((int)m_numThreads, m_threads, TRUE, INFINITE);
	
	deleteThreadData();

	for(size_t iThrd = 0; iThrd < m_numThreads; ++iThrd)
		CloseHandle(m_threads[iThrd]);

	return true;
}

//-----------------------------------------------------------------------------

void ParallelProcessor::ProcessThread(ThreadData* threadData)
{
	DWORD dwWaitResult;
	while(1)
	{
#ifdef PARALLELPROCESSOR_DEBUG
        char str[1024];
#endif	
        dwWaitResult = WaitForSingleObject(
			m_startEvent[threadData->m_index], INFINITE);

		if(threadData->m_processDataFlag)
		{
			time_t time1 = 0;

			switch(dwWaitResult)
			{
				case WAIT_OBJECT_0 :
					
					time_t start, finish;

                    if(threadData->m_block)
                    {
						start = time(NULL);

						// Process block

#ifdef PARALLELPROCESSOR_DEBUG
						dumpThreadLog(threadData->m_fpLog, "Started processing of block");
#endif		
						m_dataHandler->ProcessBlock(threadData->m_index, threadData->m_block);
						threadData->m_block = 0;

#ifdef PARALLELPROCESSOR_DEBUG
						dumpThreadLog(threadData->m_fpLog, "Finished processing of block");
#endif

						finish = time(NULL);
						time1 = finish - start;
                    }
					else
					{
						Sleep(10);// This is for giving some time to thread for getting a block
						//The value 10 milliseconds is set by experiment
					}
                    finishThreadEvent(threadData->m_index);
                    
#ifdef PARALLELPROCESSOR_DEBUG
                    sprintf_s(str, 1024, "Set finish event for thread %d (%d)", GetCurrentThreadId(), time1);
                    dumpThreadLog(threadData->m_fpLog, str);
#endif
					break;
				default : 

#ifdef PARALLELPROCESSOR_DEBUG
					printf("\n Default thread Wait Error (%d) \n",GetLastError());
#endif
					return;
					break;
			}
		}
		else
		{
            finishThreadEvent(threadData->m_index);

#ifdef PARALLELPROCESSOR_DEBUG
            sprintf_s(str, 1024, "Set finish event for thread %d", GetCurrentThreadId());
            dumpThreadLog(threadData->m_fpLog, str);
#endif
			break;
		}

	}
}

//-----------------------------------------------------------------------------

}