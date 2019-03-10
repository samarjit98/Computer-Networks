#include "networks.h"

int curr1, curr2;

struct threadargs
{
	int cfd;
};

static void sig_handler1(int signo)
{
	curr1--;
}

void* thread_func(void* args)
{
	int cfd = ((struct threadargs*)args)->cfd;

	char buff[MAXLINE];
	recv(cfd, buff, MAXLINE, 0);

	char str[10];
	sprintf(str, "%d", strlen(buff));

	send(cfd, str, strlen(str)+1, 0);

	curr2--;
}

int main(int argc, char** argv)
{
	signal(SIGUSR1, sig_handler1);

	int lfd1, lfd2, cfd;
	struct sockaddr_in servip1, servip2;
	char buff[MAXLINE];
	time_t ticks;

	curr1=0; curr2=0;

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
				if(curr1<=25)
				{
					cfd = accept(lfd1, (struct sockaddr*)NULL, NULL);

					printf("%d\n", cfd);
					curr1++;
				
					if(cfd>0)
					{
						if(fork()==0)
						{	
							close(lfd1);

							int newstdin = dup(fileno(stdin));
							int newstdout = dup(fileno(stdout));

							printf("Here1\n");

							//dup2(cfd, fileno(stdin));
							//dup2(cfd, fileno(stdout));
			
							//printf("Here2\n");

							char arg1[5], arg2[5], arg3[5];

							sprintf(arg1, "%d", newstdin);
							sprintf(arg2, "%d", newstdout);
							sprintf(arg3, "%d", cfd);

							char* args[] = {"./service1", arg1, arg2, arg3, NULL};
							execv(args[0], args);
						}
						else
						{
							close(cfd);
						}
					}
				}
			}
			else if(FD_ISSET(lfd2, &rset))
			{
				if(curr2<=15)
				{
					cfd = accept(lfd2, (struct sockaddr*)NULL, NULL);

					curr2++;
					pthread_t tid;
					struct threadargs* args = malloc(sizeof(struct threadargs));
					args->cfd = cfd;
					pthread_create(&tid, NULL, thread_func, (void*)args);
				}
			}
		}
	}
	return 0;
}