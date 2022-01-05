#include <ctime>
#include <iostream>
#include <vector>

#include "../CommonUtilReq/Include/CUPoint2D.h"
#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "../CommonUtilReq/Include/CUVector.h"
#include "../Bot/Bot.h"
#include "MapBuilder.h"
#include "Map.h"
#include "../MotionHandler/PositionTracker.h"
#include "../MotionHandler/AstarAlgo.h"
#include "../MotionHandler/PolyLineExecutor.h"
#include "../Communication/TCPServer.h"
#include "BotUtility.h"

//--------------------------------------------------------------------------------------
BotUtility::BotUtility()
{
    //ctor

    // communication
    m_server = new TCPServer();
}

//--------------------------------------------------------------------------------------
BotUtility::~BotUtility()
{
    //dtor
}

//--------------------------------------------------------------------------------------
void BotUtility::initiateUtility(const std::string &mapName,
                                 const CommonUtil::Point3D &intiationCoors,
                                 const CommonUtil::Vector &directionVec)
{
    // bot
    m_bot = new Bot();

    // cartogyaphy
    buildMap(mapName);

    // tracker
    intiateTracker(intiationCoors, directionVec);

    // motion executors
    m_polyLineExecutor = new PolyLineExecutor(m_map,
                                              m_positionTracker,
                                              m_bot->getMotorController(),
                                              m_bot->getLidar(),
                                              m_bot->getProximitySensor());
}

//--------------------------------------------------------------------------------------
void BotUtility::buildMap(const std::string &mapName)
{
    MapBuilder mapBuilder;

    CommonUtil::Polygon mapPolygon;
    mapBuilder.build(mapName, mapPolygon);

    m_map = new Map(mapPolygon);
}

//--------------------------------------------------------------------------------------
void BotUtility::intiateTracker(const CommonUtil::Point3D &intiationCoors, const CommonUtil::Vector &directionVec)
{
    if (m_map->isPointValid(intiationCoors))
        m_positionTracker = new PositionTracker(intiationCoors,
                                                directionVec,
                                                m_bot->getMotorController());

    else
    {
        cout << "Invalid initiation position" << endl;
        return;
    }
}

//--------------------------------------------------------------------------------------
Bot* BotUtility::getBot()
{
    return m_bot;
}

//--------------------------------------------------------------------------------------
PositionTracker* BotUtility::getPositionTracker()
{
    return m_positionTracker;
}

//--------------------------------------------------------------------------------------
PolyLineExecutor* BotUtility::getPolyLineExecutor()
{
    return m_polyLineExecutor;
}

//--------------------------------------------------------------------------------------
Map* BotUtility::getMap()
{
    return m_map;
}

//--------------------------------------------------------------------------------------
TCPServer* BotUtility::getServer()
{
    return m_server;
}
