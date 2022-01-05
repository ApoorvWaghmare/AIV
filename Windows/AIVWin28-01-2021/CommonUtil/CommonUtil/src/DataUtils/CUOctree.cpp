// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#pragma warning (disable: 4786)

#include <algorithm>

// Include Files
//#include "pointcloud.h"
//#include "Mesh.h"

#include "CUOctree.h"
#include "CUOctreeCell.h"
#include "CUBinaryIOBufferBlock.h"
#include "CUFileHandler.h"
#include "CUBinaryIOInterface.h"
#include "CUBInputOutputFile.h"
#include "CUBoundingBox3D.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

Octree::Octree(char* fileName)
{
	m_root = 0;

	Reset();
	m_fileName = fileName;
}

//-----------------------------------------------------------------------------

Octree::Octree()
{
	m_root = 0;

	Reset();
}

//-----------------------------------------------------------------------------

Octree::Octree(double minX, double minY, double minZ,
			   double maxX, double maxY, double maxZ)
{
	Reset();

	m_root = new OctreeCell((float)minX, (float)minY, (float)minZ, (float)maxX, (float)maxY, (float)maxZ);
	m_root->setUniqueId(0);	

	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;
}

//-----------------------------------------------------------------------------

Octree::~Octree()
{	
	delete m_root;
	m_root = 0;

	Reset();
}

//-----------------------------------------------------------------------------

void Octree::SetExtents(double minX, double minY, double minZ,
					double maxX, double maxY, double maxZ)
{

	Reset();

	if(!m_root)
	{
		m_root = new OctreeCell((float)minX, (float)minY, (float)minZ, (float)maxX, (float)maxY, (float)maxZ);
		m_root->setUniqueId(0);
	}

	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;

	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;
}

//-----------------------------------------------------------------------------

void Octree::Reset()
{
	m_minX = m_minY = m_minZ = DBL_MAX;
	m_maxX = m_maxY = m_maxZ = -DBL_MAX;	

	m_maxSpan = 0;
}

//-----------------------------------------------------------------------------

void Octree::Clear()
{
	Reset();

	if(m_root)
	{
		delete m_root;
		m_root = 0;
	}
}

//-----------------------------------------------------------------------------

bool Octree::GetSameLevelFaceNeighbourGlobalId(const std::vector<int> &globalId, DirectionForOctree dir,
											   std::vector<int> &neighbourGlobalId, int &count)const
{
	int numIds = (int)globalId.size();
	neighbourGlobalId.resize(numIds);

	count = 1;
	int iCount = numIds;

	while(iCount > 0)
	{
		neighbourGlobalId[iCount-1] = getNeighbourId(globalId[iCount-1],dir);
		DirectionForOctree tempDir = ActionTable[globalId[iCount-1]][dir];
		iCount = iCount-1;		

		if(tempDir == ONE_X_X)		
		{			
			count = count + 1;
		}
		else if(tempDir == ZERO_X_X)		
		{
			for(int jCount = iCount; jCount > 0; --jCount)
			{
				neighbourGlobalId[jCount-1] = globalId[jCount-1];
			}
			return true;
		}
	}
	neighbourGlobalId.clear();
	return false;
}

//-----------------------------------------------------------------------------

bool Octree::getSameLevelEdgeNeighbourGlobalId(const std::vector<int> &globalId, DirectionForOctree dir, 
											  std::vector<int> &neighbourGlobalId, int &lCount)const
{
	lCount = 1;
	int count = 1;

	int numOfIds = (int)globalId.size();
	neighbourGlobalId.resize(numOfIds);

	int iCount = numOfIds;
	while(iCount > 0)
	{
		neighbourGlobalId[iCount-1] = getNeighbourId(globalId[iCount-1],dir);
		DirectionForOctree direction = getDirectionOfNeighbour(globalId[iCount-1],dir);

		if(direction == Same_Parent)
		{
			iCount = iCount-1;
			for(int jCount = iCount; jCount>0; --jCount)
			{
				neighbourGlobalId[jCount-1] = globalId[jCount-1];
			}
			return true;
		}
		else if(ZERO_X_X <= direction && direction <= X_X_ONE)
		{
			iCount = iCount-1;

			std::vector<int>tempGlobalId;
			std::vector<int>tempNeighbourGlobalId;

			tempGlobalId.reserve(iCount);
			for(int jCount = 0; jCount < iCount; ++jCount)
			{
				tempGlobalId.push_back(globalId[jCount]);
			}

			bool found = GetSameLevelFaceNeighbourGlobalId(tempGlobalId,direction,tempNeighbourGlobalId,count);
			if(found)
			{
				lCount = lCount + count;
				for(int jCount = 0; jCount < iCount; ++jCount)
				{
					neighbourGlobalId[jCount] = tempNeighbourGlobalId[jCount];
				}
				return true;
			}
			else
				return false;
		}
		else
			lCount = lCount + 1;

		iCount = iCount-1;
	}
	neighbourGlobalId.clear();
	return false;
}

