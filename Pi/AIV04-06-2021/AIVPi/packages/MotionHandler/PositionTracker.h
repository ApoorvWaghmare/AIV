#ifndef POSITIONTRACKER_H
#define POSITIONTRACKER_H

#include <fstream>

#include "../CommonUtilReq/Include/CUVector.h"
#include "../Bot/Bot.h"

using namespace std;

namespace CommonUtil
{
    class Point3D;
    class Vector;
}

class MotorController;

class PositionTracker
{
    public:
        PositionTracker(const CommonUtil::Point3D &initiationCoordinates,
                        const CommonUtil::Vector &initiationDirVec,
                        MotorController* motorController);

        virtual ~PositionTracker();

        // tracking functions
        void trackPosition();

        // get position
        CommonUtil::Point3D getCurrentCoordinates();

        CommonUtil::Vector getDirectionVector();

        void dumpData();

    protected:

    private:
        // position
        CommonUtil::Point3D m_currCoords;

        CommonUtil::Vector m_directionVector;

        // members
        MotorController* m_motorController;

        Bot m_bot;

        // tolerances
        double m_omegaToUpdateDirectionVec = 0.02;

        // duration of motion in seconds
        double m_durationOfMotion = 0;

        // calculate velocity
        CommonUtil::Vector getVeclocityVector();

        //report file
        std::ofstream m_file;
};

#endif // POSITIONTRACKER_H
