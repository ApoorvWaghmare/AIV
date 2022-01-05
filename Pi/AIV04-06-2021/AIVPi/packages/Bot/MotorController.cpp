#include <iostream>
#include <softPwm.h>
#include <chrono>
#include <cmath>
#include <wiringPi.h>

#include "Bot.h"
#include "MotorController.h"

//----------------------------------------------------------------------------------------
MotorController::MotorController(Bot* bot)
{
    //ctor
    m_bot = bot;

    setGPIOPins();

    m_file.open("Results/MotorController.txt", std::ios::app);
}

//----------------------------------------------------------------------------------------
MotorController::~MotorController()
{
    //dtor
    // stop bot
    digitalWrite(M1_1, 0);
    digitalWrite(M1_2, 0);
    digitalWrite(M2_1, 0);
    digitalWrite(M2_2, 0);

    softPwmWrite(M1_EN, m_minRPMOnScale);
    softPwmWrite(M2_EN, m_minRPMOnScale);

    m_file.close();
}

//----------------------------------------------------------------------------------------
void MotorController::setGPIOPins()
{
    // motor min max rpm
    m_minRPMOnScale = 0;
    m_maxRPMOnScale = 150;

    //for motor 1
    pinMode(M1_1, OUTPUT);
	pinMode(M1_2, OUTPUT);

	softPwmCreate(M1_EN, m_minRPMOnScale, m_maxRPMOnScale);

	//for motor 2
	pinMode(M2_1, OUTPUT);
	pinMode(M2_2, OUTPUT);

	softPwmCreate(M2_EN, m_minRPMOnScale, m_maxRPMOnScale);

    // for moto1 hall sensor
    pinMode(CHAN_A1, INPUT);
    pinMode(CHAN_B1, INPUT);
    // for motor2 hall sensor
    pinMode(CHAN_A2, INPUT);
    pinMode(CHAN_B2, INPUT);
}

//----------------------------------------------------------------------------------------
void MotorController::bothWheelsForward(int w1RPMOnScale, int w2RPMOnScale)
{
    if (m_m1DirectionOfRotation != Forward)
    {
        // m1 forward
        digitalWrite(M1_1, 0);
        digitalWrite(M1_2, 1);

        m_m1DirectionOfRotation = Forward;
    }


    if (m_m2DirectionOfRotation != Forward)
    {
        //m2 forward
        digitalWrite(M2_1, 0);
        digitalWrite(M2_2, 1);

        m_m2DirectionOfRotation = Forward;
    }

    setMotorRPM(w1RPMOnScale, w2RPMOnScale);
}

//----------------------------------------------------------------------------------------
void MotorController::bothWheelsBackward(int w1RPMOnScale, int w2RPMOnScale)
{
    if (m_m1DirectionOfRotation != Backward)
    {
        // m1 backward
        digitalWrite(M1_1, 1);
        digitalWrite(M1_2, 0);

        m_m1DirectionOfRotation = Backward;
    }

    if (m_m2DirectionOfRotation != Backward)
    {
        //m2 backward
        digitalWrite(M2_1, 1);
        digitalWrite(M2_2, 0);

        m_m2DirectionOfRotation = Backward;
    }

    setMotorRPM(w1RPMOnScale, w2RPMOnScale);
}

//----------------------------------------------------------------------------------------
void MotorController::w1BackW2Forward(int w1RPMOnScale, int w2RPMOnScale)
{
    if (m_m1DirectionOfRotation != Backward)
    {
        // m1 backward
        digitalWrite(M1_1, 1);
        digitalWrite(M1_2, 0);

        m_m1DirectionOfRotation = Backward;
    }

    if (m_m2DirectionOfRotation != Forward)
    {
        //m2 forward
        digitalWrite(M2_1, 0);
        digitalWrite(M2_2, 1);

        m_m2DirectionOfRotation = Forward;
    }

    setMotorRPM(w1RPMOnScale, w2RPMOnScale);
}

//----------------------------------------------------------------------------------------
void MotorController::w1ForwardW2Backward(int w1RPMOnScale, int w2RPMOnScale)
{
    if (m_m1DirectionOfRotation != Forward)
    {
        // m1 forward
        digitalWrite(M1_1, 0);
        digitalWrite(M1_2, 1);

        m_m1DirectionOfRotation = Forward;
    }

    if (m_m2DirectionOfRotation != Backward)
    {
        //m2 backward
        digitalWrite(M2_1, 1);
        digitalWrite(M2_2, 0);

        m_m2DirectionOfRotation = Backward;
    }

    setMotorRPM(w1RPMOnScale, w2RPMOnScale);
}

//----------------------------------------------------------------------------------------
void MotorController::stopMotors()
{
    size_t w1RPMOnScale = getW1RPMOnScale();
    size_t w2RPMOnScale = getW2RPMOnScale();

    setMotorRPM(m_minRPMOnScale, m_minRPMOnScale);

    digitalWrite(M1_1, 0);
    digitalWrite(M1_2, 0);
    digitalWrite(M2_1, 0);
    digitalWrite(M2_2, 0);

    m_m1DirectionOfRotation = Stationary;
    m_m2DirectionOfRotation = Stationary;

    m_file << "-----rpm sigma-----" << endl;
    m_file << m_m1OmegaSigma << "\t \t" << m_m2OmegaSigma << endl;
    m_file << "-------------------" << endl;
}

