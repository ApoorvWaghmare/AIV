// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include <algorithm>
#include "CUOctreeAssistantForPoint3D.h"
#include "CUOctree.h"
#include "CUOctreeCell.h"
#include "CUPoint3DCache.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

OctreeAssistantForPoint3D::OctreeAssistantForPoint3D(Octree* octree, Point3DCache* point3DCache)
{
	m_octree = octree;
	m_point3DCache = point3DCache;

	size_t numpoints = m_point3DCache->GetNumPoints();
	if(numpoints== 0)
		m_octree->AddCache(m_point3DCache);
}

//-----------------------------------------------------------------------------

OctreeAssistantForPoint3D::OctreeAssistantForPoint3D(double minX, double minY, double minZ,
								 double maxX, double maxY, double maxZ, 
								 Point3DCache* point3DCache)
{
	m_octree = new Octree(minX, minY, minZ, maxX, maxY, maxZ);
	m_point3DCache = point3DCache;
	m_octree->AddCache(m_point3DCache);
}

//-----------------------------------------------------------------------------

OctreeAssistantForPoint3D::~OctreeAssistantForPoint3D()
{
}

//-----------------------------------------------------------------------------

void OctreeAssistantForPoint3D::removePointCloudData(OctreeCell *cell)const
{
	if(cell->IsLeafCell())
	{
		if(m_point3DCache)
			m_point3DCache->ClearCellPoints(cell->GetUniqueId());
	}
	else
	{
		for(int iCount = 0 ; iCount<8 ; ++iCount)
			removePointCloudData(cell->GetChild(iCount));
	}
}

//-----------------------------------------------------------------------------

bool OctreeAssistantForPoint3D::checkIfDuplicatePoint(float x1, float y1, float z1, float x2, float y2, float z2)
{
	double sqrDist = ((double)x1-(double)x2) * ((double)x1-(double)x2) +
						((double)y1-(double)y2) * ((double)y1-(double)y2) +
						((double)z1-(double)z2) * ((double)z1-(double)z2);

	if(sqrDist < ((1e-6) * (1e-6)))
		return true;

	return false;
}

//-----------------------------------------------------------------------------

void OctreeAssistantForPoint3D::AddPoint(float* point, unsigned char *color)
{
	size_t numCellPoints = 0;
	OctreeCell* cell = 0;
	std::vector<float>cellPoints;
	std::vector<unsigned char>pointsColors;
	while(1)
	{
		cell = m_octree->GetCellForPoint(point[0], point[1], point[2]);

		if(m_point3DCache)
		{
			numCellPoints = m_point3DCache->GetNumCellPoints(cell->GetUniqueId());
			cellPoints.reserve(numCellPoints*3);
			pointsColors.reserve(numCellPoints*3);
			m_point3DCache->GetCellPoints(cell->GetUniqueId(),cellPoints,pointsColors);

			for(size_t i = 0; i < cellPoints.size(); i=i+3)
			{
				if(checkIfDuplicatePoint(point[0], point[1], point[2], cellPoints[i], cellPoints[i+1], cellPoints[i+2]))
					return;
			}

			if(numCellPoints >= m_point3DCache->GetMaxNumPtsPerCell())
				m_octree->Divide(cell);
			else
				break;
		}
	}

	if(m_point3DCache)
		m_point3DCache->AddPoint(cell->GetUniqueId(), point, color);
}

//-----------------------------------------------------------------------------

void OctreeAssistantForPoint3D::GetAllLeafCells(std::vector< OctreeCell *>& leafCells)const
{
	m_octree->GetAllLeafCells(leafCells);
}

//-----------------------------------------------------------------------------

void OctreeAssistantForPoint3D::GetPointsInCell(const OctreeCell *cell,std::vector<float>& cellPoints)const
{
	std::vector<float> points;
	std::vector<unsigned char>colors;
	if(!cellPoints.empty())
		cellPoints.clear();

	if(m_point3DCache)
		m_point3DCache->GetCellPoints(cell->GetUniqueId(), points, colors);

	for(int count = 0; count < points.size(); count+=3)
	{
		cellPoints.push_back(points[count]);
		cellPoints.push_back(points[count+1]);
		cellPoints.push_back(points[count+2]);
	}
}

//-----------------------------------------------------------------------------

void OctreeAssistantForPoint3D::GetPointsWithAttributeInCell(const OctreeCell *cell,std::vector<float>& cellPoints,std::vector<float>& pointsColour)const
{
	std::vector<float> points;
	std::vector<unsigned char> colors;
	if(!cellPoints.empty())
		cellPoints.clear();

	if(!pointsColour.empty())
		pointsColour.clear();

	if(m_point3DCache)
		m_point3DCache->GetCellPoints(cell->GetUniqueId(), points, colors);

	for(int count = 0; count < points.size(); count+=3)
	{
		cellPoints.push_back(points[count]);
		cellPoints.push_back(points[count+1]);
		cellPoints.push_back(points[count+2]);

		pointsColour.push_back((float)colors[count]/255);
		pointsColour.push_back((float)colors[count+1]/255);
		pointsColour.push_back((float)colors[count+2]/255);
	}
}

//-----------------------------------------------------------------------------

void OctreeAssistantForPoint3D::GetNearestPointWithAttribute(float x, float y, float z, float *point, float *color)
{
	OctreeCell *cell = m_octree->GetCellForPoint(x, y, z);

	if(!cell)
		return;

	std::vector<float> cellPoints;
	std::vector<float> pointsColour;

	GetPointsWithAttributeInCell(cell, cellPoints, pointsColour);

	if(cellPoints.empty())
		return;

	double minDistance = DBL_MAX;
	double sqrDistance = 0.;	
	
	size_t numCellPoints = cellPoints.size();
	size_t nearestPointIndex = 0;
	for (size_t i = 0; i < numCellPoints; i = i+3)
	{
		sqrDistance = ((cellPoints[i] - x) * (cellPoints[i] - x) +
					(cellPoints[i+1] - y) * (cellPoints[i+1] - y) +
					(cellPoints[i+2] - z) * (cellPoints[i+2] - z));

		if(sqrDistance < minDistance)
		{
			minDistance = sqrDistance;
			nearestPointIndex = i;
		}
	}
	if(numCellPoints)
	{
		point[0] = cellPoints[nearestPointIndex];
		point[1] = cellPoints[nearestPointIndex+1];
		point[2] = cellPoints[nearestPointIndex+2];

		color[0] = pointsColour[nearestPointIndex];
		color[1] = pointsColour[nearestPointIndex+1];
		color[2] = pointsColour[nearestPointIndex+2];
	}
}

//-----------------------------------------------------------------------------

bool OctreeAssistantForPoint3D::IsHavingData(size_t cellId)const
{
	return m_point3DCache->IsHavingData(cellId);
}
//-----------------------------------------------------------------------------

void OctreeAssistantForPoint3D::FlushCacheData()
{
	//write all cellData present in memory to file(flush it)
	//m_point3DCache->Flush();
}
//-----------------------------------------------------------------------------
}