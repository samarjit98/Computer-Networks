#include "networks.h"

int main(int argc, char** argv)
{
	int sfd, n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servip;

	if((sfd = socket(AF_INET, SOCK_STREAM, 0))<0)printf("sock error!!");

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_port = htons(5002);
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");
	//inet_pton(AF_INET, "206.168.112.219", &servip.sin_addr);

	if(connect(sfd, (struct sockaddr*)&servip, sizeof(servip))<0)printf("Connect error\n");

	write(sfd, "1", 2);

	while((n=read(sfd, recvline, MAXLINE))>0)
	{
		recvline[n]=0;
		//printf("Here");
		fputs(recvline, stdout);
	}

	return 0;
}
