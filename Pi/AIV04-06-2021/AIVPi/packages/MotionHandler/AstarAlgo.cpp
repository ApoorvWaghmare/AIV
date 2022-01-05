#include <float.h>
#include <iostream>

#include "../CommonUtilReq/Include/CUPoint2D.h"
#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "../CommonUtilReq/Include/CUGridCell2D.h"
#include "Map.h"
#include "PositionTracker.h"
#include "AstarAlgo.h"

//--------------------------------------------------------------------------
AstarAlgo::AstarAlgo(Map* workSpaceMap, PositionTracker* positionTracker)
{
    //ctor
    m_map = workSpaceMap;

    m_positionTracker = positionTracker;

    // file to dump position on the fly
    m_file.open("Results/PlannedPath.txt", std::ios::app);
}

//--------------------------------------------------------------------------
AstarAlgo::~AstarAlgo()
{
    //dtor
}

//--------------------------------------------------------------------------
void AstarAlgo::planPath(const CommonUtil::Point2D &targetPoint, std::vector<CommonUtil::Point3D> &plannedPath)
{
    plannedPath.clear();

    CommonUtil::Point2D currentPoint(m_positionTracker->getCurrentCoordinates().GetX(),
                                     m_positionTracker->getCurrentCoordinates().GetY());

    CommonUtil::GridCell2D* currGridCell = m_map->getCellContainingPoint(currentPoint);

    CommonUtil::Point3D nextPoint3D;

    if (currGridCell != m_map->getCellContainingPoint(targetPoint))
    {
        size_t i = currGridCell->getXIndex();
        size_t j = currGridCell->getYIndex();

        while(1)
        {
            double fCost = DBL_MAX;
            CommonUtil::Point2D nextPoint;

            size_t iNext = i - 1;
            size_t jNext = j - 1;
            if(checkFCost(iNext, jNext, currentPoint, targetPoint, nextPoint, fCost))
                break;

            iNext = i - 1;
            jNext = j;
            if(checkFCost(iNext, jNext, currentPoint, targetPoint, nextPoint, fCost))
                break;

            iNext = i - 1;
            jNext = j + 1;
            if(checkFCost(iNext, jNext, currentPoint, targetPoint, nextPoint, fCost))
                break;

            iNext = i;
            jNext = j + 1;
            if(checkFCost(iNext, jNext, currentPoint, targetPoint, nextPoint, fCost))
                break;

            iNext = i + 1;
            jNext = j + 1;
            if(checkFCost(iNext, jNext, currentPoint, targetPoint, nextPoint, fCost))
                break;

            iNext = i + 1;
            jNext = j;
            if(checkFCost(iNext, jNext, currentPoint, targetPoint, nextPoint, fCost))
                break;

            iNext = i + 1;
            jNext = j - 1;
            if(checkFCost(iNext, jNext, currentPoint, targetPoint, nextPoint, fCost))
                break;

            iNext = i;
            jNext = j - 1;
            if(checkFCost(iNext, jNext, currentPoint, targetPoint, nextPoint, fCost))
                break;

            nextPoint3D.Set(nextPoint.GetX(), nextPoint.GetY(), 0);

            if ( checkPointRequirement(plannedPath, nextPoint3D) )
                plannedPath.push_back(nextPoint3D);

            currentPoint = nextPoint;
            i = m_map->getXIndexOfCellContainingPoint(currentPoint);
            j = m_map->getYIndexOfCellContainingPoint(currentPoint);
        }
    }

    nextPoint3D.Set(targetPoint.GetX(), targetPoint.GetY(), 0);
    plannedPath.push_back(targetPoint);

    dumpData(plannedPath);
}

//--------------------------------------------------------------------------
bool AstarAlgo::checkFCost(const size_t i, size_t j, CommonUtil::Point2D &currentPoint,
                           const CommonUtil::Point2D &targetPoint, CommonUtil::Point2D &nextPoint, double &fCost)
{
    bool status;

    CommonUtil::GridCell2D* cell = m_map->getCell(i, j);

    if (cell != nullptr)
    {
        if (cell == m_map->getCellContainingPoint(targetPoint))
        {
            nextPoint = targetPoint;
            status = true;
        }
        else
        {
            CommonUtil::Point2D cellCenter = cell->getCenter();

            double hCost = cellCenter.Distance(currentPoint);
            double gCost = cellCenter.Distance(targetPoint);
            double tempFcost = hCost + gCost;

            if (tempFcost < fCost)
            {
                nextPoint = cellCenter;
                fCost = tempFcost;
                status = false;
            }
            else
                status = false;
        }
    }
    else
        status = false;

    return status;
}

//--------------------------------------------------------------------------
bool AstarAlgo::checkPointRequirement(const std::vector <CommonUtil::Point3D> &plannedPath,
                                      const CommonUtil::Point3D &pnt)
{
    bool reqSatisfied = false;

    size_t plannedPathSize = plannedPath.size();

    if (plannedPathSize >= 2)
    {
        CommonUtil::Point3D p1 = plannedPath[plannedPathSize - 2];
        CommonUtil::Point3D p2 = plannedPath[plannedPathSize - 1];

        double iniSegSlope = ( p1.GetY() - p2.GetY() ) / ( p1.GetX() - p2.GetX() );
        double currSegSlope = ( p2.GetY() - pnt.GetY() ) / ( p2.GetX() - pnt.GetX() );

        if (abs(iniSegSlope - currSegSlope) > m_nextPntSlopeThresh)
            reqSatisfied = true;
    }
    else
        reqSatisfied = true;

    return reqSatisfied;
}

//--------------------------------------------------------------------------
void AstarAlgo::dumpData(const std::vector< CommonUtil::Point3D > &plannedPath)
{
    for (size_t i = 0; i < plannedPath.size(); i++)
    {
        m_file << "(" << plannedPath[i].GetX() << ", " << plannedPath[i].GetY() << ")" << endl;
    }

    m_file.close();
}

