#include "CUGridCell2D.h"
#include "CUGrid2D.h"

namespace CommonUtil
{

//----------------------------------------------------------------------------------------------------------------
GridCell2D::GridCell2D(Grid2D* grid2D, size_t xIndex, size_t yIndex, CommonUtil::Point2D &center) : m_grid2D(grid2D),
                                                                                                    m_xIndex(xIndex),
                                                                                                    m_yIndex(yIndex),
                                                                                                    m_center(center)
{

}

//----------------------------------------------------------------------------------------------------------------
GridCell2D::~GridCell2D()
{

}

//----------------------------------------------------------------------------------------------------------------
CommonUtil::Point2D GridCell2D::getCenter()
{
    return m_center;
}

//----------------------------------------------------------------------------------------------------------------
size_t GridCell2D::getXIndex()
{
    return m_xIndex;
}

//----------------------------------------------------------------------------------------------------------------
size_t GridCell2D::getYIndex()
{
    return m_yIndex;
}

}
