#include <iostream>
#include <signal.h>
#include <vector>

#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header

using namespace std;
using namespace rp::standalone::rplidar;

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

class Lidar
{
    public:
        Lidar();
        virtual ~Lidar();
        std::vector <std::vector <float> > getReadings();

    protected:

    private:
        // member variables
        const char* m_opt_com_path = "/dev/ttyUSB0";
        _u32 m_baudrateArray[2] = {115200, 256000};
        _u32 m_opt_com_baudrate = 0;
        u_result m_op_result;
        bool m_useArgcBaudrate = false;
        RPlidarDriver* m_drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
        std::vector <std::vector <float> > m_readings;

        // member functions
        void connectDrvToLid();
        void disposeDriver();
        bool checkLidHealth();
        void startMotor();
        void stopMotor();
        void scan();
};
