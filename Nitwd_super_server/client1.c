#include "networks.h"

int main(int argc, char** argv)
{
	int sfd, n;
	char buff[MAXLINE+1];
	struct sockaddr_in servip;

	if((sfd = socket(AF_INET, SOCK_STREAM, 0))<0)printf("sock error!!");

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_port = htons(5000);
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sfd, (struct sockaddr*)&servip, sizeof(servip))<0)printf("Connect error\n");;

	scanf("%s", buff);
	write(sfd, buff, strlen(buff)+1);
	read(sfd, buff, MAXLINE);
	printf("%s", buff);

	return 0;
}