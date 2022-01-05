#ifndef _GRIDCELL2D_H_
#define _GRIDCELL2D_H_

#include "CUPoint2D.h"

namespace CommonUtil
{

class Grid2D;

class GridCell2D
{
    public:
        GridCell2D(Grid2D* grid2D, size_t xIndex, size_t yIndex, CommonUtil::Point2D &center);

        ~GridCell2D();

        // getters
        CommonUtil::Point2D getCenter();

        size_t getXIndex();
        size_t getYIndex();

    protected:

    private:
        // member variables
        Grid2D* m_grid2D;

        size_t m_xIndex, m_yIndex;

        CommonUtil::Point2D m_center;
};

}

#endif
