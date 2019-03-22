#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>
#define PORT 8080

void* func(void* arg){
		char sbuf[100];
		struct sockaddr_in address;
		strcpy(sbuf, (char*)arg);
		char addr_str[100], port[7];
		int i=0;
		for( ; sbuf[i]!=':'&&i<strlen(sbuf); i++)
			addr_str[i] = sbuf[i];
		addr_str[i] = '\0'; i++; int len = i+4;
		for( ; i<len; i++)
			port[i-len-4] = sbuf[i];
		port[i-len-4] = '\0';

		int sfd = socket(AF_INET, SOCK_STREAM, 0);

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(addr_str);
		address.sin_port = ntohs(atoi(port));

		int nsfd = connect(sfd, (struct sockaddr *)&address, sizeof(address));
		printf("New Service Thread Success!\n");
		while(1){}

}

int main(int argc, char** argv){

	char sbuf[100];
	fflush(stdout);
	
	while(1){
		read(0, &sbuf, sizeof(sbuf));
		sleep(1);
		pthread_t Thread;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&Thread, &attr, func, (void*)sbuf);
		
	}
	

	

	return 0;

}
