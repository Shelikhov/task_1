#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include "ErrorCheck.h"

#define PORT 8080

class ClientSock{
	public:
		void createSocket();
		void closeSock();
		void recvMsg(auto &buffer);
		void sendMsg(auto &value);
	private:
		int udpSocket;
		struct sockaddr_in serverAddr;
		struct in_addr addr;//network address
		int checkResult;
		socklen_t len = sizeof(serverAddr);
		ErrorCheck err;//to check return value
};

void ClientSock::createSocket(){
        udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
        err.checking(udpSocket, "client socket");      
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void ClientSock::closeSock(){
        close(udpSocket);
}

void ClientSock::recvMsg(auto &buffer){
        checkResult = recvfrom(udpSocket, &buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &len);
        err.checking(checkResult, "client recvfrom");
}

void ClientSock::sendMsg(auto &value){
        checkResult = sendto(udpSocket, &value, sizeof(value), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        err.checking(checkResult, "client sendto");
}
