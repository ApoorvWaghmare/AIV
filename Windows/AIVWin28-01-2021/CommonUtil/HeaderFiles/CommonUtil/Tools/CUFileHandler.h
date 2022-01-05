// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================
#ifndef _BINARY_INPUT_OUTPUT_FILE_H_
#define _BINARY_INPUT_OUTPUT_FILE_H_

#pragma warning (disable: 4996)
#include <vector>

namespace CommonUtil
{
#pragma pack(push)
#pragma pack(1)
class FileHandler
{
	FILE *m_fp;
	bool m_fileExist;

public:

	FileHandler(char* fileName, char* mode = "wb+")
	{
		//m_filename = fileName;
		m_fp=fopen(fileName , mode);

		if(m_fp)
			m_fileExist = true;
		else
			m_fileExist = false;
	}

	~FileHandler()
	{
		if(m_fp)
			fclose(m_fp);
		//remove(m_filename);
	}

	int GetEndPosInFile();

	void SetFilePtrPosInFile(size_t pos);

	//get file pointer(the file pointer is used for a Point3DCache)
	FILE * GetFilePointer()const{return m_fp;}

	void WriteBufferIntoFile(char* data, int size);

	void ReadBufferFromFile(char* data, int size);

	bool IsFileExist()
	{
		return m_fileExist;
	}

};
#pragma pack(pop)   /* restore original from stack */
}

#endif