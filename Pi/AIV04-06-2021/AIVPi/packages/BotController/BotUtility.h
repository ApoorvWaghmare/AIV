#ifndef BOTUTILITY_H
#define BOTUTILITY_H

#include <vector>
#include <string>

using namespace std;

namespace CommonUtil
{
    class Point2D;
    class Point3D;
    class Vector;
}

class Bot;
class Map;
class PositionTracker;
class PolyLineExecutor;
class TCPServer;

class BotUtility
{
    public:
        BotUtility();
        virtual ~BotUtility();

        void initiateUtility(const std::string &mapName,
                             const CommonUtil::Point3D &intiationCoors,
                             const CommonUtil::Vector &directionVec);

        // getter
        Bot* getBot();
        PositionTracker* getPositionTracker();
        PolyLineExecutor* getPolyLineExecutor();
        Map* getMap();
        TCPServer* getServer();

    protected:

    private:
        // bot
        Bot* m_bot;
        // trackers
        PositionTracker* m_positionTracker;
        // motion executors
        PolyLineExecutor* m_polyLineExecutor;
        // map
        Map* m_map;
        // communication
        TCPServer* m_server;

        // member functions
        void buildMap(const std::string &mapName);
        void intiateTracker(const CommonUtil::Point3D &intiationCoors,
                            const CommonUtil::Vector &directionVec);

};

#endif // BOTUTILITY_H
