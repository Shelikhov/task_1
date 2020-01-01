#include <iostream>
#include <thread>

class Client{
	public:
		Client();
		~Client();
	private:
		void launching();
		std::thread clientThread;
		std::thread::id threadId;
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
}
