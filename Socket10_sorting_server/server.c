#include "networks.h"
#include <limits.h>

struct array_buff
{
	int arr[100];
	int size;
};

void selection_sort(int arr[100], int n)
{
	int mini = INT_MAX;
	int index = -1;
	for(int i=0; i<n; i++)
	{
		for(int j=i; j<n; j++)
		{
			if(arr[j]<mini)
			{
				mini = arr[j]; index = j;
			}
		}

		int tmp = arr[index];
		arr[index] = arr[i];
		arr[i] = tmp;

		mini = INT_MAX; index = -1;
	}
}

void insertion_sort(int arr[100], int n)
{
	int i, key, j; 
    for (i = 1; i < n; i++) 
    { 
        key = arr[i]; 
        j = i - 1; 
 
        while (j >= 0 && arr[j] > key) 
        { 
            arr[j + 1] = arr[j]; 
            j = j - 1; 
        } 

        arr[j + 1] = key; 
    } 
}

int main(int argc, char** argv)
{
	int lfd1, lfd2, cfd;
	struct sockaddr_in servip1, servip2;
	char buff[MAXLINE];
	time_t ticks;

	lfd1 = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servip1, sizeof(servip1));
	servip1.sin_family = AF_INET;
	servip1.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip1.sin_port = htons(5000);

	lfd2 = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servip2, sizeof(servip2));
	servip2.sin_family = AF_INET;
	servip2.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip2.sin_port = htons(5001);

	bind(lfd1, (struct sockaddr*)&servip1, sizeof(servip1));
	bind(lfd2, (struct sockaddr*)&servip2, sizeof(servip2));

	fd_set rset;
		
	listen(lfd1, LISTENQ);
	listen(lfd2, LISTENQ);

	char addr[20];
	inet_ntop(AF_INET, &(servip1.sin_addr), addr, 20);
	printf("%s\n", addr);

	inet_ntop(AF_INET, &(servip2.sin_addr), addr, 20);
	printf("%s\n", addr);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	while(1)
	{
		FD_ZERO(&rset);
		FD_SET(lfd1, &rset);
		FD_SET(lfd2, &rset);

		if(select(FD_SETSIZE, &rset, NULL, NULL, &tv))
		{	
			if(FD_ISSET(lfd1, &rset))
			{
				cfd = accept(lfd1, (struct sockaddr*)NULL, NULL);
				
				if(cfd>0)
				{
					if(fork()==0)
					{	
						close(lfd1);
						
						struct array_buff buff;
						read(cfd, &buff, sizeof(struct array_buff));

						printf("Buffer size: %d\n", buff.size);

						printf("Requested array: \n");

						for(int i=0; i<buff.size; i++)printf("%d ", buff.arr[i]);
						printf("\n");

						selection_sort(buff.arr, buff.size);

						printf("Returning array: \n");

						for(int i=0; i<buff.size; i++)printf("%d ", buff.arr[i]);
						printf("\n");

						write(cfd, (void*)&buff, sizeof(struct array_buff));

						close(cfd);
					}
					else
					{
						close(cfd);
					}
				}
			}
			else if(FD_ISSET(lfd2, &rset))
			{
				cfd = accept(lfd2, (struct sockaddr*)NULL, NULL);
				
				if(cfd>0)
				{
					if(fork()==0)
					{	
						close(lfd2);
						
						struct array_buff buff;
						read(cfd, &buff, sizeof(struct array_buff));

						printf("Buffer size: %d\n", buff.size);

						printf("Requested array: \n");

						for(int i=0; i<buff.size; i++)printf("%d ", buff.arr[i]);
						printf("\n");

						insertion_sort(buff.arr, buff.size);

						printf("Returning array: \n");

						for(int i=0; i<buff.size; i++)printf("%d ", buff.arr[i]);
						printf("\n");

						write(cfd, (void*)&buff, sizeof(struct array_buff));

						close(cfd);
					}
					else
					{
						close(cfd);
					}
				}
			}
		}
	}
	return 0;
}

/*
Output:

➜  Socket10_sorting_server git:(master) ✗ ./server
127.0.0.1
127.0.0.1
Buffer size: 3
Requested array: 
3 2 1 
Returning array: 
1 2 3 
Buffer size: 3
Requested array: 
3 2 1 
Returning array: 
1 2 3 
Buffer size: 4
Requested array: 
5 3 2 1 
Returning array: 
1 2 3 5 
Buffer size: 5
Requested array: 
3 4 2 5 1 
Returning array: 
1 2 3 4 5 

*/
