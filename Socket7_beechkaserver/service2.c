#include "networks.h"

int main(int argc, char** argv)
{
	int lfd, cfd;
	struct sockaddr_in servip;
	char buff[MAXLINE];
	time_t ticks;

	lfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip.sin_port = htons(5001);

	bind(lfd, (struct sockaddr*)&servip, sizeof(servip));
	listen(lfd, LISTENQ);

	char addr[20];
	inet_ntop(AF_INET, &(servip.sin_addr), addr, 20);

	printf("%s\n", addr);

	while(1)
	{
		cfd = accept(lfd, (struct sockaddr*)NULL, NULL);
		//printf("%d\n", cfd);
		// printf("%d\n", (int)cfd);
		if(cfd>0)
		{
			if(fork()==0)
			{	
				close(lfd);
				// write some server function here, currently returns time
				ticks = time(NULL); //return current time
				snprintf(buff, sizeof(buff), "Service 2\n");
				write(cfd, buff, strlen(buff));

				close(cfd);
			}
			else
			{
				close(cfd);
			}
		}
	}
	return 0;
}