//-----------------------------------------------------------------------------

bool Octree::getSameLevelVertexNeighbourGlobalId (const std::vector<int> &globalId, DirectionForOctree dir, 
												  std::vector<int> &neighbourGlobalId, int&lCount)const
{
	lCount = 1;
	int count = 1;

	int numOfIds = (int)globalId.size();
	neighbourGlobalId.resize(numOfIds);

	int iCount = numOfIds;
	while(iCount > 0)
	{
		neighbourGlobalId[iCount-1] = getNeighbourId(globalId[iCount-1],dir);
		DirectionForOctree direction = getDirectionOfNeighbour(globalId[iCount-1],dir);
		if(direction == Same_Parent)
		{
			iCount = iCount-1;
			for(int jCount = iCount; jCount>0; --jCount)
			{
				neighbourGlobalId[jCount-1] = globalId[jCount-1];
			}
			return true;
		}
		else if(X_ZERO_ZERO <= direction && direction <= ONE_ONE_X)
		{
			iCount = iCount-1;

			std::vector<int>tempGlobalId;
			std::vector<int>tempNeighbourGlobalId;

			tempGlobalId.reserve(iCount);
			for(int jCount = 0; jCount < iCount; ++jCount)
			{
				tempGlobalId.push_back(globalId[jCount]);
			}

			bool found = getSameLevelEdgeNeighbourGlobalId(tempGlobalId,direction,tempNeighbourGlobalId,count);
			if(found)
			{
				lCount = lCount + count;
				for(int jCount = 0; jCount < iCount; ++jCount)
				{
					neighbourGlobalId[jCount] = tempNeighbourGlobalId[jCount];
				}
				return true;
			}
			else
				return false;
		}
		else if(ZERO_X_X <= direction && direction <= X_X_ONE)
		{
			iCount = iCount-1;

			std::vector<int>tempGlobalId;
			std::vector<int>tempNeighbourGlobalId;

			tempGlobalId.reserve(iCount);
			for(int jCount = 0; jCount < iCount; ++jCount)
			{
				tempGlobalId.push_back(globalId[jCount]);
			}

			bool found = GetSameLevelFaceNeighbourGlobalId(tempGlobalId,direction,tempNeighbourGlobalId,count);
			if(found)
			{
				lCount = lCount + count;
				for(int jCount = 0; jCount < iCount; ++jCount)
				{
					neighbourGlobalId[jCount] = tempNeighbourGlobalId[jCount];
				}
				return true;
			}
			else
				return false;
		}
		else
			lCount = lCount + 1;

		iCount = iCount-1;
	}
	return false;
}

//-----------------------------------------------------------------------------

void Octree::GetNeighbourCells(const OctreeCell* cell, const std::vector<int> &cellGlobalId,
							   const std::vector <int> &neighbourGlobalId,
							   DirectionForOctree dir, int count, 
							   std::vector<OctreeCell*> &neighbourCells)const
{	
	OctreeCell *neighbourCell = (OctreeCell*)cell;
	int numOfIds = (int)cellGlobalId.size();
	
	if(count > (numOfIds/2))
	{
		neighbourCell = m_root;
		for(int iCount = 1; iCount <= numOfIds; ++iCount)
		{
			if(neighbourCell->GetChild(neighbourGlobalId[iCount-1]) == NULL )
				break;
			neighbourCell = neighbourCell->GetChild(neighbourGlobalId[iCount-1]);
		}
	}
	else
	{
		std::vector<int> globalId;
		neighbourCell->GetGlobalId(globalId);
		neighbourCell = m_root;
		for(size_t i = 0; i < (globalId.size()-count); ++i)
			neighbourCell = neighbourCell->GetChild(globalId[i]);

		for(int iCount = numOfIds-(count-1); iCount <= numOfIds ;++iCount)
		{
			if(neighbourCell->IsLeafCell())					
				break;					
			neighbourCell = neighbourCell->GetChild(neighbourGlobalId[iCount-1]);
		}
	}

	if(neighbourCell->IsLeafCell())
	{
		neighbourCells.reserve(1);
		neighbourCells.push_back(neighbourCell);
	}
	else
	{	
		DirectionForOctree direction = getOppositeDirection(dir);
		GetLeafCellsAlongDirection(neighbourCell, direction, neighbourCells);		
	}	
}

