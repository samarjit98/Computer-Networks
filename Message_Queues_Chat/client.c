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
	struct mymesg* buff;
	void* tmp;
	msgrcv(msqid, tmp, sizeof(struct mymesg), (long)getpid(), 0);
	buff = (struct mymesg*)tmp;
	printf("%s\n", buff->mtext);
}

int main(int argc, char* argv[])
{
	signal(SIGUSR1, sig_func);

	msqid = msgget(1234, 0666 | IPC_CREAT);

	while(1)
	{
		char buff[MAXLINE];
		scanf("%s", buff);

		struct mymesg* sendthis = malloc(sizeof(struct mymesg));
		sendthis->mtype = 1;
		sendthis->processno = getpid();
		strcpy(sendthis->mtext, buff);

		msgsnd(msqid, sendthis, sizeof(sendthis), 0);
		kill(atoi(argv[1]), SIGUSR1);
	}
}

