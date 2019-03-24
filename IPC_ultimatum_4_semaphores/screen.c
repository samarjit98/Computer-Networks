#include "ipc.h"

int main(int argc, char* argv[])
{
	char* screen = "./fifo_screen";
	mkfifo(screen, 0666);

	int sfd = open(screen, O_RDWR);

	while(1)
	{
		char buff[20];
		int n = read(sfd, buff, 20);
		printf("%s\n", buff);
	}
}