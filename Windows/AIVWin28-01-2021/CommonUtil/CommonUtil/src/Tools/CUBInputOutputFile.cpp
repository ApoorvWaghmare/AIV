#include "CUBInputOutputFile.h"
#include <assert.h>

//#include <sys/stat.h> 
//#include <sys/types.h>

namespace CommonUtil
{


BInputOutputFile::BInputOutputFile(const wchar_t* path, bool openForReading)
{
	m_featurePosMarker = -1;
	m_measurementPosMarker = -1;
	m_fileSizeMarker = -1;
	m_sketchPosMarker = -1;
	m_historyFeaturePosMarker = -1;
	m_layerColourPosMarker = -1;
	m_coordinateSystemMarker = -1;
	m_openForReading = openForReading;
	m_marker = -1;
	m_pointPosMarker = -1;
	m_pointCloudPosMarker = -1;
	fp = new std::fstream;
	if(m_openForReading)
	{
		fp->open(path, std::ios::in | std::ios::binary );
		m_fileClosed = false;
	}
	else
	{
		fp->open(path, std::ios::out | std::ios::binary | std::ios::trunc );
		m_fileClosed = false;
	}

	m_maxSize = 1024;
	m_Data = new char[m_maxSize];
	m_CurrentIndex = 0;
	m_FileCurrentIndex = 0;
	m_firstTime = true;
	m_writeAllData = false;
	m_versionBelow4 = false;

	computeFileSize();
}

//-----------------------------------------------------------------------------

BInputOutputFile::~BInputOutputFile()
{
	flush();
	if(fp)
	{
		fp->close();
		delete fp;
		fp = 0;
		delete []m_Data;
	}
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetWriteMode(const wchar_t* path, bool write)
{
	if(!write)
	{
		m_openForReading = true;

		if(!fp)
			fp = new std::fstream;
		if(!m_Data)
			m_Data = new char[m_maxSize];
		fp->open(path, std::ios::in | std::ios::binary );
		m_fileClosed = false;
	}
	else
	{
		if(!fp)
			fp = new std::fstream;
		if(!m_Data)
			m_Data = new char[m_maxSize];
		fp->open(path, std::ios::out | std::ios::binary | std::ios::app );
		m_openForReading = false;
		m_fileClosed = false;
	}
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::growData(int sizeToAdd)
{
	bool grow = true;
	if((m_CurrentIndex + sizeToAdd) >= m_maxSize)
	{
		fp->write(m_Data, m_CurrentIndex);
		m_CurrentIndex = 0;
		if(sizeToAdd >= m_maxSize)
			grow = false;
	}
	return grow;
}

//-----------------------------------------------------------------------------------

bool BInputOutputFile::readData(int sizeToRead)
{
	bool read = true;
	if(m_firstTime)
	{
		if(sizeToRead >= m_maxSize)
			read = false;
		else
			fp->read(m_Data, m_maxSize);
		m_firstTime = false;
	}
	
	if((m_CurrentIndex + sizeToRead) >= m_maxSize)
	{
	//	fp->seekg(-(MAXSIZE - m_CurrentIndex), ios::cur);
		int diff = m_maxSize - m_CurrentIndex;
	/*	char* tmp = new char[diff];
		memcpy(tmp, (m_Data+m_CurrentIndex), diff);*/
		memmove(m_Data, (m_Data+m_CurrentIndex), diff);
		if(sizeToRead >= m_maxSize)
		{
			__int64 filePosition = fp->tellp().seekpos();
			__int64 pos = filePosition - (m_maxSize - m_CurrentIndex);
			m_CurrentIndex = 0;
			m_firstTime = true;
			fp->seekp(pos, std::ios::beg);
			read = false;
		}
		else
		{

			fp->read((m_Data + diff), m_maxSize - diff);
		}
		m_CurrentIndex = 0;
	}
	return read;
}

//----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (char c)
{
	growData(sizeof(char));
	*(char*)(m_Data+m_CurrentIndex) = c;
	m_CurrentIndex += sizeof(char);
	m_FileCurrentIndex += sizeof(char);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (bool b)
{
	char tmp = '0';
	if(b)
		tmp = '1';
	*(BInputOutputFile*)(this) << tmp;
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (int i)
{
	growData(sizeof(int));
	*(int*)(m_Data+m_CurrentIndex) = i;
	m_CurrentIndex += sizeof(int);
	m_FileCurrentIndex += sizeof(int);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (size_t i)
{
	growData(sizeof(size_t));
	*(size_t*)(m_Data+m_CurrentIndex) = i;
	m_CurrentIndex += sizeof(size_t);
	m_FileCurrentIndex += sizeof(size_t);
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (float f)
{
	growData(sizeof(float));
	*(float*)(m_Data+m_CurrentIndex) = f;
	m_CurrentIndex += sizeof(float);
	m_FileCurrentIndex += sizeof(float);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (double d)
{
	growData(sizeof(double));
	*(double*)(m_Data+m_CurrentIndex) = d;
	m_CurrentIndex += sizeof(double);
	m_FileCurrentIndex += sizeof(double);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (const std::string& s)
{
	int length = (int)s.length();
	*(BInputOutputFile*)(this) << length;
	
	if(growData(sizeof(char)*length))
	{
		memcpy((m_Data + m_CurrentIndex), s.c_str(), sizeof(char)*length + 1);
		m_CurrentIndex += sizeof(char)*length + 1;
		m_FileCurrentIndex += sizeof(char)*length +1;
	}
	else
	{
		fp->write(s.c_str(), sizeof(char)*length + 1);
		m_FileCurrentIndex += sizeof(char)*length +1;
	}

	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (long l)
{
	growData(sizeof(long));
	*(long*)(m_Data+m_CurrentIndex) = l;
	m_CurrentIndex += sizeof(long);
	m_FileCurrentIndex += sizeof(long);
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (__int64 pos)
{
	growData(sizeof(__int64));
	*(__int64*)(m_Data+m_CurrentIndex) = pos;
	m_CurrentIndex += sizeof(__int64);
	m_FileCurrentIndex += sizeof(__int64);
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (unsigned char pos)
{
	growData(sizeof(unsigned char));
	*(unsigned char*)(m_Data+m_CurrentIndex) = pos;
	m_CurrentIndex += sizeof(unsigned char);
	m_FileCurrentIndex += sizeof(unsigned char);
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator << (unsigned short pos)
{
	growData(sizeof(unsigned short));
	*(unsigned short*)(m_Data+m_CurrentIndex) = pos;
	m_CurrentIndex += sizeof(unsigned short);
	m_FileCurrentIndex += sizeof(unsigned short);
	return *this;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::WriteIntArray(int n, int* data)
{
	*(BInputOutputFile*)(this) << n;
	if(growData(sizeof(int)*n))
	{
		memcpy((m_Data + m_CurrentIndex), data, sizeof(int)*n);
		m_CurrentIndex += sizeof(int)*n;
		m_FileCurrentIndex += sizeof(int)*n;
	}
	else
	{
		char* tmp = new char[sizeof(int)*n];
		*(int**)(tmp) = data;
		fp->write(tmp, sizeof(int)*n);
		delete []tmp;
		m_FileCurrentIndex += sizeof(int)*n;
//		fp->write((char*)data, sizeof(int)*n);
	}
	return true;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::WriteSizetArray(int n, size_t* data)
{
	if(growData(sizeof(size_t)*n))
	{
		memcpy((m_Data + m_CurrentIndex), data, sizeof(size_t)*n);
		m_CurrentIndex += sizeof(size_t)*n;
		m_FileCurrentIndex += sizeof(size_t)*n;
	}
	else
	{
		char* tmp = new char[sizeof(size_t)*n];
		*(size_t**)(tmp) = data;
		fp->write(tmp, sizeof(size_t)*n);
		delete []tmp;
		m_FileCurrentIndex += sizeof(size_t)*n;
//		fp->write((char*)data, sizeof(int)*n);
	}
	return true;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::WriteFloatArray(int n, float* data)
{
	*(BInputOutputFile*)(this) << n;
	if(growData(sizeof(float)*n))
	{
		memcpy((m_Data + m_CurrentIndex), data, sizeof(float)*n);
		m_CurrentIndex += sizeof(float)*n;
		m_FileCurrentIndex += sizeof(float)*n;
	}
	else
	{
		char* tmp = new char[sizeof(float)*n];
		*(float**)(tmp) = data;
		fp->write(tmp, sizeof(float)*n);
		delete []tmp;
		m_FileCurrentIndex += sizeof(float)*n;
	//	fp->write((char*)data, sizeof(float)*n);
	}
	return true;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::WriteDoubleArray(int n, double* data)
{
	*(BInputOutputFile*)(this) << n;
	if(growData(sizeof(double)*n))
	{
		memcpy((m_Data + m_CurrentIndex), data, sizeof(double)*n);
		m_CurrentIndex += sizeof(double)*n;
		m_FileCurrentIndex += sizeof(double)*n;
	}
	else
	{
		char* tmp = new char[sizeof(double)*n];
		*(double**)(tmp) = data;
		fp->write(tmp, sizeof(double)*n);
		delete []tmp;
		m_FileCurrentIndex += sizeof(double)*n;
	//	fp->write((char*)data, sizeof(double)*n);
	}
	return true;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::WriteCharArray(int n, char* data)
{
	if(growData(sizeof(char)*n))
	{
		memcpy((m_Data + m_CurrentIndex), data, sizeof(char)*n);
		m_CurrentIndex += sizeof(char)*n;
		m_FileCurrentIndex += sizeof(char)*n;
	}
	else
	{
		//char* tmp = new char[sizeof(char)*n];
		//*(char**)(tmp) = data;
		fp->write(data, sizeof(char)*n);
		//delete []tmp;
		m_FileCurrentIndex += sizeof(char)*n;
	//	fp->write((char*)data, sizeof(double)*n);
	}

	return true;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (char& c)
{
	readData(sizeof(char));
	c = *(char*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(char);
	m_FileCurrentIndex += sizeof(char);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (bool& b)
{
	char tmp;
	*(BInputOutputFile*)(this) >> tmp;
	if(tmp == '0')
		b = false;
	else
		b = true;

	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (int& i)
{
	readData(sizeof(int));
	i = *(int*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(int);
	m_FileCurrentIndex += sizeof(int);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (size_t& i)
{
	readData(sizeof(size_t));
	i = *(size_t*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(size_t);
	m_FileCurrentIndex += sizeof(size_t);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (float& f)
{
	readData(sizeof(float));
	f = *(float*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(float);
	m_FileCurrentIndex += sizeof(float);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (double& d)
{
	readData(sizeof(double));
	d = *(double*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(double);
	m_FileCurrentIndex += sizeof(double);
	return *this;	
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (std::string& s)
{
	int length = 0;
	*(BInputOutputFile*)(this) >> length;

	s.clear();
	if(readData(sizeof(char)*length + 1))
	{
		for(int i = 0; i < length; i++ )
		{
			s += m_Data[m_CurrentIndex];
			++m_CurrentIndex;//++;
			++m_FileCurrentIndex;
		}
		++m_CurrentIndex;
		++m_FileCurrentIndex;
		//m_CurrentIndex += sizeof(char)*length + 1;
	}
	else
	{	char* tmp = new char[length+1];
		fp->read(tmp, length+1);
		s = tmp;
		delete []tmp;
		m_FileCurrentIndex = m_FileCurrentIndex + length+1;
	}
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (long& l)
{
	readData(sizeof(long));
	l = *(long*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(long);
	m_FileCurrentIndex += sizeof(long);
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (__int64& pos)
{
	readData(sizeof(__int64));
	pos = *(__int64*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(__int64);
	m_FileCurrentIndex += sizeof(__int64);
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (unsigned char& pos)
{
	readData(sizeof(unsigned char));
	pos = *(unsigned char*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(unsigned char);
	m_FileCurrentIndex += sizeof(unsigned char);
	return *this;
}

//-----------------------------------------------------------------------------

BinaryIOInterface& BInputOutputFile::operator >> (unsigned short& pos)
{
	readData(sizeof(unsigned short));
	pos = *(unsigned short*)(m_Data+m_CurrentIndex);
	m_CurrentIndex += sizeof(unsigned short);
	m_FileCurrentIndex += sizeof(unsigned short);
	return *this;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::ReadIntArray(int& n, int*& data)
{
	n = 0;
	*(BInputOutputFile*)(this) >> n;
	data = 0;
	if(readData(sizeof(int)*n))
	{
		char* tmp = new char[sizeof(int)*n];
		memcpy(tmp, (m_Data + m_CurrentIndex), sizeof(int)*n);
		data = (int*)tmp;
		delete []tmp;
		m_CurrentIndex += sizeof(int)*n;
		m_FileCurrentIndex += sizeof(int)*n;
	}
	else
	{
		char* tmp = new char[sizeof(int)*n];
		fp->read(tmp, sizeof(int)*n);
		data = (int*)tmp;
		delete []tmp;
		m_FileCurrentIndex += sizeof(int)*n;
	}
	return true;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::ReadSizetArray(int n, size_t*& data)
{
	data = 0;
	if(readData(sizeof(size_t)*n))
	{
		char* tmp = new char[sizeof(size_t)*n];
		memcpy(tmp, (m_Data + m_CurrentIndex), sizeof(size_t)*n);
		data = (size_t*)tmp;
		delete []tmp;
		m_CurrentIndex += sizeof(size_t)*n;
		m_FileCurrentIndex += sizeof(size_t)*n;
	}
	else
	{
		char* tmp = new char[sizeof(size_t)*n];
		fp->read(tmp, sizeof(size_t)*n);
		data = (size_t*)tmp;
		delete []tmp;
		m_FileCurrentIndex += sizeof(size_t)*n;
	}
	return true;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::ReadFloatArray(int& n, float*& data)
{
	n = 0;
	*(BInputOutputFile*)(this) >> n;
	data = 0;
	if(readData(sizeof(float)*n))
	{
		char* tmp = new char[sizeof(float)*n];
		memcpy(tmp, (m_Data + m_CurrentIndex), sizeof(float)*n);
		data = (float*)tmp;
		delete []tmp;
		m_CurrentIndex += sizeof(float)*n;
		m_FileCurrentIndex += sizeof(float)*n;
	}
	else
	{
		char* tmp = new char[sizeof(float)*n];
		fp->read(tmp, sizeof(float)*n);
		data = (float*)tmp;
		delete []tmp;
		m_FileCurrentIndex += sizeof(float)*n;
	}
	return true;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::ReadDoubleArray(int& n, double*& data)
{
	n = 0;
	*(BInputOutputFile*)(this) >> n;
	data = 0;
	if(readData(sizeof(double)*n))
	{
		char* tmp = new char[sizeof(double)*n];
		memcpy(tmp, (m_Data + m_CurrentIndex), sizeof(double)*n);
		data = (double*)tmp;
		delete []tmp;
		m_CurrentIndex += sizeof(double)*n;
		m_FileCurrentIndex += sizeof(double)*n;
	}
	else
	{
		char* tmp = new char[sizeof(double)*n];
		fp->read(tmp, sizeof(double)*n);
		data = (double*)tmp;
		delete []tmp;
		m_FileCurrentIndex += sizeof(double)*n;
	}
	return true;
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::ReadCharArray(int n, char*& data)
{
	data = new char[sizeof(char)*n];
	if(readData(sizeof(char)*n))
	{
		memcpy(data, (m_Data + m_CurrentIndex), sizeof(char)*n);
		//data = (char*)tmp;
		//delete []tmp;
		m_CurrentIndex += sizeof(char)*n;
		m_FileCurrentIndex += sizeof(char)*n;
	}
	else
	{
		//char* tmp = new char[sizeof(char)*n];
		fp->read(data, sizeof(char)*n);
		//data = (char*)tmp;
		//delete []tmp;
		m_FileCurrentIndex += sizeof(char)*n;
	}
	
	return true;
}

//-----------------------------------------------------------------------------

void BInputOutputFile::flush()
{
	if(!m_writeAllData)
	{
		if(!m_openForReading)
			fp->write(m_Data, m_CurrentIndex);
		m_CurrentIndex = 0;
	}
	else
	{
		if(!m_openForReading)
			fp->write(m_Data, m_maxSize);
		
		m_FileCurrentIndex = m_FileCurrentIndex + (m_maxSize - m_CurrentIndex);
		m_CurrentIndex = 0;
	}
}

//-----------------------------------------------------------------------------

void BInputOutputFile::setMarker(__int64& marker)
{
	marker = fp->tellp().seekpos();
	marker += sizeof(char)*m_CurrentIndex;
}

//-----------------------------------------------------------------------------

void BInputOutputFile::savePositionToMarker(__int64& marker)
{
	// Flush m_Data in file
	flush();

	// Get current file position
	__int64 curPos = fp->tellp().seekpos();

	// Bring fp to marker location
	if (!fp->good())
		fp->clear();

	fp->seekp(marker, std::ios::beg);

	char* tmpData = new char[sizeof(__int64)];
	*(__int64*)(tmpData) = curPos;

	// Write curPos at marker location
	fp->write(tmpData , sizeof(__int64));

	delete []tmpData;

	// Bring fp back to current position
	if (!fp->good())
		fp->clear();

	fp->seekp(curPos, std::ios::beg);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::computeFileSize()
{
	m_fileSize = 0;
	if(!m_openForReading)
		return;

	// Bring the fp to the end of the file
	if (!fp->good())
		fp->clear();

	fp->seekp(0, std::ios::end);

	// Get the current position of the fp
	m_fileSize = fp->tellp().seekpos();

	// Bring the fp to the start of the file
	if (!fp->good())
		fp->clear();

	fp->seekp(0, std::ios::beg);
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::IsStoring()
{
	bool isStoring = false;
	if(!m_openForReading)
		isStoring = true;
	return isStoring;
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarker()
{
	//To ignore reading of specific entity we need to know how much byte size we need to jump in the file.
	//we implemented this code (write marker here) to have a facility to ignore reading of specific feature during reading of entire model. 
	//But this consumes 90% of MDL writing time. So we turned off this while solving Issue#13293: Optimization of MDL writer: 
	//setMarker(m_marker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SaveSizeToMarker()
{
	// To ignore reading of specific entity we need to know how much byte size we need to jump in the file.
	//we implemented this code to have a facility to ignore reading of specific feature during reading of entire model. 
	//But this consumes 90% of MDL writing time. So we turned off this while solving Issue#13293: Optimization of MDL writer: 
	return;
	/*
	// flush m_data if any in the file
	flush();

	// Get the current file position
	std::streampos tmp = fp->tellp();

	// Bring the fp to the marker location
	fp->seekp(m_marker, std::ios::beg);

	// compute the size to store at the marker location
	__int64 size = tmp.seekpos() - m_marker - sizeof(__int64);
	char* tmpData = new char[sizeof(__int64)];
	*(__int64*)(tmpData) = size;

	// write the size to marker location
	fp->write(tmpData , sizeof(__int64));

	// Bring fp back to current position
	fp->seekp(tmp.seekpos(), std::ios::beg);
	
	m_marker = -1;
	delete []tmpData;
	*/
}

//-----------------------------------------------------------------------------

bool BInputOutputFile::IsEndReached()
{
	bool endReached = false;

	return endReached;
}

//-----------------------------------------------------------------------------

void BInputOutputFile::ResetCurrentIndex()
{
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetPositionToStart()
{
	// flush m_data if any in file
	flush();

	// Bring the fp to the start of the file
	if (!fp->good())
		fp->clear();

	fp->seekp(0, std::ios::beg);

	m_CurrentIndex = 0;
	m_FileCurrentIndex = 0;
	m_firstTime = true;
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForBRep()
{
	setMarker(m_BRepPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForWireFrame()
{
	setMarker(m_WireFramePosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToBRepMarker()
{
	savePositionToMarker(m_BRepPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToWireFrameMarker()
{
	savePositionToMarker(m_WireFramePosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForFileSize()
{
	setMarker(m_fileSizeMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SaveFileSizeToFileSizeMarker()
{
	savePositionToMarker(m_fileSizeMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForFeature()
{
	setMarker(m_featurePosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForMeasurement()
{
	setMarker(m_measurementPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForLayer()
{
	setMarker(m_layerPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToFeatureMarker()
{
	savePositionToMarker(m_featurePosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToMeasurementMarker()
{
	savePositionToMarker(m_measurementPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToLayerMarker()
{
	savePositionToMarker(m_layerPosMarker);
}

//-----------------------------------------------------------------------------


void BInputOutputFile::SeekPosition(__int64 pos)
{
	if(m_FileCurrentIndex == pos)
		return;

	// flush m_data if any in file
	flush();

	// Bring the fp to the pos location
	if (!fp->good())
		fp->clear();

	fp->seekp(pos, std::ios::beg);

	m_CurrentIndex = 0;		//pos;
	m_FileCurrentIndex = fp->tellp();

	assert(m_FileCurrentIndex==pos);

	m_FileCurrentIndex = pos;
	m_firstTime = true;
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForSketch()
{
	setMarker(m_sketchPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForHistoryFeature()
{
	setMarker(m_historyFeaturePosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForLayerAndColour()
{
	setMarker(m_layerColourPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForCoordinateSystem()
{
	setMarker(m_coordinateSystemMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToSketchMarker()
{
	savePositionToMarker(m_sketchPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToHistoryFeatureMarker()
{
	savePositionToMarker(m_historyFeaturePosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToLayerAndColourMarker()
{
	savePositionToMarker(m_layerColourPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToCoordinateSystemMarker()
{
	savePositionToMarker(m_coordinateSystemMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForPoint()
{
	setMarker(m_pointPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMarkerForPointCloud()
{
	setMarker(m_pointCloudPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToPointMarker()
{
	savePositionToMarker(m_pointPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SavePositionToPointCloudMarker()
{
	savePositionToMarker(m_pointCloudPosMarker);
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetMaxSize()
{
	__int64 filePos = GetFileSize();
	if(filePos < m_maxSize)
		m_maxSize = (int)filePos;
}

//-----------------------------------------------------------------------------

void BInputOutputFile::Flush()
{
	if(!m_openForReading)
		fp->write(m_Data, m_CurrentIndex);

	m_CurrentIndex = 0;
}

//-----------------------------------------------------------------------------

void BInputOutputFile::SetFileCurrentPosition(__int64 fileCurrentIndex, bool readMode)
{
	m_FileCurrentIndex = fileCurrentIndex;

	if (!fp->good())
		fp->clear();

	if(readMode)
		fp->seekg(fileCurrentIndex, std::ios::beg);
	else
		fp->seekp(fileCurrentIndex, std::ios::beg);
		
	m_firstTime = true;
	m_CurrentIndex = 0;
}

//-----------------------------------------------------------------------------

void BInputOutputFile::Close()
{
	if(fp)
	{
		flush();

		fp->close();
		delete fp;
		fp = 0;
		delete []m_Data;
		m_Data = 0;
		m_fileClosed = true;
	}
}

//-----------------------------------------------------------------------------

}