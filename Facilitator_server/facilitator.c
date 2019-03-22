#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/select.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#define PORT 8080


int main(){
	int sfd[100], addrlen, max, msfd, nsfd, n=0;
	fflush(stdout);
	fd_set readfds;
	char buf[100], sbuf[100];
	struct sockaddr_in address, c_addr;

	msfd = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);

	bind(msfd, (struct sockaddr *)&address, sizeof(address));
	listen(msfd, 3);

	max = msfd;

	while(1){
		FD_ZERO(&readfds);
		FD_SET(msfd, &readfds);
		FD_SET(0, &readfds);

		select(max+1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(0, &readfds)){
			scanf("%s", &buf);
			char port[5], path[100];
			for(int i=0; i<4; i++)
				port[i] = buf[i];
			port[4] = '\0';
			for(int i=4; i<strlen(buf); i++)
				path[i-4] = buf[i];
			path[strlen(buf)-4] = '\0';
			int nfd = fileno(popen(path, "w"));
			printf("Opened %s at %d\n", path, nfd);
			sfd[n++] = nfd;
			printf("New service created\n");
		}

		if(FD_ISSET(msfd, &readfds)){
			nsfd = accept(msfd, (struct sockaddr *)&c_addr, &addrlen);
			recv(nsfd, &sbuf, sizeof(sbuf), 0);
			printf("Received new service request for %d\n", atoi(sbuf));
			//arrange to send ip:port
			char serialip[100];
			sprintf(serialip, "%s:%d", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
			printf("Sent %s\n", serialip);
			write(sfd[atoi(sbuf)], serialip, strlen(serialip));
			printf("Forwarded request to service\n");
		}
		
	}

	return 0;
}
