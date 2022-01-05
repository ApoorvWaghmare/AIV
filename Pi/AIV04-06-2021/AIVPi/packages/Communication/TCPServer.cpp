#include <iostream>
#include <string.h>
#include <fcntl.h>

#include "../CommonUtilReq/Include/CUPoint3D.h"
#include "TCPServer.h"

using namespace std;

//---------------------------------------------------------------------
TCPServer::TCPServer()
{
    //ctor
}

//---------------------------------------------------------------------
TCPServer::~TCPServer()
{
    //dtor
}

//---------------------------------------------------------------------
void TCPServer::createSocket()
{
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        TCPServerStatus connectionStatus = SOCKET_CREATION_ERROR;
    else
        TCPServerStatus connectionStatus = SOCKET_CREATED;

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ( (bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0 )
        TCPServerStatus connectionStatus = SOCKET_BINDING_UNSUCESSFULL;
    else
        TCPServerStatus connectionStatus = SOCKET_BINDING_SUCESSFULL;

    // Now server is ready to listen and verification
    if ( (listen(sockfd, 5)) != 0 )
        TCPServerStatus connectionStatus = SERVER_NOT_LISTENING;
    else
        TCPServerStatus connectionStatus = SERVER_LISTENING;

    len = sizeof(cli);
}

//---------------------------------------------------------------------
void TCPServer::acceptClient()
{
	TCPServerStatus connectionStatus = WAITING_FOR_CLIENT;

	cout << "Waiting for client..." << endl;

	connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd >= 0)
    {
		//setting flags to make the socket non blocking
		recieveFlags = fcntl(sockfd, F_GETFL, 0);
		recieveFlags |= MSG_DONTWAIT;
		recieveFlags &= ~MSG_PEEK;

        connectionStatus = CLIENT_ACCEPTED;

        cout << "Client connected" << endl;
        cout << endl;
    }
    else
		return;
}

//---------------------------------------------------------------------
double TCPServer::recieveDouble()
{
	double clientData;

	char recvBuff[BUFLEN];

	bzero(recvBuff, BUFLEN);

	int receivedBytes = recv(connfd, recvBuff, sizeof(recvBuff), recieveFlags);

	cout <<"recieved bytes = " << receivedBytes << endl;

	memcpy(&clientData, recvBuff, sizeof(double));

	return clientData;
}


//---------------------------------------------------------------------
CommonUtil::Point3D TCPServer::recievePoint3D()
{
    CommonUtil::Point3D pnt;

    double x = recieveDouble();
    double y = recieveDouble();
    double z = recieveDouble();

    pnt.Set(x, y, z);

    return pnt;
}

//---------------------------------------------------------------------
void TCPServer::sendDouble(const double val)
{
    char sendBuff[BUFLEN];

    bzero(sendBuff, BUFLEN);

	memcpy(sendBuff, &val, sizeof(double));

	send(connfd, sendBuff, sizeof(sendBuff), 0);
}

//---------------------------------------------------------------------
void TCPServer::sendPoint3D(const CommonUtil::Point3D &point)
{
    sendDouble(point.GetX());
    sendDouble(point.GetY());
    sendDouble(point.GetZ());
}

CommonUtil::Point3D recievePoint3D();

//---------------------------------------------------------------------
TCPServer::TCPServerStatus TCPServer::getConnectionStatus()
{
	return connectionStatus;
}

