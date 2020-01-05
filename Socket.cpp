#include "Socket.h"

void Socket::createSocket(){
	udpSocket = socket(AF_INET, SOCK_DGRAM, PORT);
	err.checking(udpSocket, "server socket");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	checkResult = bind(udpSocket, (const sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
	err.checking(checkResult, "server bind");
}

void Socket::closeSock(){
	close(udpSocket);
}

void Socket::recvMsg(auto &buffer){
	checkResult = recvfrom(udpSocket, &buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &len);
	err.checking(checkResult, "server recvfrom");
}

void Socket::sendMsg(auto &value){
	checkResult = sendto(udpSocket, &value, sizeof(value), 0, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr_in));
	err.checking(checkResult, "server sendto");
}
