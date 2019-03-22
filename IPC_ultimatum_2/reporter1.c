#include "ipc.h"

int main(int argc, char* argv[])
{
	char* pathname = "./fifo_rep_1";
	mkfifo(pathname, 0666);

	int fd = open(pathname, O_RDWR);

	while(1)
	{
		char buff[20];
		scanf("%s", buff);
		//printf("Before dup\n");
		dup2(fd, fileno(stdout));
		//printf("After dup\n");
		write(fd, buff, strlen(buff)+1);
	}

	return 0;
}