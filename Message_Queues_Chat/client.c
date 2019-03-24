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

	msqid = msgget(1357, 0666 | IPC_CREAT);

	int shmid = shmget(1279, sizeof(int), 0666|IPC_CREAT);
	int *servid = (int*)shmat(shmid, 0, 0);

	while(1)
	{
		char buff[MAXLINE];
		scanf("%s", buff);

		struct mymesg sendthis;
		sendthis.mtype = 1;
		sendthis.processno = getpid();
		strcpy(sendthis.mtext, buff);
	
		msgsnd(msqid, &sendthis, sizeof(struct mymesg), 0);	
		kill(*servid, SIGUSR1);
	}
}