//-----------------------------------------------------------------------------

void Octree::getChildrenIdsAlongDirection(DirectionForOctree dir, std::vector<int> &id)const
{
	id.reserve(4);
	if(dir == ZERO_X_X) //front Direction
	{
		id.push_back(0);
		id.push_back(1);
		id.push_back(2);
		id.push_back(3);
	}
	else if(dir == ONE_X_X) //back Direction
	{
		id.push_back(4);
		id.push_back(5);
		id.push_back(6);
		id.push_back(7);
	}
	else if(dir == X_ZERO_X) //up Direction
	{
		id.push_back(0);
		id.push_back(1);
		id.push_back(4);
		id.push_back(5);		
	}
	else if(dir == X_ONE_X) //down Direction
	{
		id.push_back(2);
		id.push_back(3);
		id.push_back(6);
		id.push_back(7);		
	}
	else if(dir == X_X_ZERO) //left Direction
	{
		id.push_back(0);
		id.push_back(2);
		id.push_back(4);
		id.push_back(6);		
	}
	else if(dir == X_X_ONE) //right Direction
	{
		id.push_back(1);
		id.push_back(3);
		id.push_back(5);
		id.push_back(7);		
	}
	else if(dir == X_ZERO_ZERO)
	{
		id.push_back(0);
		id.push_back(4);
	}
	else if(dir == X_ZERO_ONE)
	{
		id.push_back(1);
		id.push_back(5);		
	}
	else if(dir == X_ONE_ZERO)
	{
		id.push_back(2);
		id.push_back(6);		
	}
	else if(dir == X_ONE_ONE)
	{
		id.push_back(3);
		id.push_back(7);		
	}
	else if(dir == ZERO_X_ZERO)
	{
		id.push_back(0);
		id.push_back(2);		
	}
	else if(dir == ZERO_X_ONE)
	{
		id.push_back(1);
		id.push_back(3);		
	}
	else if(dir == ONE_X_ZERO)
	{
		id.push_back(4);
		id.push_back(6);
	}
	else if(dir == ONE_X_ONE)
	{
		id.push_back(5);
		id.push_back(7);		
	}
	else if(dir == ZERO_ZERO_X)
	{
		id.push_back(0);
		id.push_back(1);		
	}
	else if(dir == ZERO_ONE_X)
	{
		id.push_back(2);
		id.push_back(3);
	}
	else if(dir == ONE_ZERO_X)
	{
		id.push_back(4);
		id.push_back(5);
	}
	else if(dir == ONE_ONE_X)
	{
		id.push_back(6);
		id.push_back(7);		
	}
	if(dir == ZERO_ZERO_ZERO)
	{
		id.push_back(0);		
	}
	else if(dir == ZERO_ZERO_ONE)
	{
		id.push_back(1);
	}
	else if(dir == ZERO_ONE_ZERO)
	{
		id.push_back(2);
	}
	else if(dir == ZERO_ONE_ONE)
	{
		id.push_back(3);
	}
	else if(dir == ONE_ZERO_ZERO)
	{
		id.push_back(4);	
	}
	else if(dir == ONE_ZERO_ONE)
	{
		id.push_back(5);	
	}
	else if(dir == ONE_ONE_ZERO)
	{
		id.push_back(6);
	}
	else if(dir == ONE_ONE_ONE)
	{
		id.push_back(7);
	}
}

//-----------------------------------------------------------------------------

//void Octree::computeExtents(const PointCloud* pointCloud, const Mesh* mesh)
//{
//	bool considerPC = false, considerMesh = false;
//	if(pointCloud && !mesh)
//		considerPC = true;
//	else if(!pointCloud && mesh)
//		considerMesh = true;
//	else if(pointCloud && mesh)
//	{
//		considerPC = true;
//		considerMesh = true;
//
//		if(mesh->GetPointCloud() == pointCloud)
//		{
//			if(mesh->GetNumVertices() > pointCloud->GetNumTriCPPoints())
//				considerPC = false;
//			else
//				considerMesh = false;
//		}
//	}
//	if(considerPC)	
//		pointCloud->GetExtents(m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ);	
//
//	if(considerMesh)
//	{
//		double minX, maxX, minY, maxY, minZ, maxZ;
//		minX = minY = minZ = 0;
//		maxX = maxY = maxZ = 0;
//		mesh->GetExtents(minX, maxX, minY, maxY, minZ, maxZ);
//
//		if(minX < m_minX)
//			m_minX = minX;
//		if(minY < m_minY)
//			m_minY = minY;
//		if(minZ < m_minZ)
//			m_minZ = minZ;
//
//		if(maxX > m_maxX)
//			m_maxX = maxX;
//		if(maxY > m_maxY)
//			m_maxY = maxY;
//		if(maxZ > m_maxZ)
//			m_maxZ = maxZ;
//	}
//}

