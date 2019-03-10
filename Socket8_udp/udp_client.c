#include "networks.h"

int main()
{
	int cfd;
	struct sockaddr_in clip;

	cfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&clip, sizeof(clip));
	clip.sin_family = AF_INET;
	clip.sin_addr.s_addr = inet_addr("127.0.0.1");
	clip.sin_port = htons(5001);

	bind(cfd, (struct sockaddr*)&clip, sizeof(clip));

	//int sfd;
	struct sockaddr_in servip;

	//sfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");
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
			n = recvfrom(cfd, mesg, MAXLINE, 0, servip2, &len);

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