#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#define PORT 8080



int main(int argc, char** argv){
	fflush(stdout);
	int service, opt = 1;
	int sfd, addrlen, nsfd;
	char buf[100], sbuf[100];
	struct sockaddr_in address, c_addr;

	printf("Service ID: ");
	scanf("%d", &service);

	sfd = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);

	connect(sfd, (struct sockaddr *)&address, sizeof(address));
	printf("Connected!\n");
	sprintf(buf, "%d", service);
	send(sfd, buf, strlen(buf), 0);
	printf("Sent service request!\n");

	getsockname(sfd, &c_addr, &addrlen);

	close(sfd);

	bind(sfd, &c_addr, addrlen);
	listen(sfd, 3);

	//while(1){
		nsfd = accept(sfd, &address, &addrlen);
		if(nsfd<=0)
			printf("Reply from service!\n");
		else
			printf("nsfd fail\n");
	//}
		return 0;
	
}
