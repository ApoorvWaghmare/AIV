#ifndef _INPUT_OUTPUT_FILE_H_
#define _INPUT_OUTPUT_FILE_H_



#include <fstream>

#include "CUBinaryIOInterface.h"

namespace CommonUtil
{

// This class is responsible to serialize/deserialize data using file
#pragma pack(push)
#pragma pack(1)
class BInputOutputFile : public BinaryIOInterface
{
	bool m_openForReading;

	std::fstream* fp;

	char* m_Data;

	__int64 m_fileSize;
	bool m_fileClosed;

	bool m_writeAllData;
	// Markers used to store the position of data sections in file
	 __int64 m_featurePosMarker;
	 __int64 m_measurementPosMarker;
	 __int64 m_layerPosMarker;
	 __int64 m_fileSizeMarker;
	__int64 m_BRepPosMarker;
	__int64 m_WireFramePosMarker;
	__int64 m_sketchPosMarker;
	__int64 m_pointPosMarker;
	__int64 m_pointCloudPosMarker;
	__int64 m_historyFeaturePosMarker;
	__int64 m_layerColourPosMarker;
	__int64 m_marker;
	__int64 m_coordinateSystemMarker;

	// this value is used for buffer
	// its value is always in between 0 to 1024
	int m_CurrentIndex;
	__int64 m_FileCurrentIndex;
	bool m_firstTime;

	// this size is used to allocate buffer size
	int m_maxSize;

	// this flag is used to read size_t data type
	// For versions older than version 4, the size_t data type was written as int
	bool m_versionBelow4;

	bool growData(int sizeToAdd);

	bool readData(int sizeToRead);

	void flush();

	void setMarker(__int64& marker);

	void savePositionToMarker(__int64& marker);

	void computeFileSize();

public:

	BInputOutputFile(const wchar_t* path, bool openForReading);

	BInputOutputFile(){}

	~BInputOutputFile();

	// write interfaces
	BinaryIOInterface& operator << (char c);

	BinaryIOInterface& operator << (int i);

	BinaryIOInterface& operator << (size_t i);

	BinaryIOInterface& operator << (float f);

	BinaryIOInterface& operator << (double d);

	BinaryIOInterface& operator << (const std::string& s);

	BinaryIOInterface& operator << (bool b);

	BinaryIOInterface& operator << (long l);

	BinaryIOInterface& operator << (__int64 pos);

	BinaryIOInterface& operator << (unsigned char pos);

	BinaryIOInterface& operator << (unsigned short pos) ;

	bool WriteIntArray(int n, int* data);
	bool WriteSizetArray(int n, size_t* data);
	bool WriteFloatArray(int n, float* data);
	bool WriteDoubleArray(int n, double* data);
	bool WriteCharArray(int n, char* data);

	// Read interfaces
	BinaryIOInterface& operator >> (char& c);

	BinaryIOInterface& operator >> (int& i);

	BinaryIOInterface& operator >> (size_t& i);

	BinaryIOInterface& operator >> (float& f);

	BinaryIOInterface& operator >> (double& d);

	BinaryIOInterface& operator >> (std::string& s);

	BinaryIOInterface& operator >> (bool &b);

	BinaryIOInterface& operator >> (long &l);

	BinaryIOInterface& operator >> (__int64& pos);

	BinaryIOInterface& operator >> (unsigned char& pos);

	BinaryIOInterface& operator >> (unsigned short& pos);

	bool ReadIntArray(int& n, int*& data);
	bool ReadSizetArray(int n, size_t*& data);
	bool ReadFloatArray(int& n, float*& data);
	bool ReadDoubleArray(int& n, double*& data);
	bool ReadCharArray(int n, char*& data);

	bool IsStoring();

	void SetMarker();
	void SaveSizeToMarker();
	bool IsEndReached();

	void ResetCurrentIndex();

	void SetPositionToStart();
	
	void SetMarkerForBRep();
	void SetMarkerForWireFrame();
	void SetMarkerForPoint();
	void SetMarkerForPointCloud();
	void SavePositionToBRepMarker();
	void SavePositionToWireFrameMarker();
	void SavePositionToPointMarker();
	void SavePositionToPointCloudMarker();
	void SetMarkerForFileSize();
	void SaveFileSizeToFileSizeMarker();
	void SetMarkerForFeature();
	void SetMarkerForMeasurement();
	void SetMarkerForLayer();
	void SavePositionToFeatureMarker();
	void SavePositionToMeasurementMarker();
	void SavePositionToLayerMarker();
	
	__int64 GetFileSize(){ return m_fileSize; }
	void SeekPosition(__int64 pos);

	//Added for version V3
	void SetMarkerForSketch();
	void SetMarkerForHistoryFeature();
	void SetMarkerForLayerAndColour();
	void SavePositionToSketchMarker();
	void SavePositionToHistoryFeatureMarker();
	void SavePositionToLayerAndColourMarker();

	void SetMaxSize();

	__int64 GetFileCurrentPosition()
	{
		return m_FileCurrentIndex;
	}
		
	void SetFileCurrentPosition(__int64 fileCurrentIndex, bool readMode = true);

	void SetWriteAllData(bool writeAllData)
	{
		m_writeAllData = writeAllData;
	}

	void SetWriteMode(const wchar_t* path, bool write = true);

	void Flush();

	bool IsOpenForReading()
	{
		return m_openForReading;
	}

	void Close();

	bool IsFileClosed()
	{
		return m_fileClosed;
	}

	void SetFlagForVersionBelow4(bool flag){ m_versionBelow4 = flag; }

	void SetMarkerForCoordinateSystem();
	void SavePositionToCoordinateSystemMarker();
};
#pragma pack(pop)   /* restore original from stack */
}

#endif