#include "ipc.h"

int main(int argc, char* argv[])
{
	sem_t *sem1, *sem2;
	int *x, *y;

	int shmid = shmget(1669, 2*sizeof(int) + 2*sizeof(sem_t), 0666|IPC_CREAT);
	void* shmp = (void*)shmat(shmid, 0, 0);

	x = (int*)shmp; shmp = shmp + sizeof(int);
	y = (int*)shmp; shmp = shmp + sizeof(int);
	sem1 = (sem_t*)shmp; shmp = shmp + sizeof(sem_t);
	sem2 = (sem_t*)shmp;

	for(int i=0; i<10; i++)
	{
		sem_wait(sem2);
		printf("%d: %d %d\n", i, *x, *y);
		*x = *x + 2; *y = *y + 2;
		printf("%d: %d %d\n", i, *x, *y);
		sem_post(sem1);
	}
}