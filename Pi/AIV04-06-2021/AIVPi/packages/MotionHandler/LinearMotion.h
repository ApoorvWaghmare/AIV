#ifndef LINEARMOTION_H
#define LINEARMOTION_H

namespace CommonUtil
{
    class Point3D;
    class Vector;
}

class MotorController;
class ProximitySensor;
class PositionTracker;

class LinearMotion
{
    public:
        LinearMotion(PositionTracker* positionTracker, MotorController* motorController, ProximitySensor* proximitySensor);
        virtual ~LinearMotion();

        // for autonomous traversing
        void travelLinear(CommonUtil::Vector &targetVec);

    protected:

    private:
        // member variables
        PositionTracker* m_positionTracker;

        MotorController* m_motorController;

        ProximitySensor* m_proximitySensor;

        // for autonomous traversing
        void moveForward(CommonUtil::Point3D &targetCoors, CommonUtil::Vector &refVec);

        // check if target is reached
        bool isTargetReached(CommonUtil::Vector &refVec, CommonUtil::Point3D &targetCoors);

        // tolerances
        double m_targetDistTolerance = 0.05;
};

#endif // LINEARMOTION_H
