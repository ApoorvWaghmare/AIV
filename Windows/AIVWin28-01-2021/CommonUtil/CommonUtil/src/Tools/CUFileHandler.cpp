// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include "CUFileHandler.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

void FileHandler::WriteBufferIntoFile(char* data, int size)
{
	fwrite(data,sizeof(char),size,m_fp);
	fflush(m_fp);
}

//-----------------------------------------------------------------------------

void FileHandler::ReadBufferFromFile(char* data, int size)
{
	fread(data,sizeof(char),size,m_fp);
}

//-----------------------------------------------------------------------------

void FileHandler::SetFilePtrPosInFile(size_t pos)
{
	fpos_t setPos = pos;
	fsetpos(m_fp, &setPos);
}

//-----------------------------------------------------------------------------

int FileHandler::GetEndPosInFile()
{
	int FilePointerPos = 0;
	fseek (m_fp, 0, SEEK_END);  
	FilePointerPos = ftell (m_fp);
	return FilePointerPos;
}

//-----------------------------------------------------------------------------

}