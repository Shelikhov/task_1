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
		void launch();//Creating socket for the exchange data with client.
		std::thread::id threadId;
		std::thread serverThread;
		int udpSocket;
		struct sockaddr_in serverAddr, clientAddr;
		struct in_addr addr;//network address
		socklen_t len = sizeof(clientAddr);
		void check(int descriptor, const char* str);//To check result of functions such as socket, bind.
};

Server::Server(){
	serverThread = std::thread([this]{launch();});
	threadId = serverThread.get_id();
}

Server::~Server(){
	serverThread.join();
	std::cout << "server thread is free" << std::endl;
}

void Server::launch(){

/*Creating socket*/
	std::cout << "Server thread ID " << threadId << std::endl;
	udpSocket = socket(AF_INET, SOCK_DGRAM, PORT);
	check(udpSocket, "server socket");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int result = bind(udpSocket, (const sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
	check(result, "server bind");
	int number = 8;
	std::mutex mut;
		char request[7];
	while(1){
		mut.lock();
		result = recvfrom(udpSocket, &request, 7, 0, (struct sockaddr*)&clientAddr, &len);
		check(result, "server recvfrom");
		if(strcmp(request,"request") == 0){
			std::cout << request << std::endl;
		}else{
			std::cout << "bad request" << std::endl;
			break;
		}
		result = sendto(udpSocket, &number, sizeof(number), 0, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr_in));
		check(result, "server sendto");
		mut.unlock();
	}
	std::cout << request << std::endl;
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