//----------------------------------------------------------------------------------------
void MotorController::setMotorRPM(int w1RPMOnScale, int w2RPMOnScale)
{
    size_t currW1RPMOnScale = getW1RPMOnScale();
    size_t currW2RPMOnScale = getW2RPMOnScale();

    if (currW1RPMOnScale != w1RPMOnScale || currW2RPMOnScale != w2RPMOnScale)
    {
        while (1)
        {
            if (w1RPMOnScale > currW1RPMOnScale)
            {
                currW1RPMOnScale++;
                softPwmWrite(M1_EN, currW1RPMOnScale);
            }
            else if (w1RPMOnScale < currW1RPMOnScale)
            {
                currW1RPMOnScale--;
                softPwmWrite(M1_EN, currW1RPMOnScale);
            }

            if (w2RPMOnScale > currW2RPMOnScale)
            {
                currW2RPMOnScale++;
                softPwmWrite(M2_EN, currW2RPMOnScale);
            }
            else if (w2RPMOnScale < currW2RPMOnScale)
            {
                currW2RPMOnScale--;
                softPwmWrite(M2_EN, currW2RPMOnScale);
            }

            if (currW1RPMOnScale == w1RPMOnScale && currW2RPMOnScale == w2RPMOnScale)
            {
                break;
            }
        }
    }
}

//----------------------------------------------------------------------------------------
void MotorController::synchronizeBothMotors()
{
    if (m_m1OmegaSigma > 10)
    {
        int w1RPMOnScale = getW1RPMOnScale();
        int w2RPMOnScale = getW2RPMOnScale();

        while(1)
        {
            if (abs( m_m1OmegaSigma - m_m2OmegaSigma ) < 10)
            {
                break;
            }
            else if (m_m1OmegaSigma > m_m2OmegaSigma)
            {
                w1RPMOnScale--;
                w2RPMOnScale++;

                softPwmWrite(M1_EN, w1RPMOnScale);
                softPwmWrite(M2_EN, w2RPMOnScale);
            }
            else if (m_m1OmegaSigma < m_m2OmegaSigma)
            {
                w1RPMOnScale++;
                w2RPMOnScale--;

                softPwmWrite(M1_EN, w1RPMOnScale);
                softPwmWrite(M2_EN, w2RPMOnScale);
            }
        }
    }
}

//----------------------------------------------------------------------------------------
double MotorController::getM1RPM()
{
    double RPM = 0;

    if (m_m1DirectionOfRotation != Stationary)
    {
        bool chanPrevState;
        bool chanCurState;

        size_t pulsesCount = 0;

        if(digitalRead(CHAN_A1) == HIGH)
            chanPrevState = true;
        else
            chanPrevState = false;

        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        while(pulsesCount < m_pulses + 1)
        {
            if(digitalRead(CHAN_A1) == HIGH)
                chanCurState = true;
            else
                chanCurState = false;

            if(chanPrevState != chanCurState)
            {
                pulsesCount = pulsesCount + 1;
                chanPrevState = chanCurState;
            }

            std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

            if (duration > 10 && pulsesCount == 0)
                break;
        }

        if (pulsesCount != 0)
        {
            std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();

            double duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
            duration = duration / 1000;

            double pps = m_pulses / duration;
            RPM = (pps / m_pulsesPerRotation) * 60;
        }
    }

    m_file << RPM << "\t";

    m_m1OmegaSigma += abs(RPM);

    return RPM;
}

//----------------------------------------------------------------------------------------
double MotorController::getM2RPM()
{
    double RPM = 0;

    if (m_m2DirectionOfRotation != Stationary)
    {
        bool chanPrevState;
        bool chanCurState;

        size_t pulsesCount = 0;

        if(digitalRead(CHAN_A2) == HIGH)
            chanPrevState = true;
        else
            chanPrevState = false;

        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        while(pulsesCount < m_pulses + 1)
        {
            if(digitalRead(CHAN_A2) == HIGH)
                chanCurState = true;
            else
                chanCurState = false;

            if(chanPrevState != chanCurState)
            {
                pulsesCount = pulsesCount + 1;
                chanPrevState = chanCurState;
            }

            std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

            if (duration > 10 && pulsesCount == 0)
                break;
        }

        if (pulsesCount != 0)
        {
            std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();

            double duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
            duration = duration / 1000;

            double pps = m_pulses / duration;
            RPM = (pps / m_pulsesPerRotation) * 60;
        }
    }

    m_file << "\t" << RPM << endl;

    m_m2OmegaSigma += abs(RPM);

     m_sigmaPrintCounter++;
    if (m_sigmaPrintCounter >= 10)
    {
        m_file << "-----rpm sigma-----" << endl;
        m_file << m_m1OmegaSigma << "\t \t" << m_m2OmegaSigma << endl;
        m_file << "-------------------" << endl;

        m_sigmaPrintCounter = 0;
    }

    return RPM;
}

//----------------------------------------------------------------------------------------
double MotorController::getW1Velocity()
{
    return m_m1DirectionOfRotation * ( (getM1RPM() * 2 * 3.1416 * m_bot->getWheelRadius()) / 60 );
}

//----------------------------------------------------------------------------------------
double MotorController::getW2Velocity()
{
    return m_m2DirectionOfRotation * ( (getM2RPM() * 2 * 3.1416 * m_bot->getWheelRadius()) / 60 );
}

//----------------------------------------------------------------------------------------
int MotorController::getMinMotorRPMOnScale()
{
    return m_minRPMOnScale;
}

//----------------------------------------------------------------------------------------
int MotorController::getMaxMotorRPMOnScale()
{
    return m_maxRPMOnScale;
}

//----------------------------------------------------------------------------------------
int MotorController::getW1RPMOnScale()
{
    return ceil( (getM1RPM() * m_maxRPMOnScale) / m_bot->getMotorMaxRPM() );
}

//----------------------------------------------------------------------------------------
int MotorController::getW2RPMOnScale()
{
    return ceil( (getM2RPM() * m_maxRPMOnScale) / m_bot->getMotorMaxRPM() );
}
