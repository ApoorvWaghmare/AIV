// ============================================================================
//			Copyright (c) 2016 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _GRID2D_H_
#define _GRID2D_H_

// Std Include Files
#pragma warning (disable: 4786)
#include <fstream>
#include <vector>

#include "CUBoundingBox2D.h"
#include "CUPolygon.h"
#include "CUGrid2D.h"

using namespace std;

namespace CommonUtil
{

class GridCell2D;
class Point2D;

// This class represents 2D grid
class Grid2D
{
private:
	// bounding box
	CommonUtil::BoundingBox2D m_boundingBox;

	CommonUtil::Polygon m_mapPolygon;

	// span
	size_t m_xSpan, m_ySpan;

	// size of a one grid cell
	double m_gridCellSize;

	std::vector< std::vector<GridCell2D*> > m_gridCells;

	// adding grid cells to the grid
	void addGridCells();

	// check if cell is completely inside the polygon
	bool checkCellInsideMapPolygon(const CommonUtil::Point2D &v1, const CommonUtil::Point2D &v2,
                                   const CommonUtil::Point2D &v3, const CommonUtil::Point2D &v4);

    // dump full grid
    std::ofstream m_fullGridFile;

    /*
	void computeGridSpanForPolygonSegment(const Segment2D& segemnt, size_t minIndices[2],
											size_t maxIndices[2]);

	//add intersecting cell to array
	void setIntersectingCell(size_t indexX, size_t indexY);

	//get cell indices for given octree cell
	bool getCellIndicesFromCellExtend(float minExtend[2], float maxExtend[2],
										size_t& indexX, size_t& indexY);
    */

public:

	// Default constructor
	Grid2D(const CommonUtil::Polygon &mapPolygon, double gridCellSize, double boundingBoxOffSet);

	// Destructor
	~Grid2D();

	CommonUtil::GridCell2D* getGridCell(size_t xIndex, size_t yIndex);

	// getters
	size_t getXSpan();
    size_t getYSpan();

    std::vector <CommonUtil::Point2D> getCellVertices(size_t xIndex, size_t yIndex);

    GridCell2D* getCellContainingPoint(const CommonUtil::Point2D &point);

    size_t getXIndexOfCellContainingPoint(const CommonUtil::Point2D &point);
    size_t getYIndexOfCellContainingPoint(const CommonUtil::Point2D &point);

	/*
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
	*/
};

}
#endif
