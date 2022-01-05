//=============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
//=============================================================================

#ifndef _COMMONUTIL_REPORT_H_
#define _COMMONUTIL_REPORT_H_

//standard include files
#include <stdio.h>
#include <string>

namespace CommonUtil
{

//This class supports reporting functionality to a file
class Report
{
	//========================== private data members =========================

	std::string m_outputString;
	std::string m_filename;

	FILE *m_fp;

	char m_marker;

	int m_markerLength;

	bool m_mute;

	bool m_flushImmediately;

	bool m_append;

	bool m_useOutputString;

	//======================= private member functions ========================

	//close the report
	void close();

	//open the report
	FILE* fileOpen(const char* fileName, const char* mode);

public :
	
	//======================== public member functions ========================


	//************************** list of constructors *************************

	Report(const char *fileName, char marker = '=', int markerLength = 60);

	//Report(char marker = '=', int markerLength = 60);

	Report(bool mute = false, bool flushImmediately = false,char marker = '=', 
														int markerLength = 60);

	Report(const char *fileName, bool flushImmediately, char marker = '=',
					int markerLength = 60, bool mute = false, bool append = false);

	//****************************** destructor *******************************

	~Report();

	//*************************** get/set methods *****************************

	//sets the character used in marker
	void SetMarker(char c);
	
	//this function prints the marker line
	void AddMarker(int tabIndent);

	//This function prints the formated string with the data
	void Add(int tabIndent, const char *formatString, ...);

	//get the output string
	const std::string& GetOutputString()
	{
		return m_outputString;
	}

	bool isFilePointer()const 
	{
		if(m_fp)
			return true;
		else
			return false;
	}

	//**************************general methods********************************

	//flushes the data to the file
	void Flush()
	{
		if(m_fp)
			fflush(m_fp);
	}

	void SetMute(bool mute)
	{
		m_mute = mute;
	}

	void SetFileName(const char* fileName);

	//If true: if file pointer is null and m_useOuputString bool is true m_outputstring wil be used. 
	//If false: m_outputstring will not be used.
	void UseOutputString(bool flag)
	{
		m_useOutputString = flag;
	}

};

}

#endif