#include "../CommonUtilReq/Include/CUVector.h"
#include "../Bot/Bot.h"
#include "../Bot/ProximitySensor.h"
#include "../Bot/MotorController.h"
#include "PositionTracker.h"
#include "InPlaceRotation.h"

InPlaceRotation::InPlaceRotation(PositionTracker* positionTracker,
                                 MotorController* motorController,
                                 ProximitySensor* proximitySensor)
{
    //ctor
    m_positionTracker = positionTracker;

    m_motorController = motorController;
}

InPlaceRotation::~InPlaceRotation()
{
    //dtor
}


void InPlaceRotation::rotateInPlace(const CommonUtil::Vector &targetVec)
{
    CommonUtil::Vector targetUnitVec = targetVec.GetUnitVector();

    // check if rotation is necessary
    double dotProd;
    bool rotationNecessary = checkIfRotationIsNecessary(targetUnitVec, dotProd);

    if (rotationNecessary == false)
        return;

    // get direction of rotation
    bool directionOfRotation = getDirectionOfRotation(targetUnitVec);

    // rotate
    if (directionOfRotation)
        turnLeft(targetUnitVec, dotProd);

    else
        turnRight(targetUnitVec, dotProd);
}


void InPlaceRotation::turnRight(CommonUtil::Vector targetUnitVec, double &dotProd)
{
    m_motorController->w1BackW2Forward(100, 100);

    while(1)
    {
        m_positionTracker->trackPosition();

        if(checkDirectionAlignment(targetUnitVec, dotProd))
        {
            m_motorController->stopMotors();
            break;
        }
    }
}


void InPlaceRotation::turnLeft(CommonUtil::Vector targetUnitVec, double &dotProd)
{
    m_motorController->w1ForwardW2Backward(90, 90);

    while(1)
    {
        m_positionTracker->trackPosition();

        if(checkDirectionAlignment(targetUnitVec, dotProd))
        {
            m_motorController->stopMotors();
            break;
        }
    }
}


bool InPlaceRotation::checkIfRotationIsNecessary(CommonUtil::Vector targetUnitVec, double &dotProd)
{
    CommonUtil::Vector currDirectionVec = m_positionTracker->getDirectionVector();

    double tempDotProd = currDirectionVec.DotProduct(targetUnitVec);

    if ( abs(tempDotProd) > cos(m_necessaryAngleDeviationforRotation) )
        return false;
    else
    {
        dotProd = tempDotProd;
        return true;
    }
}


bool InPlaceRotation::getDirectionOfRotation(CommonUtil::Vector targetUnitVec)
{
    CommonUtil::Vector currDirectionVec = m_positionTracker->getDirectionVector();

    CommonUtil::Vector crossProd;
    currDirectionVec.CrossProduct(targetUnitVec, crossProd);

    CommonUtil::Vector k(0, 0, 1);

    return crossProd.DotProduct(k) > 0 ? true : false;
}


bool InPlaceRotation::checkDirectionAlignment(CommonUtil::Vector targetUnitVec, double &dotProd)
{
    CommonUtil::Vector currDirectionVec = m_positionTracker->getDirectionVector();

    double tempDotProd = targetUnitVec.DotProduct(currDirectionVec);

    if (tempDotProd > m_dotProdTolerence && tempDotProd < dotProd)
        return true;

    else
    {
        dotProd = tempDotProd;
        return false;
    }
}

