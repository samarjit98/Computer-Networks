#include "networks.h"

int main()
{
	int sfd;
	struct sockaddr_in servip, clip;

	sfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip.sin_port = htons(5000);

	bind(sfd, (struct sockaddr*)&servip, sizeof(servip));
		
	int n;
	socklen_t len;
	char mesg[MAXLINE];

	while(1)
	{
		struct sockaddr_in clip1;
		struct sockaddr* clip2;
		
		len = sizeof(clip);
		n = recvfrom(sfd, mesg, MAXLINE, 0, (struct sockaddr*)&clip, &len);
		printf("%d\n", (int)len);
		n = sendto(sfd, mesg, n, 0, (struct sockaddr*)&clip, len);
		printf("%d\n", n);	
	}	
}
