#include "ErrorCheck.h"

void ErrorCheck::checking(int &value, const char *msg){
	if(value == -1){
		perror(msg);
		exit(EXIT_FAILURE);
	}//else std::cout << msg << " success" << std::endl;
}
