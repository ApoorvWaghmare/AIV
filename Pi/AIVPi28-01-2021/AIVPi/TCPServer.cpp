#include <iostream>
#include <string.h>
#include <fcntl.h>

#include "TCPServer.h"

using namespace std;

TCPServer::TCPServer()
{
    //ctor
}

void TCPServer::createSocket()
{
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        cout<<"Socket connection unsucessful"<<endl;
    }
    else
        cout<<"Socket connection sucessful"<<endl;

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
    {
        cout<<"Socket binding unsucessful"<<endl;
    }
    else
        cout<<"Socket binding sucessful"<<endl;

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        cout<<"Server not listening"<<endl;
    }
    else
        cout<<"Server listening"<<endl;
    len = sizeof(cli);
}

void TCPServer::acceptClient()
{
	cout<<"Waiting for client"<<endl;

	connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd >= 0)
    {
		cout<<"Connection successful"<<endl;
		//setting flags to make the socket non blocking
		recieveFlags = fcntl(sockfd, F_GETFL, 0);
		recieveFlags |= MSG_DONTWAIT;
		recieveFlags &= ~MSG_PEEK;
        connectionStatus = true;
    }
    else
		return;
}

int TCPServer::getClientData()
{
	int clientData;

	bzero(recvBuff, MAX);

	receivedBytes = recv(connfd, recvBuff, sizeof(recvBuff), recieveFlags);

	memcpy(&clientData, recvBuff, sizeof(int));

	return clientData;
}

void TCPServer::sendData(double val)
{
	memcpy(sendBuff, &val, sizeof(double));

	send(connfd, sendBuff, sizeof(sendBuff), 0);
}

bool TCPServer::getConnectionStatus()
{
	return connectionStatus;
}

int TCPServer::getRecievedBytes()
{
	return receivedBytes;
}


TCPServer::~TCPServer()
{
    //dtor
}
