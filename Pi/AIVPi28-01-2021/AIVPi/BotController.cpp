#include <ctime>
#include <iostream>

#include "BotController.h"

using namespace std;

BotController::BotController()
{
    //ctor
}

void BotController::connectClient()
{
    m_server.createSocket();
    m_server.acceptClient();
}

void BotController::startOperations()
{
    /*
    int direction = 0;

	while(1)
	{
		int tempVal = m_server.getClientData();

		if(m_server.getRecievedBytes() == 0)
			cout<<"Connection failed from client side"<<endl;

		else if(m_server.getRecievedBytes() < -1)
			continue;

		else if(m_server.getRecievedBytes() > 0)
		{
            if(tempVal == 6)
                break;

            direction = tempVal;
		}

		m_botController.runBot(direction);
	}
	*/
}


void BotController::tempStartOperations(int direction, double distance)
{
    if(direction == 1)
        m_linearMotion.moveForward(distance);

    else if(direction == 2)
        m_linearMotion.moveBackward(distance);
}

BotController::~BotController()
{
    //dtor
}
