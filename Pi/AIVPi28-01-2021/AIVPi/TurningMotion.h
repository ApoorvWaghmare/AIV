#ifndef TURNINGMOTION_H
#define TURNINGMOTION_H

#include "MotionController.h"
#include "Bot.h"

class TurningMotion : public MotionController
{
    public:
        TurningMotion();
        virtual ~TurningMotion();
        void turnRight();
        void turnLeft();

    protected:

    private:
    double getCurvatureToCover();
    double m_wheelBase;
    Bot m_bot;
};

#endif // TURNINGMOTION_H
