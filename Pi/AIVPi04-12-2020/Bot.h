#ifndef BOT_H
#define BOT_H

#include "TCPServer.h"
#include "LinearMotion.h"

class Bot
{
    public:
        Bot();
        virtual ~Bot();
        void connectClient();
        void startOperations();
        //testing
        void tempStartOperations(int direction, double distance);

    protected:

    private:
        TCPServer m_server;
        LinearMotion m_linearMotion;
};

#endif // BOT_H
