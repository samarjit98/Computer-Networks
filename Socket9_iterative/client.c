#include "networks.h"

int flag = 0;

void* write_func(void* arg)
{
	int sfd = *((int*)arg);

	while(1)
	{
		char buff[MAXLINE];
		scanf("%s", buff);
		send(sfd, buff, sizeof(buff), 0);

		if(buff[0]=='@')
		{
			flag=1; break;
		}
	}
}

void* read_func(void* arg)
{
	int sfd = *((int*)arg);

	while(1)
	{
		if(flag==1)break;

		int n;

		char buff[MAXLINE];

		n=recv(sfd, buff, MAXLINE, 0);

		if(buff[0]=='@')break;
		printf("%s\n", buff);
	}
}

int main(int argc, char** argv)
{
	int sfd, n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servip;

	if((sfd = socket(AF_INET, SOCK_STREAM, 0))<0)printf("sock error!!");

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_port = htons(5000);
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");
	//inet_pton(AF_INET, "206.168.112.219", &servip.sin_addr);

	if(connect(sfd, (struct sockaddr*)&servip, sizeof(servip))<0)printf("Connect error\n");;

	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, read_func, (void*)&sfd);
	pthread_create(&tid2, NULL, write_func, (void*)&sfd);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}