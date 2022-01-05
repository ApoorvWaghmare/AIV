#ifndef POSITIONTRACKER_H
#define POSITIONTRACKER_H

#include <chrono>
#include "VelocityCalculator.h"

using namespace std;

class PositionTracker
{
    public:
        PositionTracker();
        virtual ~PositionTracker();
        double getXCoordinate();
        double getYCoordinate();
        void updateCoordinates();

    protected:

    private:
        double m_xCoordinate = 0, m_yCoordinate = 0;
        VelocityCalculator m_velocityCalculator;
        std::chrono::steady_clock::time_point m_start, m_stop;
};

#endif // POSITIONTRACKER_H
