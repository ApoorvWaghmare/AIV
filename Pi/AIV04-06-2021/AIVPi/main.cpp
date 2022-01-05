#include <iostream>
#include <string>
#include <vector>

#include "packages/CommonUtilReq/Include/CUPoint2D.h"
#include "packages/CommonUtilReq/Include/CUPoint3D.h"
#include "packages/CommonUtilReq/Include/CUVector.h"
#include "packages/BotController/BotProcesses.h"

using namespace std;

int main()
{
    BotProcesses* botProcesses = new BotProcesses();

    botProcesses->connectClient();

    std::string mapName = "MyRoom";
    CommonUtil::Point3D intCoors(0.5, 0.5, 0);
    CommonUtil::Vector iniDirVec(1, 0, 0);

    botProcesses->initiateBot(mapName, intCoors, iniDirVec);

    CommonUtil::Point2D target(2, 2);
    //botProcesses->goTo(target);

    CommonUtil::Point3D targetPoint1(1.5, 0.2, 0);
    //CommonUtil::Point3D targetPoint2(2.5, 0.5, 0);
    //CommonUtil::Point3D targetPoint3(2.5, -1.5, 0);

    std::vector< CommonUtil::Point3D > polyline;
    polyline.push_back(targetPoint1);
    //polyline.push_back(targetPoint2);
    //polyline.push_back(targetPoint3);

    //botProcesses->runPolyLine(polyline);

    return 0;
}
