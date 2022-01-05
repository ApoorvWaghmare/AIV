#include "CUBinaryIOBuffer.h"

namespace CommonUtil
{

#define MAXSIZE 1024

//-----------------------------------------------------------------------------

BinaryIOBuffer::BinaryIOBuffer(int bufferSize)
{
	m_isOpen = false;
	if(bufferSize > 0)
		m_MaxSize = bufferSize;
	else
		m_MaxSize = MAXSIZE;

	m_FilledSize = 0;
	m_Data = new char[m_MaxSize];
	memset(m_Data, -1, m_MaxSize);
	m_CurrentIndex = 0;
	m_marker = -1;
	m_calculatedBuffer = 0;
	m_calculateBufferFlag = false;
}

//-----------------------------------------------------------------------------

BinaryIOBuffer::BinaryIOBuffer(char* buffer, int size)
{
	m_isOpen = true;
	m_Data = buffer;
	m_MaxSize = size;
	m_FilledSize = 0;
	m_CurrentIndex = 0;
	m_marker = -1;
	m_calculatedBuffer = 0;
	m_calculateBufferFlag = false;
}
//-----------------------------------------------------------------------------

BinaryIOBuffer::BinaryIOBuffer(BinaryIOBuffer* copyFrom)
{
	m_isOpen = copyFrom->m_isOpen;
	m_MaxSize = copyFrom->m_MaxSize;
	m_Data = new char[m_MaxSize];
	memcpy(m_Data, copyFrom->m_Data, m_MaxSize);
	m_FilledSize = copyFrom->m_FilledSize;
	m_CurrentIndex = copyFrom->m_CurrentIndex;
	m_marker = copyFrom->m_marker;
	m_calculatedBuffer = copyFrom->m_calculatedBuffer;
	m_calculateBufferFlag = copyFrom->m_calculateBufferFlag;
}
//-----------------------------------------------------------------------------

BinaryIOBuffer::~BinaryIOBuffer()
{
	if(!m_isOpen)
	{
		delete []m_Data;
	}
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::growData(int sizeToAdd)
{
	if(!m_isOpen)
	{
		if((m_CurrentIndex + sizeToAdd) >= m_MaxSize)
		{
			do
			{
				m_MaxSize += MAXSIZE;
			}while(m_MaxSize < (m_CurrentIndex + sizeToAdd));

			copyAndReallocateData(m_MaxSize);
		}
	}
}

//---------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (char c)
{
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(char);
		return *this;	
	}
	growData(sizeof(char));
	*(char*)(m_Data+m_CurrentIndex) = c;
	m_CurrentIndex += sizeof(char);
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (bool b)
{
	char tmp = '0';
	if(b)
		tmp = '1';
	*(BinaryIOBuffer*)(this) << tmp;
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (int i)
{
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(int);
		return *this;	
	}
	
	growData(sizeof(int));
	*(int*)(m_Data+m_CurrentIndex) = i;
	m_CurrentIndex += sizeof(int);
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (__int64 i)
{
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(__int64);
		return *this;	
	}
	
	growData(sizeof(__int64));
	*(__int64*)(m_Data+m_CurrentIndex) = i;
	m_CurrentIndex += sizeof(__int64);
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (unsigned char pos)
{ 
	if (m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(unsigned char);
		return *this;
	}

	growData(sizeof(unsigned char));
	*(unsigned char*)(m_Data + m_CurrentIndex) = pos;
	m_CurrentIndex += sizeof(unsigned char);
	if (m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return *this; 
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (unsigned short pos) 
{ 
	if (m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(unsigned short);
		return *this;
	}

	growData(sizeof(unsigned short));
	*(unsigned short*)(m_Data + m_CurrentIndex) = pos;
	m_CurrentIndex += sizeof(unsigned short);
	if (m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return *this; 
}

//-----------------------------------------------------------------------------
//IOInterface& BinaryInputOutputBuffer::operator << (size_t i)
//{
//	int a = (int) i;
//	*(BinaryInputOutputBuffer*)(this) << a;
//	return *this;
//}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (float f)
{
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(float);
		return *this;	
	}
	
	growData(sizeof(float));
	*(float*)(m_Data+m_CurrentIndex) = f;
	m_CurrentIndex += sizeof(float);
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (double d)
{
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(double);
		return *this;	
	}
	
	growData(sizeof(double));
	*(double*)(m_Data+m_CurrentIndex) = d;
	m_CurrentIndex += sizeof(double);
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (const std::string& s)
{
	//Modified for x64 deepakp
	int length = (int)s.length();

	//********************************
	*(BinaryIOBuffer*)(this) << length;
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(char)*length + 1;
		return *this;	
	}
	
	growData(sizeof(char)*length);
	memcpy((m_Data + m_CurrentIndex), s.c_str(), sizeof(char)*length + 1);
	m_CurrentIndex += sizeof(char)*length + 1;
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator << (long l)
{
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(long);
		return *this;	
	}
	
	growData(sizeof(long));
	*(long*)(m_Data+m_CurrentIndex) = l;
	m_CurrentIndex += sizeof(long);
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return *this;	
}

//-----------------------------------------------------------------------------

bool BinaryIOBuffer::WriteIntArray(int n, int* data)
{
	*(BinaryIOBuffer*)(this) << n;
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(int)*n;
		return true;	
	}

	growData(sizeof(int)*n);
	memcpy((m_Data + m_CurrentIndex), data, sizeof(int)*n);
	m_CurrentIndex += sizeof(int)*n;
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return true;
}

//-----------------------------------------------------------------------------

bool BinaryIOBuffer::WriteFloatArray(int n, float* data)
{
	*(BinaryIOBuffer*)(this) << n;
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(float)*n;
		return true;	
	}

	growData(sizeof(float)*n);
	memcpy((m_Data + m_CurrentIndex), data, sizeof(float)*n);
	m_CurrentIndex += sizeof(float)*n;
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;

	return true;
}

//-----------------------------------------------------------------------------

bool BinaryIOBuffer::WriteDoubleArray(int n, double* data)
{
	*(BinaryIOBuffer*)(this) << n;
	if(m_calculateBufferFlag)
	{
		m_calculatedBuffer += sizeof(double)*n;
		return true;	
	}

	growData(sizeof(double)*n);
	memcpy((m_Data + m_CurrentIndex), data, sizeof(double)*n);
	m_CurrentIndex += sizeof(double)*n;
	if(m_CurrentIndex > m_FilledSize)
		m_FilledSize = m_CurrentIndex;
	return true;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator >> (char& c)
{
	c = *(char*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(char);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator >> (bool& b)
{
	char tmp;
	*(BinaryIOBuffer*)(this) >> tmp;
	if(tmp == '0')
		b = false;
	else
		b = true;

	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator >> (int& i)
{
	i = *(int*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(int);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator >> (__int64& i)
{
	i = *(__int64*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(__int64);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator >> (size_t& i)
{
	int a;
	*(BinaryIOBuffer*)(this) >> a;
	i = (size_t)a;
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator >> (float& f)
{
	f = *(float*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(float);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator >> (double& d)
{
	d = *(double*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(double);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator >> (std::string& s)
{
	int length = 0;
	*(BinaryIOBuffer*)(this) >> length;

	const int sizeofString = sizeof(char)*length + 1;

	char* tmp = new char[sizeofString];
	memcpy(tmp, (m_Data + m_CurrentIndex), sizeofString);
	s = tmp;
	delete[]tmp;

	m_CurrentIndex += sizeofString;
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BinaryIOBuffer::operator >> (long& l)
{
	l = *(long*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(long);
	return *this;	
}

//-----------------------------------------------------------------------------
BinaryIOInterface& BinaryIOBuffer::operator >> (unsigned short& pos)
{
	pos = *(unsigned short*)(m_Data + m_CurrentIndex);
	m_CurrentIndex += sizeof(unsigned short);
	return *this;
}

//-----------------------------------------------------------------------------
BinaryIOInterface& BinaryIOBuffer::operator >> (unsigned char& pos)
{
	pos = *(unsigned char*)(m_Data + m_CurrentIndex);
	m_CurrentIndex += sizeof(unsigned char);
	return *this;
}

//-----------------------------------------------------------------------------
bool BinaryIOBuffer::ReadIntArray(int& n, int*& data)
{
	n = 0;
	*(BinaryIOBuffer*)(this) >> n;
	data = 0;
	char* tmp = new char[sizeof(int)*n];
	memcpy(tmp, (m_Data + m_CurrentIndex), sizeof(int)*n);
	data = (int*)tmp;
	delete []tmp;
	m_CurrentIndex += sizeof(int)*n;
	return true;
}

//-----------------------------------------------------------------------------

bool BinaryIOBuffer::ReadFloatArray(int& n, float*& data)
{
	n = 0;
	*(BinaryIOBuffer*)(this) >> n;
	data = 0;
	char* tmp = new char[sizeof(float)*n];
	memcpy(tmp, (m_Data + m_CurrentIndex), sizeof(float)*n);
	data = (float*)tmp;
	delete []tmp;
	m_CurrentIndex += sizeof(float)*n;
	return true;
}

//-----------------------------------------------------------------------------

bool BinaryIOBuffer::ReadDoubleArray(int& n, double*& data)
{
	n = 0;
	*(BinaryIOBuffer*)(this) >> n;
	data = 0;
	char* tmp = new char[sizeof(double)*n];
	memcpy(tmp, (m_Data + m_CurrentIndex), sizeof(double)*n);
	data = (double*)tmp;
	delete []tmp;
	m_CurrentIndex += sizeof(double)*n;
	return true;
}

//-----------------------------------------------------------------------------

bool BinaryIOBuffer::IsStoring()
{
	bool isStoring = false;
	if(!m_isOpen)
		isStoring = true;
	return isStoring;
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::SetMarker()
{
	m_marker = m_CurrentIndex;
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::SaveSizeToMarker()
{
	int size = m_CurrentIndex - m_marker - sizeof(int);
	*(int*)(m_Data + m_marker) = size;
}

//-----------------------------------------------------------------------------

bool BinaryIOBuffer::IsEndReached()
{
	bool endReached = true;
	if((m_CurrentIndex < m_MaxSize) && (m_CurrentIndex <= m_FilledSize))
		endReached = false;
	return endReached;
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::DetachBuffer(char* &buffer, int &size)
{
	if(!m_isOpen)
	{
		size = m_CurrentIndex;
		buffer = m_Data;
		m_Data = NULL;
	}
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::ResetCurrentIndex()
{
	m_CurrentIndex = 0;
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::SetMarkerForBRep()
{
	//m_BRepPosMarker = m_CurrentIndex;
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::SetMarkerForWireFrame()
{
	//m_WireFramePosMarker = m_CurrentIndex;
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::SavePositionToBRepMarker()
{
	//*(int*)(m_Data + m_BRepPosMarker) = m_CurrentIndex;
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::SavePositionToWireFrameMarker()
{
	//*(int*)(m_Data + m_WireFramePosMarker) = m_CurrentIndex;
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::SetMarkerForFileSize()
{}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::SaveFileSizeToFileSizeMarker()
{}

//-----------------------------------------------------------------------------

__int64 BinaryIOBuffer::GetFileSize()
{
	return m_MaxSize;	
}

//-----------------------------------------------------------------------------

void BinaryIOBuffer::SeekPosition(__int64 pos)
{

}

//-----------------------------------------------------------------------------

int BinaryIOBuffer::GetFilledSize()const
{
	return m_FilledSize;
}

//-----------------------------------------------------------------------------
void BinaryIOBuffer::ReleaseUnusedMemory()
{
	copyAndReallocateData(m_CurrentIndex);
	m_MaxSize = m_CurrentIndex;
}

void BinaryIOBuffer::copyAndReallocateData(int newSize)
{
	// newSize >= m_CurrentIndex
	char *temp = new char[newSize];
	memset(temp, -1, newSize);
	memcpy(temp, m_Data, m_CurrentIndex);
	delete [] m_Data;
	m_Data = temp;
}

void BinaryIOBuffer::SetBoolForMemoryCalculation(bool calculateMemory)
{
	m_calculateBufferFlag = calculateMemory;
}

int BinaryIOBuffer::GetCalculatedBufferSize()const
{
	return m_calculatedBuffer;
}

}