//-----------------------------------------------------------------------------

void Octree::removeDuplicateCellsInArray(std::vector<OctreeCell*>& cells)const
{	
	std::vector<OctreeCell*> tempVector;
	tempVector.reserve(cells.size());

	for(size_t i = 0; i < cells.size(); ++i)
	{
		bool repeat = false;
		for(size_t j = 0; j < tempVector.size(); ++j)
		{
			if(cells[i] == tempVector[j])
			{
				repeat = true;
				break;
			}
		}
		if(!repeat)
			tempVector.push_back(cells[i]);
	}	
	cells = tempVector;		
}

//-----------------------------------------------------------------------------

//bool Octree::CreateOctree(const PointCloud* pointCloud, const Mesh* mesh)
//{
//	if(!pointCloud && !mesh)
//		return false;
//
//	// Compute data property like extents
//	computeExtents(pointCloud, mesh);
//
//	m_root = new CommonUtil::OctreeCell((float)m_minX, (float)m_minY, (float)m_minZ, (float)m_maxX, (float)m_maxY, (float)m_maxZ);
//	m_root->SetUniqueId(0);	
//
//	return true;
//}

//-----------------------------------------------------------------------------

bool Octree::Divide(OctreeCell *cell, size_t numCellPoints)const
{
	bool status = false;
	size_t uniqueId = cell->GetUniqueId();
	std::vector<size_t> childIds;
	childIds.reserve(8);
	createChildrenForCell(cell, &childIds);
	int depth = cell->GetDepth() + 1;
	for(size_t kCount = 0; kCount < m_Caches.size(); ++kCount)
	{
		if(m_Caches[kCount])
		{
			status = m_Caches[kCount]->DistributeCellDataAmongChildren(cell, uniqueId, depth, childIds,numCellPoints);
			if(!status)
				break;
		}
	}
	return status;
}

//-----------------------------------------------------------------------------

void Octree::ComputeMaxSpan()
{
	double maxSpan = 0;
	double xCellSize, yCellSize, zCellSize;
	xCellSize = yCellSize = zCellSize = DBL_MAX;
	m_root->GetMinXYZCellsize(xCellSize, yCellSize, zCellSize);

	double xSpan = m_root->GetSizeX();
	double ySpan = m_root->GetSizeY();
	double zSpan = m_root->GetSizeZ();

	if((xSpan/xCellSize) > maxSpan)
		maxSpan = xSpan/xCellSize;
	if((ySpan/yCellSize) > maxSpan)
		maxSpan = ySpan/yCellSize;
	if((zSpan/zCellSize) > maxSpan)
		maxSpan = zSpan/zCellSize;

	m_maxSpan = (size_t)maxSpan;
}

//-----------------------------------------------------------------------------

void Octree::GetSurroundingCells(OctreeCell* cell, std::vector<OctreeCell*>& surroundCells)const
{
	std::vector<int> globalId;
	cell->GetGlobalId(globalId);
	std::vector<int> neighbourGlobalId;
	int count = 0;

	if(!surroundCells.empty())
		surroundCells.clear();

	/*if(cell->GetLevelOneNeighbours())
	{
		surroundCells = *cell->GetLevelOneNeighbours();
		return;
	}*/

	//for 6 face neighbours
	for(int iDir = ZERO_X_X; iDir <= X_X_ONE; ++iDir)
	{
		bool stat = GetSameLevelFaceNeighbourGlobalId(globalId, (DirectionForOctree)iDir, neighbourGlobalId, count);
		if(stat)		
			GetNeighbourCells(cell, globalId, neighbourGlobalId, (DirectionForOctree)iDir, count, surroundCells);		
	}
	//for 12 edge neighbours  
	for(int iDir = X_ZERO_ZERO; iDir <= ONE_ONE_X ; ++iDir)
	{
		bool stat = getSameLevelEdgeNeighbourGlobalId(globalId, (DirectionForOctree)iDir, neighbourGlobalId, count);
		if(stat)		
			GetNeighbourCells(cell, globalId, neighbourGlobalId, (DirectionForOctree)iDir, count, surroundCells);
		
	}
	//for 8 vertex neighbours
	for(int iDir = ZERO_ZERO_ZERO; iDir < ONE_ONE_ONE ; ++iDir)
	{
		bool stat = getSameLevelVertexNeighbourGlobalId(globalId, (DirectionForOctree)iDir, neighbourGlobalId, count);
		if(stat)
			GetNeighbourCells(cell, globalId, neighbourGlobalId, (DirectionForOctree)iDir, count, surroundCells);
	}
	
	removeDuplicateCellsInArray(surroundCells);
	cell->setLevelOneNeighbours(surroundCells);	
}

