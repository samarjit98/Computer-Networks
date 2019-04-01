#include "networks.h"

int main(int argc, char* argv[]){
	int sfd;
	struct sockaddr_in servip;

	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket error!"); exit(0);
	}

	servip.sin_family = AF_INET;
	servip.sin_port = htons(atoi(argv[1]));
	servip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sfd, (struct sockaddr*)&servip, sizeof(servip)) < 0){
		perror("Connect error!"); exit(0);
	}

	char buff[20];
	scanf("%s", buff);
	send(sfd, buff, strlen(buff)+1, 0);
	scanf("%s", buff);
	send(sfd, buff, strlen(buff)+1, 0);

	return 0;
}