// ============================================================================
//			Copyright (c) 2016 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _GRID2D_H_
#define _GRID2D_H_

// Std Include Files
#pragma warning (disable: 4786)
#include <vector>
#include "CUCommonConstants.h"

namespace CommonUtil
{

class Point2D;
class Segment2D;
class OctreeCell;

// This class represents 2D grid
class Grid2D
{
	// grid limits
	float m_minX, m_maxX; // limits in X direction
	float m_minY, m_maxY; // limits in Y direction

	size_t m_xSpan, m_ySpan;

	// size of a one grid cell
	double m_gridCellSizeX;

	double m_gridCellSizeY;

	std::vector<std::vector<CellStatus>> m_flagStatus;

	void computeGridSpanForPolygonSegment(const Segment2D& segemnt, size_t minIndices[2], 
											size_t maxIndices[2]);

	//add intersecting cell to array
	void setIntersectingCell(size_t indexX, size_t indexY);

	//get cell indices for given octree cell
	bool getCellIndicesFromCellExtend(float minExtend[2], float maxExtend[2], 
										size_t& indexX, size_t& indexY);

public:

	// Default constructor
	Grid2D();

	// Destructor
	~Grid2D();

	//set extends 
	void SetExtents(float minX, float minY, float maxX, float maxY);

	//for given point get cell
	bool GetCellIndicesFromPoint(const Point2D& point, size_t& indexX, size_t& indexY)const;

	//get extend for given grid cell
	void GetExtendForGivenGridCell(size_t indexX, size_t indexY, float minIndices[2],
									float maxIndices[2]);

	void CheckIntersectingCellsInSegment(const Segment2D& segment);

	void CheckIntersectingCellsInSegment1(const Segment2D& segment);

	bool CheckForIntersection(const OctreeCell* octreeCell);

	//get shape status
	CellStatus GetShapeStatusForGivenIndices(size_t indexX, size_t indexY);

	//get span for cell
	void GetSpan(size_t& xSpan, size_t& yspan)const;

	void GetCenterForCell(size_t indexX, size_t indexY, float& centerX, float& centerY);

	//set cell status
	void SetCellStatus(size_t indexX, size_t indexY,CellStatus cellStatus);
};

}
#endif