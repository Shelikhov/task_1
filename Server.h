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
#include "ErrorCheck.h"

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
		void recvMsg(auto &buffer);
		int udpSocket;
		struct sockaddr_in serverAddr, clientAddr;
		struct in_addr addr;//network address
		socklen_t len = sizeof(clientAddr);
		int checkResult;
		void sendMsg(auto &value);
		ErrorCheck err;//to check return value
};
