#include "VelocityCalculator.h"

VelocityCalculator::VelocityCalculator()
{
    //ctor
    m_wheelRadius = m_bot.getWheelRadius();
}

double VelocityCalculator::getVelocityMag()
{
    double velocity = (m_rpmCalculator.getM1RPM(5) * 2 * 3.1416 * m_wheelRadius) / 60;
    cout << "Velocity = " << velocity << "m/s" << endl;

    return velocity; //velocity is returned in m/s
}

VelocityCalculator::~VelocityCalculator()
{
    //dtor
}
