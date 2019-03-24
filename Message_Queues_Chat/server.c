#include "ipc.h"

int msqid;
struct mymesg
{
	long mtype;
	int processno;
	char mtext[512];
};

int processno[100];
int curr = 0;

static void sig_func(int signo)
{
	struct mymesg buff;
	msgrcv(msqid, &buff, sizeof(struct mymesg), 1, 0);

	printf("%s\n", buff.mtext);

	int currproc = buff.processno;
	int flag = 0;
	for(int i=0; i<curr; i++)
	{
		if(currproc == processno[i])
		{
			flag = 1;
		}
		else
		{
			buff.mtype = (long)processno[i];
			msgsnd(msqid, &buff, sizeof(struct mymesg), 0);
			kill(processno[i], SIGUSR1);
		}
	}

	if(flag==0)
	{
		processno[curr] = currproc;
		curr++;
	}
}

int main(int argc, char* argv[])
{
	printf("%d\n", getpid());
	signal(SIGUSR1, sig_func);

	msqid = msgget(1356, 0666 | IPC_CREAT);

	while(1);

	return 0;
}