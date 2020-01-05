#include "Client.h"

Client::Client(){
	clientThread = std::thread([this]{launch();});
	threadId = clientThread.get_id();
}

Client::~Client(){
	clientThread.join();
	std::cout << "client thread is free" << std::endl;
}

void Client::createSocket(){
	udpSocket = socket(AF_INET, SOCK_DGRAM, PORT);
	err.checking(udpSocket, "client socket");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
}

void Client::sendMsg(auto &value){
	checkResult = sendto(udpSocket, &value, sizeof(value), 0, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
	err.checking(checkResult, "client sendto");
}

void Client::recvMsg(auto &buffer){
	checkResult = recvfrom(udpSocket, &buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &len);
	err.checking(checkResult, "client recvfrom");
}

void Client::launch(){
	std::cout << "Client ID " << threadId << std::endl;
	createSocket();
	char request[] = "request";
	int count = 30;
	sendMsg(request);
	std::mutex mut;
	int number;
	while(count > 0){
		std::this_thread::sleep_for(std::chrono::seconds(DELAY));
		--count;
		mut.lock();
		recvMsg(number);
		std::cout << number << std::endl;
		sendMsg(request);
		mut.unlock();
	}
	char end[] = "end";
	sendMsg(end);
	close(udpSocket);
}
