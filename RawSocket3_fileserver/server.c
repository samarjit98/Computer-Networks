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

	if(bind(sfd, (struct sockaddr*)&servip, sizeof(servip)) < 0){
		perror("Bind error!"); exit(0);
	}
	int reuse = 1;
	if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
		perror("Sockopt error!"); exit(0);
	}
	listen(sfd, LISTENQ);

	while(1){
		int nsfd = accept(sfd, NULL, NULL);

		if(nsfd>0){
			if(fork()==0){
				close(sfd);

				int rsfd;
				struct sockaddr_in systemip;

				if((rsfd = socket(AF_INET, SOCK_RAW, 10)) < 0){
					perror("Raw socket error!"); exit(0);
				}

				char buff[20];
				recv(nsfd, buff, 20, 0);

				printf("%s\n", buff);

				systemip.sin_family = AF_INET;
				systemip.sin_addr.s_addr = inet_addr(buff);

				if(connect(rsfd, (struct sockaddr*)&systemip, sizeof(systemip)) < 0){
					perror("Raw connect error!"); exit(0);
				}

				recv(nsfd, buff, 20, 0);

				printf("%s\n", buff);

				FILE* f = fopen(buff, "r");
				if(f!=NULL)printf("File opened!!\n");

				dup2(rsfd, 1);
				dup2(fileno(f), 0);

				fprintf(stderr, "After dup!!\n");

				char* arg[] = {"./reader", NULL};
				execvp(arg[0], arg);
			}
			else{
				close(nsfd);
			}
		}
	}
}