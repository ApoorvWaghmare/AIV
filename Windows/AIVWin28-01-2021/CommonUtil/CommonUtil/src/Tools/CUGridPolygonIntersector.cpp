// ============================================================================
//			Copyright (c) 2016 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include "CUGridPolygonIntersector.h"
#include "CUPolygon.h"
#include "CUSegment2d.h"
#include "CUGrid2D.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------
// Default constructor
GridPolygonIntersector::GridPolygonIntersector()
{
	m_grid = nullptr;
	m_polygon = nullptr;
}

//-----------------------------------------------------------------------------
// Destructor
GridPolygonIntersector::~GridPolygonIntersector()
{
	
}

//-----------------------------------------------------------------------------
void GridPolygonIntersector::checkCellInsideOrOutsidePolygon()
{
	bool inside = false;
	//get span for cells 
	size_t xSpan = 0;
	size_t ySpan = 0;

	//xSpan and ySpan both should be same
	m_grid->GetSpan(xSpan,ySpan);

	for(size_t xSpanCount = 0; xSpanCount < xSpan; ++xSpanCount)
	{
		for(size_t ySpanCount = 0; ySpanCount < ySpan; ++ySpanCount)
		{
			//create index 
			//and check weather intersecting or not
			CellStatus cellStatus = m_grid->GetShapeStatusForGivenIndices(xSpanCount,ySpanCount);

			//if not intersecting
			if(cellStatus != Intersecting)
			{
				//get center of cells
				float centerX = 0.;
				float centerY = 0.;

				m_grid->GetCenterForCell(xSpanCount,ySpanCount,centerX,centerY);

				//check this center points in cells for point in polygon
				Point2D centerPt(centerX,centerY);

				inside = m_polygon->IsPointInPolygon(centerPt);

				CellStatus cellStatus;
				if(inside)
					cellStatus = Inside;
				else
					cellStatus = Outside;

				m_grid->SetCellStatus(xSpanCount,ySpanCount,cellStatus);
			}
		}
	}
}
//-----------------------------------------------------------------------------
//set grid 
void GridPolygonIntersector::SetGrid(Grid2D* grid)
{
	m_grid = grid;
}

//-----------------------------------------------------------------------------
//set polygon
void GridPolygonIntersector::SetPolygon(Polygon* polygon)
{
	m_polygon = polygon;
}

//-----------------------------------------------------------------------------
void GridPolygonIntersector::Excute()
{
	//get polygon points
	std::vector<Point2D> polygonPts;
	polygonPts = m_polygon->GetPointArray();

	//for each segement find intersecting cells from grid
	for(size_t ptCount = 0; ptCount < polygonPts.size(); ++ptCount)
	{
		Point2D startPt;
		Point2D endPt;

		startPt = polygonPts[ptCount];

		if(ptCount != (polygonPts.size() - 1))
			endPt = polygonPts[ptCount+1];
		else
			endPt = polygonPts[0];

		//create segemnt
		Segment2D segment(startPt,endPt);

		//check for intersecting cells
		m_grid->CheckIntersectingCellsInSegment1(segment);
	}

	//check for inside cells
	//get all other cells
	checkCellInsideOrOutsidePolygon();
}

//-----------------------------------------------------------------------------

void GridPolygonIntersector::GetPointStatusInCell(const Point2D& point, bool& intersecting,
													bool& inside, bool& outside)
{
	size_t indexX = -1;
	size_t indexY = -1;

	bool status = m_grid->GetCellIndicesFromPoint(point,indexX,indexY);

	if(status)
	{
		//check in array
		CellStatus cellStatus = m_grid->GetShapeStatusForGivenIndices(indexX,indexY);

		if(cellStatus == Intersecting)
			intersecting = true;

		if(cellStatus == Inside)
			inside = true;
	
		if(cellStatus == Outside)
			outside = true;
	}
}

//-----------------------------------------------------------------------------
}