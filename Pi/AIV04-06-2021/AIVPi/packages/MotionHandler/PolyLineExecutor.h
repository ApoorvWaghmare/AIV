#ifndef POLYLINEEXECUTOR_H
#define POLYLINEEXECUTOR_H

#include <vector>
#include <fstream>
#include <string>

namespace CommonUtil
{
    class Point2D;
    class Point3D;
}

class Lidar;
class Map;
class PositionTracker;
class MotorController;
class Lidar;
class ProximitySensor;
class InPlaceRotation;
class LinearMotion;
class Scanner;
class AstarAlgo;
class PolyLinePlanner;

class PolyLineExecutor
{
    public:
        PolyLineExecutor(Map* mapObj, PositionTracker* positionTracker,
                         MotorController* motorController, Lidar* lidar, ProximitySensor* proximitySensor);
        virtual ~PolyLineExecutor();

        void travelToTarget(const CommonUtil::Point2D &target);

        void executePolyLine(std::vector< CommonUtil::Point3D > polyLine);

        void scanAlongPolyLine(std::vector< CommonUtil::Point3D > polyLine);

    protected:

    private:
        // member variables
        Map* m_map;
        AstarAlgo* m_astarAlgo;

        PolyLinePlanner* m_polyLinePlanner;

        Scanner* m_scanner;

        InPlaceRotation* m_inPlaceRotation;

        LinearMotion* m_linearMotion;

        PositionTracker* m_positionTracker; //temp

        // member functions
        void dumpPointData(std::string &fileName, std::vector< CommonUtil::Point3D > &points);
};

#endif // POLYLINEEXECUTOR_H
