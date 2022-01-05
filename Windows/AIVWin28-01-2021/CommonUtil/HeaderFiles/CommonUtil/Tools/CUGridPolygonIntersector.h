// ============================================================================
//			Copyright (c) 2016 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _GRID_POLYGON_INTERSECTOR_H_
#define _GRID_POLYGON_INTERSECTOR_H_

// Std Include Files
#pragma warning (disable: 4786)

namespace CommonUtil
{
class Polygon;
class Grid2D;
class Segment2D;
class Point2D;
class OctreeCell;

// This class is used to find intersection between grid and ploygon. 
//and set gridcell status inside, outside and intersecting.

class GridPolygonIntersector
{
	Grid2D* m_grid;
	Polygon* m_polygon;

	void computeGridSpanForPolygonSegment(const Segment2D& segemnt, size_t minIndices[2], size_t maxIndices[2]);

	//
	void addOrRemoveIntersectingCells(const Segment2D& segment);

	//check for inside cells in polygon
	void checkCellInsideOrOutsidePolygon();

public:

	// Default constructor
	GridPolygonIntersector();

	// Destructor
	~GridPolygonIntersector();

	//set grid 
	void SetGrid(Grid2D* grid);

	//set polygon
	void SetPolygon(Polygon* polygon);

	//
	void Excute();

	//check points is in given intersecting cells
	void GetPointStatusInCell(const Point2D& point, bool& intersecting, bool& inside, bool& outside);

};

}
#endif