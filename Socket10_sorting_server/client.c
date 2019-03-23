#include "networks.h"

struct array_buff
{
	int arr[100];
	int size;
};

int main(int argc, char** argv)
{
	int sfd, n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servip;

	if((sfd = socket(AF_INET, SOCK_STREAM, 0))<0)printf("sock error!!");

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_port = htons(atoi(argv[1]));
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");

	while(connect(sfd, (struct sockaddr*)&servip, sizeof(servip))<0)printf("Connect error\n");

	printf("Enter data:\n");

	struct array_buff* buff;
	buff = malloc(sizeof(struct array_buff));

	printf("Enter size: "); scanf("%d", &buff->size);

	printf("Enter array element: ");

	for(int i=0; i<buff->size; i++)
	{
		scanf("%d", &buff->arr[i]);
	}

	write(sfd, (void*)buff, sizeof(struct array_buff));
	
	read(sfd, buff, sizeof(struct array_buff));

	printf("Buffer size: %d\n", buff->size);

	printf("Sorted array from socket: \n");

	for(int i=0; i<buff->size; i++)
	{
		printf("%d ", buff->arr[i]);
	}
	printf("\n");

	close(sfd);

	return 0;
}

/*

Output:

➜  Socket10_sorting_server git:(master) ✗ ./client 5001
Enter data:
Enter size: 3
Enter array element: 3
2
1
Buffer size: 3
Sorted array from socket: 
1 2 3 
➜  Socket10_sorting_server git:(master) ✗ ./client 5000
Enter data:
Enter size: 3
Enter array element: 3
2
1
Buffer size: 3
Sorted array from socket: 
1 2 3 
➜  Socket10_sorting_server git:(master) ✗ ./client 5001
Enter data:
Enter size: 4
Enter array element: 5
3
2
1
Buffer size: 4
Sorted array from socket: 
1 2 3 5 
➜  Socket10_sorting_server git:(master) ✗ ./client 5000
Enter data:
Enter size: 5
Enter array element: 3
4
2
5
1
Buffer size: 5
Sorted array from socket: 
1 2 3 4 5 

*/