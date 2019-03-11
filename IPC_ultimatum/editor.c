#include "ipc.h"

int main(int argc, char* argv[])
{
	char* pathname1 = "./fifo_rep_1";
	mkfifo(pathname1, 0666);
	char* pathname2 = "./fifo_rep_2";
	mkfifo(pathname2, 0666);
	char* pathname3 = "./fifo_rep_3";
	mkfifo(pathname3, 0666);
	char* pathname4 = "./fifo_editor_1";
	mkfifo(pathname4, 0666);
	char* pathname5 = "./fifo_editor_2";
	mkfifo(pathname5, 0666);
	char* pathname6 = "./fifo_doc_1";
	mkfifo(pathname6, 0666);

	int fd1, fd2, fd3, nfd1, nfd2, dfd;

	printf("Before opening\n");

	fd1 = open(pathname1, O_RDWR);
	fd2 = open(pathname2, O_RDWR);
	fd3 = open(pathname3, O_RDWR);
	nfd1 = open(pathname4, O_RDWR);
	nfd2 = open(pathname5, O_RDWR);
	dfd = open(pathname6, O_RDWR);

	printf("After opening\n");

	fd_set rset;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	int read_flag = 0;

	while(1)
	{
		FD_ZERO(&rset);
		FD_SET(fd1, &rset);
		FD_SET(fd2, &rset);
		FD_SET(fd3, &rset);

		//printf("Before select\n");

		if(select(FD_SETSIZE, &rset, NULL, NULL, &tv))
		{	
			printf("After select\n");
			if(FD_ISSET(fd1, &rset))
			{
				char buff[20];
				int n = read(fd1, buff, 20);

				printf("%s\n", buff);

				if(buff[0]=='/' && buff[1]=='d')
				{
					write(dfd, buff, n);
				}

				if(read_flag==0)
				{
					write(nfd1, buff, n); read_flag = 1;
				}
				else
				{
					write(nfd2, buff, n); read_flag = 0;
				}
			}
			else if(FD_ISSET(fd2, &rset))
			{
				char buff[20];
				int n = read(fd2, buff, 20);

				printf("%s\n", buff);

				if(buff[0]=='/' && buff[1]=='d')
				{
					write(dfd, buff, n);
				}

				if(read_flag==0)
				{
					write(nfd1, buff, n); read_flag = 1;
				}
				else
				{
					write(nfd2, buff, n); read_flag = 0;
				}
			}
			else if(FD_ISSET(fd3, &rset))
			{
				char buff[20];
				int n = read(fd3, buff, 20);

				printf("%s\n", buff);

				if(buff[0]=='/' && buff[1]=='d')
				{
					write(dfd, buff, n);
				}

				if(read_flag==0)
				{
					write(nfd1, buff, n); read_flag = 1;
				}
				else
				{
					write(nfd2, buff, n); read_flag = 0;
				}
			}
		}
	}

	return 0;
}