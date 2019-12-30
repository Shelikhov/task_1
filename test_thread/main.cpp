#include <iostream>
#include <thread>
#include <mutex>

std::mutex mut;

void fun1(){
	for(int t = 0; t < 3; ++t){
		mut.lock();
		std::cout << "thread 1" << std::endl;
		mut.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

void fun2(){
	for(int i = 0; i < 3; ++i){
		mut.lock();
		std::cout << "thread 2" << std::endl;
		mut.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main(){
	std::thread thread1(fun1);
	std::thread thread2(fun2);

	std::cout << thread1.get_id() << std::endl;
	std::cout << thread2.get_id() << std::endl;

	thread1.join();
	thread2.join();
	return 0;
}
