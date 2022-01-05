#ifndef MOTORCONTROLLER_H_INCLUDED
#define MOTORCONTROLLER_H_INCLUDED

#include <iostream>
#include "RaspPiInterface.h"

using namespace std;

// looking from behind the bot, right motor is motor 1
#define M1_1 26
#define M1_2 27

#define M2_1 28
#define M2_2 29

using namespace std;

class MotorController : public RaspPiInterface
{
    public:
        MotorController();
        virtual ~MotorController();
        void setGPIOPins();
        void m1Forward();
        void m1Backward();
        void m1Stop();
        void m2Forward();
        void m2Backward();
        void m2Stop();

    protected:

    private:
};


#endif // MOTORCONTROLLER_H_INCLUDED
