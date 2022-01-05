// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _POINT3D_CACHE_H_
#define _POINT3D_CACHE_H_

// Std Include Files

#include <vector>
#include <assert.h>
#include <unordered_map>
#include "stdio.h"
#include "CUCPPoint3D.h"

namespace CommonUtil
{
class Point3DCellData
{
public:
	std::vector<float> m_3dPoints;
	std::vector<unsigned char> m_colors;
	size_t m_cellId;

	bool IsEmpty()const
	{
		return true;
	}
};
}

#include "CUCacheInterface.h"
#include "CUFileHandler.h"
#include "CUCacheManager.h"
#include "CUOctreeCell.h"
#pragma warning(disable:4996)
#pragma once 
namespace CommonUtil
{

class FileHandler;

class Point3DCache : public CacheInterface
{
	FileHandler* m_binaryIOFile;
	CacheManager<Point3DCellData>* m_cacheManager;

	FILE *m_fp;

	int m_maxNumPtsPerCell;
	int m_maxCellInMemory;
	int m_numPoints;
	bool m_colorPresent;
	void writeDataToFile(Point3DCellData* cellData);

	void readDataFromFile(Point3DCellData* cellData);
	
	void setHavingData(size_t cellId, bool havingData);
	
public:

	Point3DCache();
	
	~Point3DCache();
	
	size_t GetMaxNumPtsPerCell()const{ return m_maxNumPtsPerCell; }

	int GetNumPoints()const{return m_numPoints;}

	void AddCell(size_t cellId);

	void AddPoint(size_t cellId, float point[3], unsigned char colorInfo[3]);
	
	void GetCellPoints(size_t cellId, std::vector<float>& points, std::vector<unsigned char>& colorInfo);
	
	bool IsHavingData(size_t cellId)const;

	size_t GetNumCellPoints(size_t cellId);

	void ClearCellPoints(size_t cellId);
	
	Point3DCellData* getCellData(size_t cellId);
	
	bool LoadCellForEntityId(size_t entityId){return true;}

	void Flush();
	
	bool DistributeCellDataAmongChildren(const OctreeCell* octreeCell, size_t cellID, int depth, std::vector<size_t>& childIds, size_t numCellPoints);

	void GetDisplayMeshData(size_t cellId, std::vector<float>& vertices, std::vector<float>& normals){}

	void AddLeafCell(int depth, size_t cellId);

	void WriteDataIntoFile(size_t index);

	void SetFile(BInputOutputFile *file){}

	void SetHavingColor(bool color)
	{
		m_colorPresent = color;
	}

	bool IsHavingColor()const
	{
		return m_colorPresent;
	}
	void GetDisplayPointData(size_t cellId, std::vector<float>& points, std::vector<size_t >& pointIndices, 
						std::vector<unsigned char>& colorInfo, std::vector<unsigned char>& flags,
						bool partialData = false){}

	void GetDisplayPointData(size_t cellId, std::vector<const CommonUtil::CPPoint3D* >& points){}

	const std::vector<size_t >& GetCellPointsIndices(size_t cellId)const
	{ 
		std::vector<size_t > test;
		return test;
	}

	const CommonUtil::CPPoint3D& GetCpPoint3DFromPtIndex(size_t ptIndexInArray)const
	{
		CommonUtil::CPPoint3D cp;
		return cp;
	}

	const CommonUtil::CPPoint3D& GetCpPoint3DFromCellBasedPtIndex(size_t bitIndex)
	{
		CommonUtil::CPPoint3D cp;
		return cp;
	}

	virtual bool SetSelectionToCellData(size_t index, bool selected, bool invertSelection, 
										bool pointId, std::vector<size_t>& selPointIndices, 
										bool applyOnlyHiddenPt){return false;}

	virtual void SetVisibilityToCellData(size_t index, bool visible, bool pointId){}

	bool WriteCache(size_t &fileSize, size_t &cacheMangrfileSize)const{return true;}

	void GetPointsInCell(size_t cellId, std::vector<float>& points)const{}

	void UpdateProperty(size_t numPoints, double ext[6]){}

	size_t GetCPPoint3DIndexInArray(size_t pointIndex){return 0;}

	size_t GetCPPointIndex(size_t indexInArray){return 0;}
	
	void GetDisplayMeshData(std::vector<float>& vertices, std::vector<unsigned int > &facetVerticesIndex, std::vector<float> &normals){};

	void GetDisplayMeshData(std::vector<float>& vertices, std::vector<unsigned int > &facetVerticesIndex, std::vector<float> &normals, std::vector<unsigned char> &colors){};
};
}

#endif