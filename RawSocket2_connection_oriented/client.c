#include "networks.h"

int main(int argc, char* argv[]){
	int tcpfd;
	struct sockaddr_in tcpip;
	int tcpiplen;

	if((tcpfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket error!"); exit(0);
	}

	tcpip.sin_family = AF_INET;
	tcpip.sin_port = htons(atoi(argv[1]));
	tcpip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	tcpiplen = sizeof(tcpip);

	if(connect(tcpfd, (struct sockaddr*)&tcpip, tcpiplen) < 0){
		perror("Connect error!"); exit(0);
	}

	while(1){
		char msg[2048];
		scanf("%s", msg);
		send(tcpfd, msg, strlen(msg)+1, 0);
	}
	close(tcpfd);
	return 0;
}