// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _OCTREE_H_
#define _OCTREE_H_

// Std Include Files
#pragma warning (disable: 4786)
#include <vector>
#include "CUCacheInterface.h"
#include "CUPoint3DF.h"
namespace CommonUtil
{
// Forward Declarations
class OctreeCell;
class BoundingBox3D;
//class PointCloud;	
//class Mesh;

enum DirectionForOctree {//Face neighbour directions
						ZERO_X_X, ONE_X_X, X_ZERO_X, X_ONE_X, X_X_ZERO, X_X_ONE,
						//Edge neighbour directions
						X_ZERO_ZERO, X_ZERO_ONE, X_ONE_ZERO, X_ONE_ONE,
						ZERO_X_ZERO, ZERO_X_ONE, ONE_X_ZERO, ONE_X_ONE,
						ZERO_ZERO_X, ZERO_ONE_X, ONE_ZERO_X, ONE_ONE_X,
						//Vertex neighbour directions
						ZERO_ZERO_ZERO, ZERO_ZERO_ONE, ZERO_ONE_ZERO, ZERO_ONE_ONE,
						ONE_ZERO_ZERO, ONE_ZERO_ONE, ONE_ONE_ZERO, ONE_ONE_ONE,
						Same_Parent, Contrary_Neighbour};

//Action table for finding neighbours
static const DirectionForOctree ActionTable[8][26] = {
														{ ONE_X_X, ZERO_X_X, ONE_X_X, ZERO_X_X, ONE_X_X, ZERO_X_X,
														  Contrary_Neighbour, X_ZERO_X, X_X_ZERO, Same_Parent, Contrary_Neighbour, ZERO_X_X,
														  X_X_ZERO, Same_Parent, Contrary_Neighbour, ZERO_X_X, X_ZERO_X, Same_Parent,
														  Contrary_Neighbour,ZERO_ZERO_X, ZERO_X_ZERO, ZERO_X_X, X_ZERO_ZERO, X_ZERO_X, X_X_ZERO, Same_Parent },

														{ ONE_X_X, ZERO_X_X, ONE_X_X, ZERO_X_X, ZERO_X_X, ONE_X_X,
														  X_ZERO_X, Contrary_Neighbour, Same_Parent, X_X_ONE, ZERO_X_X, Contrary_Neighbour, 
														  Same_Parent, X_X_ONE, Contrary_Neighbour, ZERO_X_X, X_ZERO_X, Same_Parent, 
														  ZERO_ZERO_X, Contrary_Neighbour, ZERO_X_X, ZERO_X_ONE, X_ZERO_X, X_ZERO_ONE, Same_Parent, X_X_ONE },

														{ ONE_X_X, ZERO_X_X, ZERO_X_X, ONE_X_X, ONE_X_X, ZERO_X_X, 
														  X_X_ZERO, Same_Parent, Contrary_Neighbour, X_ONE_X, Contrary_Neighbour, ZERO_X_X, 
														  X_X_ZERO, Same_Parent, ZERO_X_X , Contrary_Neighbour, Same_Parent, X_ONE_X,
														  ZERO_X_ZERO, ZERO_X_X, Contrary_Neighbour, ZERO_ONE_X, X_X_ZERO, Same_Parent, X_ONE_ZERO, X_ONE_X },

														{ ONE_X_X, ZERO_X_X, ZERO_X_X, ONE_X_X, ZERO_X_X, ONE_X_X, 
														  Same_Parent, X_X_ONE, X_ONE_X, Contrary_Neighbour, ZERO_X_X, Contrary_Neighbour, 
														  Same_Parent, X_X_ONE, ZERO_X_X , Contrary_Neighbour, Same_Parent, X_ONE_X, 
														  ZERO_X_X, ZERO_X_ONE, ZERO_ONE_X, Contrary_Neighbour, Same_Parent, X_X_ONE, X_ONE_X, X_ONE_ONE },

														{ ZERO_X_X, ONE_X_X, ONE_X_X, ZERO_X_X, ONE_X_X, ZERO_X_X, 
														  Contrary_Neighbour, X_ZERO_X, X_X_ZERO, Same_Parent, X_X_ZERO, Same_Parent,
														  Contrary_Neighbour, ONE_X_X, X_ZERO_X, Same_Parent, Contrary_Neighbour, ONE_X_X, 
														  X_ZERO_ZERO, X_ZERO_X, X_X_ZERO, Same_Parent, Contrary_Neighbour, ONE_ZERO_X, ONE_X_ZERO, ONE_X_X },

														{ ZERO_X_X, ONE_X_X, ONE_X_X, ZERO_X_X, ZERO_X_X, ONE_X_X,
														  X_ZERO_X, Contrary_Neighbour, Same_Parent, X_X_ONE, Same_Parent, X_X_ONE, 
														  ONE_X_X, Contrary_Neighbour, X_ZERO_X, Same_Parent, Contrary_Neighbour, ONE_X_X, 
														  X_ZERO_X, X_ZERO_ONE, Same_Parent, X_X_ONE, ONE_ZERO_X, Contrary_Neighbour, ONE_X_X, ONE_X_ONE },

														{ ZERO_X_X, ONE_X_X, ZERO_X_X, ONE_X_X, ONE_X_X, ZERO_X_X,
														  X_X_ZERO, Same_Parent, Contrary_Neighbour, X_X_ONE, X_X_ZERO, Same_Parent,
														  Contrary_Neighbour, ONE_X_X, Same_Parent, X_ONE_X, ONE_X_X, Contrary_Neighbour,
														  X_X_ZERO, Same_Parent, X_ONE_ZERO, X_ONE_X, ONE_X_ZERO, ONE_X_X, Contrary_Neighbour, ONE_ONE_X },

														{ ZERO_X_X, ONE_X_X, ZERO_X_X, ONE_X_X, ZERO_X_X, ONE_X_X,
														  Same_Parent, X_X_ONE, X_ONE_X, Contrary_Neighbour, Same_Parent, X_X_ONE, 
														  ONE_X_X, Contrary_Neighbour, Same_Parent, X_ONE_X, ONE_X_X, Contrary_Neighbour,
														  Same_Parent, X_X_ONE, X_ONE_X, X_ONE_ONE, ONE_X_X, ONE_X_ONE, ONE_ONE_X, Contrary_Neighbour },

													};

//Value table for finding neighbours
static const int ValueTable[8][26] = { { 4, 4, 2, 2, 1, 1, 3, 3, 3, 3, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7 },
									   { 5, 5, 3, 3, 0, 0, 2, 2, 2, 2, 4, 4, 4, 4, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6 },
									   { 6, 6, 0, 0, 3, 3, 1, 1, 1, 1, 7, 7, 7, 7, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5 },
									   { 7, 7, 1, 1, 2, 2, 0, 0, 0, 0, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4 },
									   { 0, 0, 6, 6, 5, 5, 7, 7, 7, 7, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3 },
									   { 1, 1, 7, 7, 4, 4, 6, 6, 6, 6, 0, 0, 0, 0, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2 },
									   { 2, 2, 4, 4, 7, 7, 5, 5, 5, 5, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
									   { 3, 3, 5, 5, 6, 6, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
								 	 };

//Opposite Direction Table
static const DirectionForOctree OppositeDirection[26] = { ONE_X_X, ZERO_X_X, X_ONE_X, X_ZERO_X, X_X_ONE, X_X_ZERO, 
														  X_ONE_ONE, X_ONE_ZERO, X_ZERO_ONE, X_ZERO_ZERO, 
														  ONE_X_ONE, ONE_X_ZERO, ZERO_X_ONE, ZERO_X_ZERO, 
														  ONE_ONE_X, ONE_ZERO_X, ZERO_ONE_X, ZERO_ZERO_X, 
														  ONE_ONE_ONE, ONE_ONE_ZERO, ONE_ZERO_ONE, ONE_ZERO_ZERO, 
														  ZERO_ONE_ONE, ZERO_ONE_ZERO, ZERO_ZERO_ONE, ZERO_ZERO_ZERO };
#pragma pack(push)
#pragma pack(1)
class Octree
{
	OctreeCell* m_root;

	//Octree Extents
	double m_minX, m_minY, m_minZ;
	double m_maxX, m_maxY, m_maxZ;

	size_t m_maxSpan;	

	std::vector<CacheInterface*> m_Caches;

	//file pointer to read write octree
	char* m_fileName;

	//Get neighbour Id for given Id in given direction
	int getNeighbourId(int id, DirectionForOctree dir)const{ return ValueTable[id][dir]; }

	//it returns a direction for given Id in given direction
	DirectionForOctree getDirectionOfNeighbour(int id, DirectionForOctree dir)const{ return ActionTable[id][dir]; }	

	//for given GlobalId and Direction, it gives GlobalId of Edge neighbour at same level of octree
	bool getSameLevelEdgeNeighbourGlobalId(const std::vector<int> &globalId, DirectionForOctree dir, 
										   std::vector<int> &neighbourGlobalId, int &lCount)const;

	//for given GlobalId and Direction, it gives GlobalId of Vertex neighbour at same level of octree
	bool getSameLevelVertexNeighbourGlobalId (const std::vector<int> &globalId, DirectionForOctree dir, 
											  std::vector<int> &neighbourGlobalId, int&lCount)const;	

	//Get opposite direction for given direction
	DirectionForOctree getOppositeDirection(DirectionForOctree dir)const{ return OppositeDirection[dir]; }

	//Get Children Ids  Along given Direction
	void getChildrenIdsAlongDirection(DirectionForOctree dir, std::vector<int> &ids)const;
	
	//void computeExtents(const PointCloud* pointCloud, const Mesh* mesh);

	void removeDuplicateCellsInArray(std::vector<OctreeCell *>& cells)const;

	void createChildrenForCell(OctreeCell *cell, std::vector<size_t>* childIds = nullptr)const;

public:	
		
	// Default constructor
	Octree(char* fileName);

	Octree();
	
	Octree(double minX, double minY, double minZ,
		   double maxX, double maxY, double maxZ);

	// Destructor
	~Octree();

	void SetExtents(double minX, double minY, double minZ,
					double maxX, double maxY, double maxZ);
	void Reset();

	void Clear();

	// Public functions for Octree
	//bool CreateOctree(const PointCloud* pointCloud, const Mesh* mesh);

	bool Divide(OctreeCell *cell, size_t numCellPoints = 0)const;

	void ComputeMaxSpan();

	OctreeCell* GetRootCell()const{ return m_root; }

	//get All surrounding Cells
	void GetSurroundingCells(OctreeCell* cell, std::vector<OctreeCell *>& surroundCells)const;

	//Checks whether given cell contains cache Data Points other than given cache
	bool IsOtheCacheDataPresentInCell(const OctreeCell* cell, CacheInterface* cache);

	//Returns whether surrounding Cells within given 'maxDistance' contains Points Data of Other cache(Cloud)
	bool IsSurroundCellsWithinDistanceContainsOtherCacheData(const OctreeCell* cell, const double maxDistance, const CacheInterface* cache) const;

	//get Leaf Cells Along Direction
	void GetLeafCellsAlongDirection(const OctreeCell* cell, DirectionForOctree dir, std::vector< OctreeCell *> &neighbourCells)const;

	//for given GlobalId and Direction, it gives GlobalId of Face neighbour at same level of octree
	bool GetSameLevelFaceNeighbourGlobalId(const std::vector<int> &globalId, DirectionForOctree dir, 
										   std::vector<int> &neighbourGlobalId, int &count)const;

	//get neighbour cells in given direction
	void GetNeighbourCells(const OctreeCell* cell, const std::vector<int> &cellGlobalId, const std::vector <int> &neighbourGlobalId,
		DirectionForOctree dir, int count, std::vector<OctreeCell *> &neighbourCells)const;

	bool IsInValidRange(float X, float Y, float Z)const
	{
		if (X > m_maxX || Y > m_maxY || Z > m_maxZ || X < m_minX || Y < m_minY || Z < m_minZ)
			return false;

		return true;
	}

	void GetExtents(double ext[6])const
	{
		ext[0] = m_minX;
		ext[1] = m_minY;
		ext[2] = m_minZ;
		ext[3] = m_maxX;
		ext[4] = m_maxY;
		ext[5] = m_maxZ;
	}
	OctreeCell* GetCellForPoint(float x, float y, float z)const;

	OctreeCell* GetCellWithId(size_t parentId, int parentDepth, bool createNewCell = false)const;
	
	void GetAllLeafCells(std::vector<OctreeCell *>& leafCells)const;

	//Returns leaf cells Having Data
	void GetAllLeafDataCells(std::vector<OctreeCell *>& leafDataCells)const;

	size_t GetMaxSpan()const{ return m_maxSpan; }

	void AddCache(CacheInterface* cache);
	
	void RemoveCache(CacheInterface* cache);

	void Save(size_t &fileSize);
	void ReadOctree();
	bool IsOctreeExist();

	
	void GetNextLayerCells(const std::vector<OctreeCell *>& prevCells,
		const std::vector<OctreeCell*>& prevLayerCells,
						std::vector<OctreeCell*>& surroundCells)const;


	//get cells within distance
	void GetCellsWithInDistance(const Point3DF &point, BoundingBox3D *boundingBox,
		std::vector<OctreeCell*>& cells) const;

	void GetCellsWithInDistance(const Point3DF &point, const double maxDistance, std::vector<OctreeCell*>& cells) const;

	//Delete levelOneNeighbours from leaf cells
	void DeleteLevelOneNeighboursFromLeafCells();

	void RemoveCellsOfFirstVectorInSecondVector(const std::vector<OctreeCell*>& firstVector, std::vector<OctreeCell*>& secondVector)const;
	
	//Get read or write octree file name
	char* GetReadOrWriteFileName()const{ return m_fileName;};

	size_t SizeOf()const;
};
#pragma pack(pop)   /* restore original from stack */
}

#endif