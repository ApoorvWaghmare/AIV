#include "../CommonUtilReq/Include/CUPoint2D.h"
#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "../CommonUtilReq/Include/CUVector.h"
#include "../Communication/TCPServer.h"
#include "../MotionHandler/PolyLineExecutor.h"
#include "BotUtility.h"
#include "BotProcesses.h"

//--------------------------------------------------------------------------------------
BotProcesses::BotProcesses()
{
    //ctor
    m_botUtility = new BotUtility();
}

//--------------------------------------------------------------------------------------
BotProcesses::~BotProcesses()
{
    //dtor
}

//--------------------------------------------------------------------------------------
void BotProcesses::initiateBot(const std::string &mapName,
                               const CommonUtil::Point3D &intiationCoors,
                               const CommonUtil::Vector &directionVec)
{
    TCPServer* server = m_botUtility->getServer();

    CommonUtil::Point3D startPoint = server->recievePoint3D();

    cout << "point recieved: " << "(" << startPoint.GetX() << ", "
                                      << startPoint.GetY() << ", "
                                      << startPoint.GetZ() << ")" << endl;

    m_botUtility->initiateUtility(mapName,
                                  startPoint,
                                  directionVec);
}

//--------------------------------------------------------------------------------------
void BotProcesses::goTo(const CommonUtil::Point2D &target)
{
    PolyLineExecutor* polyLineExecutor = m_botUtility->getPolyLineExecutor();
    polyLineExecutor->travelToTarget(target);
}

//--------------------------------------------------------------------------------------
void BotProcesses::runPolyLine(const std::vector< CommonUtil::Point3D > &polyLine)
{
    PolyLineExecutor* polyLineExecutor = m_botUtility->getPolyLineExecutor();
    polyLineExecutor->executePolyLine(polyLine);
}

//--------------------------------------------------------------------------------------
void BotProcesses::scanAlongPolyLine(const std::vector< CommonUtil::Point3D > &polyLine)
{
    PolyLineExecutor* polyLineExecutor = m_botUtility->getPolyLineExecutor();
    polyLineExecutor->scanAlongPolyLine(polyLine);
}

//--------------------------------------------------------------------------------------
void BotProcesses::connectClient()
{
    TCPServer* server = m_botUtility->getServer();
    server->createSocket();
    server->acceptClient();
}
