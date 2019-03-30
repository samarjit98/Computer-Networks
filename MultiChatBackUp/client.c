#include "networks.h"

void* reader(void* argt);
void* writer(void* argt);

int main(int argc, char** argv){
	int sfd;
	struct sockaddr_in servip;

	if((sfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("Socket error!"); exit(0);
	}

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_port = htons(atoi(argv[1]));
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sfd, (struct sockaddr*)&servip, sizeof(servip))<0){
		perror("Connect error!");
	}

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, reader, (void*)&sfd);
	pthread_create(&tid2, NULL, writer, (void*)&sfd);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}

void* reader(void* argt){
	int sfd = *((int*)argt);
	printf("%d\n", sfd);

	while(1){
		char buff[20];
		read(sfd, &buff, 20);
		printf("%s\n", buff);
	}
}

void* writer(void* argt){
	int sfd = *((int*)argt);
	printf("%d\n", sfd);

	while(1){
		char buff[20];
		scanf("%s", buff);
		write(sfd, buff, strlen(buff)+1);
	}
}