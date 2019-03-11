#include "ipc.h"

int flag=0;

int main(int argc, char* argv[])
{
	char* pathname = "./fifo_editor_2";
	mkfifo(pathname, 0666);

	char* screen = "./fifo_screen";
	mkfifo(screen, 0666);

	int fd = open(pathname, O_RDWR);
	int sfd = open(screen, O_RDWR);

	int shmid = shmget(1234, sizeof(int), 0666|IPC_CREAT);
	int *flag = shmat(shmid, 0, 0);
	*flag = 0;

	while(1)
	{
		char buff[20];
		int n = read(fd, buff, 20);

		printf("%s\n", buff);

		if(buff[0]=='.' && buff[1]=='p')
		{
			while(*flag==1);
			*flag = 1;

			char portno[6];
			portno[0] = buff[2]; portno[1] = buff[3]; portno[2] = buff[4]; portno[3] = buff[5]; portno[4] = '\0';

			int cfd;
			char recvline[MAXLINE+1];
			struct sockaddr_in servip;

			if((cfd = socket(AF_INET, SOCK_STREAM, 0))<0)printf("sock error!!");

			bzero(&servip, sizeof(servip));
			servip.sin_family = AF_INET;
			servip.sin_port = htons(atoi(portno));
			servip.sin_addr.s_addr = inet_addr("127.0.0.1");

			if(connect(cfd, (struct sockaddr*)&servip, sizeof(servip))<0)printf("Connect error\n");;

			while((n=read(cfd, recvline, MAXLINE))>0)
			{
				recvline[n]=0;
				write(sfd, recvline, strlen(buff)+1);
			}

			close(cfd);

			*flag = 0;
		}
		else
		{
			while(*flag==1);
			*flag = 1;
			write(sfd, buff, strlen(buff)+1);
			*flag = 0;
		}
	}
}