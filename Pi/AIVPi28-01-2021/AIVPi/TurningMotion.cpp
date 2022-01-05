#include "TurningMotion.h"

TurningMotion::TurningMotion()
{
    //ctor
    m_wheelBase = m_bot.getWheelBase();
}

double TurningMotion::getCurvatureToCover()
{
    return ( 2 * 3.14 * (m_wheelBase / 2) ) / 4;
}

void TurningMotion::turnRight()
{
    m_motorController.m1Backward();
    m_motorController.m2Forward();

    while(1)
    {
        m_positionTracker.updateCoordinates();

        if(m_positionTracker.getXCoordinate() >= getCurvatureToCover())
        {
            m_motorController.m1Stop();
            m_motorController.m2Stop();
            break;
        }
    }
}

void TurningMotion::turnLeft()
{
    m_motorController.m1Forward();
    m_motorController.m2Backward();

    while(1)
    {
        m_positionTracker.updateCoordinates();

        if(m_positionTracker.getXCoordinate() >= getCurvatureToCover())
        {
            m_motorController.m1Stop();
            m_motorController.m2Stop();
            break;
        }
    }
}

TurningMotion::~TurningMotion()
{
    //dtor
}
