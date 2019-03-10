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
	struct mymesg* buff;
	void* tmp;
	msgrcv(msqid, tmp, sizeof(struct mymesg), 1, 0);
	buff = (struct mymesg*)tmp;

	printf("%s", buff->mtext);

	int currproc = buff->processno;
	int flag = 0;
	for(int i=0; i<curr; i++)
	{
		if(currproc == processno[i])
		{
			flag = 1;
		}
		else
		{
			buff->mtype = processno[i];
			msgsnd(msqid, (void*)buff, sizeof(buff), 0);
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

	msqid = msgget(1234, 0666 | IPC_CREAT);

	while(1);

	return 0;
}