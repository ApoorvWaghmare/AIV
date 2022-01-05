#ifndef BOTCONTROLLER_H
#define BOTCONTROLLER_H

#include "TCPServer.h"
#include "LinearMotion.h"

class BotController
{
    public:
        BotController();
        virtual ~BotController();
        void connectClient();
        void startOperations();
        //testing
        void tempStartOperations(int direction, double distance);

    protected:

    private:
        TCPServer m_server;
        LinearMotion m_linearMotion;
};

#endif // BOTCONTROLLER_H
