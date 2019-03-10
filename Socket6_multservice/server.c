#include "networks.h"

int main(int argc, char** argv)
{
	int lfd1, lfd2, cfd;
	struct sockaddr_in servip1, servip2;
	char buff[MAXLINE];
	time_t ticks;

	lfd1 = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servip1, sizeof(servip1));
	servip1.sin_family = AF_INET;
	servip1.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip1.sin_port = htons(5000);

	lfd2 = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servip2, sizeof(servip2));
	servip2.sin_family = AF_INET;
	servip2.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip2.sin_port = htons(5001);

	bind(lfd1, (struct sockaddr*)&servip1, sizeof(servip1));
	bind(lfd2, (struct sockaddr*)&servip2, sizeof(servip2));

	fd_set rset;
		
	listen(lfd1, LISTENQ);
	listen(lfd2, LISTENQ);

	char addr[20];
	inet_ntop(AF_INET, &(servip1.sin_addr), addr, 20);
	printf("%s\n", addr);

	inet_ntop(AF_INET, &(servip2.sin_addr), addr, 20);
	printf("%s\n", addr);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	while(1)
	{
		FD_ZERO(&rset);
		FD_SET(lfd1, &rset);
		FD_SET(lfd2, &rset);

		if(select(FD_SETSIZE, &rset, NULL, NULL, &tv))
		{	
			if(FD_ISSET(lfd1, &rset))
			{
				cfd = accept(lfd1, (struct sockaddr*)NULL, NULL);
				dup2(cfd, 1);
			
				if(cfd>0)
				{
					if(fork()==0)
					{	
						close(lfd1);
						// write some server function here
						char *args[]={"./service1",NULL}; 
        					execvp(args[0],args); 

						close(cfd);
					}
					else
					{
						close(cfd);
					}
				}
			}
			else if(FD_ISSET(lfd2, &rset))
			{
				cfd = accept(lfd2, (struct sockaddr*)NULL, NULL);
				dup2(cfd, 1);
				
				if(cfd>0)
				{
					if(fork()==0)
					{	
						close(lfd2);
						// write some server function here
						char *args[]={"./service2",NULL}; 
        					execvp(args[0],args); 

						close(cfd);
					}
					else
					{
						close(cfd);
					}
				}
			}
		}
	}
	return 0;
}
