#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "Lidar.h"

Lidar::Lidar()
{
    //ctor
    connectDrvToLid();

    if (!checkLidHealth())
        cout << "Error occured with Lidar" << endl;
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
void Lidar::scanAndGetData(std::vector<CommonUtil::Point3D> &readings)
{
    readings.clear();
    readings.reserve(6000);

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
                double theta = degreeToRadian( nodes[pos].angle_z_q14 * 90.f / (1 << 14) );
                double radius  = mmToMeters( nodes[pos].dist_mm_q2 / 4.0f );

                if (radius)
                {
                    CommonUtil::Point3D tempPoint(radius, theta);
                    readings.push_back(tempPoint);
                }
            }
        }
    }

    stopMotor();
}


// degree to radian
double Lidar::degreeToRadian(double degree)
{
    return degree * (M_PI / 180);
}


// mm to meters
double Lidar::mmToMeters(double mm)
{
    return mm / 1000;
}


Lidar::~Lidar()
{
    //dtor
    stopMotor();
    disposeDriver();
}
