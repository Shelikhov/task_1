#include "UdpSock.h"

void UdpSock::createServSock(){
        udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
        err.checking(udpSocket, "server socket");
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = PORT;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        checkResult = bind(udpSocket, (const sockaddr *)&serverAddr, sizeof(serverAddr));
        err.checking(checkResult, "server bind");
	len = sizeof(clientAddr);
}


void UdpSock::createCliSock(){
        udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
        err.checking(udpSocket, "client socket");
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = PORT;
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr = serverAddr;
	len = sizeof(serverAddr);
}

void UdpSock::closeSock(){
        close(udpSocket);
}

void UdpSock::recvMsg(auto &buffer){
        checkResult = recvfrom(udpSocket, &buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &len);
        err.checking(checkResult, "server recvfrom");
}

void UdpSock::sendMsg(auto &value){
        checkResult = sendto(udpSocket, &value, sizeof(value), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
        err.checking(checkResult, "server sendto");
}
