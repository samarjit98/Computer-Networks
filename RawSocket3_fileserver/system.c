#include "networks.h"

int main(){
	int rsfd;
	struct sockaddr_in systemip;

	if((rsfd = socket(AF_INET, SOCK_RAW, 10)) < 0){
		perror("Raw socket error!"); exit(0);
	}

	systemip.sin_family = AF_INET;
	systemip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(rsfd, (struct sockaddr*)&systemip, sizeof(systemip)) < 0){
		perror("Raw bind error!"); exit(0);
	}

	while(1){
		struct sockaddr_in raddr;
		int raddrlen = sizeof(raddr);

		char buff[2048];
		if(recvfrom(rsfd, buff, 2048, 0, (struct sockaddr*)&raddr, &raddrlen) > 0){
			printf("%s\n", (buff+(sizeof(struct iphdr))));
		}
	}
	return 0;
}