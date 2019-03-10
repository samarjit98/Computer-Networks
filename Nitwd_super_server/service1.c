#include "networks.h"

int main(int argc, char* argv[])
{
	printf("Here3\n");
	int newstdin = atoi(argv[1]), newstdout = atoi(argv[2]), cfd = atoi(argv[3]);

	char buff[MAXLINE];

	printf("%d %d\n", newstdin, newstdout);
	read(cfd, buff, MAXLINE);
	printf("%s\n", buff);

	read(newstdin, buff, MAXLINE);
	write(cfd, buff, strlen(buff)+1);

	close(cfd);
	kill(getppid(), SIGUSR1);
	return 0;
}