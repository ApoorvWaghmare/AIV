#ifndef POLYLINEPLANNER_H
#define POLYLINEPLANNER_H

namespace CommonUtil
{
    class Point3D;
}

class PoisitionTracker;

class PolyLinePlanner
{
    public:
        PolyLinePlanner(PositionTracker* positionTracker);
        virtual ~PolyLinePlanner();

        CommonUtil::Vector getNextTargetVector(CommonUtil::Point3D targetPoint3D);

    protected:

    private:
        PositionTracker* m_positionTracker;
};

#endif // POLYLINEPLANNER_H
