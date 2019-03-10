#include "networks.h"

int flag=0;
pthread_t tidgod;

static void sig_usr(int signo)
{
	pthread_cancel(tidgod);
	//signal(SIGUSR1, sig_usr);
}

void* read_func(void* arg)
{
	int sfd = *((int*)arg);

	while(1)
	{
		int n;

		char buff[MAXLINE];

		n=recv(sfd, buff, MAXLINE, 0);
		printf("%s\n", buff);

		if(buff[0]=='@')
		{
			flag=1; 
			kill(getpid(), SIGUSR1);
			break;
		}
	}
}

void* write_func(void* arg)
{
	int sfd = *((int*)arg);

	while(1)
	{
		char buff[MAXLINE];
		scanf("%s", buff);
		send(sfd, buff, sizeof(buff), 0);
	}
}

int main(int argc, char** argv)
{
	int lfd, cfd;
	struct sockaddr_in servip;
	char buff[MAXLINE];
	time_t ticks;
	signal(SIGUSR1, sig_usr);

	lfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip.sin_port = htons(5000);

	bind(lfd, (struct sockaddr*)&servip, sizeof(servip));
	listen(lfd, LISTENQ);

	char addr[20];
	inet_ntop(AF_INET, &(servip.sin_addr), addr, 20);

	printf("%s\n", addr);

	while(1)
	{
		cfd = accept(lfd, (struct sockaddr*)NULL, NULL);
		// printf("%d\n", (int)cfd);

		pthread_t tid1, tid2;

		pthread_create(&tid1, NULL, read_func, (void*)&cfd);
		pthread_create(&tid2, NULL, write_func, (void*)&cfd);
		tidgod = tid2;
		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);

		flag=0;

		close(cfd);
	}
	return 0;
}