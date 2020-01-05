#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <mutex>
#include <string.h>

#define PORT 17

class Server{
	public:
		Server();
		~Server();
	private:
		std::thread serverThread;
		std::thread::id threadId;
		void launch();//Creating socket for the exchange data with client.
		void createSocket();
		int receiveReq();
		int udpSocket;
		struct sockaddr_in serverAddr, clientAddr;
		struct in_addr addr;//network address
		socklen_t len = sizeof(clientAddr);
		void check(int descriptor, const char* str);//To check result of functions such as socket, bind, recvfrom, sendto.
		int checkResult;
		void sendResp(auto &value);
};

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
	check(udpSocket, "server socket");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	checkResult = bind(udpSocket, (const sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
	check(checkResult, "server bind");
}

int Server::receiveReq(){
	char buffer[512];
	checkResult = recvfrom(udpSocket, &buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &len);
	check(checkResult, "server recvfrom");
	std::string str = buffer;
	if(str == "request"){
		std::cout << str << std::endl;
	}else{
		std::cout << str << std::endl;
		std::cout << "bad request" << std::endl;
		return 0;
	}
	return 1;
}

void Server::sendResp(auto &value){
	checkResult = sendto(udpSocket, &value, sizeof(value), 0, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr_in));
	check(checkResult, "server sendto");
}
	
void Server::launch(){
	std::cout << "Server thread ID: " << threadId << std::endl;
	createSocket();
	int number = 8;
	std::mutex mut;
	while(1){
		mut.lock();
		if(receiveReq() == 0) break;
		sendResp(number);
		mut.unlock();
	}
	close(udpSocket);
	return;
}

void Server::check(int descriptor, const char *str){
	if(descriptor == -1){
		perror(str);
		exit(EXIT_FAILURE);
	}else{
		std::cout << str << " success" << std::endl;
	}
}
