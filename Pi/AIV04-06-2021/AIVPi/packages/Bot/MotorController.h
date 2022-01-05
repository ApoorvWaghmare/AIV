#ifndef MOTORCONTROLLER_H_INCLUDED
#define MOTORCONTROLLER_H_INCLUDED

#include <fstream>

#define CHAN_A2 2
#define CHAN_B2 3

#define CHAN_A1 4
#define CHAN_B1 5

// looking from behind the bot, right motor is motor 1
#define M1_1 26
#define M1_2 27
#define M1_EN 24

#define M2_1 28
#define M2_2 29
#define M2_EN 25

#include "Bot.h"

using namespace std;

class MotorController
{
    public:
        MotorController(Bot* bot);

        virtual ~MotorController();

        // pin setting function
        // forward, forward right, forward left
        void bothWheelsForward(int w1RPMOnScale, int w2RPMOnScale);
        // backward, backward right, backward left
        void bothWheelsBackward(int w1RPMOnScale, int w2RPMOnScale);
        // inplace left
        void w1BackW2Forward(int w1RPMOnScale, int w2RPMOnScale);
        // inplace right
        void w1ForwardW2Backward(int w1RPMOnScale, int w2RPMOnScale);
        // stop motors
        void stopMotors();

        // wheel velocity calculators
        double getW1Velocity();
        double getW2Velocity();

        // get min max motor rpm
        int getMinMotorRPMOnScale();
        int getMaxMotorRPMOnScale();

        // get rpm on scale
        int getW1RPMOnScale();
        int getW2RPMOnScale();

        // syanchronize rpm of both the motors
        void synchronizeBothMotors();

    protected:

    private:
        // member variables

        // speed increasing delay in ms
        size_t m_pulses = 3;

        Bot* m_bot; // bot object

        double m_pulsesPerRotation = 810; //depends on the hall sensor

        // motor rotation direction
        enum DirectionOfRotation
        {
            Backward = -1,
            Stationary,
            Forward
        };

        // min max motor rpm
        size_t m_minRPMOnScale, m_maxRPMOnScale;

         // motor rotation direction
        DirectionOfRotation m_m1DirectionOfRotation = Stationary, m_m2DirectionOfRotation = Stationary;

        // member functions

        void setGPIOPins();

        // rpm calculators
        double getM1RPM();
        double getM2RPM();

        // motor spinning function
        void setMotorRPM(int w1RPMOnScale, int w2RPMOnScale);

        //report file
        std::ofstream m_file;

        size_t m_sigmaPrintCounter = 0;
        double m_m1OmegaSigma = 0, m_m2OmegaSigma = 0;
};


#endif // MOTORCONTROLLER_H_INCLUDED
