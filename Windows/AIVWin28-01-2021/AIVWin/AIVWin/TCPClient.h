#pragma once

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 800
#define DEFAULT_PORT "8085"

class TCPClient
{
private:
	char m_recvbuf[DEFAULT_BUFLEN];     //server dumps info in this data structure
	char m_sendbuf[DEFAULT_BUFLEN];     //client writes data to be sent in this array 
	int m_recvbuflen = DEFAULT_BUFLEN;
	int m_sendbuflen = DEFAULT_BUFLEN;
	SOCKET m_ConnectSocket = INVALID_SOCKET;
	int m_iResult;

public:
	enum TCPConnectionStatus
	{
		WSA_DATA_STARTUP_FAIL = 0,
		GET_ADD_INFO_FAIL,
		SOCKET_FAIL,
		SERVER_CONNECTION_FAIL,
		INITIAL_DATA_SEND_FAIL,
		CONNECTION_SUCCESSFUL
	};
	TCPClient();
	~TCPClient();
	TCPConnectionStatus connectToPi();
	void sendInt(int message);
	void endConnection();
	double recieveDouble();
	int recieveInt();
};