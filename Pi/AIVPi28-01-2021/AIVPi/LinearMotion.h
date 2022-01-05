#ifndef LINEARMOTION_H
#define LINEARMOTION_H

#include "MotionController.h"

class LinearMotion : public MotionController
{
    public:
        LinearMotion();
        virtual ~LinearMotion();
        void moveForward(double distance);
        void moveBackward(double distance);

    protected:

    private:
};

#endif // LINEARMOTION_H
