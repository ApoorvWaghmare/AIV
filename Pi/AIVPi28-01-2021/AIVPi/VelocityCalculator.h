#ifndef VELOCITYCALCULATOR_H
#define VELOCITYCALCULATOR_H

#include <iostream>
#include "RPMCalculator.h"
#include "Bot.h"

using namespace std;

class VelocityCalculator
{
    public:
        VelocityCalculator();
        virtual ~VelocityCalculator();
        double getVelocityMag();       //velocity in m/s

    protected:

    private:
        Bot m_bot;
        double m_wheelRadius;
        RPMCalculator m_rpmCalculator;
};

#endif // VELOCITYCALCULATOR_H
