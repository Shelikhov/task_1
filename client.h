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
		void launch();
		std::thread clientThread;
		std::thread::id threadId;
		int udpSocket;
		struct sockaddr_in serverAddr;
		struct in_addr addr;
		socklen_t len = sizeof(serverAddr);
		void check(int descriptor, const char* str);
};

Client::Client(){
	clientThread = std::thread([this]{launch();});
	threadId = clientThread.get_id();
}

Client::~Client(){
	clientThread.join();
	std::cout << "client thread is free" << std::endl;
}

void Client::launch(){
	std::cout << "Client ID " << threadId << std::endl;
	udpSocket = socket(AF_INET, SOCK_DGRAM, PORT);
	check(udpSocket, "client socket");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	char request[] = "request";
	int count = 30, result;
	int number;
	result = sendto(udpSocket, &request, 7, 0, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
	check(result, "client sendto");
	std::mutex mut;
	while(count > 0){
		std::this_thread::sleep_for(std::chrono::seconds(DELAY));
		--count;
		mut.lock();
		result = recvfrom(udpSocket, &number, sizeof(number), 0, (struct sockaddr*)&serverAddr, &len);
		check(result, "client recvfrom");
		std::cout << number << std::endl;
		result = sendto(udpSocket, &request, 7, 0, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
		check(result, "client sendto");
		mut.unlock();
	}
	std::string endRequest = "end";
	result = sendto(udpSocket, &endRequest, endRequest.size(), 0, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
	check(result, "client sendto");
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
