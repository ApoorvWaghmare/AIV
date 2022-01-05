#ifndef RPMCALCULATOR_H
#define RPMCALCULATOR_H

#include <chrono>
#include "RaspPiInterface.h"

#define CHAN_A2 2
#define CHAN_B2 3

#define CHAN_A1 4
#define CHAN_B1 5

using namespace std;

class RPMCalculator : public RaspPiInterface
{
    public:
        RPMCalculator();
        virtual ~RPMCalculator();
        void setGPIOPins();
        double getM1RPM(int pulses);
        double getM2RPM(int pulses);

    protected:

    private:
        std::chrono::steady_clock::time_point m_start, m_stop;
        bool m_chanPrevState;
        bool m_chanCurState;
        double m_pulsesPerRotation = 810; //depends on the hall sensor

};

#endif // RPMCALCULATOR_H
