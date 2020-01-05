#include <iostream>
#include <string>

int main(){
	char buf[34] = "request";
	std::string str = buf;
	if (str == "request") std::cout << "s" << std::endl;
	return 0;
}