//-----------------------------------------------------------------------------
//Checks whether given cell contains cache Data Points other than given cache
bool Octree::IsOtheCacheDataPresentInCell(const OctreeCell* cell, CacheInterface* cache)
{
	size_t cellID = cell->GetUniqueId();
	bool status = false;

	for(size_t iCache = 0; iCache < m_Caches.size(); ++iCache)
	{
		CacheInterface* tmpCache = m_Caches[iCache];
		
		if( (tmpCache != cache) && (tmpCache->IsHavingData(cellID)) )
		{
			status = true;
			break;
		}
	}

	return status;
}

//-----------------------------------------------------------------------------
//Returns whether surrounding Cells within given 'maxDistance' contains Points Data of Other cache(Cloud)
bool Octree::IsSurroundCellsWithinDistanceContainsOtherCacheData(const OctreeCell* cell, const double maxDistance,
																 const CacheInterface* cache) const
{
	bool status = false;

	if(!cell || !cache || (maxDistance < 0.0))
		return status;

	//Form BoundingBox
	float centerX = 0.0, centerY = 0.0, centerZ = 0.0;
	cell->GetCenter(centerX, centerY, centerZ);

	float sizeX = cell->GetSizeX();
	float sizeY = cell->GetSizeY();
	float sizeZ = cell->GetSizeZ();
	

	double minX = (centerX - maxDistance - sizeX/2), maxX = (centerX + maxDistance + sizeX/2);
	double minY = (centerY - maxDistance - sizeY/2), maxY = (centerY + maxDistance + sizeY/2);
	double minZ = (centerZ - maxDistance - sizeZ/2), maxZ = (centerZ + maxDistance + sizeZ/2);
	
	BoundingBox3D sphereBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
	const CommonUtil::Point3DF point(centerX, centerY, centerZ);
	std::vector<OctreeCell*> neighbourCells;

	//Get All Cells within Distance
	GetCellsWithInDistance(point, &sphereBoundingBox, neighbourCells);

	if(neighbourCells.empty())
		return status;

	//Check whether Other Cache Data Present In neighbourCells
	size_t cellID = 0;

	for(size_t iCell = 0; iCell < neighbourCells.size(); ++iCell)
	{
		const OctreeCell* neighbourCell = neighbourCells[iCell];

		if(cell == neighbourCell)
			continue;

		cellID = neighbourCell->GetUniqueId();

		for(size_t iCache = 0; iCache < m_Caches.size(); ++iCache)
		{
			CacheInterface* tmpCache = m_Caches[iCache];
			
			if( (tmpCache != cache) && (tmpCache->IsHavingData(cellID)) )
			{
				status = true;
				break;
			}
		}

		if(status)
			break;
	}

	return status;
}

//-----------------------------------------------------------------------------

void Octree::GetLeafCellsAlongDirection(const OctreeCell* cell, DirectionForOctree dir, std::vector<OctreeCell*> &neighbourCells)const
{	
	if(cell->IsLeafCell())
	{
		neighbourCells.push_back((OctreeCell*)cell);
		return;
	}
	std::vector<int> childIds;
	getChildrenIdsAlongDirection(dir, childIds);

	int numChildren = (int)childIds.size();
	for(int iCount = 0 ; iCount < numChildren ; ++iCount)
	{		
		OctreeCell *tempCell = cell->GetChild(childIds[iCount]);
		if(tempCell)	
			GetLeafCellsAlongDirection(tempCell, dir, neighbourCells);		
	}
}

//-----------------------------------------------------------------------------

OctreeCell* Octree::GetCellForPoint(float x, float y, float z)const
{
	return m_root->GetCellForPoint(x, y, z);
}

//-----------------------------------------------------------------------------

