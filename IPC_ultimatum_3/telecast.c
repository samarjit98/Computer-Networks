#include "ipc.h"

int main(int argc, char* argv[])
{
	int lfd, cfd;
	struct sockaddr_in servip;
	char buff[MAXLINE];
	time_t ticks;

	lfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip.sin_port = htons(5002);

	bind(lfd, (struct sockaddr*)&servip, sizeof(servip));
	listen(lfd, LISTENQ);

	char addr[20];
	inet_ntop(AF_INET, &(servip.sin_addr), addr, 20);

	printf("%s\n", addr);

	while(1)
	{
		cfd = accept(lfd, (struct sockaddr*)NULL, NULL);
		scanf("%s", buff);
		write(cfd, buff, strlen(buff));
		close(cfd);
	}
}