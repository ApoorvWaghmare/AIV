// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// Std Include Files

#include <vector>
#include <assert.h>
#include <unordered_map>
#include "stdio.h"
#include "CUCommonConstants.h"
#include <Windows.h>
#include"CUPoint3DCache.h"
#include "CUOctreeCell.h"
#include "CUCacheInterface.h"
#include "CUFileHandler.h"
#include "CUCacheManager.h"


namespace CommonUtil
{
	void Point3DCache::writeDataToFile(Point3DCellData* cellData)
	{
		//write data to file
		//write number of points in cell 
		size_t numPointsInCell = (cellData->m_3dPoints.size())/3;
		fwrite(&numPointsInCell, sizeof(size_t), 1, m_fp);
		fflush(m_fp);

		if(numPointsInCell != 0)
		{
			const size_t BUFSZ = numPointsInCell*3;
			//Write points data 
			fwrite(&(cellData->m_3dPoints)[0],sizeof(float),BUFSZ,m_fp);
			fwrite(&(cellData->m_colors)[0],sizeof(unsigned char),BUFSZ,m_fp);
			fflush(m_fp);
		}

		//Write remaining data as junk data
		size_t JUNKBUFSZ = (m_maxNumPtsPerCell - numPointsInCell)*3;

		if(JUNKBUFSZ != 0)
		{
			float junkPointData[3];
			unsigned char junkColorData[3];
			fwrite(junkPointData,sizeof(float),JUNKBUFSZ,m_fp);
			fwrite(junkColorData,sizeof(unsigned char),JUNKBUFSZ,m_fp);
			fflush(m_fp);
		}
	}

//-----------------------------------------------------------------------------

	void Point3DCache::readDataFromFile(Point3DCellData* cellData)
	{
		//Read number of points from file
		size_t numPointsInCell = 0;
		fread(&numPointsInCell, sizeof(size_t), 1, m_fp);

		if(numPointsInCell != 0)
		{
			cellData->m_3dPoints.resize(numPointsInCell*3);
			cellData->m_colors.resize(numPointsInCell*3);
			const size_t BUFSZ = numPointsInCell*3;
			fread(&(cellData->m_3dPoints)[0], sizeof(float), BUFSZ, m_fp);
			fread(&(cellData->m_colors)[0], sizeof(unsigned char), BUFSZ, m_fp);
		}
	}

//-----------------------------------------------------------------------------

	void Point3DCache::setHavingData(size_t cellId, bool havingData)
	{
		m_cacheManager->SetHavingData(cellId,havingData);
	}

//-----------------------------------------------------------------------------

	Point3DCache::Point3DCache()
	{
		m_maxNumPtsPerCell = (int)CommonConstants::MAX_NUM_POINTS_IN_CELL;
		m_maxCellInMemory = (int)CommonConstants::MAX_NUM_CELLS_IN_MEMORY;
		m_numPoints = 0;

		//Not required for octree read write logic
		char temporaryPath[1024];
		GetTempPathA(1024, temporaryPath);
		UINT temp = 0;
		m_colorPresent = false;
		LPCSTR lpPrefixString = "P3DC";//appended to filename
		char *uniqueFileName = new char[1024];
		GetTempFileNameA(temporaryPath, lpPrefixString, temp, uniqueFileName);

		//char* uniqueFileName = "C:\\Temp\\PointCache.txt";
		m_binaryIOFile = new FileHandler(uniqueFileName);
		m_fp = m_binaryIOFile->GetFilePointer();
		m_cacheManager = new CacheManager<Point3DCellData>(m_maxCellInMemory,CommonConstants::MAX_DEPTH_LEVEL,m_binaryIOFile,this);
	}

//-----------------------------------------------------------------------------

	Point3DCache::~Point3DCache()
	{
		if(m_cacheManager)
		{
			delete m_cacheManager;
			m_cacheManager = 0;
		}
	}

//-----------------------------------------------------------------------------

	void Point3DCache::AddCell(size_t cellId)
	{
		Point3DCellData* cellData = getCellData(cellId);
	}

//-----------------------------------------------------------------------------

