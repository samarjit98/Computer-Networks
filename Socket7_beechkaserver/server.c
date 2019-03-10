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
	servip.sin_port = htons(5002);

	bind(lfd, (struct sockaddr*)&servip, sizeof(servip));
		
	listen(lfd, LISTENQ);

	char addr[20];
	inet_ntop(AF_INET, &(servip.sin_addr), addr, 20);
	printf("%s\n", addr);

	while(1)
	{
		
		cfd = accept(lfd, (struct sockaddr*)NULL, NULL);	
			
		if(cfd>0)
		{
			if(fork()==0)
			{	
				close(lfd);
				
				char ch[2];
				read(cfd, ch, 2);

				if(ch[0]=='1')
				{
					int sfd;
					struct sockaddr_in servip2;

					if((sfd = socket(AF_INET, SOCK_STREAM, 0))<0)printf("sock error!!");

					bzero(&servip2, sizeof(servip2));
					servip2.sin_family = AF_INET;
					servip2.sin_port = htons(5000);
					servip2.sin_addr.s_addr = inet_addr("127.0.0.1");

					if(connect(sfd, (struct sockaddr*)&servip2, sizeof(servip2))<0)printf("Connect error\n");

					char recvline[MAXLINE+1]; int n;

					while((n=read(sfd, recvline, MAXLINE))>0)
					{
						write(cfd, recvline, n);
					}

					close(sfd);
					//close(cfd);
				}
				else if(ch[0]=='2')
				{
					int sfd;
					struct sockaddr_in servip2;

					if((sfd = socket(AF_INET, SOCK_STREAM, 0))<0)printf("sock error!!");

					bzero(&servip2, sizeof(servip2));
					servip2.sin_family = AF_INET;
					servip2.sin_port = htons(5001);
					servip2.sin_addr.s_addr = inet_addr("127.0.0.1");

					if(connect(sfd, (struct sockaddr*)&servip2, sizeof(servip2))<0)printf("Connect error\n");

					char recvline[MAXLINE+1]; int n;

					while((n=read(sfd, recvline, MAXLINE))>0)
					{
						write(cfd, recvline, n);
					}

					close(sfd);
					//close(cfd);
				}

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
