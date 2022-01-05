#include "stdafx.h"
#include "TCPClient.h"
#include <iostream>

using namespace std;

TCPClient::TCPClient()
{
}


TCPClient::TCPConnectionStatus TCPClient::connectToPi()
{
	const char *ipAddress = "192.168.1.102";
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	// Initialize Winsock
	m_iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (m_iResult != 0)
	{
		cout << "WSAStartup failed with error: " << m_iResult << endl;
		return WSA_DATA_STARTUP_FAIL;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	m_iResult = getaddrinfo(ipAddress, DEFAULT_PORT, &hints, &result);
	if (m_iResult != 0)
	{
		cout << "Getaddrinfo failed with error: " << m_iResult << endl;
		return GET_ADD_INFO_FAIL;
		WSACleanup();
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{

		// Create a SOCKET for connecting to server
		m_ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (m_ConnectSocket == INVALID_SOCKET)
		{
			cout << "Socket failed with error: " << WSAGetLastError() << endl;
			WSACleanup();
			return SOCKET_FAIL;
		}

		// Connect to server.
		m_iResult = connect(m_ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (m_iResult == SOCKET_ERROR)
		{
			closesocket(m_ConnectSocket);
			m_ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (m_ConnectSocket == INVALID_SOCKET)
	{
		cout << "Unable to connect to server!" << endl;
		WSACleanup();
		return SERVER_CONNECTION_FAIL;
	}

	return CONNECTION_SUCCESSFUL;
}


void TCPClient::sendInt(int message)
{
	memcpy(m_sendbuf, &message, sizeof(int));
	m_iResult = send(m_ConnectSocket, m_sendbuf, m_sendbuflen, 0);
	if (m_iResult == 0)
		cout << "No message to send" << endl;
	else
		cout << "Connection failed" << endl;
}


double TCPClient::recieveDouble()
{
	double tempVal;

	m_iResult = recv(m_ConnectSocket, m_recvbuf, sizeof(m_recvbuf), 0);
	memcpy(&tempVal, m_recvbuf, sizeof(double));

	return tempVal;
}


int TCPClient::recieveInt()
{
	int tempVal;
	m_iResult = recv(m_ConnectSocket, m_recvbuf, sizeof(m_recvbuf), 0);
	memcpy(&tempVal, m_recvbuf, sizeof(int));

	return tempVal;
}

void TCPClient::endConnection()
{
	// shutdown the connection since no more data will be sent
	m_iResult = shutdown(m_ConnectSocket, SD_SEND);
	if (m_iResult == SOCKET_ERROR)
	{
		cout << "Shutdown failed with error: " << WSAGetLastError() << endl;
		closesocket(m_ConnectSocket);
		WSACleanup();
	}

	// cleanup
	closesocket(m_ConnectSocket);
	WSACleanup();
}


TCPClient::~TCPClient()
{
}