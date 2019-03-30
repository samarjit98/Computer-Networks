#include "networks.h"

int main(int argc, char** argv){
	int lfd, cfd;
	struct sockaddr_in servip;
	char buff[MAXLINE];
	time_t ticks;

	if((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket error!"); exit(0);
	}
	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servip.sin_port = htons(atoi(argv[1]));

	if(bind(lfd, (struct sockaddr*)&servip, sizeof(servip)) < 0){
		perror("Bind error!"); exit(0);
	}
	listen(lfd, LISTENQ);

	char addr[20];
	inet_ntop(AF_INET, &(servip.sin_addr), addr, 20);

	printf("%s\n", addr);

	while(1){
		cfd = accept(lfd, (struct sockaddr*)NULL, NULL);
		
		char buff[10];
		scanf("%s", buff);
		write(cfd, buff, strlen(buff)+1);

		close(cfd);
	}
	return 0;
}