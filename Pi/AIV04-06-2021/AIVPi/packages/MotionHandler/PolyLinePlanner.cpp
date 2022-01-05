#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "PositionTracker.h"
#include "PolyLinePlanner.h"

PolyLinePlanner::PolyLinePlanner(PositionTracker* positionTracker)
{
    //ctor
    m_positionTracker = positionTracker;
}

PolyLinePlanner::~PolyLinePlanner()
{
    //dtor
}


CommonUtil::Vector PolyLinePlanner::getNextTargetVector(CommonUtil::Point3D targetPoint3D)
{
    CommonUtil::Point3D currPoint3D = m_positionTracker->getCurrentCoordinates();

    CommonUtil::Vector targetVec(currPoint3D, targetPoint3D);

    return targetVec;
}

