#include <wiringPi.h>

#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "../CommonUtilReq/Include/CUVector.h"
#include "../Bot/MotorController.h"
#include "../Bot/ProximitySensor.h"
#include "PositionTracker.h"
#include "LinearMotion.h"

LinearMotion::LinearMotion(PositionTracker *positionTracker,
                           MotorController* motorController,
                           ProximitySensor* proximitySensor) : m_positionTracker(positionTracker),
                                                               m_motorController(motorController),
                                                               m_proximitySensor(proximitySensor)
{
    //ctor
}

LinearMotion::~LinearMotion()
{
    //dtor
}


void LinearMotion::travelLinear(CommonUtil::Vector &targetVec)
{
    // calculate target coords
    CommonUtil::Point3D targetCoors;
    CommonUtil::Point3D currCoors = m_positionTracker->getCurrentCoordinates();
    targetVec.GetPointAlongVector(currCoors, targetCoors, targetVec.GetMagnitude());

    // get reference vec
    CommonUtil::Vector refVec(targetCoors.GetX() - currCoors.GetX(),
                              targetCoors.GetY() - currCoors.GetY(),
                              targetCoors.GetZ() - currCoors.GetZ());

    moveForward(targetCoors, refVec);
}


void LinearMotion::moveForward(CommonUtil::Point3D &targetCoors, CommonUtil::Vector &refVec)
{
    // setting target vec to direction vec pid controller
    CommonUtil::Vector tempVec = refVec;
    double tol;
    tempVec.Normalize(tol);

    // setting motors
    int m1RPM = m_motorController->getMaxMotorRPMOnScale();
    int m2RPM = m_motorController->getMaxMotorRPMOnScale();

    m_motorController->bothWheelsForward(m1RPM, m2RPM);

    bool obstacleStop = false;
    while(1)
    {
        m_positionTracker->trackPosition();

        if(isTargetReached(refVec, targetCoors))
        {
            m_motorController->stopMotors();
            break;
        }
        else if (obstacleStop == false && m_proximitySensor->getDistance() < 10)
        {
            m_motorController->stopMotors();
            obstacleStop = true;
        }
        else if (obstacleStop == true && m_proximitySensor->getDistance() > 10)
        {
            delay(500);
            m_motorController->bothWheelsForward(m1RPM, m2RPM);
            obstacleStop = false;
        }

        if (obstacleStop == true)
            delay(1000);
    }
}


bool LinearMotion::isTargetReached(CommonUtil::Vector &refVec, CommonUtil::Point3D &targetCoors)
{
    CommonUtil::Point3D currCoors = m_positionTracker->getCurrentCoordinates();
    CommonUtil::Vector currVec(targetCoors.GetX() - currCoors.GetX(),
                               targetCoors.GetY() - currCoors.GetY(),
                               targetCoors.GetZ() - currCoors.GetZ());

    if (refVec.DotProduct(currVec) <= 0)
        return true;
    else
        return false;
}
