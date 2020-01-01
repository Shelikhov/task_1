#include <thread>
#include <iostream>

class Server{
	public:
		Server();
		~Server();
	private:
		void launching();
		std::thread::id threadId;
		std::thread serverThread;
};

Server::Server(){
	serverThread = std::thread([this]{launching();});
	threadId = serverThread.get_id();
}

Server::~Server(){
	serverThread.join();
}

void Server::launching(){
	std::cout << "Server thread ID " << threadId << std::endl;
}