	void Point3DCache::AddPoint(size_t cellId, float point[3], unsigned char colorInfo[3])
	{
		setHavingData(cellId, true);
		Point3DCellData* cellData = getCellData(cellId);

		for(int i = 0; i < 3; ++i)
		{
			(cellData->m_3dPoints).push_back(point[i]);
			(cellData->m_colors).push_back(colorInfo[i]);
		}

		m_numPoints++;
	}

//-----------------------------------------------------------------------------

	void Point3DCache::GetCellPoints(size_t cellId, std::vector<float>& points, std::vector<unsigned char>& colorInfo)
	{
		Point3DCellData* cellData = getCellData(cellId);
		points = cellData->m_3dPoints;
		colorInfo = cellData->m_colors;
	}

//-----------------------------------------------------------------------------

	bool Point3DCache::IsHavingData(size_t cellId)const
	{
		return m_cacheManager->IsHavingData(cellId);
	}
//-----------------------------------------------------------------------------

	size_t Point3DCache::GetNumCellPoints(size_t cellId)
	{
		size_t numPoints = 0;

		Point3DCellData* cellData = getCellData(cellId);
		numPoints = cellData->m_3dPoints.size()/3;

		return numPoints;
	}

//-----------------------------------------------------------------------------

	void Point3DCache::ClearCellPoints(size_t cellId)
	{
		Point3DCellData* cellData = getCellData(cellId);
		m_numPoints -= (int)cellData->m_3dPoints.size()/3;

		cellData->m_3dPoints.clear();
		cellData->m_colors.clear();
	}

//-----------------------------------------------------------------------------

	Point3DCellData* Point3DCache::getCellData(size_t cellId)
	{
		Point3DCellData* cellData = nullptr;
		bool readData = false;
		int readOffset = -1;
		m_cacheManager->GetCellData(cellId,readData,readOffset,cellData);
		cellData->m_cellId = cellId;

		if(readData)
		{
			//set offset to write a cellData
			m_binaryIOFile->SetFilePtrPosInFile(readOffset);

			//read data tfrom file
			cellData->m_3dPoints.clear();
			cellData->m_colors.clear();
			readDataFromFile(cellData);
		}

		return cellData;
	}

//-----------------------------------------------------------------------------

	void Point3DCache::Flush()
	{
		std::vector<size_t> allCellId;
		m_cacheManager->GetAllCellIdInMemory(allCellId);

		for(size_t cellIdCount = 0; cellIdCount < allCellId.size(); ++cellIdCount)
		{
			Point3DCellData* cellData = nullptr;
			bool readData = false;
			int readOffset = -1;
			m_cacheManager->GetCellData(allCellId[cellIdCount],readData,readOffset,cellData,false);
			cellData->m_cellId = allCellId[cellIdCount];
		}
	}

//-----------------------------------------------------------------------------

	bool Point3DCache::DistributeCellDataAmongChildren(const OctreeCell* octreeCell, size_t cellID, int depth, std::vector<size_t>& childIds, size_t numCellPoints)
	{
		std::vector<float> points;
		std::vector<unsigned char> colors;
		GetCellPoints(cellID, points, colors);
		ClearCellPoints(cellID);
	
		m_cacheManager->Divide(cellID, depth, childIds);

		int id = 0;
		size_t numPoints = points.size()/3;
		for(size_t jCount = 0; jCount < numPoints; ++jCount)
		{
			id = 0;
			id = octreeCell->GetChildIdForPoint(points[3*jCount], points[3*jCount+1], points[3*jCount+2]);	
			float *tmpPoint = &points[3*jCount];
			unsigned char *color =&colors[3*jCount];
			AddPoint(octreeCell->GetChild(id)->GetUniqueId(), tmpPoint, color);
		}
		return true;
	}

//-----------------------------------------------------------------------------

	void Point3DCache::AddLeafCell(int depth, size_t cellId)
	{
		m_cacheManager->AddNewCellInfo(depth,cellId);
		AddCell(cellId);
	}

//-----------------------------------------------------------------------------

	void Point3DCache::WriteDataIntoFile(size_t index)
	{
		Point3DCellData* cellData = nullptr;

		bool readData = false;
		int readOffset = -1;

		cellData = m_cacheManager->GetCellInMemory(index);

		writeDataToFile(cellData);

		//clear celldata
		cellData->m_3dPoints.clear();
		cellData->m_colors.clear();
	}

//-----------------------------------------------------------------------------

}