OctreeCell* Octree::GetCellWithId(size_t parentId, int parentDepth, bool createNewCell)const
{
	OctreeCell* octreeCell = m_root;
	for(int iCount = 1; iCount <= parentDepth; ++iCount)
	{
		int id = ((parentId >> (64 - 3*iCount)) & 7);
		OctreeCell* childCell = octreeCell->GetChild(id);

		if(createNewCell && !childCell)
		{
			createChildrenForCell(octreeCell);
			childCell = octreeCell->GetChild(id);
		}
		octreeCell = childCell;
	}

	return octreeCell;
}

//-----------------------------------------------------------------------------

void Octree::GetAllLeafCells(std::vector<OctreeCell*>& leafCells)const
{
	if(m_root->IsLeafCell())
	{
		leafCells.push_back(m_root);
	}
	else
	{
		m_root->GetAllLeafCells(leafCells);
	}
}

//-----------------------------------------------------------------------------

//Returns leaf cells Having Data
void Octree::GetAllLeafDataCells(std::vector<OctreeCell*>& leafDataCells)const
{
	//Get All Leaf Cells
	std::vector<OctreeCell*> leafCells;
	GetAllLeafCells(leafCells);

	CacheInterface* cache = nullptr;
	size_t cellID = 0;

	//Get All leaf Cells having Points Data
	for(size_t iCell = 0; iCell < leafCells.size(); ++iCell)
	{
		//Gel leaf Cell and ask cell that is it in overlapping region
		OctreeCell* leafCell = leafCells[iCell];
		cellID = leafCell->GetUniqueId();

		for(size_t iCache = 0; iCache < m_Caches.size(); ++iCache)
		{
			cache = m_Caches[iCache];
			
			if(cache->IsHavingData(cellID))
			{
				leafDataCells.push_back(leafCell);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------

void Octree::AddCache(CacheInterface* cache)
{
	std::vector <CacheInterface*>::iterator iter = 
		std::find(m_Caches.begin(), m_Caches.end(), cache);

	if(iter == m_Caches.end())
	{
		m_Caches.push_back(cache);
		std::vector<OctreeCell*> leafCells;
		GetAllLeafCells(leafCells);

		for(size_t cellctr = 0; cellctr < leafCells.size(); ++cellctr)
			cache->AddLeafCell(leafCells[cellctr]->GetDepth(), leafCells[cellctr]->GetUniqueId());
	}
}

//-----------------------------------------------------------------------------

void Octree::RemoveCache(CacheInterface* cache)
{
	size_t numCaches = m_Caches.size();
	for(unsigned int iCnt = 0; iCnt < numCaches; ++iCnt)
	{
		if(cache && (cache == m_Caches[iCnt]))
		{
			m_Caches[iCnt] = nullptr;
			break;
		}
	}
}

//-----------------------------------------------------------------------------

void Octree::Save(size_t &fileSize)
{
	BInputOutputFile file(L"C:\\Temp\\Octree.oct", false);

	float minX, minY, minZ, maxX, maxY, maxZ;
	m_root->GetExtents(minX, minY, minZ, maxX, maxY, maxZ);

	file << minX;
	file << minY;
	file << minZ;
	file << maxX;
	file << maxY;
	file << maxZ;

	//get all leaf cell of octree 
	std::vector<OctreeCell*> leafCells;
	GetAllLeafCells(leafCells);
	
	//write numLeafCell
	size_t numLeafCell = leafCells.size();
	file << (__int64)numLeafCell;

	for(size_t cellctr = 0; cellctr < numLeafCell; ++cellctr)
	{
		file << (__int64)leafCells[cellctr]->GetUniqueId();
		file << leafCells[cellctr]->GetDepth();
		file << leafCells[cellctr]->GetFlags();
	}

	fileSize = file.GetFileCurrentPosition();
}

//-----------------------------------------------------------------------------

void Octree::ReadOctree()
{
	BInputOutputFile file(L"C:\\Temp\\Octree.oct", true);

	//read extent 
	float minX, minY, minZ, maxX, maxY, maxZ;
	
	file >> minX;
	file >> minY;
	file >> minZ;
	file >> maxX;
	file >> maxY;
	file >> maxZ;

	//create root 
	this->SetExtents(minX, minY, minZ, maxX, maxY, maxZ);

	//read numLeafCell
	__int64 numLeafCell = -1;
	__int64 octreeCellId = -1;
	int depth = -1;
	unsigned char leafFlags; 
	file >> numLeafCell;

	for(size_t cellctr = 0; cellctr < numLeafCell; ++cellctr)
	{
		//get octreeCell id and depth 
		file >> octreeCellId;
		file >> depth;
		file >> leafFlags;

		//create cell for octree
		OctreeCell* octreeCell = GetCellWithId(octreeCellId, depth, true);
		octreeCell->SetFlags(leafFlags);
	}
}

//-----------------------------------------------------------------------------

void Octree::createChildrenForCell(OctreeCell *cell, std::vector<size_t>* childIds)const
{
	size_t uniqueId = cell->GetUniqueId();
	float minX = 0., minY = 0., minZ = 0., maxX = 0., maxY = 0., maxZ = 0.;
	minX = cell->m_centerX - (cell->m_sizeX/2);
	minY = cell->m_centerY - (cell->m_sizeY/2);
	minZ = cell->m_centerZ - (cell->m_sizeZ/2);
	maxX = cell->m_centerX + (cell->m_sizeX/2);
	maxY = cell->m_centerY + (cell->m_sizeY/2);
	maxZ = cell->m_centerZ + (cell->m_sizeZ/2);
	
	//Set leaf cell flag to false
	cell->setLeafCell(false);

	cell->m_children = new OctreeCell[8];
	cell->m_children[0] = OctreeCell(minX, cell->m_centerY, minZ, cell->m_centerX, maxY, cell->m_centerZ);
	cell->m_children[1] = OctreeCell(cell->m_centerX, cell->m_centerY, minZ, maxX, maxY, cell->m_centerZ);
	cell->m_children[2] = OctreeCell(minX, minY, minZ, cell->m_centerX, cell->m_centerY, cell->m_centerZ);
	cell->m_children[3] = OctreeCell(cell->m_centerX, minY, minZ, maxX, cell->m_centerY, cell->m_centerZ);
	cell->m_children[4] = OctreeCell(minX, cell->m_centerY, cell->m_centerZ, cell->m_centerX, maxY, maxZ);
	cell->m_children[5] = OctreeCell(cell->m_centerX, cell->m_centerY, cell->m_centerZ, maxX, maxY, maxZ);
	cell->m_children[6] = OctreeCell(minX, minY, cell->m_centerZ, cell->m_centerX, cell->m_centerY, maxZ);
	cell->m_children[7] = OctreeCell(cell->m_centerX, minY, cell->m_centerZ, maxX, cell->m_centerY, maxZ);

	int depth = cell->GetDepth() + 1;
	for(size_t jCount = 0; jCount < 8; ++jCount)
	{
		OctreeCell *childCell = cell->GetChild((int)jCount);
		childCell->setDepth(depth);

		size_t cellUniqueId = uniqueId;
		//For Large Data Handling, use 64 bit version and and cell data has 64bit Id of which first 54 bits are used to give unique Id to octree cell. 
		//Hense bit shifting by 10+(3*depth)
		cellUniqueId |= (jCount<<(3*(18-depth)+10));
		childCell->setLeafCell(true);
		childCell->setUniqueId(cellUniqueId);
		if(childIds)
			childIds->push_back(cellUniqueId);
	}
}

//-----------------------------------------------------------------------------

void Octree::GetNextLayerCells(const std::vector<OctreeCell*>& prevCells, const std::vector<OctreeCell*>& prevLayerCells,
	std::vector<OctreeCell*>& surroundCells)const
{
	surroundCells.clear();
	
	for(size_t iCount = 0; iCount < prevCells.size(); ++iCount)
		prevCells[iCount]->SetVisited(true);

	for(size_t iCount = 0; iCount < prevLayerCells.size(); ++iCount)
	{
		OctreeCell* cell = prevLayerCells[iCount];
		std::vector<OctreeCell*> *levelOneNeighbours = cell->GetLevelOneNeighbours();

		std::vector<OctreeCell*> neighbourCells;
		if(!levelOneNeighbours)
			GetSurroundingCells(cell, neighbourCells);
		else								
			neighbourCells = *levelOneNeighbours;		
		
		for(size_t jCount = 0; jCount < neighbourCells.size(); ++jCount)
		{
			OctreeCell* cell = neighbourCells[jCount];
			if(!cell->IsVisited())
			{
				cell->SetVisited(true);					
				surroundCells.push_back(cell);				
			}
		}		
	}

	for(size_t iCount = 0; iCount < surroundCells.size(); ++iCount)
		surroundCells[iCount]->SetVisited(false);
	
	for(size_t iCount = 0; iCount < prevCells.size(); ++iCount)	
		prevCells[iCount]->SetVisited(false);
}
//-----------------------------------------------------------------------------

bool Octree::IsOctreeExist()
{
	FileHandler binaryIoFile(m_fileName,"r");
	return (binaryIoFile.IsFileExist());
}

//-----------------------------------------------------------------------------

void Octree::GetCellsWithInDistance(const CommonUtil::Point3DF &point, BoundingBox3D *boundingBox,
	std::vector<OctreeCell*>& cells) const
{
	
	float x = point.GetX();
	float y = point.GetY();
	float z = point.GetZ();

	OctreeCell* pointCell = GetRootCell()->GetCellForPoint(x, y, z);
	if(!pointCell)
		return;	

	//CommonUtil::CUBoundingBox3D sphereBoundingBox(x-maxDistance, y-maxDistance, z-maxDistance, x+maxDistance, y+maxDistance, z+maxDistance);	

	std::vector<OctreeCell*> prevCells;
	std::vector<OctreeCell*> prevLayerCells;

	prevCells.push_back(pointCell);
	prevLayerCells.push_back(pointCell);
	cells.push_back(pointCell);

	std::vector<OctreeCell*> localLayerCells;
	do
	{
		localLayerCells.clear();

		//get Surrounding Cells For Cells Of prevLayerCells		
		GetNextLayerCells(prevCells, prevLayerCells, localLayerCells);			
		
		if(!localLayerCells.empty())
		{
			prevCells.insert(prevCells.end(), localLayerCells.begin(), localLayerCells.end());			
		}

		//if cells in layer are not within distanceTol then delete that cell from localLayerCells		
		std::vector<OctreeCell*> tempLocalLayerCells;
		tempLocalLayerCells.reserve(localLayerCells.size());
		for(size_t iCount = 0; iCount < localLayerCells.size(); ++iCount)
		{
			OctreeCell* tempCell = localLayerCells[iCount];
			float minX, minY, minZ, maxX, maxY, maxZ;
			tempCell->GetExtents(minX, minY, minZ, maxX, maxY, maxZ);
			BoundingBox3D tempBoundingBox(minX, maxX, minY, maxY, minZ, maxZ);

			if(tempBoundingBox.IsIntersecting(*boundingBox))
				tempLocalLayerCells.push_back(tempCell);						
		}

		localLayerCells = tempLocalLayerCells;	
		prevLayerCells = localLayerCells;

		if(!localLayerCells.empty())
		{
			cells.insert(cells.end(), localLayerCells.begin(), localLayerCells.end());
		}

	}while(!localLayerCells.empty());
}

//-----------------------------------------------------------------------------

void Octree::DeleteLevelOneNeighboursFromLeafCells()
{
	std::vector<OctreeCell*>leafCells;
	GetAllLeafCells(leafCells);
	size_t numLeafCells = leafCells.size();
	for(unsigned int lCnt = 0; lCnt < numLeafCells; ++lCnt)
	{
		leafCells[lCnt]->deleteLevelOneNeighbours();
	}
}


//-----------------------------------------------------------------------------

void Octree::RemoveCellsOfFirstVectorInSecondVector(const std::vector<OctreeCell *>& firstVector, std::vector<OctreeCell *>& secondVector)const
{	
	if(firstVector.empty())
		return;

	for(size_t iCount = 0; iCount < firstVector.size(); ++iCount)
		firstVector[iCount]->SetVisited(true);

	std::vector<OctreeCell *>tempVector;
	tempVector.reserve(secondVector.size());
	for(size_t iCount = 0; iCount < secondVector.size(); ++iCount)
	{
		OctreeCell* cell = secondVector[iCount];
		if(!cell->IsVisited())
			tempVector.push_back(cell);
	}
	secondVector = tempVector;

	for(size_t iCount = 0; iCount < firstVector.size(); ++iCount)
		firstVector[iCount]->SetVisited(false);
}

//-----------------------------------------------------------------------------

void Octree::GetCellsWithInDistance(const CommonUtil::Point3DF &point, const double maxDistance,
	std::vector<OctreeCell*>& cells) const
{
	float x = point.GetX(), y = point.GetY(), z = point.GetZ();
	BoundingBox3D sphereBoundingBox(x-maxDistance, y-maxDistance, z-maxDistance, x+maxDistance, y+maxDistance, z+maxDistance);

	GetCellsWithInDistance(point, &sphereBoundingBox, cells);
}

//-----------------------------------------------------------------------------

size_t Octree::SizeOf()const
{
	size_t size = sizeof(*this);
	if(m_root)
		size += m_root->SizeOf();

	return size;
}

//-----------------------------------------------------------------------------

}