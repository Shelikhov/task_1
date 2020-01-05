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
