// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_TIMESLICE_H_
#define _COMMONUTIL_TIMESLICE_H_

//standard include files
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include "CUReport.h"

namespace CommonUtil
{

//This class supports Time measurment functionality
class TimeSlice
{
	//========================== private data members =========================

	std::string m_comment;

	clock_t m_startTime;
	clock_t m_endTime;

	int m_totalTime;
	int m_numStartStops;
	
	bool m_stopped;

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	TimeSlice();

	TimeSlice(std::string s);
	
	//****************************** destructor *******************************

	~TimeSlice()
	{}

	//*************************** get/set methods *****************************

	double GetTime()const
	{
		return (double)m_totalTime/CLOCKS_PER_SEC;
	}

	//************************* general methods *******************************

	void Start();

	void Stop();

	void Reset();

	void Clean();

	void Report(FILE *fp);

	void Report(CommonUtil::Report& reportObj);

};


//This class is collection of timeslices
class TimeSlices
{
	//========================== private data members =========================

	std::vector<TimeSlice*> m_slices;

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	TimeSlices()
	{}

	//****************************** destructor *******************************

	~TimeSlices();

	//************************* general methods *******************************

	TimeSlice* Add(const std::string &string);

	void Report(const char* fileName);

	void Report(CommonUtil::Report& reportObj);

	void Clean();

};
extern TimeSlices TIMESLICES;

}

#endif
