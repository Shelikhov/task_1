#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

class Client{
	public:
		Client();
		~Client();
	private:
		void launching();
		std::thread clientThread;
		std::thread::id threadId;
		int udpSocket;
		struct sockaddr_in clientAddr;
		struct in_addr addr;
		void check(int descriptor, const char *str);
};

Client::Client(){
	clientThread = std::thread([this]{launching();});
	threadId = clientThread.get_id();
}

Client::~Client(){
	clientThread.join();
}

void Client::launching(){
	std::cout << "Client ID " << threadId << std::endl;
	udpSocket = socket(AF_LOCAL, SOCK_DGRAM, 17);
	check(udpSocket, "client socket");
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = 17;
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	std::string str = "anything";
	sendto(udpSocket, &str, 8, 0, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr_in));
	close(udpSocket);
}

void Client::check(int descriptor, const char *str){
	if (descriptor == -1){
		perror(str);
		exit(EXIT_FAILURE);
	}
}
