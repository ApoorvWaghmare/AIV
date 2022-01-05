#include <chrono>
#include <cmath>
#include <iostream>

#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "../Bot/MotorController.h"
#include "PositionTracker.h"

//-------------------------------------------------------------------------------------------
PositionTracker::PositionTracker(const CommonUtil::Point3D &initiationCoordinates,
                                 const CommonUtil::Vector &initiationDirVec,
                                 MotorController* motorController) :
                                 m_currCoords(initiationCoordinates),
                                 m_directionVector(initiationDirVec),
                                 m_motorController(motorController)
{
    double tol;
    m_directionVector.Normalize(tol);

    // file to dump position on the fly
    m_file.open("Results/PositionTrackerOutput.txt", std::ios::app);
}

//-------------------------------------------------------------------------------------------
PositionTracker::~PositionTracker()
{
    //dtor
    m_file.close();
}

//-------------------------------------------------------------------------------------------
void PositionTracker::trackPosition()
{
    CommonUtil::Vector velocityVec = getVeclocityVector();

    velocityVec.ScalarMultiply(m_durationOfMotion);

    CommonUtil::Point3D UpdatedCoors;
    velocityVec.GetPointAlongVector(m_currCoords, UpdatedCoors, velocityVec.GetMagnitude());
    m_currCoords = UpdatedCoors;

    dumpData();
}

//-------------------------------------------------------------------------------------------
CommonUtil::Point3D PositionTracker::getCurrentCoordinates()
{
    return m_currCoords;
}

//-------------------------------------------------------------------------------------------
CommonUtil::Vector PositionTracker::getDirectionVector()
{
    return m_directionVector;
}

//-------------------------------------------------------------------------------------------
CommonUtil::Vector PositionTracker::getVeclocityVector()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    double v1 = m_motorController->getW1Velocity();
    double v2 = m_motorController->getW2Velocity();

    std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();

    double duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    m_durationOfMotion = duration / 1000;

    double omega = (v1 - v2) / m_bot.getWheelBase();
    double velocityMag = (v1 + v2) / 2;

    if (abs(omega) > m_omegaToUpdateDirectionVec)
    {
        double thetaAlongPointOfRotation = omega * m_durationOfMotion;

        CommonUtil::Vector k(0, 0, 1);
        bool directionVectUpdate = false;

        m_directionVector = m_directionVector.GetOtherVectorFromCrossProd(k, thetaAlongPointOfRotation, directionVectUpdate); // updating direction vec

        double tol = 0;
        bool status = m_directionVector.Normalize(tol);
    }

    CommonUtil::Vector velocityVec = m_directionVector;
    velocityVec.ScalarMultiply(velocityMag);

    return velocityVec;
}

//-------------------------------------------------------------------------------------------
void PositionTracker::dumpData()
{
    m_file << "x = " << m_currCoords.GetX() << "\t" << "y = " << m_currCoords.GetY() << endl;
    m_file << "i = " << m_directionVector.GetI() << "\t" << "j = " << m_directionVector.GetJ() << endl;
    m_file << endl;
}

