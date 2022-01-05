#include "LinearMotion.h"

LinearMotion::LinearMotion()
{
    //ctor
}

void LinearMotion::moveForward(double distance)
{
    m_motorController.m1Forward();
    m_motorController.m2Forward();

    while(1)
    {
        m_positionTracker.updateCoordinates();

        if(m_positionTracker.getXCoordinate() >= distance)
        {
            m_motorController.m1Stop();
            m_motorController.m2Stop();
            break;
        }
    }
}

void LinearMotion::moveBackward(double distance)
{
    m_motorController.m1Backward();
    m_motorController.m2Backward();

    while(1)
    {
        m_positionTracker.updateCoordinates();

        if(m_positionTracker.getXCoordinate() >= distance)
        {
            m_motorController.m1Stop();
            m_motorController.m2Stop();
            break;
        }
    }
}

LinearMotion::~LinearMotion()
{
    //dtor
}
