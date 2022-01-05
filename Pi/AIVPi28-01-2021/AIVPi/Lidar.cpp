#include "Lidar.h"


Lidar::Lidar()
{
    //ctor
    connectDrvToLid();

    if (!checkLidHealth())
        cout << "Error occured with Lidar" << endl;
    else
        cout << "Lidar conected succesfully" << endl;
}


// connect driver to lidar
void Lidar::connectDrvToLid()
{
    if (!m_drv)
    {
        cout << "insufficent memory, exit" << endl;
        exit(-2);
    }

    rplidar_response_device_info_t devinfo;
    bool connectSuccess = false;
    // make connection...
    if (m_useArgcBaudrate)
    {
        if (!m_drv)
            m_drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);

        if (IS_OK(m_drv->connect(m_opt_com_path, m_opt_com_baudrate)))
        {
            m_op_result = m_drv->getDeviceInfo(devinfo);

            if (IS_OK(m_op_result))
            {
                connectSuccess = true;
            }
            else
            {
                delete m_drv;
                m_drv = NULL;
            }
        }
    }
    else
    {
        size_t baudRateArraySize = (sizeof(m_baudrateArray))/ (sizeof(m_baudrateArray[0]));
        for (size_t i = 0; i < baudRateArraySize; ++i)
        {
            if (!m_drv)
                m_drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);

            if (IS_OK(m_drv->connect(m_opt_com_path, m_baudrateArray[i])))
            {
                m_op_result = m_drv->getDeviceInfo(devinfo);

                if (IS_OK(m_op_result))
                {
                    connectSuccess = true;
                    break;
                }
                else
                {
                    delete m_drv;
                    m_drv = NULL;
                }
            }
        }
    }

    if (!connectSuccess)
    {

        cout << "Error, cannot bind to the specified serial port " << m_opt_com_path << endl;
        disposeDriver();
    }
}


// dispose driver
void Lidar::disposeDriver()
{
    RPlidarDriver::DisposeDriver(m_drv);
    m_drv = NULL;
}


// check lidar health
bool Lidar::checkLidHealth()
{
    u_result     m_op_result;
    rplidar_response_device_health_t healthinfo;

    m_op_result = m_drv->getHealth(healthinfo);
    if (IS_OK(m_op_result))
    {
        // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        cout << "RPLidar health status : " << healthinfo.status << endl;
        if (healthinfo.status == RPLIDAR_STATUS_ERROR)
        {
            cout << "Error, rplidar internal error detected. Please reboot the device to retry" << endl;
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        cout << "Error, cannot retrieve the lidar health code : " << m_op_result << endl;
        return false;
    }
}


// start motor
void Lidar::startMotor()
{
    m_drv->startMotor();
}


// stop motor
void Lidar::stopMotor()
{
    m_drv->stopMotor();
}


// scan with lidar
void Lidar::scan()
{
    startMotor();

    m_drv->startScan(0, 1);

    for (int i = 0; i < 10; i++)
    {
        rplidar_response_measurement_node_hq_t nodes[8192];
        size_t   count = _countof(nodes);

        m_op_result = m_drv->grabScanDataHq(nodes, count);
        if (IS_OK(m_op_result))
        {
            m_drv->ascendScanData(nodes, count);
            for (int pos = 0; pos < (int)count ; ++pos)
            {
                std::vector <float> tempVec;

                float angle = nodes[pos].angle_z_q14 * 90.f / (1 << 14);
                float distance  = nodes[pos].dist_mm_q2 / 4.0f;

                tempVec.push_back(angle);
                tempVec.push_back(distance);

                m_readings.push_back(tempVec);
            }
        }
    }

    stopMotor();
}


// get readings
std::vector <std::vector <float> > Lidar::getReadings()
{
    scan();

    return m_readings;
}


Lidar::~Lidar()
{
    //dtor
    stopMotor();
    disposeDriver();
}
