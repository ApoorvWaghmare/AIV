#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "../CommonUtilReq/Include/CUVector.h"
#include "../Bot/Lidar.h"
#include "PointTransformer.h"
#include "../MotionHandler/PositionTracker.h"
#include "Scanner.h"

//-------------------------------------------------------------------------------------------
Scanner::Scanner(PositionTracker* positionTracker, Lidar* lidar)
{
    //ctor
    m_lidar = lidar;

    m_positionTracker = positionTracker;

    m_pointTransformer = new PointTransformer();
}

//-------------------------------------------------------------------------------------------
Scanner::~Scanner()
{
    //dtor
}

//-------------------------------------------------------------------------------------------
void Scanner::scan(std::vector< CommonUtil::Point3D > &points)
{
    m_lidar->scanAndGetData(points);

    // get current position
    CommonUtil::Point3D currPosition = m_positionTracker->getCurrentCoordinates();
    double angle = getAngleWithX();

    m_pointTransformer->transformPoints(points, currPosition.GetX(), currPosition.GetY(),
                                        currPosition.GetZ(), angle);
}

//-------------------------------------------------------------------------------------------
double Scanner::getAngleWithX()
{
    CommonUtil::Vector currentDirVec = m_positionTracker->getDirectionVector();

    CommonUtil::Vector i(1, 0, 0);

    double angle = currentDirVec.GetAngle(i);

    CommonUtil::Vector signVec;
    i.CrossProduct(currentDirVec, signVec);

    int sign;
    double signVecK = signVec.GetK();
    if (signVecK < 0)
        sign = -1;
    else if (signVecK > 0)
        sign = 1;
    else
        sign = 0;

    return sign * angle;
}
