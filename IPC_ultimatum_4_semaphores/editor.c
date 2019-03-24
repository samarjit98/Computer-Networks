#include "ipc.h"

int main(int argc, char* argv[])
{
	char* pathname1 = "./fifo_rep_1";
	mkfifo(pathname1, 0666);
	char* pathname2 = "./fifo_rep_2";
	mkfifo(pathname2, 0666);
	char* pathname3 = "./fifo_rep_3";
	mkfifo(pathname3, 0666);
	char* pathname4 = "./fifo_editor";
	mkfifo(pathname4, 0666);
	char* pathname5 = "./fifo_doc_1";
	mkfifo(pathname5, 0666);

	int fd1, fd2, fd3, nfd, dfd;

	printf("Before opening\n");

	fd1 = open(pathname1, O_RDWR);
	fd2 = open(pathname2, O_RDWR);
	fd3 = open(pathname3, O_RDWR);
	nfd = open(pathname4, O_RDWR);
	dfd = open(pathname5, O_RDWR);

	printf("After opening\n");

	fd_set rset;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	int shmid = shmget(1245, sizeof(int), 0666|IPC_CREAT);
	int *writeflag = shmat(shmid, 0, 0);
	*writeflag = 2;

	sem_t* sem;
	int shmsem = shmget(1269, sizeof(sem_t), 0666|IPC_CREAT);
	sem = (sem_t*)shmat(shmsem, 0, 0);
	sem_init(sem, 1, 1);

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
				else
				{
					write(nfd, buff, n); 
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
				else
				{
					write(nfd, buff, n); 
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
				else
				{
					write(nfd, buff, n); 
				}
			}
		}
	}

	return 0;
}