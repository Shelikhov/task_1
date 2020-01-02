#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Server{
	public:
		Server();
		~Server();
	private:
		void launching();//Creating socket for the exchange data with client.
		std::thread::id threadId;
		std::thread serverThread;
		int udpSocket;
		struct sockaddr_in serverAddr;
		struct in_addr addr;//network address
		void check(int descriptor, const char *str);//To check result of functions such as socket, bind.
};

Server::Server(){
	serverThread = std::thread([this]{launching();});
	threadId = serverThread.get_id();
}

Server::~Server(){
	serverThread.join();
}

void Server::launching(){

/*Creating socket*/
	std::cout << "Server thread ID " << threadId << std::endl;
	udpSocket = socket(AF_LOCAL, SOCK_DGRAM, 17);
	check(udpSocket, "server socket");
//	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = 17;
	serverAddr.sin_addr.s_addr = inet_aton("127.0.0.1", &addr);
	int result = bind(udpSocket, (const sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
	check(result, "socket");
	std::string *str;
	recvfrom(udpSocket, str, 12, 0, NULL, NULL);
	std::cout << *str << std::endl;
	close(udpSocket);
}

void Server::check(int descriptor, const char *str){
	if(descriptor == -1){
		perror(str);
		exit(EXIT_FAILURE);
	}
}
