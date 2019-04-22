#include "networks.h"

typedef struct{
	char type[10];
	char mesg[100];
	char src[20];
	char dest[20];
} mesg_t;

char name[20];

void* sender(void* argt){
	int connfd = *((int*)argt);
	while(1){
		mesg_t mesg;
		printf(">TYPE: ");
		scanf("%s", mesg.type);

		if(strcmp(mesg.type, "BROADCAST")==0){
			printf(">MESG: ");
			scanf("%s", mesg.mesg);
			/*
			fflush(stdin);
			fgets(mesg.mesg, 100, stdin);
			*/
			strcpy(mesg.src, name);
			send(connfd, &mesg, sizeof(mesg), 0);
		}
		else if(strcmp(mesg.type, "PING")==0){
			printf(">NAME: ");
			scanf("%s", mesg.dest);
			printf(">MESG: ");
			scanf("%s", mesg.mesg);
			/*
			fflush(stdin);
			fgets(mesg.mesg, 100, stdin);
			*/
			strcpy(mesg.src, name);
			send(connfd, &mesg, sizeof(mesg), 0);
		}
	}
}

void* receiver(void* argt){
	int connfd = *((int*)argt);
	while(1){
		mesg_t mesg;
		recv(connfd, &mesg, sizeof(mesg), 0);
		printf("\n");
		if(strcmp(mesg.type, "BROADCAST")==0){
			printf("BROADCAST from %s: %s\n", mesg.src, mesg.mesg);
		}
		else if(strcmp(mesg.type, "PING")==0){
			printf("PING from %s: %s\n", mesg.src, mesg.mesg);
		}	
		fflush(stdout);
		printf(">TYPE: ");	
	}
}

int main(int argc, char* argv[]){
	int connfd;	
	struct sockaddr_in servip;

	if((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket error"); exit(0);
	}

	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servip.sin_port = htons(atoi(argv[1]));

	if(connect(connfd, (struct sockaddr*)&servip, sizeof(servip)) < 0){
		perror("Connect error"); exit(0);
	}

	printf(">Enter your name:"); scanf("%s", name);

	int pid = (int)getpid();
	send(connfd, &pid, sizeof(int), 0);
	send(connfd, name, 20, 0);

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, sender, (void*)&connfd);
	pthread_create(&tid2, NULL, receiver, (void*)&connfd);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}