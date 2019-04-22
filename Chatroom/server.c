#include "networks.h"

typedef struct{
	int connfd;
	char cli_name[20];
	int pid;
} client_t;

typedef struct{
	char type[10];
	char mesg[100];
	char src[20];
	char dest[20];
} mesg_t;

client_t* clients;
int max_clients;
int curr_client = 0;

void* serve_client(void* argt){
	client_t cli;
	cli.connfd = *((int*)argt);
	recv(cli.connfd, &cli.pid, sizeof(int), 0);
	recv(cli.connfd, cli.cli_name, 20, 0);
	clients[curr_client++] = cli; 

	while(1){
		mesg_t mesg;
		recv(cli.connfd, &mesg, sizeof(mesg), 0);

		if(strcmp(mesg.type, "BROADCAST")==0){
			for(int i=0; i<curr_client; i++){
				if(clients[i].pid != cli.pid)send(clients[i].connfd, &mesg, sizeof(mesg), 0);
			}
		}
		else if(strcmp(mesg.type, "PING")==0){
			for(int i=0; i<curr_client; i++){
				if(strcmp(mesg.dest, clients[i].cli_name)==0){
					send(clients[i].connfd, &mesg, sizeof(mesg), 0);
				}
			}
		}
	}
}

int main(int argc, char* argv[]){
	printf("Enter max number of clients to handle:"); scanf("%d", &max_clients);
	clients = (client_t*)malloc(max_clients*sizeof(client_t));

	int listenfd;
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket error"); exit(0);
	}
	struct sockaddr_in servip;

	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servip.sin_port = htons(atoi(argv[1]));

	if(bind(listenfd, (struct sockaddr*)&servip, sizeof(servip)) < 0){
		perror("Bind error"); exit(0);
	}
	listen(listenfd, LISTENQ);

	while(1){
		int connfd = accept(listenfd, NULL, NULL);

		if(connfd > 0){
			pthread_t tid;
			pthread_create(&tid, NULL, serve_client, (void*)&connfd);
		}
	}
	return 0;
}