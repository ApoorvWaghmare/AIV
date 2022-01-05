#include "BotController.h"

using namespace std;

//--------------------------------------------------------------------------------------
BotController::BotController(const std::string &mapName,
                             const CommonUtil::Point3D &intiationCoors,
                             const CommonUtil::Vector &directionVec)
{
    //ctor

    // cartogyaphy
    buildMap(mapName);

    // devices
    m_lidar = new Lidar();
    m_motorController = new MotorController();
    m_proximitySensor = new ProximitySensor();

    // tracker
    intiateTracker(intiationCoors, directionVec);

    // motion executors
    m_polyLineExecutor = new PolyLineExecutor(m_map, m_positionTracker, m_motorController, m_lidar, m_proximitySensor);

    // communication
    m_server = new TCPServer();
}

//--------------------------------------------------------------------------------------
BotController::~BotController()
{
    //dtor
}



//--------------------------------------------------------------------------------------
void BotController::travelToTarget(const CommonUtil::Point2D &target)
{
    if (m_map->isPointValid(target))
        m_polyLineExecutor->travelToTarget(target);
    else
    {
        cout << "Destination invalid" << endl;
    }
}





