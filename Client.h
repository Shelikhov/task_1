#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <mutex>
#include "ErrorCheck.h"

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
		void sendMsg(auto &value);
		void recvMsg(auto &buffer);
		int udpSocket;
		struct sockaddr_in serverAddr;
		struct in_addr addr;
		socklen_t len = sizeof(serverAddr);
		int checkResult;
		ErrorCheck err;//to check return value
};
