// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _OCTREE_CELL_H_
#define _OCTREE_CELL_H_

// Std Include Files
#pragma warning (disable: 4786)
#include <vector>
#include <assert.h>

class CacheInterface;

namespace CommonUtil
{
// Forward Declarations
class CUBoundingBox3D;
class Point3D;

class OctreeCell
{
	union
	{
		CommonUtil::OctreeCell* m_children;
		size_t m_uniqueId;
	};

	//For Display
	double m_minDepth;
	int m_pixelRectminX, m_pixelRectminY, m_pixelRectmaxX, m_pixelRectmaxY;
	//

	float m_centerX, m_centerY, m_centerZ;
	float m_sizeX, m_sizeY, m_sizeZ;
	size_t m_checkedPointIndex;
	std::vector <CommonUtil::OctreeCell*> *m_levelOneNeighbours;

	//flag  check(for isAllPtSeed(Triangulation))		visited		leafCell		Depth Of Octree
	//bit       8th	 									7th			 6t  		5th,4th,3rd,2nd,1st

	//flag		unused flags					allpointsSelected	allpointsinvalid	 allpointshidden
	//bit		16th 15th 14th 13th 12th		11th				10th				 9th
	unsigned short m_flags;

	void setDepth(int depth){ m_flags |= depth; }	

	void setLeafCell(bool leafCell)
	{
		if(leafCell)
			m_flags |= (1<<5) ;  //set 6th bit to one by oring m_flags with (1<<5)(0000 0000 0010 0000);
		else
			m_flags &= 65503;		//set 6th bit to zero by anding m_flags with 223 (1111 1111 1101 1111);
	}

	void setLevelOneNeighbours(const std::vector <OctreeCell*>& levelOneNeighbours);

	void deleteLevelOneNeighbours();
	OctreeCell()
	{
		m_flags = 0;
		m_uniqueId = 0;
		m_levelOneNeighbours = 0;
		SetChecked(true);
		SetAllPointsVisibility(true);
		m_checkedPointIndex = 0;
		m_minDepth = FLT_MAX;
		m_pixelRectminX = INT_MAX;
		m_pixelRectminY = INT_MAX;
		m_pixelRectmaxX = 0;
		m_pixelRectmaxY = 0;
	}
	OctreeCell(float minX, float minY, float minZ,
			   float maxX, float maxY, float maxZ);

	~OctreeCell();

	void setUniqueId(size_t uniqueId){ m_uniqueId = uniqueId; }

	friend class Octree;

public:

	void GetCenter(float &centerX, float &centerY, float &centerZ)const
	{
		centerX = m_centerX;
		centerY = m_centerY;
		centerZ = m_centerZ;
	};

	void SetPixelRect(int minX, int minY, int maxX, int maxY)
	{
		 m_pixelRectminX = minX;
		 m_pixelRectminY = minY;
		 m_pixelRectmaxX = maxX;
		 m_pixelRectmaxY = maxY;
	}

	void GetPixelRect(int &minX, int &minY, int &maxX, int &maxY)const
	{
		 minX = m_pixelRectminX;
		 minY = m_pixelRectminY;
		 maxX = m_pixelRectmaxX;
		 maxY = m_pixelRectmaxY;
	}

	//Depth means Distance of OctreeCell from screen
	void SetMinDepth(double minDepth)
	{
		m_minDepth = minDepth;
	}

	double GetMinDepth()const
	{
		return m_minDepth;
	}

	//Get Appropriate child id of parent cell for given point
	int GetChildIdForPoint(float x, float y, float z)const;

	float GetAvgCellSize()const{ return (m_sizeX + m_sizeY + m_sizeZ)/3; }

	float GetDiagonalLengthOfCell()const;
	
	float GetSizeX()const{ return m_sizeX; }

	float GetSizeY()const{ return m_sizeY; }

	float GetSizeZ()const{ return m_sizeZ; }

	void GetExtents(float &minX, float &minY, float &minZ, float &maxX, float &maxY, float &maxZ)const;

