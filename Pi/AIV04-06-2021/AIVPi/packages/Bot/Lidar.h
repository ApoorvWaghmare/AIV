#pragma once

#include <iostream>
#include <signal.h>
#include <vector>

#include "../../../RPLidarSDK/include/rplidar.h"

using namespace std;
using namespace rp::standalone::rplidar;

namespace CommonUtil
{
    class Point3D;
}

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

class Lidar
{
    public:
        Lidar();
        virtual ~Lidar();
        void scanAndGetData(std::vector<CommonUtil::Point3D> &readings);

    protected:

    private:
        // member variables
        const char* m_opt_com_path = "/dev/ttyUSB0";
        _u32 m_baudrateArray[2] = {115200, 256000};
        _u32 m_opt_com_baudrate = 0;
        u_result m_op_result;
        bool m_useArgcBaudrate = false;
        RPlidarDriver* m_drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);

        // member functions
        void connectDrvToLid();
        void disposeDriver();
        bool checkLidHealth();
        void startMotor();
        void stopMotor();
        double degreeToRadian(double degree);
        double mmToMeters(double mm);
};
