#ifndef BOT_H
#define BOT_H

#include <iostream>

using namespace std;

class Lidar;
class MotorController;
class ProximitySensor;

class Bot
{
    public:
        Bot();
        virtual ~Bot();

        // getters
        double getWheelRadius();
        double getWheelBase();
        size_t getMotorMaxRPM();

        Lidar* getLidar();
        MotorController* getMotorController();
        ProximitySensor* getProximitySensor();

    protected:

    private:
        // physical parameters
        //all distances in meters
        double m_wheelRadius = 0.056;
        double m_wheelBase = 0.27;
        size_t m_motorMaxRPM = 75;

        // devices
        Lidar* m_lidar;
        MotorController* m_motorController;
        ProximitySensor* m_proximitySensor;
};

#endif // BOT_H
