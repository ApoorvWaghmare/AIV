#ifndef _BINARY_IO_BUFFER_H_
#define _BINARY_IO_BUFFER_H_

#include "CUBinaryIOInterface.h"

namespace CommonUtil
{

// Used for undo/redo recording purpose
#pragma pack(push)  /* push current to stack */
#pragma pack(1)     /* set to 1 byte boundary */
class BinaryIOBuffer : public BinaryIOInterface
{
	bool m_isOpen;

	char* m_Data;

	// following variables are used for undo/redo
	// so they are kept as integer 
	int m_MaxSize;
	int m_FilledSize;
	int m_CurrentIndex;
	int m_marker;

	int m_calculatedBuffer;

	bool m_calculateBufferFlag;

	void growData(int sizeToAdd);

	void copyAndReallocateData(int newSize);

public:

	BinaryIOBuffer(int bufferSize);

	BinaryIOBuffer(BinaryIOBuffer* copyFrom);

	BinaryIOBuffer(char* buffer, int size);


	~BinaryIOBuffer();

	BinaryIOInterface& operator << (char c);

	BinaryIOInterface& operator << (int i);

	//BinaryIOInterface& operator << (size_t i);

	BinaryIOInterface& operator << (float f);

	BinaryIOInterface& operator << (double d);

	BinaryIOInterface& operator << (const std::string& s);

	BinaryIOInterface& operator << (bool b);

	BinaryIOInterface& operator << (long l);

	BinaryIOInterface& operator << (__int64 pos);

	BinaryIOInterface& operator << (unsigned char pos);

	BinaryIOInterface& operator << (unsigned short pos);

	bool WriteIntArray(int n, int* data);
	bool WriteFloatArray(int n, float* data);
	bool WriteDoubleArray(int n, double* data);
	bool WriteCharArray(int n, char* data){return true;}

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
	bool ReadFloatArray(int& n, float*& data);
	bool ReadDoubleArray(int& n, double*& data);
	bool ReadCharArray(int n, char*& data){return true;}

	bool IsStoring();

	void SetMarker();
	void SaveSizeToMarker();
	bool IsEndReached();

	void DetachBuffer(char* &buffer, int &size);

	void ResetCurrentIndex();

	void SetMarkerForBRep();
	void SetMarkerForWireFrame();
	void SavePositionToBRepMarker();
	void SavePositionToWireFrameMarker();
	void SavePositionToPointMarker(){}
	void SavePositionToPointCloudMarker(){}
	void SetMarkerForFileSize();
	void SaveFileSizeToFileSizeMarker();

	__int64 GetFileSize();
	void SeekPosition(__int64 pos);
	int GetFilledSize()const;
	void ReleaseUnusedMemory();
	void SetBoolForMemoryCalculation(bool calculateMemory);
	int GetCalculatedBufferSize()const;

	void SetWriteAllData(bool writeAllData) {}

	//Added for version V3
	void SetMarkerForSketch()
	{}
	void SetMarkerForHistoryFeature()
	{}
	void SetMarkerForLayerAndColour()
	{}
	void SavePositionToSketchMarker()
	{}
	void SavePositionToHistoryFeatureMarker()
	{}
	void SavePositionToLayerAndColourMarker()
	{}

	//Added for version V7.1
	void SetMarkerForCoordinateSystem()
	{}
	void SavePositionToCoordinateSystemMarker()
	{}

	void SetMarkerForFeature()
	{}
	void SavePositionToFeatureMarker()
	{}
	void SavePositionToMeasurementMarker()
	{}
	void SavePositionToLayerMarker()
	{}
};
#pragma pack(pop)   /* restore original from stack */
}
#endif