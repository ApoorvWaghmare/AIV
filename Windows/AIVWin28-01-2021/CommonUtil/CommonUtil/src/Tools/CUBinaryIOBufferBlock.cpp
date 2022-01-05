// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include "CUBinaryIOBufferBlock.h"
#include "CUPoint3DCache.h"

namespace CommonUtil
{

#define NEXTBUFFEROFFSETSIZE sizeof(size_t)

//-----------------------------------------------------------------------------

BinaryIOBufferBlock::BinaryIOBufferBlock(int maxsize, FileHandler* binaryIOFile)
{
	m_nextBuffer = false;
	m_binaryIOFile = binaryIOFile;
	m_maxSize = maxsize;
	m_data = new char[m_maxSize];
	memset(m_data, -1, m_maxSize);
	m_CurrentIndex = 0;
}

//-----------------------------------------------------------------------------

BinaryIOBufferBlock::~BinaryIOBufferBlock()
{
	if(m_data)
		delete []m_data;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBufferBlock::operator << (size_t i)
{
	size_t size = sizeof(size_t);
	char* data = (char*)&i;
	writeDataToBuffer(size,data);
	return *this;
}

//-----------------------------------------------------------------------------

bool BinaryIOBufferBlock::WriteFloatArray(int n, float* data)
{
	size_t size = sizeof(float)*n;
	char* data1 = (char*)data;
	writeDataToBuffer(size,data1);

	return true;
}
//-----------------------------------------------------------------------------
BinaryIOInterface& BinaryIOBufferBlock::operator << (int i)
{
	size_t size = sizeof(int);
	char* data = (char*)&i;
	writeDataToBuffer(size,data);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBufferBlock::operator << (float i)
{
	size_t size = sizeof(float);
	char* data = (char*)&i;
	writeDataToBuffer(size,data);
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBufferBlock::operator << (double i)
{
	size_t size = sizeof(double);
	char* data = (char*)&i;
	writeDataToBuffer(size,data);
	return *this;
}
//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBufferBlock::operator << (unsigned char i)
{
	size_t size = sizeof(unsigned char);
	char* data = (char*)&i;
	writeDataToBuffer(size,data);
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBufferBlock::operator >> (int& i)
{
	size_t size = sizeof(int);
	char* data = (char*)&i;

	readDatafromBuffer(size,data);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBufferBlock::operator >> (size_t& i)
{
	size_t size = sizeof(size_t);
	char* data = (char*)&i;

	readDatafromBuffer(size,data);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBufferBlock::operator >> (float& i)
{
	size_t size = sizeof(float);
	char* data = (char*)&i;
	readDatafromBuffer(size,data);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBufferBlock::operator >> (double& i)
{
	size_t size = sizeof(double);
	char* data = (char*)&i;
	readDatafromBuffer(size,data);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBufferBlock::operator >> (unsigned char& i)
{
	size_t size = sizeof(unsigned char);
	char* data = (char*)&i;
	readDatafromBuffer(size,data);
	return *this;	
}

//-----------------------------------------------------------------------------

bool BinaryIOBufferBlock::ReadFloatArray(int &n, float* &data)
{
	size_t size = sizeof(float)*n;
	char* data1 = (char*)data;
	readDatafromBuffer(size,data1);
	data = (float*)(data1);

	return true;
}

//-----------------------------------------------------------------------------

void BinaryIOBufferBlock::ReleaseBuffer()
{
	if(IsNextBuffer())
	{
		int pos = m_binaryIOFile->GetEndPosInFile();
		m_binaryIOFile->SetFilePtrPosInFile(pos);
	}
	m_binaryIOFile->WriteBufferIntoFile(m_data,m_maxSize);
}

//-----------------------------------------------------------------------------

size_t BinaryIOBufferBlock::getEmptySpaceCountInBuffer()
{
	size_t emptySpaceCount =  (m_maxSize - NEXTBUFFEROFFSETSIZE) - m_CurrentIndex;
	return emptySpaceCount;
}

//-----------------------------------------------------------------------------

void BinaryIOBufferBlock::ClearBuffer()
{
	memset(m_data, -1, m_maxSize);
	m_CurrentIndex = 0;
}

//-----------------------------------------------------------------------------
void BinaryIOBufferBlock::GetBuffer()
{
	m_binaryIOFile->ReadBufferFromFile(m_data,m_maxSize);
}

//-----------------------------------------------------------------------------

void BinaryIOBufferBlock::writeDataToBuffer(size_t size, char* data)
{
	size_t fillingIndex = 0;
	size_t totalSize = size;
	while(size!=0)
	{
		//calculate empty space in buffer
		size_t emptySpace = getEmptySpaceCountInBuffer();
		while(emptySpace!=0)
		{
			if(size < emptySpace)
			{
				if(size!=0)
				{
					memcpy((m_data + m_CurrentIndex), (data + fillingIndex), sizeof(char)*size);
					m_CurrentIndex += sizeof(char)*size;
					fillingIndex += sizeof(char)*size;
					emptySpace -= sizeof(char)*size;
					size = 0;
				}
				else
					break;
			}
			else
			{
				if(fillingIndex!=totalSize)
				{
					memcpy((m_data + m_CurrentIndex), (data + fillingIndex), sizeof(char)*emptySpace);
					m_CurrentIndex += sizeof(char)*emptySpace;
					size -= sizeof(char)*emptySpace;
					fillingIndex += sizeof(char)*emptySpace;
					emptySpace = 0;
				}
				else
					break;
			}
		}

		if(emptySpace == 0)
		{
			size_t pos = m_binaryIOFile->GetEndPosInFile();
			size_t nextBufferPos = pos + m_maxSize;
			*(size_t*)(m_data + (m_maxSize - NEXTBUFFEROFFSETSIZE)) = nextBufferPos;

			m_binaryIOFile->SetFilePtrPosInFile(pos); 
			m_binaryIOFile->WriteBufferIntoFile(m_data,m_maxSize);
			ClearBuffer();

			//set flag of next buffer true
			m_nextBuffer = true;
		}
	}
}

//-----------------------------------------------------------------------------

void BinaryIOBufferBlock::readDatafromBuffer(size_t size, char* &data)
{ 
	size_t fillingIndex = 0;
	size_t totalSize = size;
	while(size!=0)
	{
		//calculate empty space in buffer
		size_t emptySpace = getEmptySpaceCountInBuffer();
		while(emptySpace!=0)
		{
			if(size < emptySpace)
			{
				if(size!=0)
				{
					memcpy((data + fillingIndex), (m_data + m_CurrentIndex), sizeof(char)*size);
					m_CurrentIndex += sizeof(char)*size;
					fillingIndex += sizeof(char)*size;
					emptySpace -= sizeof(char)*size;
					size = 0;
				}
				else
					break;
			}
			else
			{
				if(fillingIndex!=totalSize)
				{
					memcpy((data + fillingIndex), (m_data + m_CurrentIndex), sizeof(char)*emptySpace);
					m_CurrentIndex += sizeof(char)*emptySpace;
					size -= sizeof(char)*emptySpace;
					fillingIndex += sizeof(char)*emptySpace;
					emptySpace = 0;
				}
				else
					break;
			}
		}

		if(emptySpace == 0)
		{
			//write buffer into file 
			size_t nextBufferPos = *(size_t*)(m_data + (m_maxSize - NEXTBUFFEROFFSETSIZE));
			m_binaryIOFile->SetFilePtrPosInFile(nextBufferPos);
			ClearBuffer();
			m_binaryIOFile->ReadBufferFromFile(m_data,m_maxSize);
		}
	}
}

//-----------------------------------------------------------------------------

}