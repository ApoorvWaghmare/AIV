// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include <stdarg.h>
#include <string.h>

//util include files
#include "CUReport.h"
//#include "CUSecureUtil.h"


namespace CommonUtil
{

//-----------------------------------------------------------------------------

Report::Report(const char *fileName, char marker, int markerLength)
		: m_filename(fileName),
		  m_marker(marker),
		  m_markerLength(markerLength)
{
	m_fp = fileOpen(fileName, "w");
	m_mute = false;
	m_append = false;
	m_flushImmediately = false;
	m_useOutputString = false;
}

//-----------------------------------------------------------------------------

Report::Report(bool mute, bool flushImmediately, char marker, int markerLength)
{
	m_mute = mute;
	m_append = false;
	m_marker = marker;
	m_markerLength = markerLength;
	m_fp = 0;
	m_flushImmediately = flushImmediately;
	m_useOutputString = false;
}

//-----------------------------------------------------------------------------

Report::Report(const char *fileName, bool flushImmediately, char marker,
						int markerLength, bool mute, bool append) : m_filename(fileName)
{
	m_fp = 0;

	m_marker = marker;
	m_markerLength = markerLength;
	m_flushImmediately = flushImmediately;
	m_mute = mute;
	m_append = append;
	m_useOutputString = false;
	if(!m_mute)
	{
		if(append)
			m_fp = fileOpen(fileName, "a");
		else
			m_fp = fileOpen(fileName, "w");
	}
}
//-----------------------------------------------------------------------------
Report::~Report()
{
	close();
}

//-----------------------------------------------------------------------------

void Report::SetMarker(char c)
{
	m_marker = c;
}

//-----------------------------------------------------------------------------

void Report::AddMarker(int tabIndent)
{
	if(m_mute)
		return;

	if (!m_fp && !m_useOutputString)
		return;

	//marks the indentation
	for(int i = 0; i < tabIndent; ++i)
	{
		if(m_fp)
			fprintf(m_fp,"\t");
		else if(m_useOutputString)
			m_outputString += "\t";
	}

	//prints the marker line
	for(int i = 0; i < m_markerLength; ++i)
	{
		if(m_fp)
			fprintf(m_fp, "%c", m_marker);
		else if(m_useOutputString)
			m_outputString += m_marker;
	}
	if(m_fp)
		fprintf(m_fp, "\n");
	else if(m_useOutputString)
		m_outputString += "\n";

	if(m_fp && m_flushImmediately)
		fflush(m_fp);
}

//-----------------------------------------------------------------------------

void Report::Add(int tabIndent, const char *formatString, ...)
{
	if(m_mute)
		return;

	if (!m_fp && !m_useOutputString)
		return;

	//marks the Indentation
	for(int i = 0; i < tabIndent; i++)
	{
		if(m_fp)
			fprintf(m_fp,"\t");
		else if(m_useOutputString)
			m_outputString += "\t";
	}

	va_list ap;
	va_start(ap, formatString);

	char buf[1000];
	strcpy(buf, formatString);

	char *tmpPtr = strchr(buf, '%');
	char *currentPtr = 0;

	if (tmpPtr)
	{
		currentPtr = tmpPtr + 1;
		*tmpPtr = 0;
	}
	if(m_fp)
		fprintf(m_fp, buf);
	else if(m_useOutputString)
		m_outputString += buf;

	while (currentPtr)
	{
		char tmpFormatString[200];
		tmpFormatString[0] = '%';

		int i = 0;
		for (;; ++i)
		{
			char c = currentPtr[i];

			tmpFormatString[i+1] =  c;
			tmpFormatString[i+2] = 0;

			if (c == 'e' || c =='E' || c == 'f' || c == 'g' || c == 'G')
			{
				double d = va_arg(ap, double);
				if(m_fp)
					fprintf (m_fp, tmpFormatString, d);
				else if (m_useOutputString)
				{
					char tmpBuf[2000];
					sprintf(tmpBuf, tmpFormatString, d);
					m_outputString += tmpBuf;
				}


				break;
			}
			else if (c == 'c' || c == 'C' || c == 'd' || c == 'i' || c == 'o' ||
					 c == 'u' || c == 'x' || c == 'X')
			{
				int val = va_arg(ap, int);
				if(m_fp)
					fprintf(m_fp, tmpFormatString, val);
				else if (m_useOutputString)
				{
					char tmpBuf[2000];
					sprintf(tmpBuf, tmpFormatString, val);
					m_outputString += tmpBuf;
				}
				break;
			}
			else if (c == 'n' || c == 'P')
			{
				void *p = va_arg(ap, void *);
				if(m_fp)
					fprintf (m_fp, tmpFormatString, p);
				else if (m_useOutputString)
				{
					char tmpBuf[2000];
					sprintf(tmpBuf, tmpFormatString, p);
					m_outputString += tmpBuf;
				}

				break;
			}
			else if (c == 's' || c == 'S')
			{
				char *str = va_arg(ap, char *);
				if(m_fp)
					fprintf (m_fp, tmpFormatString, str);
				else if (m_useOutputString)
				{
					char tmpBuf[2000];
					sprintf(tmpBuf, tmpFormatString, str);
					m_outputString += tmpBuf;
				}

				break;
			}
		}

		currentPtr += i+1;

		char *tmpPtr = strchr(currentPtr, '%');
		if (tmpPtr)
		{
			*tmpPtr = 0;
		}
		if(m_fp)
			fprintf(m_fp, currentPtr);
		else if(m_useOutputString)
			m_outputString += currentPtr;

		if (tmpPtr)
			currentPtr = tmpPtr + 1;
		else
			currentPtr = 0;
	}
	if(m_fp && m_flushImmediately)
		fflush(m_fp);

	va_end(ap);
}

//-----------------------------------------------------------------------------

void Report::SetFileName(const char* fileName)
{
	if(!m_mute)
	{
		if(m_append)
			m_fp = fileOpen(fileName, "a");
		else
			m_fp = fileOpen(fileName, "w");
	}
}

//---------------------------------------------------------------------

void Report::close()
{
	if (m_fp)
	{
		fclose(m_fp);
		m_fp = 0;
	}
}

//-----------------------------------------------------------------------------
FILE* Report::fileOpen(const char* fileName, const char* mode)
{
	FILE* fp = 0;

//#ifdef REPORT_ENABLED
	if (mode)
		fp = fopen(fileName, mode);
//#endif REPORT_ENABLED

	return fp;
}

//-----------------------------------------------------------------------------
}
