#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include "ErrorCheck.h"

#define PORT 8080

class ServSock{
	public:
		void createSocket();
		void closeSock();
		void recvMsg(auto &buffer);
		void sendMsg(auto &value);
	private:
		int udpSocket;
		struct sockaddr_in serverAddr, clientAddr;
		struct in_addr addr;//network address
		int checkResult;
		socklen_t len = sizeof(clientAddr);
		ErrorCheck err;//to check return value
};

void ServSock::createSocket(){
        udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
        err.checking(udpSocket, "server socket");
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = PORT;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        checkResult = bind(udpSocket, (const sockaddr *)&serverAddr, sizeof(serverAddr));
        err.checking(checkResult, "server bind");
}

void ServSock::closeSock(){
        close(udpSocket);
}

void ServSock::recvMsg(auto &buffer){
        checkResult = recvfrom(udpSocket, &buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &len);
        err.checking(checkResult, "server recvfrom");
}

void ServSock::sendMsg(auto &value){
        checkResult = sendto(udpSocket, &value, sizeof(value), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
        err.checking(checkResult, "server sendto");
}
