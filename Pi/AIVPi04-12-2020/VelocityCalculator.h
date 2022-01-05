#ifndef VELOCITYCALCULATOR_H
#define VELOCITYCALCULATOR_H

#include <iostream>
#include "RPMCalculator.h"

using namespace std;

class VelocityCalculator
{
    public:
        VelocityCalculator();
        virtual ~VelocityCalculator();
        double getVelocityMag();       //velocity in m/s

    protected:

    private:
        double m_wheelRadius = 0.03835;  //radius in meters
        RPMCalculator m_rpmCalculator;
};

#endif // VELOCITYCALCULATOR_H
