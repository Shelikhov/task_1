#include "Server.h"

Server::Server(){
	serverThread = std::thread([this]{launch();});
	threadId = serverThread.get_id();
}

Server::~Server(){
	serverThread.join();
	std::cout << "server thread is free" << std::endl;
}

void Server::createSocket(){
	udpSocket = socket(AF_INET, SOCK_DGRAM, PORT);
	err.checking(udpSocket, "server socket");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	checkResult = bind(udpSocket, (const sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
	err.checking(checkResult, "server bind");
}

void Server::recvMsg(auto &buffer){
	checkResult = recvfrom(udpSocket, &buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &len);
	err.checking(checkResult, "server recvfrom");
}

void Server::sendMsg(auto &value){
	checkResult = sendto(udpSocket, &value, sizeof(value), 0, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr_in));
	err.checking(checkResult, "server sendto");
}
	
void Server::launch(){
	std::cout << "Server thread ID: " << threadId << std::endl;
	createSocket();
	int number = 8;
	char buffer[512];
	std::string str;
	std::mutex mut;
	while(1){
		mut.lock();
		recvMsg(buffer);
		str = buffer;
		if(str == "request"){
			std::cout << str << std::endl;
		}else{
			std::cout << str << std::endl;
			std::cout << "bad request" << std::endl;
			break;
		}
		sendMsg(number);
		mut.unlock();
	}
	close(udpSocket);
	return;
}
