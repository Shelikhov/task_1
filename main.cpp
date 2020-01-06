#include <chrono>
#include "UdpSock.cpp"
#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <random>
#include <ctime>

#define BUF_SIZE 512
#define SECONDS 30//client life time

/*Generates random number for responding to client*/
int rand(){
	std::random_device e;
	std::uniform_int_distribution<int> dis(0, 4294967295);
	return dis(e);
}

void printThrId(const char* msg){
	std::cout << msg << " thread ID: " << std::this_thread::get_id() << std::endl;
}

char* curTime(){
	std::time_t time = std::time(nullptr);
	return std::asctime(std::localtime(&time));
}

/*Server receives requests from client and sends random numbers*/
void ServFun(){
	printThrId("Server");

	char buffer[BUF_SIZE];
	std::string msg;
	int number;
	std::mutex mut;

	UdpSock server;
	server.createServSock();
	while(1){
		mut.lock();
		server.recvMsg(buffer);
		msg = buffer;
		if(msg == "request"){//either to reply or to terminate process
			std::cout << "Request from client. " << curTime() << msg << std::endl;
		}else if(msg == "terminate"){
			std::cout << "Request from client " << curTime() << msg << std::endl;
			break;
		}else{
			std::cout << curTime() << "Incorrect request from client: " << msg << std::endl;
			continue;
		}
		number = rand();
		server.sendMsg(number);
		mut.unlock();
	}
	server.closeSock();
}

/*Client sends requests to server each 1 second and receives random values*/
void CliFun(){
	printThrId("Client");

	char request[] = "request", end[] = "terminate";
	int count = SECONDS, msg;
	std::mutex mut2;

	UdpSock client;
	client.createCliSock();
	client.sendMsg(request);
	while(count > 0){
		std::this_thread::sleep_for(std::chrono::seconds(1));
		--count;
		mut2.lock();
		client.recvMsg(msg);
		std::cout << "Response from server. " << curTime() << msg << std::endl;
		client.sendMsg(request);
		mut2.unlock();
	}
	client.sendMsg(end);
	client.closeSock();
}

int main(){
	std::thread serverThread([]{ServFun();});
	std::thread clientThread([]{CliFun();});

	serverThread.join();
	clientThread.join();
	return 0;
}
