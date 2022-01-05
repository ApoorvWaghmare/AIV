#ifndef TCPSERVER_H
#define TCPSERVER_H

#define MAX 800
#define PORT 8080
#define SA struct sockaddr

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

class TCPServer
{
    public:
        TCPServer();
        virtual ~TCPServer();
        //declarations needed for tcp server setup
		void createSocket();
		void acceptClient();
		int getClientData();
		void sendData(double val);
		bool getConnectionStatus();
		//checking mechaisms
		int getRecievedBytes();

    protected:

    private:
        //declarations needed for tcp server setup
		int sockfd, connfd;
		socklen_t len;
		struct sockaddr_in servaddr, cli;
		char recvBuff[MAX];
		char sendBuff[MAX];
		int recieveFlags;
		//checking mechanisms
		bool connectionStatus = false;
		int receivedBytes;
};

#endif // TCPSERVER_H
