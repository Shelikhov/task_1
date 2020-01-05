#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <mutex>

#define PORT 17
#define DELAY 1

class Client{
	public:
		Client();
		~Client();
	private:
		std::thread clientThread;
		std::thread::id threadId;
		void launch();
		void createSocket();
		void sendReq(auto &value);
		void receiveResp();
		int udpSocket;
		struct sockaddr_in serverAddr;
		struct in_addr addr;
		socklen_t len = sizeof(serverAddr);
		void check(int descriptor, const char* str);
		int checkResult;
};

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
	check(udpSocket, "client socket");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
}

void Client::sendReq(auto &value){
	checkResult = sendto(udpSocket, &value, sizeof(value), 0, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
	check(checkResult, "client sendto");
}

void Client::receiveResp(){
	int number;
	checkResult = recvfrom(udpSocket, &number, sizeof(number), 0, (struct sockaddr*)&serverAddr, &len);
	check(checkResult, "client recvfrom");
	std::cout << number << std::endl;
}

void Client::launch(){
	std::cout << "Client ID " << threadId << std::endl;
	createSocket();
	char request[] = "request";
	int count = 30;
	sendReq(request);
	std::mutex mut;
	while(count > 0){
		std::this_thread::sleep_for(std::chrono::seconds(DELAY));
		--count;
		mut.lock();
		receiveResp();
		sendReq(request);
		mut.unlock();
	}
	char end[] = "end";
	sendReq(end);
	close(udpSocket);
}

void Client::check(int descriptor, const char *str){
	if (descriptor == -1){
		perror(str);
		exit(EXIT_FAILURE);
	}else{
		std::cout << str << " success" << std::endl;
	}
}
