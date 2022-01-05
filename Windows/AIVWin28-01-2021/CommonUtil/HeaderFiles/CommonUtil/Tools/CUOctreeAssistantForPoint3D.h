// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _OCTREE_ASSISTANT_POINT3D_H_
#define _OCTREE_ASSISTANT_POINT3D_H_

#include <vector>

namespace CommonUtil
{
class Octree;
class OctreeCell;
class Point3DCache;

class OctreeAssistantForPoint3D
{
	void removePointCloudData(OctreeCell *cell)const;

	void analyzeForPointCloud(OctreeCell *Cell, int &numTotalCells, int &numNonLeafCells, int &numLeafCells, 
							  int &numRareCells, int &numGoodCells,
							  int &minPointsInCell, int &maxPointsInCell, int &level)const;

	bool checkIfDuplicatePoint(float x1, float y1, float z1, float x2, float y2, float z2);

protected:

	Octree* m_octree;

	Point3DCache* m_point3DCache;

public:			

	OctreeAssistantForPoint3D(Octree* octree, Point3DCache* point3DCache);

	OctreeAssistantForPoint3D(double minX, double minY, double minZ,
					double maxX, double maxY, double maxZ, Point3DCache* point3DCache);

	~OctreeAssistantForPoint3D();

	void RemovePointCloudData()const;

	void AddPoint(float* point, unsigned char *color);

	void GetAllLeafCells(std::vector< OctreeCell *>& leafCells)const;

	void GetPointsInCell(const OctreeCell *cell,std::vector<float>& cellPoints)const;

	void GetPointsWithAttributeInCell(const OctreeCell *cell,std::vector<float>& cellPoints,std::vector<float>& pointsColour)const;

	void GetNearestPointWithAttribute(float x, float y, float z, float *nearestPoint, float *color);

	Octree* GetOctree()const{ return m_octree; }

	bool IsHavingData(size_t cellId)const;

	void  FlushCacheData();
};

}

#endif