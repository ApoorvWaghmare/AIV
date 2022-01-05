#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H

#include "MotorController.h"
#include "PositionTracker.h"

class MotionController
{
    public:
        MotionController();
        virtual ~MotionController();

    protected:
        MotorController m_motorController;
        PositionTracker m_positionTracker;

    private:
};

#endif // MOTIONCONTROLLER_H
