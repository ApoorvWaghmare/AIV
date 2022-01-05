#ifndef MAP_H
#define MAP_H

#include <fstream>

#include "../CommonUtilReq/Include/CUPolygon.h"

using namespace std;

namespace CommonUtil
{
    class Point2D;
    class Polygon;
    class BoundingBox2D;
    class GridCell2D;
    class Grid2D;
}

class Map
{
    public:
        // constructor
        Map(const CommonUtil::Polygon &mapPolygon);

        // destructor
        virtual ~Map();

        // check point validity in map
        bool isPointValid(const CommonUtil::Point2D &point);

        // getters
        size_t getXSpan();
        size_t getYSpan();

        size_t getGridCellSize();

        CommonUtil::GridCell2D* getCellContainingPoint(const CommonUtil::Point2D &point);

        CommonUtil::GridCell2D* getCell(size_t xIndex, size_t yIndex);

        size_t getXIndexOfCellContainingPoint(const CommonUtil::Point2D &point);
        size_t getYIndexOfCellContainingPoint(const CommonUtil::Point2D &point);

    protected:

    private:
        // member variables

        CommonUtil::Polygon m_mapPolygon;

        CommonUtil::Grid2D* m_grid;

        // temperorily hard coded
        double m_gridCellSize = 0.2;
        double m_workSpaceBoundingBoxOffSet = 0.1;

        // dump map polygon file
        std::ofstream m_mapPolygonFile;
        void dumpMapPolygon();

        // dump valid grids
        std::ofstream m_validGridFile;
        void dumpValidGrid();
};

#endif // MAP_H
