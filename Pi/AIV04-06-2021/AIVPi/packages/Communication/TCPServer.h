#ifndef TCPSERVER_H
#define TCPSERVER_H

#define BUFLEN 800000
#define PORT 8085
#define SA struct sockaddr

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

namespace CommonUtil
{
    class Point3D;
}

class TCPServer
{
    public:
        TCPServer();
        virtual ~TCPServer();

        //declarations needed for tcp server setup
		void createSocket();
		void acceptClient();

		// sending and recieving functions
        CommonUtil::Point3D recievePoint3D();
		void sendPoint3D(const CommonUtil::Point3D &point);

    protected:

    private:
        enum TCPServerStatus
        {
            SERVER_DOWN = 0,
            SOCKET_CREATED,
            SOCKET_CREATION_ERROR,
            SOCKET_BINDING_SUCESSFULL,
            SOCKET_BINDING_UNSUCESSFULL,
            SERVER_LISTENING,
            SERVER_NOT_LISTENING,
            WAITING_FOR_CLIENT,
            CLIENT_ACCEPTED
        };

        //declarations needed for tcp server setup
		int sockfd, connfd;
		socklen_t len;
		struct sockaddr_in servaddr, cli;
		int recieveFlags;

		//checking mechanisms
		TCPServerStatus connectionStatus = SERVER_DOWN;

		// member functions
		TCPServerStatus getConnectionStatus();

		double recieveDouble();
		void sendDouble(const double val);
};

#endif // TCPSERVER_H
