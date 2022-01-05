#include <wiringPi.h>
#include "ProximitySensor.h"

//---------------------------------------------
ProximitySensor::ProximitySensor()
{
    //ctor
    setGPIOPins();
}

//---------------------------------------------
ProximitySensor::~ProximitySensor()
{
    //dtor
}

//---------------------------------------------
void ProximitySensor::setGPIOPins()
{
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    digitalWrite(TRIG, LOW);
}


double ProximitySensor::getDistance()
{
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG, LOW);

    while(digitalRead(ECHO) == LOW);

    long startTime = micros();
    while(digitalRead(ECHO) == HIGH);
    long travelTime = micros() - startTime;

    int distance = travelTime / 58;

    return distance;
}
