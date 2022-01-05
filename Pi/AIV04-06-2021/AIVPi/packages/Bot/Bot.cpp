#include "../Bot/Lidar.h"
#include "../Bot/MotorController.h"
#include "../Bot/ProximitySensor.h"
#include <wiringPi.h>

#include "Bot.h"

//-----------------------------------
Bot::Bot()
{
    //ctor
    wiringPiSetup();

    m_motorController = new MotorController(this);
    m_lidar = new Lidar();
    m_proximitySensor = new ProximitySensor();
}

//-----------------------------------
Bot::~Bot()
{
    //dtor
}

//-----------------------------------
Lidar* Bot::getLidar()
{
    return m_lidar;
}

//-----------------------------------
MotorController* Bot::getMotorController()
{
    return m_motorController;
}

//-----------------------------------
ProximitySensor* Bot::getProximitySensor()
{
    return m_proximitySensor;
}

//-----------------------------------
double Bot::getWheelRadius()
{
    return m_wheelRadius;
}

//-----------------------------------
double Bot::getWheelBase()
{
    return m_wheelBase;
}

//-----------------------------------
size_t Bot::getMotorMaxRPM()
{
    return m_motorMaxRPM;
}

