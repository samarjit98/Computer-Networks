#include "ipc.h"

int main(int argc, char* argv[])
{
	char* pathname = "./fifo_rep_2";
	mkfifo(pathname, 0666);

	int fd = open(pathname, O_WRONLY);

	while(1)
	{
		char buff[20];
		scanf("%s", buff);
		//dup2(fd, fileno(stdout));
		write(fd, buff, strlen(buff)+1);
	}

	return 0;
}