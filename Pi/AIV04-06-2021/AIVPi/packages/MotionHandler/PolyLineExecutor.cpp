#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "../CommonUtilReq/Include/CUVector.h"
#include "../Bot/MotorController.h"
#include "../Bot/ProximitySensor.h"
#include "../Cartography/Scanner.h"
#include "Map.h"
#include "../MotionHandler/AstarAlgo.h"
#include "PositionTracker.h"
#include "InPlaceRotation.h"
#include "LinearMotion.h"
#include "PolyLinePlanner.h"
#include "PolyLineExecutor.h"

//----------------------------------------------------------------------------------------------------------
PolyLineExecutor::PolyLineExecutor(Map* mapObj, PositionTracker* positionTracker,
                                   MotorController* motorController, Lidar* lidar, ProximitySensor* proximitySensor)
{
    //ctor
    m_map = mapObj;

    m_astarAlgo = new AstarAlgo(mapObj, positionTracker);

    m_polyLinePlanner = new PolyLinePlanner(positionTracker);

    m_scanner = new Scanner(positionTracker, lidar);

    m_inPlaceRotation = new InPlaceRotation(positionTracker, motorController, proximitySensor);

    m_linearMotion = new LinearMotion(positionTracker, motorController, proximitySensor);

    m_positionTracker = positionTracker;
}

//----------------------------------------------------------------------------------------------------------
PolyLineExecutor::~PolyLineExecutor()
{
    //dtor
}

//----------------------------------------------------------------------------------------------------------
void PolyLineExecutor::travelToTarget(const CommonUtil::Point2D &target)
{
    if (m_map->isPointValid(target))
    {
        std::vector< CommonUtil::Point3D > plannedPolyLine;

        m_astarAlgo->planPath(target, plannedPolyLine);

        for (int i = 0; i < plannedPolyLine.size(); i++)
        {
            CommonUtil::Vector targetVec = m_polyLinePlanner->getNextTargetVector(plannedPolyLine[i]);

            m_inPlaceRotation->rotateInPlace(targetVec);
            m_linearMotion->travelLinear(targetVec);

            m_positionTracker->dumpData();
        }
    }
    else
    {
        cout << "Destination invalid" << endl;
    }
}

//----------------------------------------------------------------------------------------------------------
void PolyLineExecutor::executePolyLine(std::vector< CommonUtil::Point3D > polyLine)
{
    for (int i = 0; i < polyLine.size(); i++)
    {
        CommonUtil::Vector targetVec = m_polyLinePlanner->getNextTargetVector(polyLine[i]);

        m_inPlaceRotation->rotateInPlace(targetVec);
        m_linearMotion->travelLinear(targetVec);

        m_positionTracker->dumpData();
    }
}

//----------------------------------------------------------------------------------------------------------
void PolyLineExecutor::scanAlongPolyLine(std::vector< CommonUtil::Point3D > polyLine)
{
    std::vector< CommonUtil::Point3D > readings;
    size_t numOfBotPosition = polyLine.size();
    readings.reserve(numOfBotPosition * 6000);

    // scanning origin(0, 0)
    std::vector< CommonUtil::Point3D > currentPositionGlobalPoints;

    m_scanner->scan(currentPositionGlobalPoints);

    //readings.insert(readings.end(), currentPositionGlobalPoints.begin(), currentPositionGlobalPoints.end());

    std::string fileName = "Results/LidarReadings_"+ std::to_string(0)+".txt";
    dumpPointData(fileName, currentPositionGlobalPoints);

    for (int i = 0; i < numOfBotPosition; i++)
    {
        CommonUtil::Vector targetVec = m_polyLinePlanner->getNextTargetVector(polyLine[i]);

        m_inPlaceRotation->rotateInPlace(targetVec);
        m_linearMotion->travelLinear(targetVec);

        // scanning
        m_scanner->scan(currentPositionGlobalPoints);

        //readings.insert(readings.end(), currentPositionGlobalPoints.begin(), currentPositionGlobalPoints.end());

        std::string fileName = "Results/LidarReadings_"+ std::to_string(i + 1)+".txt";
        dumpPointData(fileName, currentPositionGlobalPoints);
    }
}

//----------------------------------------------------------------------------------------------------------
void PolyLineExecutor::dumpPointData(std::string &fileName, std::vector< CommonUtil::Point3D > &points)
{
    std::ofstream file;
    file.open(fileName.c_str(), std::ios::app);

    for(int i = 0; i < points.size(); i++)
    {
        const CommonUtil::Point3D &tempPoint = points[i];

        file << tempPoint.GetX() << "," << tempPoint.GetY() << "," << tempPoint.GetZ() << endl;
    }
}
