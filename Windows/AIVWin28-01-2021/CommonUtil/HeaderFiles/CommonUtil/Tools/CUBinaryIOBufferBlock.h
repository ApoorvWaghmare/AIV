// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================
#ifndef _BINARY_INPUT_OUTPUT_BUFFER_H_
#define _BINARY_INPUT_OUTPUT_BUFFER_H_

#include "CUBinaryIOInterface.h"
namespace CommonUtil
{
class FileHandler;
#pragma pack(push)
#pragma pack(1)
class BinaryIOBufferBlock: BinaryIOInterface 
{
	bool m_nextBuffer;
	size_t m_CurrentIndex;
	int m_maxSize;
	char* m_data;
	
	FileHandler* m_binaryIOFile;

	//write data to buffer
	void writeDataToBuffer(size_t size, char* data);

	//read data from buffer
	void readDatafromBuffer(size_t size, char* &data);
	
	size_t getEmptySpaceCountInBuffer();

public:

	BinaryIOBufferBlock(int maxsize, FileHandler* m_binaryIOFile);
	~BinaryIOBufferBlock();

	BinaryIOInterface& operator << (int i);
	BinaryIOInterface& operator << (size_t i);
	BinaryIOInterface& operator << (float i);
	BinaryIOInterface& operator << (double i);
	BinaryIOInterface& operator << (unsigned char i);

	BinaryIOInterface& operator << (char c){return *this;}
	BinaryIOInterface& operator << (const std::string& s){return *this;}
	BinaryIOInterface& operator << (bool b) {return *this;}	///
	BinaryIOInterface& operator << (long l) {return *this;}	// For Issue#40692
	BinaryIOInterface& operator << (__int64 pos){return *this;}
	BinaryIOInterface& operator << (unsigned short pos) {return *this;}

	bool WriteFloatArray(int n, float* data);
	bool WriteIntArray(int n, int* data) {return true;}
	bool WriteDoubleArray(int n, double* data){return true;}
	bool WriteCharArray(int n, char* data){return true;}

	BinaryIOInterface& operator >> (int& i);
	BinaryIOInterface& operator >> (size_t& i);
	BinaryIOInterface& operator >> (float& i);
	BinaryIOInterface& operator >> (double& i);
	BinaryIOInterface& operator >> (unsigned char& i);
	BinaryIOInterface& operator >> (std::string& s){return *this;}
	BinaryIOInterface& operator >> (char& c) {return *this;}
	BinaryIOInterface& operator >> (bool& b){return *this;}
	BinaryIOInterface& operator >> (long& l){return *this;}
	BinaryIOInterface& operator >> (__int64& pos) {return *this;}
	BinaryIOInterface& operator >> (unsigned short& pos){return *this;}

	bool ReadFloatArray(int &n, float* &data);
	bool ReadIntArray(int& n, int*& data){return true;}
	bool ReadDoubleArray(int& n, double*& data) {return true;}
	bool ReadCharArray(int n, char*& data){return true;}

	void ReleaseBuffer();
	void GetBuffer();

	void ClearBuffer();

	bool IsNextBuffer()const{return m_nextBuffer;}

	 void SetMarker() {}
	 void SaveSizeToMarker() {}
	 bool IsStoring() {return true;}
	 bool IsEndReached() {return true;}
	
	 void ResetCurrentIndex() {}
	
	 void SetMarkerForFileSize() {}
	 void SaveFileSizeToFileSizeMarker() {}
	 void SetMarkerForFeature(){}
	 void SavePositionToFeatureMarker() {}
	 void SavePositionToMeasurementMarker(){}
	  void SavePositionToLayerMarker(){}
	 __int64 GetFileSize(){return 0;}
	 void SeekPosition(__int64 pos) {}

	 void SetWriteAllData(bool writeAllData){}

	 void SetMarkerForBRep()
	 {}
	 void SetMarkerForWireFrame()
	 {}
	 void SavePositionToBRepMarker()
	 {}
	 void SavePositionToWireFrameMarker()
	 {}
	 void SavePositionToPointMarker()
	 {}
	 void SavePositionToPointCloudMarker()
	 {}

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
};
#pragma pack(pop)   /* restore original from stack */
}
#endif