#include <chrono>
#include "UdpSock.cpp"
#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <random>

void launch1(){
	std::thread::id threadId = std::this_thread::get_id();
	std::cout << "Server thread ID: " << threadId << std::endl;

	UdpSock server;
	server.createServSock();
	char buffer[512];
	std::string str;
	std::mutex mut;
	std::random_device e;
	std::uniform_int_distribution<int> dis(0, 4294967295);
	int number;
	while(1){
		mut.lock();
		number = dis(e);
		server.recvMsg(buffer);
		str = buffer;
		if(str == "request"){
			std::cout << str << std::endl;
		}else{
			std::cout << str << std::endl;
			std::cout << "bad request"  << std::endl;
			break;
		}
		server.sendMsg(number);
		mut.unlock();
	}
	server.closeSock();
}

void launch2(){
	std::thread::id threadId = std::this_thread::get_id();
	std::cout << "Client thread ID: " << threadId << std::endl;
	UdpSock client;
	client.createCliSock();
	char request[] = "request";
	int count = 30;
	client.sendMsg(request);
	std::mutex mut2;
	int number;
	while(count > 0){
		std::this_thread::sleep_for(std::chrono::seconds(1));
		--count;
		mut2.lock();
		client.recvMsg(number);
		std::cout << number << std::endl;
		client.sendMsg(request);
		mut2.unlock();
	}
	char end[] = "end";
	client.sendMsg(end);
	client.closeSock();
}

int main(){
	std::thread serverThread([]{launch1();});
	std::thread clientThread([]{launch2();});

	serverThread.join();
	clientThread.join();
	return 0;
}
