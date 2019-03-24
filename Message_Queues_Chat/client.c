#include "ipc.h"

int msqid;
struct mymesg
{
	long mtype;
	int processno;
	char mtext[512];
};

static void sig_func(int signo)
{
	struct mymesg buff;
	msgrcv(msqid, &buff, sizeof(struct mymesg), (long)getpid(), 0);
	printf("%s\n", buff.mtext);
}

int main(int argc, char* argv[])
{
	signal(SIGUSR1, sig_func);

	msqid = msgget(1356, 0666 | IPC_CREAT);

	while(1)
	{
		char buff[MAXLINE];
		scanf("%s", buff);

		struct mymesg sendthis;
		sendthis.mtype = 1;
		sendthis.processno = getpid();
		strcpy(sendthis.mtext, buff);
	
		msgsnd(msqid, &sendthis, sizeof(struct mymesg), 0);	
		kill(atoi(argv[1]), SIGUSR1);
	}
}

