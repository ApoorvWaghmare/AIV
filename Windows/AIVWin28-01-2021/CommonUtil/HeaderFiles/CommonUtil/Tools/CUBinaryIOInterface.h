#ifndef _INPUT_OUTPUT_INTERFACE_H_
#define _INPUT_OUTPUT_INTERFACE_H_

#include <vector>

// This is the interface class for serializing/deserializing data 
#pragma pack(push)
#pragma pack(1)
class BinaryIOInterface
{
public:
	virtual ~BinaryIOInterface() {}

	// Write interfaces
	virtual BinaryIOInterface& operator << (char c) = 0;
	virtual BinaryIOInterface& operator << (int i) = 0;
	//virtual BinaryIOInterface& operator << (size_t i) = 0;
	virtual BinaryIOInterface& operator << (float f) = 0;
	virtual BinaryIOInterface& operator << (double d) = 0;
	virtual BinaryIOInterface& operator << (const std::string& s) = 0;
	virtual BinaryIOInterface& operator << (bool b) = 0;	///
	virtual BinaryIOInterface& operator << (long l) = 0;	// For Issue#40692
	virtual BinaryIOInterface& operator << (__int64 pos) = 0;
	virtual BinaryIOInterface& operator << (unsigned char pos) = 0;
	virtual BinaryIOInterface& operator << (unsigned short pos) = 0;

	virtual bool WriteIntArray(int n, int* data) = 0;
	virtual bool WriteFloatArray(int n, float* data) = 0;
	virtual bool WriteDoubleArray(int n, double* data) = 0;
	virtual bool WriteCharArray(int n, char* data) = 0;


	//read interfaces
	virtual BinaryIOInterface& operator >> (std::string& s) = 0;
	virtual BinaryIOInterface& operator >> (char& c) = 0;
	virtual BinaryIOInterface& operator >> (int& i) = 0;
	virtual BinaryIOInterface& operator >> (size_t& i) = 0;
	virtual BinaryIOInterface& operator >> (float& f) = 0;
	virtual BinaryIOInterface& operator >> (double& d) = 0;
	virtual BinaryIOInterface& operator >> (bool& b) = 0;	
	virtual BinaryIOInterface& operator >> (long& l) = 0;
	virtual BinaryIOInterface& operator >> (__int64& pos) = 0;
	virtual BinaryIOInterface& operator >> (unsigned char& pos) = 0;
	virtual BinaryIOInterface& operator >> (unsigned short& pos) = 0;

	virtual bool ReadIntArray(int& n, int*& data) = 0;
	virtual bool ReadFloatArray(int& n, float*& data) = 0;
	virtual bool ReadDoubleArray(int& n, double*& data) = 0;
	virtual bool ReadCharArray(int n, char*& data) = 0;

	virtual void SetMarker() = 0;
	virtual void SaveSizeToMarker() = 0;
	virtual bool IsStoring() = 0;
	virtual bool IsEndReached() = 0;

	virtual void ResetCurrentIndex() = 0;

	virtual void SetMarkerForBRep() = 0;
	virtual void SetMarkerForWireFrame() = 0;
	virtual void SetMarkerForFileSize() = 0;
	virtual void SavePositionToBRepMarker() = 0;
	virtual void SavePositionToWireFrameMarker() = 0;
	virtual void SavePositionToPointMarker() = 0;
	virtual void SavePositionToPointCloudMarker() = 0;
	virtual void SaveFileSizeToFileSizeMarker() = 0;
	virtual void SetMarkerForFeature() = 0;
	virtual void SavePositionToFeatureMarker() = 0;
	virtual void SavePositionToMeasurementMarker() = 0;
	virtual void SavePositionToLayerMarker() = 0;
	virtual __int64 GetFileSize() = 0;
	virtual void SeekPosition(__int64 pos) = 0;

	virtual void SetWriteAllData(bool writeAllData) = 0;
	
	//Added for version V3
	virtual void SetMarkerForSketch() = 0;
	virtual void SetMarkerForHistoryFeature() = 0;
	virtual void SetMarkerForLayerAndColour() = 0;

	virtual void SavePositionToSketchMarker() = 0;
	virtual void SavePositionToHistoryFeatureMarker() = 0;
	virtual void SavePositionToLayerAndColourMarker() = 0;

	virtual void SetMarkerForCoordinateSystem() = 0;
	virtual void SavePositionToCoordinateSystemMarker() = 0;
};
#pragma pack(pop)   /* restore original from stack */
#endif 