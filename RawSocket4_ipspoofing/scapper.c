#include "networks.h"

int main(int argc, char* argv[]){
	int rawfd;
	struct sockaddr_in rawip;
	struct sockaddr_in raddr;
	int rawiplen, udpiplen, raddrlen;

	char msg[2048]; int msglen;

	struct iphdr hdrip;
	int iphdrlen = sizeof(hdrip);

	if((rawfd = socket(AF_INET, SOCK_RAW, 10)) < 0){  
		perror("Raw socket error!"); exit(0);
	}
	int reuse = 1;
	if(setsockopt(rawfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
		perror("Raw sockopt error!"); exit(0);
	}

	rawip.sin_family = AF_INET;
	rawip.sin_addr.s_addr = inet_addr(argv[1]);
	rawiplen = sizeof(rawip);

	if(bind(rawfd, (struct sockaddr*)&rawip, rawiplen) < 0){
		perror("Raw bind error!"); exit(0);
	}

	int i=0;
	while(1){
		raddrlen = sizeof(raddr);
		msglen = recvfrom(rawfd, msg, 2048, 0, (struct sockaddr*)&raddr, &raddrlen);

		if(msglen <= 0)continue;

		hdrip = *((struct iphdr*)msg);

		printf("Raw socket: %d\n", i);
		printf("hl: %d, version: %d, ttl: %d, protocol: %d\n", hdrip.ihl, hdrip.version, hdrip.ttl, hdrip.protocol);
		printf("src: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.saddr)));
		printf("dest: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.daddr)));
		printf("\n");
		i++;
	}
	close(rawfd);
	return 0;
}

/*
Output:

Raw socket: 0
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 1
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 2
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 3
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 4
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 5
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 6
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 7
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 8
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 9
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 10
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 11
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 12
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 13
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

Raw socket: 14
hl: 5, version: 4, ttl: 64, protocol: 10
src: 69.69.69.69
dest: 127.0.0.1

*/