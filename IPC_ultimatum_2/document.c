#include "ipc.h"

int main(int argc, char* argv[])
{
	char* pathname = "./fifo_doc_1";
	mkfifo(pathname, 0666);

	int fd = open(pathname, O_RDWR);

	while(1)
	{
		char buff[20];
		read(fd, buff, 20);

		printf("%s \n", buff);
	}
}