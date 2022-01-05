#ifndef SCANNER_H
#define SCANNER_H

#include <vector>

using namespace std;

namespace CommonUtil
{
    class Point3D;
    class Vector;
}

class Lidar;
class PointTransformer;
class PositionTracker;

class Scanner
{
    public:
        Scanner(PositionTracker* positionTracker, Lidar* lidar);
        virtual ~Scanner();

        void scan(std::vector< CommonUtil::Point3D > &points);

    protected:

    private:
        // member variables
        Lidar* m_lidar;

        PositionTracker* m_positionTracker;

        PointTransformer* m_pointTransformer;

        // member functions
        double getAngleWithX();
};

#endif // SCANNER_H