	float GetMinX()const;
	float GetMinY()const;
	float GetMinZ()const;
						;
	float GetMaxX()const;
	float GetMaxY()const;
	float GetMaxZ()const;

	//get child of given localId
	OctreeCell *GetChild(int id)const;

	//Get GlobalId of cell
	void GetGlobalId(std::vector<int> &globalId)const;

	size_t GetParentId(size_t childId, int &parentDepth)const;

	//check if OctreeCell is leaf cell
	bool IsLeafCell()const{	return ((m_flags & (1<<5)) != 0); } //if anding of m_flags & (1<<5)(0000 0000 0010 0000) is not equal to zero then leafCell flag is true


	void SetVisited(bool visited)
	{
		if(visited)
			m_flags |= (1<<6) ;  //set 7th bit to one by oring m_flags with (1<<6)(0000 0000 0100 0000);
		else
			m_flags &= 65471;		//set 7th bit to zero by anding m_flags with 191(1111 1111 1011 1111);
	}

	bool IsVisited()const{ return ((m_flags & (1<<6)) != 0); } //if anding of m_flags & (1<<6)(0000 0000 0100 0000) is not equal to zero then Visited flag is true

	void SetChecked(bool ckeck)
	{
		if(ckeck)
			m_flags |= (1<<7) ;  //set 8th bit to one by oring m_flags with (1<<7)(0000 0000 1000 0000);
		else
			m_flags &= 65407;		//set 8th bit to zero by anding m_flags with 65407(1111 1111 0111 1111);
	}	
	
	bool IsChecked()const{ return ((m_flags & (1<<7)) != 0); } //if anding of m_flags & (1<<7)(0000 0000 1000 0000) is not equal to zero then checked flag is true

	//For Display
	void SetAllPointsVisibility(bool visible)
	{
		if(visible)
			m_flags |= (1<<8) ;  //set 9th bit to one by oring m_flags with (1<<8)(0000 0001 0000 0000);
		else
			m_flags &= 65279;	 //set 9th bit to zero by anding m_flags with 65279(1111 1110 1111 1111);
	}

	bool GetAllPointsVisibility()const{ return ((m_flags & (1<<8)) != 0); } //if anding of m_flags & (1<<8)(0000 0001 0000 0000) is not equal to zero then checked flag is true

	void SetSelectedFlag(bool selected)
	{
		if(selected)
			m_flags |= (1<<9) ;  //set 9th bit to one by oring m_flags with (1<<9)(0000 0010 0000 0000);
		else
			m_flags &= 65023;	 //set 9th bit to zero by anding m_flags with 65023(1111 1101 1111 1111);
	}

	bool IsSelected()const{ return ((m_flags & (1<<9)) != 0); } //if anding of m_flags & (1<<9)(0000 0010 0000 0000) is not equal to zero then checked flag is true

	std::vector <OctreeCell*>* GetLevelOneNeighbours()const{ return m_levelOneNeighbours; }
	
	unsigned short GetFlags()const{return m_flags;}
	
	void SetFlags(unsigned char flags){m_flags = flags;}

	OctreeCell* GetCellForPoint(float x, float y, float z)const;

	double GetMinDistanceFromCell(float x, float y, float z)const;

	void GetMinXYZCellsize(double &xCellSize, double &yCellSize, double &zCellSize)const;

	//	bool IsBoxIntersecting(const CUBoundingBox3D& box)const;

	void GetNumLeafCells(size_t& numLeafCells)const;

	void ResetVisited();

	size_t GetUniqueId()const;

	void GetAllLeafCells(std::vector< OctreeCell *>& leafCells);

	int GetDepth()const{ return (m_flags & 31); } //31(0000 0000 0001 1111)

	double GetMinExternalDistanceFromCell(float x, float y, float z)const;

	//get corner points for cell
	void GetCornerPoints(std::vector<CommonUtil::Point3D>& cornerPts)const;

	//temp Method
	size_t GetFirstUncheckedPoint()
	{
		return m_checkedPointIndex;
	}

	void SetFirstUncheckedPoint(size_t index)
	{
		m_checkedPointIndex = index;
	}
	size_t SizeOf()const;
};

}
#endif