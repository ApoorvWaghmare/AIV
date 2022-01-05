// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================
#include "CUSecureUtil.h"

//util include files
#include "CUTimeslice.h"


namespace CommonUtil
{
TimeSlices TIMESLICES;
//-----------------------------------------------------------------------------

TimeSlice::TimeSlice(std::string s)
		  : m_comment(s)
{
	m_stopped = false;
	m_totalTime = 0;
	m_startTime = 0;
	m_endTime = 0;
	m_numStartStops = 0;
}

//-----------------------------------------------------------------------------

TimeSlice::TimeSlice()
{
	m_stopped = false;
	m_totalTime = 0;
	m_startTime = 0;
	m_endTime = 0;
	m_numStartStops = 0;
}

//-----------------------------------------------------------------------------

void TimeSlice::Start()
{
	m_stopped = false;
	m_startTime = clock();
}

//-----------------------------------------------------------------------------

void TimeSlice::Stop()
{
	m_endTime = clock();
	m_stopped = true;
	m_totalTime += m_endTime - m_startTime;
	m_numStartStops++;
}

//-----------------------------------------------------------------------------

void TimeSlice::Reset()
{
	m_stopped = false;
	m_totalTime = 0;
	m_startTime = 0;
	m_endTime = 0;
	m_numStartStops = 0;
}

//-----------------------------------------------------------------------------

void TimeSlice::Clean()
{
	m_stopped = false;
	m_totalTime = 0;
	m_startTime = 0;
	m_endTime = 0;
	m_numStartStops = 0;
}

//-----------------------------------------------------------------------------

void TimeSlice::Report(FILE *fp)
{
	if(fp)
	{
		if (m_stopped)
		{
			const char *comment = m_comment.c_str();
			fprintf (fp, "%s", comment);
			fprintf (fp, "(%d) %5.2lf\n", m_numStartStops,
				((double)m_totalTime/CLOCKS_PER_SEC));
		}
		else
		{
			const char *comment = m_comment.c_str();
			fprintf(fp, "TimeSlice was not stopped : %s\n", comment);			
		}
	}
}
//-----------------------------------------------------------------------------

void TimeSlice::Report(CommonUtil::Report& reportObj)
{
		if (m_stopped)
		{
			char *comment = &(*m_comment.begin());
			reportObj.Add(0, "%s", comment);
			reportObj.Add(0, "(%d) %5.2lf\n", m_numStartStops,
				((double)m_totalTime/CLOCKS_PER_SEC));
		}
		else
		{
			char *comment = &(*m_comment.begin());
			reportObj.Add(0, "TimeSlice was not stopped : %s\n", comment);
		}		
}

//-----------------------------------------------------------------------------

TimeSlices::~TimeSlices()
{
	Clean();
}

//-----------------------------------------------------------------------------

TimeSlice* TimeSlices::Add(const std::string &string)
{
	TimeSlice *slice = new TimeSlice(string);
	m_slices.push_back(slice);
	return slice;
}

//-----------------------------------------------------------------------------

void TimeSlices::Report(CommonUtil::Report& reportObj)
{
	for(int i = 0; i < m_slices.size(); i++)
	{
		TimeSlice* tmpSlice = m_slices[i];
		tmpSlice->Report(reportObj);
	}
}
//-----------------------------------------------------------------------------
void TimeSlices::Report(const char* fileName)
{
	FILE* fp;
	fp = s_fopen(fileName, "w");

	if(fp)
	{
		for(int i = 0; i < m_slices.size(); i++)
		{
			TimeSlice* tmpSlice = m_slices[i];
			tmpSlice->Report(fp);
		}
		fclose(fp);
	}
}
//-----------------------------------------------------------------------------
void TimeSlices::Clean()
{
	for(int i = 0; i < m_slices.size(); i++)
	{
		TimeSlice* tmpSlice = m_slices[i];
		delete tmpSlice;
	}
	m_slices.clear();
}
//-----------------------------------------------------------------------------

}