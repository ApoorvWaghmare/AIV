#include "RPMCalculator.h"

#include <iostream>

using namespace std;

RPMCalculator::RPMCalculator()
{
    //ctor
}

void RPMCalculator::setGPIOPins()
{
    // for moto1
    pinMode(CHAN_A1, INPUT);
    pinMode(CHAN_B1, INPUT);
    // for motor2
    pinMode(CHAN_A2, INPUT);
    pinMode(CHAN_B2, INPUT);
}

double RPMCalculator::getM1RPM(int pulses)
{
    int pulsesCount = 0;

    if(digitalRead(CHAN_A1) == HIGH)
        m_chanPrevState = true;
    else
        m_chanPrevState = false;

    m_start = std::chrono::steady_clock::now();
    while(pulsesCount < pulses + 1)    m_stop = std::chrono::steady_clock::now();

    {
        if(digitalRead(CHAN_A1) == HIGH)
            m_chanCurState = true;
        else
            m_chanCurState = false;

        if(m_chanPrevState != m_chanCurState)
        {
            pulsesCount = pulsesCount + 1;
            m_chanPrevState = m_chanCurState;
        }
    }
    m_stop = std::chrono::steady_clock::now();

    double duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_stop - m_start).count();
    duration = duration / 1000;

    double pps = pulses / duration;
    double RPM = (pps / m_pulsesPerRotation) * 60;

    cout << "RPM = " << RPM << endl;

    return RPM;
}

double RPMCalculator::getM2RPM(int pulses)
{
    int pulsesCount = 0;

    if(digitalRead(CHAN_A2) == HIGH)
        m_chanPrevState = true;
    else
        m_chanPrevState = false;

    m_start = std::chrono::steady_clock::now();
    while(pulsesCount < pulses + 1)
    {
        if(digitalRead(CHAN_A2) == HIGH)
            m_chanCurState = true;
        else
            m_chanCurState = false;

        if(m_chanPrevState != m_chanCurState)
        {
            pulsesCount = pulsesCount + 1;
            m_chanPrevState = m_chanCurState;
        }
    }
    m_stop = std::chrono::steady_clock::now();

    double duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_stop - m_start).count();
    duration = duration / 1000;

    double pps = pulses / duration;
    double RPM = (pps / m_pulsesPerRotation) * 60;

    cout << "RPM = " << RPM << endl;

    return RPM;
}

RPMCalculator::~RPMCalculator()
{
    //dtor
}
