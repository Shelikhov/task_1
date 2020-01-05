#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include "ErrorCheck.h"

#define PORT 17

class Socket{
	public:
		void createSocket();
		void closeSock();
		void recvMsg(auto &buffer);
		void sendMsg(auto &value);
	private:
		int udpSocket;
		struct sockaddr_in serverAddr, clientAddr;
		struct in_addr addr;//network address
		socklen_t len = sizeof(clientAddr);
		int checkResult;
		ErrorCheck err;//to check return value
};
