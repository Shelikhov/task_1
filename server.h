#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

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
		struct sockaddr_in serverAddr;
		struct in_addr addr;//network address
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
	char msg[10];
	int a = 8;
	result = recvfrom(udpSocket, &msg, 10, 0, NULL, NULL);
	check(result, "server recvfrom");
	while(msg == "request"){
		std::cout << msg << std::endl;
		result = sendto(udpSocket, &a, 4, 0, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
		check(result, "server sendto");
		result = recvfrom(udpSocket, &msg, 10, 0, NULL, NULL);
		check(result, "server recvfrom");
	}
	std::cout << msg << std::endl;
	close(udpSocket);
	return;
}

void Server::check(int descriptor, const char *str){
	if(descriptor == -1){
		perror(str);
		exit(EXIT_FAILURE);
	}
}
