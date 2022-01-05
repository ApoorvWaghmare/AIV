// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#pragma warning (disable: 4786)
#include "CUOctreeCell.h"
#include "CUpoint3d.h"
#include "CUCacheInterface.h"
#include "CUSizeOf.h"

namespace CommonUtil
{

	static const size_t DepthIdTable[18] = { 0xE000000000000000, 0xFC00000000000000, 0xFF80000000000000, 0xFFF0000000000000,
											0xFFFE000000000000, 0xFFFFC00000000000, 0xFFFFF80000000000, 0xFFFFFF0000000000,
											0xFFFFFFE000000000, 0xFFFFFFFC00000000 , 0xFFFFFFFF80000000, 0xFFFFFFFFF0000000,
											0xFFFFFFFFFE000000, 0xFFFFFFFFFFC00000, 0xFFFFFFFFFFF80000, 0xFFFFFFFFFFFF0000,
											0xFFFFFFFFFFFFE000, 0xFFFFFFFFFFFFFC00 };

//-----------------------------------------------------------------------------

OctreeCell::OctreeCell(float minX, float minY, float minZ,
					   float maxX, float maxY, float maxZ)
{
	
	m_centerX = (maxX + minX) / 2;
	m_centerY = (maxY + minY) / 2;
	m_centerZ = (maxZ + minZ) / 2;

	m_sizeX = maxX - minX;
	m_sizeY = maxY - minY;
	m_sizeZ = maxZ - minZ;

	m_children = 0;
	m_uniqueId = 0;

	m_levelOneNeighbours = 0;
	
	m_flags = 0;
	//set leafCell Flag to true & visited flag to false;
	SetVisited(false);
	setLeafCell(true);
	SetChecked(true);
	SetAllPointsVisibility(true);
	m_checkedPointIndex = 0;
	m_pixelRectminX = INT_MAX;
	m_pixelRectminY = INT_MAX;
	m_pixelRectmaxX = 0;
	m_pixelRectmaxY = 0;
}

//-----------------------------------------------------------------------------

OctreeCell::~OctreeCell()
{
	if(!IsLeafCell())
	{
		if(m_children)
		{
/*
			for(int iCount = 0 ; iCount<8 ; ++iCount)
			{
				if(m_children[iCount])
				{
					delete m_children[iCount];
					m_children[iCount] = 0;
				}
			}
*/
			delete [] m_children;
			m_children = 0;
		}
	}

	if(m_levelOneNeighbours)
		delete m_levelOneNeighbours;
}

//-----------------------------------------------------------------------------

void OctreeCell::deleteLevelOneNeighbours()
{
	if(m_levelOneNeighbours)
		delete m_levelOneNeighbours;
	m_levelOneNeighbours = nullptr;
}

//-----------------------------------------------------------------------------

void OctreeCell::setLevelOneNeighbours(const std::vector <OctreeCell*>& levelOneNeighbours)
{
	if(!m_levelOneNeighbours)
		m_levelOneNeighbours = new std::vector <OctreeCell*>;
	
	*m_levelOneNeighbours = levelOneNeighbours;		
}

//-----------------------------------------------------------------------------

void OctreeCell::GetExtents(float &minX, float &minY, float &minZ,
							float &maxX, float &maxY, float &maxZ)const
{
	minX = m_centerX - (m_sizeX/2);
	minY = m_centerY - (m_sizeY/2);
	minZ = m_centerZ - (m_sizeZ/2);
	maxX = m_centerX + (m_sizeX/2);
	maxY = m_centerY + (m_sizeY/2);
	maxZ = m_centerZ + (m_sizeZ/2);
}

//-----------------------------------------------------------------------------

float OctreeCell::GetDiagonalLengthOfCell()const
{
	float minX = m_centerX - (m_sizeX/2);
	float minY = m_centerY - (m_sizeY/2);
	float minZ = m_centerZ - (m_sizeZ/2);
	float maxX = m_centerX + (m_sizeX/2);
	float maxY = m_centerY + (m_sizeY/2);
	float maxZ = m_centerZ + (m_sizeZ/2);

	Point3D minPoint(minX, minY, minZ);
	Point3D maxPoint(maxX, maxY, maxZ);

	float doagonalLength = (float)minPoint.Distance(maxPoint);

	return doagonalLength; 
}

//-----------------------------------------------------------------------------

float OctreeCell::GetMinX()const
{
	return m_centerX - (m_sizeX/2);
}

//-----------------------------------------------------------------------------

float OctreeCell::GetMinY()const
{
	return m_centerY - (m_sizeY/2);
}
//-----------------------------------------------------------------------------

float OctreeCell:: GetMinZ()const
{
	return m_centerZ - (m_sizeZ/2);
}

//-----------------------------------------------------------------------------

float OctreeCell::GetMaxX()const
{
	return m_centerX + (m_sizeX/2);
}
//-----------------------------------------------------------------------------

float OctreeCell::GetMaxY()const
{
	return m_centerY + (m_sizeY/2);
}
//-----------------------------------------------------------------------------

float OctreeCell::GetMaxZ()const
{
	return m_centerZ + (m_sizeZ/2);
}

//-----------------------------------------------------------------------------

OctreeCell* OctreeCell::GetChild(int id)const
{ 
	if(IsLeafCell())
		return 0;

	if(!m_children)
		return 0;

	return &m_children[id];
}

//-----------------------------------------------------------------------------

void OctreeCell::GetGlobalId(std::vector<int > &globalId)const
{
	int depth = GetDepth();
	
	for(int iCount = 1; iCount <= depth; ++iCount)
	{
		int id = ((m_uniqueId >> (64 - 3*iCount)) & 7);
		globalId.push_back(id);
	}
}

//-----------------------------------------------------------------------------

size_t OctreeCell::GetParentId(size_t childId, int &parentDepth)const
{
	int depth = GetDepth();
	parentDepth = depth - 1;

	size_t parentId = childId & DepthIdTable[parentDepth - 1];

	//size_t parentId = m_cellProperty->m_uniqueId >> (64 - 3*parentDepth);

	//parentId = parentId << (64 - 3*parentDepth);

	return parentId;
}

//-----------------------------------------------------------------------------

int OctreeCell::GetChildIdForPoint(float x, float y, float z)const
{	
	int childId = 0;
	if(x >= m_centerX)
		childId = (childId | 1);//set 3rd bit to 1	

	if(y < m_centerY)
		childId = (childId | 2);//set 2nd bit to 1

	if(z >= m_centerZ)
		childId = (childId | 4);//set 1st bit to 1

	return childId;
}

//-----------------------------------------------------------------------------

OctreeCell* OctreeCell::GetCellForPoint(float x, float y, float z)const
{
	if(IsLeafCell())
		return (OctreeCell *)this;
	else
	{
		OctreeCell* octreeCell = 0;
		int id = GetChildIdForPoint(x,y,z);
		octreeCell = m_children[id].GetCellForPoint(x, y, z);
		return octreeCell;
	}	 
}

//-----------------------------------------------------------------------------

double OctreeCell::GetMinDistanceFromCell(float x, float y, float z)const
{	
	double minDist = DBL_MAX;

	double minX = m_centerX - (m_sizeX/2);
	double minY = m_centerY - (m_sizeY/2);
	double minZ = m_centerZ - (m_sizeZ/2);
	double maxX = m_centerX + (m_sizeX/2);
	double maxY = m_centerY + (m_sizeY/2);
	double maxZ = m_centerZ + (m_sizeZ/2);

	int childId = GetChildIdForPoint(x, y, z);
	if(childId == 0)
	{		
		if((x - minX) < minDist)
			minDist = x - minX;
		if((maxY - y) < minDist)
			minDist = maxY - y;
		if((z - minZ) < minDist)
			minDist = z - minZ;		
	}
	else if(childId == 1)
	{
		if((maxX - x) < minDist)
			minDist = maxX - x;
		if((maxY - y) < minDist)
			minDist = maxY - y;
		if((z - minZ) < minDist)
			minDist = z - minZ;	
	}
	else if(childId == 2)
	{	
		if((x - minX) < minDist)
			minDist = x - minX;
		if((y - minY) < minDist)
			minDist = y - minY;
		if((z - minZ) < minDist)
			minDist = z - minZ;	
	}
	else if(childId == 3)
	{		
		if((maxX - x) < minDist)
			minDist = maxX - x;
		if((y - minY) < minDist)
			minDist = y - minY;
		if((z - minZ) < minDist)
			minDist = z - minZ;	
	}
	else if(childId == 4)
	{		
		if((x - minX) < minDist)
			minDist = x - minX;
		if((maxY - y) < minDist)
			minDist = maxY - y;
		if((maxZ - z) < minDist)
			minDist = maxZ - z;	
	}
	else if(childId == 5)
	{
		if((maxX - x) < minDist)
			minDist = maxX - x;
		if((maxY - y) < minDist)
			minDist = maxY - y;
		if((maxZ - z) < minDist)
			minDist = maxZ - z;
	}
	else if(childId == 6)
	{
		if((x - minX) < minDist)
			minDist = x - minX;
		if((y - minY) < minDist)
			minDist = y - minY;
		if((maxZ - z) < minDist)
			minDist = maxZ - z;	
	}
	else if(childId == 7)
	{
		if((maxX - x) < minDist)
			minDist = maxX - x;
		if((y - minY) < minDist)
			minDist = y - minY;
		if((maxZ - z) < minDist)
			minDist = maxZ - z;
	}
	return minDist;
}

//-----------------------------------------------------------------------------

void OctreeCell::GetMinXYZCellsize(double &xCellSize, double &yCellSize, double &zCellSize)const
{
	if(IsLeafCell())
	{	
		if(m_sizeX < xCellSize)
			xCellSize = m_sizeX;

		if(m_sizeY < yCellSize)
			yCellSize = m_sizeY;

		if(m_sizeZ < zCellSize)
			zCellSize = m_sizeZ;
	}
	else
	{
		for(int iCount = 0 ; iCount<8 ; ++iCount)
			m_children[iCount].GetMinXYZCellsize(xCellSize, yCellSize, zCellSize);
	}
}

//-----------------------------------------------------------------------------

//bool OctreeCell::IsBoxIntersecting(const CUBoundingBox3D& box)const
//{
//	double minX, minY, minZ, maxX, maxY, maxZ;
//	box.GetExtents(minX, minY, minZ, maxX, maxY, maxZ);
//
//	double minX1 = m_centerX - (m_sizeX/2);
//	double minY1 = m_centerY - (m_sizeY/2);
//	double minZ1 = m_centerZ - (m_sizeZ/2);
//	double maxX1 = m_centerX + (m_sizeX/2);
//	double maxY1 = m_centerY + (m_sizeY/2);
//	double maxZ1 = m_centerZ + (m_sizeZ/2);
//
//	if( (minX > maxX1 || maxX < minX1) ||
//		(minY > maxY1 || maxY < minY1) ||
//		(minZ > maxZ1 || maxZ < minZ1) )
//		return false;
//
//	return true;
//}

//-----------------------------------------------------------------------------

void OctreeCell::GetNumLeafCells(size_t& numLeafCells)const
{
	if(IsLeafCell())
		numLeafCells++;
	else
	{
		for(int iCount = 0 ; iCount<8 ; ++iCount)
			m_children[iCount].GetNumLeafCells(numLeafCells);
	}
}

//-----------------------------------------------------------------------------

void OctreeCell::ResetVisited()
{
	if(IsLeafCell())
		SetVisited(false);
	else
	{
		for(int iCount = 0 ; iCount<8 ; ++iCount)
			m_children[iCount].ResetVisited();
	}
}

//-----------------------------------------------------------------------------

size_t OctreeCell::GetUniqueId()const
{ 
	assert(IsLeafCell()); return m_uniqueId; 
}

//-----------------------------------------------------------------------------

void OctreeCell::GetAllLeafCells(std::vector< OctreeCell *>& leafCells)
{
	if(IsLeafCell())
	{
		leafCells.push_back(this);
		return;
	}
	else if(m_children)
	{
		for(int iChild = 0; iChild < 8; ++iChild)
		{
			m_children[iChild].GetAllLeafCells(leafCells);
		}
	}
}
//-----------------------------------------------------------------------------

double OctreeCell::GetMinExternalDistanceFromCell(float x, float y, float z)const
{
	float minX = 0.0, minY = 0.0, minZ = 0.0, maxX = 0.0, maxY = 0.0, maxZ = 0.0;
	GetExtents(minX, minY, minZ, maxX, maxY, maxZ);

	float distX = 0.0, distY = 0.0, distZ = 0.0;

	if(x > minX && x < maxX)
		distX = 0.0;
	else
	{
		if(x < minX)
			distX = minX - x;
		if(x > minX)
			distX = x - maxX;
	}

	if(y > minY && y < maxY)
		distY = 0.0;
	else
	{
		if(y < minY)
			distY = minY - y;
		if(y > minY)
			distY = y - maxY;
	}

	if(z > minZ && z < maxZ)
		distZ = 0.0;
	else
	{
		if(distZ < minZ)
			distZ = minZ - z;
		if(z > minZ)
			distZ = z - maxZ;
	}

	double sqrDist = (distX*distX + distY*distY + distZ*distZ);

	return sqrDist;
}

//-----------------------------------------------------------------------------

//get corner points for cell
void OctreeCell::GetCornerPoints(std::vector<CommonUtil::Point3D>& cornerPts)const
{
	cornerPts.resize(8);
	float extents[6];

	//get extends for cell
	GetExtents(extents[0],extents[1],extents[2],extents[3],extents[4],extents[5]);

	//first point
	Point3D firstPt(extents[0],extents[1],extents[2]);
	cornerPts[0] = firstPt;

	//second point
	Point3D secondPt(extents[3],extents[1],extents[2]);
	cornerPts[1] = secondPt;

	//third point
	Point3D thirdPt(extents[0],extents[4],extents[2]);
	cornerPts[2] = thirdPt;

	//forth point
	Point3D forthPt(extents[3],extents[4],extents[2]);
	cornerPts[3] = forthPt;

	//fifth point
	Point3D fifthPt(extents[0],extents[1],extents[5]);
	cornerPts[4] = fifthPt;

	//sixth point
	Point3D sixthPt(extents[3],extents[1],extents[5]);
	cornerPts[5] = sixthPt;

	//seventh point
	Point3D seventhPt(extents[0],extents[4],extents[5]);
	cornerPts[6] = seventhPt;

	//eighth  point
	Point3D eighthPt(extents[3],extents[4],extents[5]);
	cornerPts[7] = eighthPt;
}

//-----------------------------------------------------------------------------

size_t OctreeCell::SizeOf()const
{
	size_t size = sizeof(*this);
	if(IsLeafCell())
		size+= size;
	else
	{
		for(size_t childCount = 0 ; childCount < 8 ; ++childCount)
		{
			size+= GetChild(childCount)->SizeOf();
		}
	}

	if(m_levelOneNeighbours)
	{
		SizeOfVector(*m_levelOneNeighbours);
	}

	return size;
}

//-----------------------------------------------------------------------------
}
