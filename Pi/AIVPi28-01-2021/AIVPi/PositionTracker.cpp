#include "PositionTracker.h"

PositionTracker::PositionTracker()
{
    //ctor
}

double PositionTracker::getXCoordinate()
{
    return m_xCoordinate;
}

double PositionTracker::getYCoordinate()
{
    return m_yCoordinate;
}

void PositionTracker::updateCoordinates()
{
    m_start = std::chrono::steady_clock::now();
    double velocity = m_velocityCalculator.getVelocityMag();
    m_stop = std::chrono::steady_clock::now();

    double time = std::chrono::duration_cast<std::chrono::milliseconds>(m_stop - m_start).count();
    time = time / 1000;

    double xTravelled = velocity * time;

    m_xCoordinate = m_xCoordinate + xTravelled;

    cout << "Distance travelled = " << m_xCoordinate << endl;
}

PositionTracker::~PositionTracker()
{
    //dtor
}
