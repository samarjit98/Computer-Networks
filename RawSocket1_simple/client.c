#include "networks.h"

int main(int argc, char* argv[]){
	int udpfd;
	struct sockaddr_in udpip;
	int udpiplen;

	if((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Socket error!"); exit(0);
	}

	udpip.sin_family = AF_INET;
	udpip.sin_port = htons(atoi(argv[1]));
	udpip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	udpiplen = sizeof(udpip);

	while(1){
		char msg[2048];
		scanf("%s", msg);
		sendto(udpfd, msg, strlen(msg)+1, 0, (struct sockaddr*)&udpip, udpiplen);
	}
	close(udpfd);
	return 0;
}