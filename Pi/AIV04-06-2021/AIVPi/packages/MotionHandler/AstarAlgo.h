#ifndef ASTARALGO_H;
#define ASTARALGO_H

#include <fstream>
#include <vector>

using namespace std;

namespace CommonUtil
{
    class Point2D;
    class Point3D;
    class GridCekk2D;
}

class Map;
class PositionTracker;

class AstarAlgo
{
    public:
        AstarAlgo(Map* workSpaceMap, PositionTracker* positionTracker);
        virtual ~AstarAlgo();

        // path planning function
        void planPath(const CommonUtil::Point2D &targetPoint, std::vector<CommonUtil::Point3D> &plannedPath);

    protected:

    private:
        // member variables
        Map* m_map;

        PositionTracker* m_positionTracker;

        double m_nextPntSlopeThresh = 0.4142135;

        // member functions
        bool checkFCost(const size_t i, size_t j,
                        CommonUtil::Point2D &currentPoint,
                        const CommonUtil::Point2D &targetPoint,
                        CommonUtil::Point2D &nextPoint,
                        double &fCost);

        bool checkPointRequirement(const std::vector<CommonUtil::Point3D> &plannedPath, const CommonUtil::Point3D &pnt);

        //report file
        std::ofstream m_file;

        void dumpData(const std::vector< CommonUtil::Point3D > &plannedPath);
};

#endif // ASTARALGO_H
