#include "../CommonUtilReq/Include/CUPoint2D.h"
#include "../CommonUtilReq/Include/CUBoundingBox2D.h"
#include "../CommonUtilReq/Include/CUGridCell2D.h"
#include "../CommonUtilReq/Include/CUGrid2D.h"
#include "Map.h"

//---------------------------------------------------------------------------------
Map::Map(const CommonUtil::Polygon &mapPolygon) : m_mapPolygon(mapPolygon)
{
    //ctor
    m_grid = new CommonUtil::Grid2D(mapPolygon, m_gridCellSize, m_workSpaceBoundingBoxOffSet);

    // file to dump position on the fly
    m_mapPolygonFile.open("Results/MapPolygon.txt", std::ios::app);
    dumpMapPolygon();

    m_validGridFile.open("Results/ValidGrid.txt", std::ios::app);
    dumpValidGrid();
}

//---------------------------------------------------------------------------------
Map::~Map()
{
    //dtor
    m_mapPolygonFile.close();
}

//---------------------------------------------------------------------------------
bool Map::isPointValid(const CommonUtil::Point2D &point)
{
    bool status = true;

    if (m_grid->getCellContainingPoint(point) == nullptr)
        status = false;

    return status;
}

//---------------------------------------------------------------------------------
CommonUtil::GridCell2D* Map::getCell(size_t xIndex, size_t yIndex)
{
    return m_grid->getGridCell(xIndex, yIndex);
}

//---------------------------------------------------------------------------------
size_t Map::getXSpan()
{
    return m_grid->getXSpan();
}

//---------------------------------------------------------------------------------
size_t Map::getYSpan()
{
    return m_grid->getYSpan();
}

//---------------------------------------------------------------------------------
size_t Map::getGridCellSize()
{
    return m_gridCellSize;
}

//---------------------------------------------------------------------------------
CommonUtil::GridCell2D* Map::getCellContainingPoint(const CommonUtil::Point2D &point)
{
    return m_grid->getCellContainingPoint(point);
}

//---------------------------------------------------------------------------------

size_t Map::getXIndexOfCellContainingPoint(const CommonUtil::Point2D &point)
{
    return m_grid->getXIndexOfCellContainingPoint(point);
}

//---------------------------------------------------------------------------------
size_t Map::getYIndexOfCellContainingPoint(const CommonUtil::Point2D &point)
{
    return m_grid->getYIndexOfCellContainingPoint(point);
}

//---------------------------------------------------------------------------------
void Map::dumpMapPolygon()
{
    std::vector <CommonUtil::Point2D> polygonPoints = m_mapPolygon.GetPointArray();

    for (size_t i = 0; i < polygonPoints.size() - 1; i++)
    {
        CommonUtil::Point2D point1 = polygonPoints[i];
        CommonUtil::Point2D point2 = polygonPoints[i + 1];

        m_mapPolygonFile << "Line\t" << point1.GetX() << "," << point1.GetY() << "," << 0 << "\t"
                         << point2.GetX() << "," << point2.GetY() << "," << 0 << endl;
    }

    m_mapPolygonFile << "Line\t"
                     << polygonPoints[polygonPoints.size() - 1].GetX() << "," << polygonPoints[polygonPoints.size() - 1].GetY() << "," << 0
                     << "\t"
                     << polygonPoints[0].GetX() << "," << polygonPoints[0].GetY() << "," << 0
                     << endl;
}

//---------------------------------------------------------------------------------
void Map::dumpValidGrid()
{
    for (size_t i = 0; i < m_grid->getXSpan(); i++)
    {
        for (size_t j = 0; j < m_grid->getYSpan(); j++)
        {
            std::vector <CommonUtil::Point2D> tempCellVertices;
            tempCellVertices.reserve(4);

            if (m_grid->getGridCell(i, j) != nullptr)
            {
                tempCellVertices = m_grid->getCellVertices(i, j);

                for (size_t k = 0; k < 3; k++)
                {
                    CommonUtil::Point2D point1 = tempCellVertices[k];
                    CommonUtil::Point2D point2 = tempCellVertices[k + 1];

                    m_validGridFile << "Line\t" << point1.GetX() << "," << point1.GetY() << "," << 0 << "\t"
                                    << point2.GetX() << "," << point2.GetY() << "," << 0 << endl;
                }

                m_validGridFile << "Line\t"
                                << tempCellVertices[3].GetX() << "," << tempCellVertices[3].GetY() << "," << 0
                                << "\t"
                                << tempCellVertices[0].GetX() << "," << tempCellVertices[0].GetY() << "," << 0
                                << endl;

               tempCellVertices.clear();
            }
        }
    }
}
