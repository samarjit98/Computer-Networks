#include "networks.h"

int main()
{
	int cfd;
	struct sockaddr_in clip;

	if((cfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Sock error!"); exit(0);
	}

	bzero(&clip, sizeof(clip));
	clip.sin_family = AF_INET;
	clip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	clip.sin_port = htons(5001);

	if(bind(cfd, (struct sockaddr*)&clip, sizeof(clip)) < 0)
	{
		perror("Bind error!"); exit(0);
	}

	struct sockaddr_in servip;

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servip.sin_port = htons(5000);

	if(fork()==0)
	{
		while(1)
		{
			int n;
			socklen_t len;
			char mesg[MAXLINE];

			struct sockaddr* servip2;
			len = sizeof(servip);
			n = recvfrom(cfd, mesg, MAXLINE, 0, (struct sockaddr*)&servip, &len);

			printf("%s\n", mesg);
		}	
	}	
	else
	{
		while(1)
		{
			char mesg[MAXLINE];
			scanf("%s", mesg);

			int ret = sendto(cfd, mesg, strlen(mesg), 0, (struct sockaddr*)&servip, sizeof(servip));
			printf("%d\n", ret);
		}
	}
		
	return 0;
}
