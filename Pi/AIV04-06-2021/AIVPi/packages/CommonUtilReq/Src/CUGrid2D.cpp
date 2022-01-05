// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// Std Include Files
#include "CUGrid2D.h"
#include "CUPoint2D.h"
#include "CUGridCell2D.h"
//#include "CUsegment2d.h"

#include "CUPoint3D.h"
#include "CUVector.h"
//#include "CUOctreeCell.h"
#include <float.h>

namespace CommonUtil
{
//-----------------------------------------------------------------------------

Grid2D::Grid2D(const CommonUtil::Polygon &mapPolygon, double gridCellSize, double boundingBoxOffSet) : m_mapPolygon(mapPolygon),
                                                                                                       m_gridCellSize(gridCellSize)
{
	m_boundingBox = m_mapPolygon.GetBoundingBox(boundingBoxOffSet);

	m_xSpan = (size_t)((m_boundingBox.GetMaxX() - m_boundingBox.GetMinX()) /  m_gridCellSize ) + 1;
	m_ySpan = (size_t)((m_boundingBox.GetMaxY() - m_boundingBox.GetMinY()) /  m_gridCellSize ) + 1;

    m_fullGridFile.open("Results/FullGrid.txt", std::ios::app);

	addGridCells();
}

//-----------------------------------------------------------------------------
Grid2D::~Grid2D()
{
    m_fullGridFile.close();
}

//-----------------------------------------------------------------------------
void Grid2D::addGridCells()
{
    m_gridCells.resize(m_xSpan);

    for (size_t i = 0; i < m_xSpan; i++)
    {
        m_gridCells[i].resize(m_ySpan, nullptr);
    }

    for (size_t j = 0; j < m_ySpan; j++)
    {
        for (size_t i = 0; i < m_xSpan; i++)
        {
            double xV1 = m_boundingBox.GetMinX() + (i * m_gridCellSize);
            double yV1 = m_boundingBox.GetMinY() + (j * m_gridCellSize);

            CommonUtil::Point2D v1(xV1, yV1),
                                v2(xV1 + m_gridCellSize, yV1),
                                v3(xV1 + m_gridCellSize, yV1 + m_gridCellSize),
                                v4(xV1, yV1 + m_gridCellSize);

            // full grid dump
            m_fullGridFile << "Line\t"
                           << v1.GetX() << "," << v1.GetY() << "," << 0
                           << "\t"
                           << v2.GetX() << "," << v2.GetY() << "," << 0
                           << endl;

            m_fullGridFile << "Line\t"
                           << v2.GetX() << "," << v2.GetY() << "," << 0
                           << "\t"
                           << v3.GetX() << "," << v3.GetY() << "," << 0
                           << endl;

            m_fullGridFile << "Line\t"
                           << v3.GetX() << "," << v3.GetY() << "," << 0
                           << "\t"
                           << v4.GetX() << "," << v4.GetY() << "," << 0
                           << endl;

            m_fullGridFile << "Line\t"
                           << v4.GetX() << "," << v4.GetY() << "," << 0
                           << "\t"
                           << v1.GetX() << "," << v1.GetY() << "," << 0
                           << endl;

            if (checkCellInsideMapPolygon(v1, v2, v3, v4))
            {
                double xCenter = xV1 + (m_gridCellSize / 2);
                double yCenter = yV1 + (m_gridCellSize / 2);

                CommonUtil::Point2D cellCenter(xCenter, yCenter);

                m_gridCells[i][j] = new CommonUtil::GridCell2D(this, i, j, cellCenter);
            }
        }
    }
}

//-----------------------------------------------------------------------------
bool Grid2D::checkCellInsideMapPolygon(const CommonUtil::Point2D &v1, const CommonUtil::Point2D &v2,
                                       const CommonUtil::Point2D &v3, const CommonUtil::Point2D &v4)
{
    bool status = true;

    if (!m_mapPolygon.IsPointInPolygon(v1))
        status = false;
    else if (!m_mapPolygon.IsPointInPolygon(v2))
        status = false;
    else if (!m_mapPolygon.IsPointInPolygon(v3))
        status = false;
    else if (!m_mapPolygon.IsPointInPolygon(v4))
        status = false;

    return status;
}

//-----------------------------------------------------------------------------
CommonUtil::GridCell2D* Grid2D::getGridCell(size_t xIndex, size_t yIndex)
{
    CommonUtil::GridCell2D* gridCellPtr;

    if (xIndex < 0 && xIndex > m_xSpan)
        gridCellPtr = nullptr;
    else if (yIndex < 0 && yIndex > m_ySpan)
        gridCellPtr = nullptr;
    else
        gridCellPtr = m_gridCells[xIndex][yIndex];

    return gridCellPtr;
}

//-----------------------------------------------------------------------------
size_t Grid2D::getXSpan()
{
    return m_xSpan;
}

//-----------------------------------------------------------------------------
size_t Grid2D::getYSpan()
{
    return m_ySpan;
}

//-----------------------------------------------------------------------------
GridCell2D* Grid2D::getCellContainingPoint(const CommonUtil::Point2D &point)
{
    double x = ( point.GetX() - m_boundingBox.GetMinX() ) / m_gridCellSize;
    double y = ( point.GetY() - m_boundingBox.GetMinY() ) / m_gridCellSize;

    size_t i = (size_t) ( x );
    size_t j = (size_t) ( y );

    GridCell2D* cell = nullptr;

    if (i >= 0 && i < m_xSpan && j >= 0 && j < m_ySpan)
        cell = m_gridCells[i][j];

    return cell;
}

//-----------------------------------------------------------------------------
size_t Grid2D::getXIndexOfCellContainingPoint(const CommonUtil::Point2D &point)
{
    double x = ( point.GetX() - m_boundingBox.GetMinX() ) / m_gridCellSize;
    return (size_t) ( x );
}

//-----------------------------------------------------------------------------
size_t Grid2D::getYIndexOfCellContainingPoint(const CommonUtil::Point2D &point)
{
    double y = ( point.GetY() - m_boundingBox.GetMinY() ) / m_gridCellSize;
    return (size_t) ( y );
}

//-----------------------------------------------------------------------------
std::vector <CommonUtil::Point2D> Grid2D::getCellVertices(size_t xIndex, size_t yIndex)
{
    std::vector <CommonUtil::Point2D> cellVertices;
    cellVertices.reserve(4);

    CommonUtil::Point2D center = getGridCell(xIndex, yIndex)->getCenter();

    double tempVal = m_gridCellSize / 2;

    CommonUtil::Point2D tempPoint1(center.GetX() - tempVal, center.GetY() - tempVal);
    cellVertices.push_back(tempPoint1);

    CommonUtil::Point2D tempPoint2(center.GetX() - tempVal, center.GetY() + tempVal);
    cellVertices.push_back(tempPoint2);

    CommonUtil::Point2D tempPoint3(center.GetX() + tempVal, center.GetY() + tempVal);
    cellVertices.push_back(tempPoint3);

    CommonUtil::Point2D tempPoint4(center.GetX() + tempVal, center.GetY() - tempVal);
    cellVertices.push_back(tempPoint4);

    return cellVertices;
}

/*
//-----------------------------------------------------------------------------
void Grid2D::computeGridSpanForPolygonSegment(const Segment2D& segment, size_t minIndices[2], size_t maxIndices[2])
{
	//get start and end points for segment
	Point2D startPt = segment.GetStartPoint();
	Point2D EndPt = segment.GetEndPoint();

	//get indices for given cell
	size_t i1 = 0;
	size_t j1 = 0;

	size_t i2 = 0;
	size_t j2 = 0;

	//get cell indices for a start point
	GetCellIndicesFromPoint(startPt,i1,j1);

	//get cell indices for a end point
	GetCellIndicesFromPoint(EndPt,i2,j2);

	minIndices[0] = i1 > i2 ? i2 : i1;
	maxIndices[0] = i1 > i2 ? i1 : i2;

	minIndices[1] = j1 > j2 ? j2 : j1;
	maxIndices[1] = j1 > j2 ? j1 : j2;
}

//-----------------------------------------------------------------------------
void Grid2D::setIntersectingCell(size_t indexX, size_t indexY)
{
	if(!m_flagStatus.empty())
	{
		m_flagStatus[indexX][indexY] = Intersecting;
	}
}

//-----------------------------------------------------------------------------
//get cell indices for given octree cell
bool Grid2D::getCellIndicesFromCellExtend(float minExtend[2], float maxExtend[2],
												size_t& indexX, size_t& indexY)
{
	//if octree cell is inside the grid
	if(minExtend[0] > m_minX && maxExtend[0] < m_maxX &&
		minExtend[1] > m_minY && maxExtend[1] < m_maxY)
	{
		//for left bottom
		size_t minIndexX = 0;
		size_t minIndexY = 0;

		minIndexX = (size_t)((minExtend[0] - m_minX) / (m_gridCellSizeX));
		minIndexX = (size_t)((minExtend[1] - m_minY) / (m_gridCellSizeY));
	}
	else
	{
		//octree cell larger than a grid


	}

	return true;
}

//-----------------------------------------------------------------------------
void Grid2D::SetExtents(float minX, float minY, float maxX, float maxY)
{
	m_minX = minX;
	m_minY = minY;

	m_maxX = maxX;
	m_maxY = maxY;

	//set grid cellsize
	m_gridCellSizeX = (m_maxX - m_minX)/10;

	m_gridCellSizeY = (m_maxY - m_minY)/10;
}

//-----------------------------------------------------------------------------
//get cell for given point
bool Grid2D::GetCellIndicesFromPoint(const Point2D& point, size_t& indexX, size_t& indexY)const
{
	bool stat = false;

	if(point.GetX() >= m_minX || point.GetY() >= m_minY)
	{
		indexX = (size_t)((point.GetX() - m_minX) / (m_gridCellSizeX));
		indexY = (size_t)((point.GetY() - m_minY) / (m_gridCellSizeY));

		if((indexX != (size_t)-1) && (indexY != (size_t)-1))
		{

			if(indexX == m_xSpan)
				indexX -= 1;

			if(indexY == m_ySpan)
				indexY -= 1;

			if(indexX < m_xSpan && indexY < m_ySpan)
				stat = true;
		}
	}
	return stat;
}

//-----------------------------------------------------------------------------
//get extend for given grid cell
void Grid2D::GetExtendForGivenGridCell(size_t indexX, size_t indexY, float minIndices[2],
									float maxIndices[2])
{
	float minX = float((m_gridCellSizeX * indexX) + m_minX);
	float minY = float((m_gridCellSizeY * indexY) + m_minY);

	float maxX = float(m_gridCellSizeX + minX);
	float maxY = float(m_gridCellSizeX + minY);

	minIndices[0] = minX;
	minIndices[1] = minY;

	maxIndices[0] = maxX;
	maxIndices[1] = maxY;
}

//-----------------------------------------------------------------------------

void Grid2D::CheckIntersectingCellsInSegment(const Segment2D& segment)
{
	size_t minIndices[2];
	size_t maxIndices[2];

	computeGridSpanForPolygonSegment(segment ,minIndices, maxIndices);

	Point2D startPt = segment.GetStartPoint();
	Point2D EndPt = segment.GetEndPoint();

	// starting from first  point of segment and up to second point,
	// compute points on segment at step distance as 1/10th of grid cell size
	// For each point, find cell for point (using GetCellFromCoordinates() )
	// and add set intersecting cell

	//get indices for given cell
	size_t i1 = 0;
	size_t j1 = 0;

	//get cell indices for a start point
	bool stat = GetCellIndicesFromPoint(startPt,i1,j1);

	if(stat)
	{
		//set first cell as a intersecting cell
		setIntersectingCell(i1,j1);
	}

	bool status = false;

	if(stat)
	{
		double stepGridSize = sqrt(m_gridCellSizeX * m_gridCellSizeX + m_gridCellSizeY * m_gridCellSizeY);
		//double stepGridSize = (m_gridCellSizeX * m_gridCellSizeX + m_gridCellSizeY * m_gridCellSizeY);
		double stepDistance = stepGridSize / 10;
		double distance = stepDistance;

		Point2D ptOnEdge;
		Vector vect(startPt,EndPt);

		double length =  segment.GetLength();
		do
		{
			//get point at step disatnce
			vect.GetPointAlongVector(startPt,ptOnEdge,distance);

			//
			size_t xIndex = 0;
			size_t yIndex = 0;

			status = GetCellIndicesFromPoint(ptOnEdge,xIndex,yIndex);

			if(!status)
				break;

			//set cell as intersecting
			setIntersectingCell(xIndex,yIndex);

			distance += stepDistance;

			if(distance > length)
				break;

		}while(distance < length);


		if(status)
		{
			//get cell indices for a start point
			status = GetCellIndicesFromPoint(EndPt,i1,j1);
		}

		if(status)
		{
			//set first cell as a intersecting cell
			setIntersectingCell(i1,j1);
		}
	}
}

//-----------------------------------------------------------------------------
void Grid2D::CheckIntersectingCellsInSegment1(const Segment2D& segment)
{
	bool stat = true;
	while(stat)
	{
		Point2D startPt = segment.GetStartPoint();
		Point2D endPt = segment.GetEndPoint();

		if(startPt.IsEqual(endPt))
			break;

		//get indices for given cell
		size_t startPtCellIndexX = 0;
		size_t startPtCellIndexY = 0;

		//set starting point as intersecting
		stat = GetCellIndicesFromPoint(startPt,startPtCellIndexX,startPtCellIndexY);

		if(stat)
			setIntersectingCell(startPtCellIndexX,startPtCellIndexY);

		size_t endPtCellIndexX = 0;
		size_t endPtCellIndexY = 0;

		stat = GetCellIndicesFromPoint(endPt,endPtCellIndexX,endPtCellIndexY);

		int indexXdiffer = int(endPtCellIndexX - startPtCellIndexX);
		if(indexXdiffer < 0)
			indexXdiffer *= -1;

		int indexYdiffer = int(endPtCellIndexY - startPtCellIndexY);
		if(indexYdiffer < 0)
			indexYdiffer *= -1;

		//check starting pt and end point in same cell
		if(indexXdiffer == 0 && indexYdiffer == 0)
			stat = false;

		if(stat)
			setIntersectingCell(endPtCellIndexX,endPtCellIndexY);

		//check for adjacent cell
		//if cellIndex diiference is one then these cell are adjacent cell
		bool nonAdjacentCell = false;

		if(stat)
		{
			if(indexXdiffer == 1 || indexYdiffer == 1)
			{
				if(indexXdiffer == 1 && indexYdiffer == 1)
					stat = false;
				else if(indexXdiffer == 1 && indexYdiffer == 0)
					stat = false;
				else if(indexXdiffer == 0 && indexYdiffer == 1)
					stat = false;
				else
					nonAdjacentCell = true;
			}
			else
				nonAdjacentCell = true;
		}

		//if index is not adjacent then devide segement and check for
		if(nonAdjacentCell && stat)
		{
			//get center point
			Point2D midPt = segment.GetMidPoint();

			//create two segement
			Segment2D firstSegment(startPt,midPt);
			Segment2D secondSegment(midPt,endPt);

			//follow same procedure
			CheckIntersectingCellsInSegment1(firstSegment);
			CheckIntersectingCellsInSegment1(secondSegment);

			break;
		}
	}
}


//-----------------------------------------------------------------------------
CellStatus Grid2D::GetShapeStatusForGivenIndices(size_t indexX, size_t indexY)
{
	CellStatus shapeStatus;
	if(!m_flagStatus.empty())
	{
		//check in array
		shapeStatus = m_flagStatus[indexX][indexY];
	}

	return shapeStatus;
}

//-----------------------------------------------------------------------------
//get span for cell
void Grid2D::GetSpan(size_t& xSpan, size_t& yspan)const
{
	xSpan = m_xSpan;
	yspan = m_ySpan;
}

//-----------------------------------------------------------------------------
void Grid2D::GetCenterForCell(size_t indexX, size_t indexY, float& centerX, float& centerY)
{
	float minIndices[2] = {0,0};
	float maxIndices[2] = {0,0};

	//get extend for given cell
	GetExtendForGivenGridCell(indexX,indexY,minIndices,maxIndices);

	centerX = (minIndices[0] + maxIndices[0]) / 2;
	centerY = (minIndices[1] + maxIndices[1]) / 2;
}

//-----------------------------------------------------------------------------
void Grid2D::SetCellStatus(size_t indexX, size_t indexY,CellStatus cellStatus)
{
	if(!m_flagStatus.empty())
	{
		//set cell status in array
		m_flagStatus[indexX][indexY] = cellStatus;
	}
}
*/

}
