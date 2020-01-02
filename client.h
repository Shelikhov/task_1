#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

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
		struct sockaddr_in clientAddr;
		struct in_addr addr;
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
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = PORT;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	char msg[] = "request";
	int count = 30, result, number;
	while(count > 0){
		result = sendto(udpSocket, &msg, 10, 0, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr_in));
		check(result, "client sendto");
		result = recvfrom(udpSocket, &number, 4, 0, NULL, NULL);
		check(result, "client recvfrom");
		std::this_thread::sleep_for(std::chrono::seconds(DELAY));
		--count;
	}
	char endMsg[] = "end";
	result = sendto(udpSocket, &endMsg, 10, 0, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr_in));
	check(result, "client sendto");
	close(udpSocket);
}

void Client::check(int descriptor, const char *str){
	if (descriptor == -1){
		perror(str);
		exit(EXIT_FAILURE);
	}
}
