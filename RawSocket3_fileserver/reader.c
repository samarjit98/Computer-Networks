#include "networks.h"

int main(int argc, char* argv[]){
	char buff[2048]; int n;

	while((n = read(0, buff, 2048)) > 0){
		buff[n] = 0;
		write(2, buff, strlen(buff));
		write(1, buff, strlen(buff));
	}
	buff[0] = '\n'; buff[1] = 0;
	write(1, buff, strlen(buff));
	return 0;
}