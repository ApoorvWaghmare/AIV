#ifndef BOTPROCESSES_H
#define BOTPROCESSES_H

#include <vector>

using namespace std;

namespace CommonUtil
{
    class Point2D;
    class Point3D;
    class Vector;
}

class BotUtility;

class BotProcesses
{
    public:
        BotProcesses();
        virtual ~BotProcesses();

        void initiateBot(const std::string &mapName,
                         const CommonUtil::Point3D &intiationCoors,
                         const CommonUtil::Vector &directionVec);

         void goTo(const CommonUtil::Point2D &target);
         void runPolyLine(const std::vector< CommonUtil::Point3D > &polyLine);
         void scanAlongPolyLine(const std::vector< CommonUtil::Point3D > &polyLine);

         void connectClient();

    protected:

    private:
        BotUtility* m_botUtility;
};

#endif // BOTPROCESSES_H
