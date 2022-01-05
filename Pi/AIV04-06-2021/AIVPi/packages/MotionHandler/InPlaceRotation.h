#ifndef INPLACEROTATION_H
#define INPLACEROTATION_H

#include <cmath>

namespace CommonUtil
{
    class Vector;
}

class Bot;
class MotorController;
class PositionTracker;
class ProximitySensor;

class InPlaceRotation
{
    public:
        InPlaceRotation(PositionTracker* positionTracker, MotorController* motorController, ProximitySensor* proximitySensor);
        virtual ~InPlaceRotation();

        // for autonomous traversing
        void rotateInPlace(const CommonUtil::Vector &targetVec);

    protected:

    private:
        // member variables
        PositionTracker* m_positionTracker;
        MotorController* m_motorController;
        ProximitySensor* m_proximitySensor;

        // for autonoumous traversing
        void turnRight(CommonUtil::Vector targetUnitVec, double &dotProd);
        void turnLeft(CommonUtil::Vector targetUnitVec, double &dotProd);

        // check if rotation is necessary
        bool checkIfRotationIsNecessary(CommonUtil::Vector targetUnitVec, double &dotProd);

        // checking direction vec till it matches target vec
        bool checkDirectionAlignment(CommonUtil::Vector targetUnitVec, double &dotProd);

        // get direction in which bot should rotate
        bool getDirectionOfRotation(CommonUtil::Vector targetUnitVec);

        // tolerances
        double m_necessaryAngleDeviationforRotation = 0.0349066;
        double m_dotProdTolerence = 0.9;
};

#endif // INPLACEROTATION_H
